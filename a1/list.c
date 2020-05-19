#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"



//Declare statically allocated LIST heads and NODEs
NODE listNodes[MAX_NUM_NODES];
LIST listHeads[MAX_NUM_HEADS];

//Initialized keeps track of whether init() has been called or not BO
bool initialized = false;

LIST *freeHead;
NODE *freeNode;



//Helper function which prints out our lists.
void printList(LIST *list)
{	
	printf("List items are: ");
	NODE *currItem = list->head;
	for(int i = 0; i < list->size; i++)
	{	
	
		printf("%d ",*(int *)currItem->item);
		currItem = currItem->next;
	}
	printf("\n");

}

void printListString(LIST *list)
{
	NODE *currItem = list->head;
	for(int i = 0; i < list->size; i++)
	{
		printf("%s ",(char *)currItem->item);
		currItem = currItem->next;
	}
	printf("\n");




}

//Initialize statically allocated LIST heads and NODEs
void init()
{
	
	//Initialize all nodes in listHeads so that they're ready to be added onto the list heads
	for(int i = 0; i < MAX_NUM_NODES-1; i++)
	{
		listNodes[i].item = NULL;
		listNodes[i].next = &listNodes[i+1];
		listNodes[i].prev = NULL;
	}
	
	//Initialize the last node in the node list.


	listNodes[MAX_NUM_NODES-1].item = NULL;
	listNodes[MAX_NUM_NODES-1].next = NULL;
	listNodes[MAX_NUM_NODES-1].prev = NULL;


	freeNode = &listNodes[0];


	//Initialize all list heads except the first and the last in listHeads
	for(int i = 1; i < MAX_NUM_HEADS-1; i++)
	{
		listHeads[i].head = NULL;
		listHeads[i].curr = NULL;
		listHeads[i].tail = NULL;
		listHeads[i].next = &listHeads[i+1];
		listHeads[i].prev = &listHeads[i-1];
		listHeads[i].size = 0;
		listHeads[i].currBeforeStart = NULL;
		listHeads[i].currBeyondEnd = NULL;
	}	

	//Initialize the first list head in list head list
	listHeads[0].next = &listHeads[1];
	listHeads[0].prev = NULL;
	listHeads[0].head = NULL;
	listHeads[0].curr = NULL;
	listHeads[0].tail = NULL;
	listHeads[0].size = 0;
	listHeads[0].currBeforeStart = NULL;
	listHeads[0].currBeyondEnd = NULL;

	//Initialize the last list head in list head list
	listHeads[MAX_NUM_HEADS-1].next = NULL;
	listHeads[MAX_NUM_HEADS-1].prev = &listHeads[MAX_NUM_HEADS-2];
	listHeads[MAX_NUM_HEADS-1].head = NULL;
	listHeads[MAX_NUM_HEADS-1].curr = NULL;
	listHeads[MAX_NUM_HEADS-1].tail = NULL;
	listHeads[MAX_NUM_HEADS-1].size = 0;
	listHeads[MAX_NUM_HEADS-1].currBeforeStart = NULL;
	listHeads[MAX_NUM_HEADS-1].currBeyondEnd = NULL;
	
	freeHead = &listHeads[0];
	initialized = true;
}


LIST *ListCreate() 
{
	if (!initialized)
	{
		init();
	}

		
	if (freeHead != NULL)
	{
		LIST *currList = freeHead;
		freeHead = freeHead->next;
		return currList;
	}
	else
	{
		//ListCreate has failed at this point so we return a NULL pointer.
		printf("ListCreate error: Cannot create anymore lists.  List heads are exhausted. \n");
		return NULL;
	}
	
}



int ListCount(LIST *list)
{
	return list->size;	
}

void *ListFirst(LIST *list)
{
	
	//If the list is empty, print an error message and return a null pointer.
	if(list->size == 0)
	{
		printf("ListFirst error: List is empty, cannot return the first item.  Returning a null pointer..\n");
		list->head = NULL;
		return list->head;
	}
	//else return the head of the list and point the current pointer at it
	else
	{
		list->currBeforeStart = false;
		list->currBeyondEnd = false;
		list->curr = list->head;
		return list->curr->item;
	}
}

