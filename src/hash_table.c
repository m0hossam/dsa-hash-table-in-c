#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

const int HT_PRIME = 151; //arbitrary prime number > 128 needed for hashing
const int HT_INITIAL_BASE_SIZE = 53; //arbitrary hash-table max size

//Prototypes:

static void free_item(item* ht_item);
static linked_list* del_ht_item(hash_table* ht, linked_list* list, item* ht_item);
static void del_ht_list(linked_list* list);
static item* search_ht_list(linked_list* list, const char* k);
static linked_list* new_list(item* head);
static void insert_list_item(linked_list* list, item* new_item);
static item* new_ht_item(const char* k, const char* v);
hash_table* new_ht();
void del_ht(hash_table* ht);
static int hash_ht_item(const char* s, const int special_prime, const int num_buckets);
void insert_ht_item(hash_table* ht, const char* key, const char* value);
char* search_ht_item(hash_table* ht, const char* key);
void remove_ht_item(hash_table* ht, const char* key);

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
static void del_ht_list(linked_list* list)
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

//creates new hash-table with size = HT_INITIAL_BASE_SIZE
hash_table* new_ht() 
{
	hash_table* ht = malloc(sizeof(hash_table));
	ht->size = next_prime(HT_INITIAL_BASE_SIZE);
	ht->count = 0;
	ht->lists = calloc((size_t)ht->size, sizeof(linked_list*)); //array of pointers to linked lists
	return ht;
}

//delete entire hash-table
void del_ht(hash_table* ht) 
{ 
	for (int i = 0; i < ht->size; i++) 
	{
		linked_list* list = ht->lists[i];
		if (list != NULL) 
		{
			del_ht_list(list);
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