//Hash Table & Linked List Structures and API Prototypes

//Structures:

typedef struct s_item
{
	char* key;
	char* value;
	struct s_item* next;
} item;

typedef struct
{
	item* head;
} linked_list;

typedef struct
{
	int size;
	int count;
	linked_list** lists;
} hash_table;

//API Prototypes:

hash_table* new_ht();
void del_ht(hash_table* ht);
void insert_ht_item(hash_table* ht, const char* key, const char* value);
char* search_ht_item(hash_table* ht, const char* key);
void remove_ht_item(hash_table* ht, const char* key);