void *ListLast(LIST *list)
{
	//If the list is empty, print an error message and return a null pointer.
	if(list->size == 0)
	{
		printf("ListLast error: List is empty, cannot return the last item.  Returning a null pointer..\n");
		list->tail = NULL;
		return list->tail;
	}
	//else the return the tail of the list and point the current pointer at it
	else
	{
		list->currBeforeStart = false;
		list->currBeyondEnd = false;	
		list->curr = list->tail;
		return list->curr->item;
	}
}

void *ListNext(LIST *list)
{
	
	//If the current pointer is beyond the end of the list, return NULL pointer
	if(list->currBeyondEnd)
	{
		list->currBeyondEnd = true;
		list->curr = NULL;
		return list->curr;
	}
	//If the current pointer is pointing to before the start of the list, ListNext advances pointer to the head.
	else if(list->currBeforeStart)
	{
		list->curr = list->head;
		list->currBeforeStart = false;
		return list->curr->item;	
	}
	//If the current pointer is pointing to the tail, we return a null pointer.
	else if(list->curr == list->tail)
	{
		list->currBeyondEnd = true;	
		list->curr = list->curr->next;
		return list->curr;
	}
	//If the current pointer is pointing to a valid node inside the list, advance the pointer to the next node and return its item.
	else
	{
		list->curr = list->curr->next;
		return list->curr->item;
	}
}

void *ListPrev(LIST *list)
{
	
	//If the current pointer is before the start of the list, return NULL pointer.
	if(list->currBeforeStart)
	{
		list->currBeforeStart = true;
		list->curr = NULL;
		return list->curr;
	}
	//If the current pointer is pointing to after the end of the list, list prev backtracks the pointer to point at the  tail.
	else if(list->currBeyondEnd)
	{
		list->curr = list->tail;
		list->currBeyondEnd = false;
		return list->curr->item;		

	}
	//If the current pointer is pointing at the list's head, listPrev backtracks the pointer to point before the start of the list.
	else if(list->curr == list->head)
	{
		list->currBeforeStart = true;
		list->curr = list->curr->prev;
 		return list->curr;
		
	}
	//If the current pointer is pointing to a valid node inside the list, advance the pointer and return its value.
	else
	{
		list->curr = list->curr->prev;
		return list->curr->item;
	}


}

void *ListCurr(LIST *list)
{
	//If the list is empty, return a NULL pointer because current pointer should be NULL.
	if(list->size == 0)
	{
		printf("ListCurr error: Empty list.  Cannot return the current item.  Returning a null pointer instead..\n");
		list->curr = NULL;
		return list->curr;	
	}
	//If the pointer is pointing to a location before the list, return a NULL pointer.
	else if(list->currBeforeStart)
	{
		list->curr = NULL;
		return list->curr;
	}
	//If the pointer is pointing to a location beyond the end of the list, return a NULL pointer.
	else if(list->currBeyondEnd)
	{
		list->curr = NULL;
		return list->curr;
	}
	//Else, return the current pointer's item
	else 
	{
		return list->curr->item;
	}
}

