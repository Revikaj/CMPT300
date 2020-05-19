#define MAX_NUM_NODES 32773
#define MAX_NUM_HEADS 10


typedef struct NODE 
{
	void *item; //This field indicates that the node has an item.
	struct NODE *next; //This field indicates that the node has a pointer to another node.
	struct NODE *prev;

}NODE;


typedef struct LIST
{
	unsigned int size;
	bool currBeyondEnd; //Boolean which indicates whether the list head is pointing to a node beyond the end of the list
	bool currBeforeStart; //Boolean which indicates whether the list head is pointing to node beyond the start of the list	       
	struct NODE *head;
	struct LIST *next;
	struct LIST *prev;	
	struct NODE *curr;
	struct NODE *tail;

}LIST;

void printList(LIST *list);

void printListString(LIST *list);

void init();

LIST *ListCreate();

int ListCount(LIST *list);

void *ListFirst(LIST *list);

void *ListLast(LIST *list);

void *ListNext(LIST *list);

void *ListPrev(LIST *list);

void *ListCurr(LIST *list);

int ListAdd(LIST *list, void * item);

int ListInsert(LIST *list, void * item);

int ListAppend(LIST *list, void * item);

int ListPrepend(LIST *list, void * item);

void *ListRemove(LIST *list);

void ListConcat(LIST *list1, LIST *list2);

void ListFree(LIST *list, void (*itemFree)());

void *ListTrim(LIST *list);

void *ListSearch(LIST *list, int (*comparator)(), void *comparisonArg);








