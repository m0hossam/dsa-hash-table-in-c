#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"

const int HT_PRIME = 151; //arbitrary prime number > 128 needed for hashing
const int HT_INITIAL_BASE_SIZE = 53; //arbitrary hash-table max size

//Prototypes:

static void free_item(item* ht_item);
static linked_list* del_ht_item(hash_table* ht, linked_list* list, item* ht_item);
static linked_list* del_ht_list(linked_list* list);
static item* search_ht_list(linked_list* list, const char* k);
static linked_list* new_list(item* head);
static void insert_list_item(linked_list* list, item* new_item);
static item* new_ht_item(const char* k, const char* v);
static void resize_ht(hash_table* ht, const int dir);
static hash_table* new_resized_ht(const int base_size);
hash_table* new_ht();
void del_ht(hash_table* ht);
static int hash_ht_item(const char* s, const int special_prime, const int num_buckets);
void insert_ht_item(hash_table* ht, const char* key, const char* value);
char* search_ht_item(hash_table* ht, const char* key);
void remove_ht_item(hash_table* ht, const char* key);

//Maths Functions Needed for Resizing:

int is_prime(const int x) 
{
	if (x < 2) 
	{
		return -1; //undefined
	}
	if (x < 4) 
	{
		return 1; //prime
	}
	if ((x % 2) == 0) 
	{
		return 0; //not prime
	}
	for (int i = 3; i <= floor(sqrt((double)x)); i += 2) 
	{
		if ((x%i) == 0) 
		{
			return 0;
		}
	}
	return 1;
}

int next_prime(int x) 
{
	while (is_prime(x) != 1) 
	{
		x++;
	}
	return x;
}

//Functions:

//deallocates memory used for an item
static void free_item(item* ht_item)
{
	free(ht_item->key);
	free(ht_item->value);
	free(ht_item);
	ht_item = NULL;
}

//deletes item from linked list inside hash-table
static linked_list* del_ht_item(hash_table* ht, linked_list* list, item* ht_item) 
{ 
	//linked list traversal:
	item* cur = list->head;
	item* prev = cur;
	while (strcmp(cur->key, ht_item->key) != 0) //searching for item (precaution)
	{
		if (cur->next == NULL) //key not found
		{
			return NULL;
		}
		prev = cur;
		cur = cur->next;
	}

	if (cur == list->head)
	{
		if (list->head->next == NULL) //delete entire list
		{
			free_item(cur);
			free(list);
			list = NULL;
		}
		else //change linked list's head
		{
			list->head = cur->next;
			free_item(cur);
		}
	}
	else
	{
		prev->next = cur->next;
		free_item(cur);
	}

	ht->count--;
	return list;
}

//deletes an entire list from the hash-table
static linked_list* del_ht_list(linked_list* list)
{
	item* cur = list->head;
	while (cur != NULL)
	{
		item* tmp = cur;
		cur = cur->next;
		free_item(tmp);
	}
	free(list);
	list = NULL;
	return list;
}

//search for an item in the linked list with a specific key
static item* search_ht_list(linked_list* list, const char* k)
{
	item* cur = list->head;
	while (strcmp(cur->key, k) != 0)
	{
		cur = cur->next;
		if (cur == NULL) //key not found
		{
			return NULL;
		}
	}
	return cur;
}

//create a new linked list
static linked_list* new_list(item* head)
{
	linked_list* list = malloc(sizeof(list));
	list->head = head;
	return list;
}

//insert an item as first in the linked list
static void insert_list_item(linked_list* list, item* new_item)
{
	new_item->next = list->head;
	list->head = new_item;
}

//creates new item with key-value pair
static item* new_ht_item(const char* k, const char* v)
{
	item* ht_item = malloc(sizeof(item));
	ht_item->key = strdup(k); //this uses malloc, remember to free
	ht_item->value = strdup(v);
	ht_item->next = NULL;
	return ht_item;
}

//creates & initializes a new hash-table
static hash_table* new_resized_ht(const int base_size) 
{
	hash_table* ht = malloc(sizeof(hash_table));
	ht->base_size = base_size;
	ht->size = next_prime(ht->base_size); 
	ht->count = 0;
	ht->lists = calloc((size_t)ht->size, sizeof(linked_list*)); 
	return ht; 
}