int ListAdd(LIST *list, void * item)
{
	
	//If the nodes are exhausted, return failure
	if(freeNode == NULL)
	{
		printf("ListAdd error:  Cannot add anymore nodes, nodes are exhausted.\n");
		return -1;
	}
		
	
	//In the case where the list is empty
	if(list->size == 0)
	{	
		
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->next = NULL;
		node->prev = NULL;

		list->head = node;
		list->tail = node;
		list->curr = node;
		list->size++;
		return 0;
	}
	//Consider the case where the current list pointer is pointing to before the start of the list
	else if (list->size > 0 && list->currBeforeStart)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->next = list->head;
		list->head->prev = node;
		node->prev = NULL;
		
		list->head = node;
		list->curr = node;
		list->currBeforeStart = false;
		list->size++;
		return 0;
	}
	//Consider the case where the current list pointer is pointing to beyond the end of the list
	else if (list->size > 0 && list->currBeyondEnd)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->next = NULL;
		node->prev = list->tail;
		list->tail->next = node;

		list->tail = node;
		list->curr = node;
		list->currBeyondEnd = false;
		list->size++;
		return 0;
	}
	//Consider the case where the current list pointer is pointing to the tail of the list
	else if (list->size > 0 && list->curr == list->tail)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->next = NULL;
		node->prev = list->tail;

		list->tail->next = node;
		list->tail = node;
		list->curr = node;
		list->size++;
		return 0;
	}
	else
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->prev = list->curr;
		node->next = list->curr->next;
		list->curr->next->prev = node;

		list->curr->next = node;
		list->curr = node;
		list->size++;
		return 0;
	}
	
	return -1;
}

int ListInsert(LIST *list, void * item) 
{	
	//If the nodes are all exhausted, return failure.
	if(freeNode == NULL)
	{
		printf("ListInsert error: Cannot insert anymore nodes.  Nodes are exhausted\n");
		return -1;
	}

	//If the list is empty, insert the item as the only item in the list and point the current pointer at it.
	if(list->size == 0)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->next = NULL;
		node->prev = NULL;
	
		list->head = node;
		list->curr = node;
		list->tail = node;
		list->size++;
		return 0;
	}
	//If the current pointer is pointing to before the start of the list, insert the item and set the current
	//pointer to point at it instead.
	else if(list->size > 0 && list->currBeforeStart)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->next = list->head;
		node->prev = NULL;
		list->head->prev = node;

		list->head = node;
		list->curr = node;
		list->currBeforeStart = false;
		list->size++;
		return 0;
	}
	//If the current pointer is pointing to after the end of the list, insert the item and set the current pointer
	//to point at it instead.
	else if(list->size > 0 && list->currBeyondEnd)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->next = NULL;
		node->prev = list->tail;
		list->tail->next = node;

		list->tail = node;
		list->curr = node;
		list->currBeyondEnd = false;
		list->size++;
		return 0;
	}
	else if(list->size > 0 && list->curr == list->head)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->next = list->head;
		node->prev = NULL;

		list->head->prev = node;
		list->head = node;
		list->curr = node;
		list->size++;
		return 0;
	}
	else
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->next = list->curr;
		node->prev = list->curr->prev;
		list->curr->prev->next = node;		

		list->curr->prev = node;
		list->curr = node;	
		list->size++;
		return 0;
	}

	return -1;
}

int ListAppend(LIST *list, void * item)
{
	//Consider the case where the nodes are all exhausted.  If so, return failure.
	if(freeNode == NULL)
	{
		printf("ListAppend error: Cannot append anymore nodes.  Nodes are exhausted\n");
		return -1;
	}
	//If list is empty, append the item to the list and make it the head,tail and curr.
	else if (list->size == 0)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->prev = NULL;
		node->next = NULL;

		list->head = node;
		list->tail = node;
		list->curr = node;
		list->size++;
		return 0;
	}
	//If the current pointer is pointing to before the start of the list, append the item 
	//and re-direct the current pointer to point at it instead.
	else if (list->size > 0 && list->currBeforeStart)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;
		
		node->item = item;
		node->prev = list->tail;
		node->next = NULL;
		list->tail->next = node;

		list->tail = node;
		list->curr = node;
		list->currBeforeStart = false;
		list->size++;
		return 0;

	}
	//If the current pointer is pointing to after the end of the list, append the item 
	//and re-direct the current pointer to point at it instead.
	else if (list->size > 0 && list->currBeyondEnd)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->prev = list->tail;
		node->next = NULL;
		list->tail->next = node;

		list->tail = node;
		list->curr = node;
		list->currBeyondEnd = false;
		list->size++;
		return 0;

	}
	//else add node to the end of the list and re-direct the current pointer to point at it.
	else
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->prev = list->tail;
		node->next = NULL;
		list->tail->next = node;

		list->tail = node;
		list->curr = node;
		list->size++;
		return 0;
	}
}

