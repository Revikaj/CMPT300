#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include <string.h>

/* This file is the test driver file for list.c and list.h
*/

void itemFree(void *ptr)
{
	ptr = NULL;
}


int comparator(void *item, void *comparisonArg)
{
	if(item == comparisonArg)
	{
		return 1;
	}
	else
	{
		return 0;
	}


}


int main()
{
			
	LIST *l = ListCreate();

	LIST *l2 = ListCreate();

	LIST *l3 = ListCreate();

	/*
	char str[50];
	while(1)
	{

		fgets(str, 50, stdin);
		if(*str == '!')
		{
			memset(str, '\0', 50);
			break;
		}
		ListAdd(l,str);
		printListString(l);
	}*/





	ListCurr(l3);
	

	printf("List successfully created\n");
//	printf("Memory address of l: %p\n",&l);
//	printf("Size of l: %u\n\n",ListCount(l));

	int i = 1;
	int k  = 2;
	int m = 3;
	int n = 4;
	int z = 5;


	printf("Adding item to list...\n");
	ListAppend(l, &i);	
	printList(l);	
	printf("Size of l after add: %u\n",ListCount(l));
	printf("curr item: %d\n\n",*(int *)ListCurr(l));	

	ListAppend(l, &z);
	printList(l);
	printf("Size of l after add: %u\n",ListCount(l));
	printf("curr item: %d\n\n",*(int *)ListCurr(l));


	ListFree(l, itemFree);
	printList(l);
/*
	printf("Removing item from list...\n");
	ListRemove(l);
	printList(l);
	printf("Size of l after remove: %u\n\n",ListCount(l));

	printf("last: %d",*(int *)ListLast(l));
	printf("curr: %d",*(int *)ListCurr(l));
//	ListPrev(l);
	printf("Removing again...\n");
	ListRemove(l);
	printList(l);
	printf("Size of l after remove: %u\n\n",ListCount(l));
*/
	//This creates a segmentation fault.  Cannot return the current item if there is no current item.
	//printf("curr item: %d\n",*(int *)ListCurr(l));
/*
	printf("Prepending item to list...\n");
	ListPrepend(l, &k);
	printList(l);
	printf("Size of l after prepend: %u\n",ListCount(l));
	printf("curr item: %d\n\n",*(int *)ListCurr(l));

	printf("Appending item to list...\n");
	ListAppend(l, &z);
	printList(l);
	printf("Size of l after append: %u\n",ListCount(l));
	printf("curr item: %d\n",*(int *)ListCurr(l));
	printList(l);

	printf("Appending items to list 2...\n");
	ListAppend(l2,&m);
	ListAppend(l2,&n);
	printList(l2);

	printf("Concatenating the two lists...\n");
	ListConcat(l,l2);
	printList(l);	
	printf("Curr item: %d\n", *(int *)ListCurr(l));

	ListTrim(l);
	printList(l);	

	ListSearch(l, comparator, &m);
	ListNext(l);

	ListSearch(l, comparator, &m);
//	printf("curr item after list search: %d\n", *(int *)ListCurr(l));

	ListFree(l, itemFree);

	printList(l);
	ListSearch(l, comparator, &m);
	//printf("Curr item after list: %d\n", *(int *)ListCurr(l));	
	//printf(" list size: %d\n", ListCount(l));

	//Testing ListNext and ListPrev, ListRemove, ListInsert and ListPrepend.	
	ListInsert(l3, &z);
	ListInsert(l3, &m);
	
	ListInsert(l3, &n);
	ListNext(l3);
	ListInsert(l3, &i);
	ListPrepend(l3, &k);
	ListPrev(l3);
	ListPrepend(l3,&m);
	ListNext(l3);
	ListNext(l3);
	ListNext(l3);
		
	ListRemove(l3);
	ListRemove(l3);
	ListPrev(l3);
	ListPrev(l3);
	ListPrev(l3);
	ListRemove(l3);
	ListRemove(l3);
	ListRemove(l3);
	ListRemove(l3);
	printList(l3);


*/	//At this point, we are moving the current pointer to point before the start of the list.
	//ListNext(l3);


	



	//printf("next of l3: %d\n", *(int *)ListNext(l3));	
	//printf("prev of l3: %d\n", *(int *)ListPrev(l3));
//	printf("next of l3: %d\n", *(int *)ListNext(l3));
	

	return 0;
}