//resize hash-table to bigger(dir=1) or smaller(dir=-1) size
static void resize_ht(hash_table* ht, const int dir) 
{
	int new_size;
	if (dir == 1) //resize to bigger size
	{
		new_size = ht->base_size * 2;
	}
	else if (dir == -1) //resize to smaller size
	{
		new_size = ht->base_size / 2;
	}
	else 
	{
		return;
	}

	const int base_size = new_size;
	if (base_size < HT_INITIAL_BASE_SIZE) //resizing to less than minimum
	{
		return;
	}
	
	//insert existing items into new HT:
	hash_table* new_ht = new_resized_ht(base_size);
	for (int i = 0; i < ht->size; i++) 
	{
		linked_list* list = ht->lists[i];
		if (list != NULL)
		{
			item* cur = list->head;
			while (cur != NULL)
			{
				insert_ht_item(new_ht, cur->key, cur->value);
				cur = cur->next;
			}
		}
	}

	//update base_size & count:
	ht->base_size = new_ht->base_size;
	ht->count = new_ht->count;

	//swaping is done to make the freeing/deleting easier:

	//swap size:
	const int tmp_size = ht->size;
	ht->size = new_ht->size;
	new_ht->size = tmp_size;

	//swap items:
	linked_list** tmp_lists = ht->lists;
	ht->lists = new_ht->lists;
	new_ht->lists = tmp_lists;

	del_ht(new_ht);
}

//returns new hash-table with base size = HT_INITIAL_BASE_SIZE
hash_table* new_ht() 
{
	return new_resized_ht(HT_INITIAL_BASE_SIZE);
}

//delete entire hash-table
void del_ht(hash_table* ht) 
{ 
	for (int i = 0; i < ht->size; i++) 
	{
		linked_list* list = ht->lists[i];
		if (list != NULL) 
		{
			ht->lists[i] = del_ht_list(list);
		}
	}
	free(ht->lists); 
	free(ht);
	ht = NULL;
}

/*
Generic hashing algorithm
s = key to be hashed
special_prime = prime number > 128 (max number for standard ascii chars)
num_buckets = max number of slots inside HT
returns index of hashed key
*/
static int hash_ht_item(const char* s, const int special_prime, const int num_buckets) 
{ 
	long hash = 0;
	const int len_s = strlen(s);
	for (int i = 0; i < len_s; i++) 
	{ 
		hash += (long)pow(special_prime, len_s - (i + 1)) * s[i];
		hash %= num_buckets; //ensure the index is between 0 & (num_buckets-1)
	}
	return (int)hash;
}

//insert a key-value pair into hash-table
void insert_ht_item(hash_table* ht, const char* key, const char* value) 
{ 
	const int load = ht->count * 100 / ht->size;
	if (load > 70) 
	{
		resize_ht(ht, 1);
	}

	int index = hash_ht_item(key, HT_PRIME, ht->size); 
	linked_list* list = ht->lists[index];
	item* new_item = new_ht_item(key, value);

	if (list == NULL) //create a new list
	{
		list = new_list(new_item);
		ht->lists[index] = list;
	}
	else
	{
		item* dup_item = search_ht_list(list, key); //searching for duplicated key
		if (dup_item == NULL) //key is new
		{
			insert_list_item(list, new_item);
		}
		else //key already exists in the list
		{
			list = del_ht_item(ht, list, dup_item);
			if (list == NULL) //deleting the key-item deleted the whole list
			{
				list = new_list(new_item);
				ht->lists[index] = list;
			}
			else //list still exists after deleting key-item
			{
				insert_list_item(list, new_item);
			}
		}
	}
	ht->count++;
}

//search for an item with a specific key in the hash-table
char* search_ht_item(hash_table* ht, const char* key) 
{ 
	int index = hash_ht_item(key, HT_PRIME, ht->size);
	linked_list* list = ht->lists[index];

	if (list != NULL)
	{
		item* ht_item = search_ht_list(list, key);
		if (ht_item != NULL)
		{
			return ht_item->value;
		}
	}
	return NULL;
}

//remove an item with a specific key from the hash-table
void remove_ht_item(hash_table* ht, const char* key) 
{
	const int load = ht->count * 100 / ht->size;
	if (load < 10) 
	{
		resize_ht(ht, -1);
	}

	int index = hash_ht_item(key, HT_PRIME, ht->size);
	linked_list* list = ht->lists[index];

	if (list != NULL)
	{
		item* ht_item = search_ht_list(list, key);
		if (ht_item != NULL)
		{
			ht->lists[index] = del_ht_item(ht, list, ht_item);
		}
	}
}