int ListPrepend(LIST *list, void * item)
{
	
	//Consider the case where the nodes are all exhausted.  If so, return failure
	if(freeNode == NULL)
	{
		printf("ListPrepend error: Cannot prepend anymore nodes.  Nodes are exhausted\n");
		return -1;
	}
	//If the list is empty, prepend the item as the only item in the list.
	else if (list->size == 0)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->prev = NULL;
		node->next = NULL;

		list->head = node;
		list->tail = node;
		list->curr = node;
		list->size++;
		return 0;
	}
	//If the current pointer is pointing to before the start of the list, prepend the item and set the current pointer
	//to point at it instead.
	else if (list->size > 0 && list->currBeforeStart)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->prev = NULL;
		node->next = list->head;
		list->head->prev = node;
		
		list->head = node;
		list->curr = node;
		list->currBeforeStart = false;
		list->size++;
		return 0;
	}
	//If the current pointer is pointing to after the end of the list, prepend the item and set the current pointer
	//to point at it instead.
	else if (list->size > 0 && list->currBeyondEnd)
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->prev = NULL;
		node->next = list->head;
		list->head->prev = node;
		
		list->head = node;
		list->curr = node;
		list->currBeyondEnd = false;
		list->size++;
		return 0;	
	}
	else
	{
		NODE *node = freeNode;
		freeNode = freeNode->next;

		node->item = item;
		node->prev = NULL;
		node->next = list->head;
		list->head->prev = node;

		list->head = node;
		list->curr = node;
		list->size++;
		return 0;
	}
}

void *ListRemove(LIST *list)
{
	//Consider the case where the list is empty
	if(list->size == 0)
	{
		printf("ListRemove error: List is empty, cannot remove any items\n");
		return NULL;
	}
	//If the current pointer is outside the list, return NULL.
	else if (list->currBeforeStart || list->currBeyondEnd)
	{
		return NULL;
	}
	//Consider the case where the list has only one item.
	else if(list->size == 1 && list->curr)
	{

		//We need to add the node we are about to remove back to our supply of nodes 
		NODE *replenishedNode = list->curr;

		//Save the item of the node the current pointer is pointing to in a temporary node before we remove it.
		NODE *currNode = list->curr;

		//Delete the current node from our list..
		list->head = NULL;
		list->curr = NULL;
		list->tail = NULL;
		list->size--;


		//Replenish the deleted node back to our supply of nodes.
		//Consider the case if we have exhausted all our nodes
		if(freeNode == NULL)
		{
			replenishedNode->prev = NULL;
			replenishedNode->next = NULL;
			replenishedNode->item = NULL;
			freeNode = replenishedNode;
		}
		else
		{
			replenishedNode->prev = NULL;
			replenishedNode->next = freeNode;
			replenishedNode->item = NULL;
			freeNode = replenishedNode;
		}

		return currNode->item;
	}
	//Consider the case where the list has multiple items and the current pointer is pointing to a valid node not outside the list.
	else
	{
		
		NODE *replenishedNode = list->curr;

		NODE *currNode = list->curr;

		//Delete the node. Case 1: The current pointer is pointing to the tail of the list
		if(list->curr == list->tail)
		{
			list->tail = list->tail->prev;
			list->tail->next = NULL;
			list->curr = list->tail->next;
			list->currBeyondEnd = true;
			list->size--;

		}
		//Delete the node. Case 2: The current pointer is pointing to the head of the list
		else if(list->curr == list->head)
		{

			list->head = list->curr->next;
			list->curr = list->head;
			list->curr->prev = NULL;
			list->size--;
		}	
		//Delete the node. Case 3: The current pointer is pointing to a node which is neither the head or tail.
		else
		{


			list->curr->prev->next = list->curr->next;
			list->curr->next->prev = list->curr->prev;
			
			NODE *prevNode = list->curr->prev;

			list->curr = list->curr->next;
			list->curr->prev = NULL;
			list->curr->prev = prevNode;
			list->size--;
		}

		//Replenish the deleted node back into our node supply.

		if(freeNode == NULL)
		{
			replenishedNode->prev = NULL;
			replenishedNode->next = NULL;
			replenishedNode->item = NULL;
			freeNode = replenishedNode;
		}
		else
		{
			replenishedNode->prev = NULL;
			replenishedNode->next = freeNode;
			replenishedNode->item = NULL;
			freeNode = replenishedNode;
		}
		
		return currNode->item;
	}
}

void ListConcat(LIST *list1, LIST *list2)
{
	
	LIST *replenishedHead = list2;
	NODE *currNode = list1->curr;

	//We will re-direct the pointers and merge list 2 into list 1.
	if(list1->tail && list2->head)
	{
		list1->tail->next = list2->head;
		list2->head->prev = list1->tail;
		list1->tail = list2->tail;
		list1->size += list2->size;
	}
	//In the case that list 1 is empty and list 2 is not, the concatenated list becomes list 2.  Current pointer is set to point
	//outside the list (before the start).
	else if(list1->size == 0 && list2->head)
	{
		list1->head = list2->head;
		list1->tail = list2->tail;
		list1->curr = NULL;
		currNode = list1->curr;
		list1->currBeforeStart = true;
		list1->size += list2->size;	
	}


	//Delete list 2
	list2->size = 0;
        list2->currBeforeStart = false;
	list2->currBeyondEnd = false;
	list2->next = NULL;
	list2->prev = NULL;
	list2->curr = NULL;
	list2->head = NULL;
	list2->tail = NULL;	
	list2 = NULL;

	//Initialize the head that we retrieve after deleting list2
	replenishedHead->size = 0;
	replenishedHead->currBeforeStart = false;
	replenishedHead->currBeyondEnd = false;
	replenishedHead->next = NULL;
	replenishedHead->prev = NULL;
	replenishedHead->curr = NULL;
	replenishedHead->head = NULL;
	replenishedHead->tail = NULL;

	//Replenish the deleted list head back into our supply of list heads	
	//Case 1: Our head supply is exhausted
	if(freeHead == NULL)
	{
		 
		freeHead = replenishedHead;
	}
	//Case 2: Our head supply is not exhausted
	else
	{
		replenishedHead->next = freeHead;
		freeHead->prev = replenishedHead;
		freeHead = replenishedHead;
	}

	list1->curr = currNode;
}

void ListFree(LIST *list, void (*itemFree)())
{
	if (list->size == 0)
	{
		printf("ListFree error: Cannot free list, list is already empty \n");
		//return NULL;
	}
	else
	{	
		int count = 0;
			
		while(list->curr != NULL)
		{
					
			//Go through each node in the list starting from the head and remove each node as well as free it.
			list->curr = list->head;
			NODE *replenishedNode = list->curr;
			(*itemFree)(list->curr);
			ListRemove(list);	
				
			//Reclaim the node that we removed from the list.
			replenishedNode->next = NULL;
			replenishedNode->prev = NULL;
			replenishedNode->item = NULL;

				
			if(freeNode == NULL)
			{
				freeNode = replenishedNode;
			}
			else
			{
				replenishedNode->next = freeNode;
				replenishedNode->prev = NULL;
				freeNode->prev = replenishedNode;
				freeNode = replenishedNode;
			}
			
		}

		
		//After freeing the list of all its items, we will now delete the list and reclaim its head.
		//
		LIST *replenishedHead = list;

		//Delete the list
		list->next = NULL;
		list->prev = NULL;
		list->head = NULL;
		list->curr = NULL;
		list->tail = NULL;
		list->currBeforeStart = false;
		list->currBeyondEnd = false;
		list->size = 0;
		list = NULL;
		
		//Initialize the head we are about to re-claim.
		replenishedHead->next = NULL;
		replenishedHead->prev = NULL;
		replenishedHead->head = NULL;
		replenishedHead->curr = NULL;
		replenishedHead->tail = NULL;
		replenishedHead->currBeforeStart = false;
		replenishedHead->currBeyondEnd = false;
		replenishedHead->size = 0;

		//Re-claim the head.
		if(freeHead == NULL)
		{
			freeHead = replenishedHead;
		}
		else
		{
			replenishedHead->next = freeHead;
			freeHead->prev = replenishedHead;
			freeHead = replenishedHead;	

		}		
	}

}

void *ListTrim(LIST *list)
{
	
	//Case 1: We have an empty list
	if(list->size == 0 || list == NULL)
	{
		return NULL;
	}
	//Case 2: We have a list with one item only.  (tail = curr)
	else if(list->size == 1)
	{	
		NODE *replenishedNode = list->tail;
		NODE *lastNode = list->tail;

		//Delete the last node
		list->head = NULL;
		list->curr = NULL;
		list->tail = NULL;
		list->size--;

		//Reclaim the node and add it back to our supply of nodes.
		if(freeNode == NULL)
		{
			replenishedNode->next = NULL;
			replenishedNode->prev = NULL;
			replenishedNode->item = NULL;
			freeNode = replenishedNode;
		}
		else
		{
			replenishedNode->next = freeNode;
			replenishedNode->prev = NULL;
			freeNode->prev = replenishedNode;
			freeNode = replenishedNode;
		}

		return lastNode->item;

	}

	//Consider the case where we have a list with multiple items where the current node is not the tail
	else
	{
		NODE *replenishedNode = list->tail;
		NODE *lastNode = list->tail;

		//Delete the last node
		list->tail = list->tail->prev;
		list->tail->next = NULL;
		list->size--;

		list->curr = list->tail;

		//Replenish our node supply with the node we just removed.
		if(freeNode == NULL)
		{
			replenishedNode->next = NULL;
			replenishedNode->prev = NULL;
			replenishedNode->item = NULL;
			freeNode = replenishedNode;
		}
		else
		{
			replenishedNode->next = freeNode;
			replenishedNode->prev = NULL;
			freeNode->prev = replenishedNode;
			freeNode = replenishedNode;

		}

		return lastNode->item;
	}
}


void *ListSearch(LIST *list, int (*comparator)(), void *comparisonArg)
{
	//If the list is empty, there are no items to search for.  Return a null pointer.
	if(list->size == 0)
	{
		printf("ListSearch error: List is empty.  No items to search for\n");
		return NULL;
	}
	//If the current pointer is before the start of the list, point it at the head instead and start searching from there.
	else if(list->size > 0 && list->currBeforeStart)
	{
		list->curr = list->head;
		list->currBeforeStart = false;
	}
	//If the current pointer is already at the end, return a null pointer.
	else if(list->currBeyondEnd)
	{
		printf("ListSearch error: Item not found.  Return null pointer.. \n");
		list->curr = NULL;
		list->currBeyondEnd = true;
		return list->curr;
	}
	

	//Traverse the list, starting at current pointer looking for a match.
	while(list->curr != NULL && !list->currBeforeStart && !list->currBeyondEnd)
	{
		//If we find a match, leave the current pointer where it is and return the item.
		if((*comparator)(list->curr->item, comparisonArg))
		{
			printf("Item found: %d\n",*(int *)list->curr->item);
			return list->curr->item;
		}
		//Else, continue traversing the list searching for a match.
		else
		{
			list->curr = list->curr->next;
		}
	}

	//If there is no match found, leave the current pointer beyond the end of the list.
	printf("ListSearch error:  Item not found.  Returning null pointer.. \n");
	list->curr = NULL;
	list->currBeyondEnd = true;
	return list->curr;
}	



