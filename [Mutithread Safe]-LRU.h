/////////////////////////////////////////
/////////////LRU Cache
//////////*****IBRAR YUNUS*********
////////////v0.1 Multi-thread Safe
/////////////// v0.1 ~ Sun Nov 23 12:06 AM
///////////////////Compiled ~ Elementary OS Luna ~ GCC
////////////////////////////////////////


#include<stdio.h>
#include<stdlib.h>


//////STRUCTS 
typedef struct HashNode {
		int value;
		struct HashNode *next;	//Pointer to the next LL Node
		struct HashNode *prev;
		struct LinkedList *Pointer;
} HashNode;

typedef struct TableElement {
		struct HashNode *chain; //for
}TableElement;	

typedef struct LinkedList{
		struct LinkedList *nextLL;
		struct LinkedList *prevLL;
		int LLvalue;
} LinkedList;


////////////////////Definitions
int Hash_Init(int numOfBuckets);
int Hash_Insert(int x, LinkedList *ptr);
void Hash_Dump();	
void MemCheck();
int	Hash_CountElements();	
int	Hash_CountBucketElements(int bucketNumber);
int LRU_Insert(int element);	
void TraverseLinkedList();
void LRU_Init(int size);
int LRU_Access(int element);
int Hash_AlreadyExists( int bucketNumber, int x);
int	LRU_Remove(int element);

///Globals
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2=PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_init(&lock, NULL);
struct TableElement *MyTable;
int totalBuckets;
int LRUSIZE;

struct LinkedList *head;
struct LinkedList *tail;
int MaxSizeCounter_2;
struct LinkedList *AnotherPointer; // For use in HashRemove and HashInsert


////////////////////////////////////////////////
///MAIN
///Used For debuggin purposes. Do not use
///
///
///////////////////////////////////////////////
/*int main (void)
{
// Hash_Init(60);
// Hash_Insert(230958);
// Hash_Insert(2593);
// Hash_Insert(342);
// Hash_Insert(18);
// Hash_Insert(2);
// Hash_Insert(1);
// Hash_Insert(78);
// Hash_Insert(78);
// Hash_Dump();	
// printf("Total Elements: %d\n", Hash_CountElements());
// printf("Number of Elements in bucket 18: %d \n", Hash_CountBucketElements(18));

// if(Hash_Remove(2) == -1 )
//  printf("DOES NOT EXIST IN THE HASH TABLE\n");  
// if(Hash_Remove(18) == -1 )
//  printf("DOES NOT EXIST IN THE HASH TABLE\n"); 
// if(Hash_Remove(61) == -1 )
//  printf("61  DOES NOT EXIST IN THE HASH TABLE\n"); 

// Hash_Dump();	
// printf("Total Elements: %d\n", Hash_CountElements());
// printf("Number of Elements in bucket 18: %d \n", Hash_CountBucketElements(18));
	LRU_Init (5);
	LRU_Insert(4);
	LRU_Insert(5);
	//LRU_Insert(5);
	//LRU_Insert(6);
	LRU_Insert(2);
	LRU_Insert(6);
	TraverseLinkedList();
	LRU_Access(2);
	Hash_Dump();
	printf("******\n");
	TraverseLinkedList();
	LRU_Size();
	printf("Head is %d\n", head->LLvalue);
	printf("Tail is %d\n", tail->LLvalue);
}*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////LRU Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LRU_Init(int size)
{
	Hash_Init(size);		// Initializes the Hash Table
	LRUSIZE = size;		// For checking the Linked List
}

int LRU_Insert(int element)	
{
	//printf("Nillarz\n");
	pthread_mutex_lock(&lock);	
	static  MaxSizeCounter = 0; ;

	static int tailCheck = 0;
	int returner = 0;

	if( element <=  0 )
	{
		//printf("you tried to insert negative or zero value\n");
		pthread_mutex_unlock(&lock);
		return -1;
	}
	if( Hash_AlreadyExists(element%totalBuckets, element) == -1)
	{
		//printf("ALLREADY EXIST!\n");
		pthread_mutex_unlock(&lock);
		return -1;
	}
	if(MaxSizeCounter == LRUSIZE)
	{
		//printf("******************************************************\n");
		MaxSizeCounter--;
		returner = IsLRUFull();
	}

			struct LinkedList *a = malloc( 1 * sizeof(LinkedList));
			if( head == NULL)
			{
				a->LLvalue = element;
				a->nextLL = NULL;
				a->prevLL = NULL;
				head = a;
				tail = a;
				//printf("Temp Pointer is %p  and HEAD is %p: \n", a, head);
				Hash_Insert(element, a);
				MaxSizeCounter++;
				
			}
			else 
			{
				if(tailCheck != 1)
				{
					tail = head;
					tailCheck =1;
				}
				a->LLvalue = element;
				a->nextLL= head;
				head->prevLL = a;
				head = a;
				head->prevLL = NULL;
				//printf("Temp Pointer is %p  and HEAD is %p: \n", a, head);
				Hash_Insert(element, a);
				MaxSizeCounter++;

			}
	MaxSizeCounter_2 = MaxSizeCounter;
	pthread_mutex_unlock(&lock);
	return returner;
}
  
void TraverseLinkedList()
{
	//printf("Checking Linked List Elements\n");
	if( head != NULL)
	{
		
		struct LinkedList *temp;
		temp = head;
		while(1)
		{
			//printf("Value:  %d  and Pointer : %p \n", temp->LLvalue, temp);
			if(temp->nextLL == NULL)
				break;
			temp = temp->nextLL;
		}
	}
}

int IsLRUFull ()
{
		 LinkedList *b =NULL;
		 int returner = 0 ;

		 if(head == NULL)
		 {
		 	return -1; ////Change this
		 }

		if(head->nextLL != NULL)
		{
			//printf("LRU IS FULL\n");
			b = tail;
			b->prevLL->nextLL = NULL;
			tail = b->prevLL;
			tail->nextLL = NULL;
			Hash_Remove(b->LLvalue);
			return b->LLvalue;
		}
		else
		{
			//printf("2  LRU IS FULL\n");
			Hash_Remove(head->LLvalue);
			int temp = head->LLvalue;
			head = NULL;
			return temp;
				///Still working
		}
			return returner;
}

int	LRU_Size()	
{
	//printf("Total element in LRU are %d \n", MaxSizeCounter_2);
	//	printf("bluh and totalBuckets is: %d \n", MaxSizeCounter_2 - totalBuckets) );

	return MaxSizeCounter_2;
}
  
int	LRU_Remove(int element)	
{
	pthread_mutex_lock(&lock);
	if( Hash_AlreadyExists(element % totalBuckets, element) == 0)
	{
		//printf("Element does not exist int he hash table\n");
		pthread_mutex_unlock(&lock);
		return -1;
	}
	else
	{
		struct LinkedList *a;
		a = AnotherPointer;
		//printf(" dereferencing %d \n", AnotherPointer->LLvalue);

		if( a == head)
		{
			if( head->nextLL == NULL)
			{
				head =NULL;
				Hash_Remove(element);
			}
			else
			{
				head = head->nextLL;
				head->prevLL = NULL;
				Hash_Remove(element);
			}
		}
		else
		{
			if( a->nextLL == NULL)
			{
				tail = a->prevLL;
				tail->nextLL = NULL;
			}
			else
			{
				a->prevLL->nextLL = a->nextLL;
				a->nextLL->prevLL = a ->prevLL;
			}
		}
	}
	pthread_mutex_unlock(&lock);
	return 0;
}
  

int LRU_Access(int element)
{
	pthread_mutex_lock(&lock);

	if( Hash_AlreadyExists(element % totalBuckets, element) == 0)
	{
		//printf("Element does not exist int he hash table\n");
		pthread_mutex_unlock(&lock);
		return -1;
	}
	else
	{
		struct LinkedList *c;
		c = AnotherPointer;

		if( c == head){}
			//printf("Done Nothing\n");

		else 
		{
			if( c == tail)
			{
				//printf("Brambey\n");
				tail = c->prevLL;
				c->prevLL->nextLL = NULL;
				c->nextLL =head;
				head->prevLL = c;
				head = c;
			}
			else
			{
				c->prevLL->nextLL = c->nextLL;
				c->nextLL->prevLL = c->prevLL;
				c->nextLL =head;
				head->prevLL =c;
				head = c;
			}
		}
	}
	pthread_mutex_unlock(&lock);
	return 0;
}	
  	
  













///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////LRU Functions END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////
///MEMCHECK
///Used For debuggin purposes. Do not use
///
///
///////////////////////////////////////////////
void MemCheck() 
{
	int a;
	for (a =0; a < totalBuckets; a++)
	{
		//printf("Mem Location %d Table Address: %p Chain Address: %p \n", a, &MyTable[a], &(MyTable[a].chain));
	}

}

////////////////////////////////////////////////
///HASH INIT
///Initiatilzes the Hash Table to the size of 
///numOfBuckets
///
///////////////////////////////////////////////
int Hash_Init( int numOfBuckets)
{
	//pthread_mutex_lock(&lock);
	int i;
	//printf("Initializing your HashTable with %d elements\n", numOfBuckets);
	if( numOfBuckets < 1)
	 {
		//printf("Incorrect HashTable size set - EXITING");
		//pthread_mutex_unlock(&lock);
		return 0;
	 }		

	MyTable = malloc(numOfBuckets * sizeof(struct TableElement));
	
	 if ( MyTable == NULL)
	 {
	 	//printf("Could not Resize Array");
		//pthread_mutex_unlock(&lock);
	 	return 0;
	 }	
	 totalBuckets = numOfBuckets;
	//pthread_mutex_unlock(&lock);
	return 1;
	
}


////////////////////////////////////////////////
///HASHINSERT
///Inserts a interger in the hash table. 
///If the number already exists, then doe not insert and returns -1 ( else returns 0)
///
///////////////////////////////////////////////
int Hash_Insert(int x, LinkedList *TempPointer)
{
	//pthread_mutex_lock(&lock);
	int index;
	index = x % totalBuckets;
	
	//Temp Node creation
	struct HashNode *temp = malloc( 1 * sizeof(HashNode));
	struct HashNode *temp2 =  malloc( 1 * sizeof(HashNode));
	void *ptr;
	ptr = MyTable[index].chain;
	
	 if(MyTable[index].chain != NULL)
	 {	
	 	(*temp2).value = x;
	 	(*temp2).Pointer = TempPointer;
	 	MyTable[index].chain = &(*temp2); 
	 	(*temp2).next = ptr;
		(*temp2).prev = (HashNode*)&MyTable[index].chain;
		(*temp2).next->prev = &(*temp2);	 	
	 }
	if(MyTable[index].chain == NULL)
	{
		(*temp).value = x;
		(*temp).Pointer = TempPointer;
		(*temp).next =NULL;
		(*temp).prev = (HashNode*) &MyTable[index].chain;
		MyTable[index].chain = &(*temp);
	}
	//pthread_mutex_unlock(&lock);
	return 0; //On successful insertion
}


////////////////////////////////////////////////
///HASHREMOVE
///Removes a number from the Hash Table
///If the number does not exist ~ Returns 0
///Else Returns 1
///////////////////////////////////////////////
int	Hash_Remove(int  x)
{
	//pthread_mutex_lock(&lock);
	int index = x % totalBuckets;
	//printf("Trying to remove %d from HashTable\n", x);
	if(MyTable[index].chain == NULL)
	{
		//printf("%d does not exist in the hashtable", x);
		//pthread_mutex_unlock(&lock);
		return -1;					//////////////Return -1 If element does not exist in the hash table
	}
	if(MyTable[index].chain != NULL)
			{
				/////////printf("In Hash Delete\n");
				struct HashNode *temp3;
				temp3 =MyTable[index].chain;
				//////////printf("Index Chain is %p \n", MyTable[index].chain);
				 while(1)
				 {
				 	if(temp3->value == x)
				 	{			
				 		//////////If another element exists
				 		if(temp3->next == NULL)
				 		{
				 			if(temp3 == MyTable[index].chain)
				 			{
				 				MyTable[index].chain = NULL;
				 			}
				 			else
				 			{
				 				temp3->prev->next = NULL;
				 				temp3->prev = NULL;
				 			}
				 		}
				 		//////////If another element does NOT exists
				 		else
				 		{
				 			if(temp3 == MyTable[index].chain)
				 			{
				 		//////////		//printf("In Hash Remover\n");
				 				MyTable[index].chain = MyTable[index].chain->next;
				 				MyTable[index].chain-> prev = NULL;
				 			}
				 			//Need to add some stuff here
				 			else
				 			{
				 		/////////		//printf("In Null Remover\n");
				 				temp3->prev->next = temp3->next;
				 				temp3->next->prev = temp3->prev;
				 			}
				 		}
						//pthread_mutex_unlock(&lock);
				 		return 0;			//////////////Return 0  If element EXISTS in the hash table
				 	}
				 	if(temp3->next == NULL)
				 		break;
				 	
				 	temp3 = temp3->next;
				 }
				//pthread_mutex_unlock(&lock);
				 return -1;  //////////////Return -1 If element does not exist in the hash table
			}		
}




////////////////////////////////////////////////
///HASHDUMP
///To display the HashTable to the User
///FORMAT:
////
///Index,(space)Memory Location
///(space)values
///////////////////////////////////////////////
void Hash_Dump()	
{
	int i; 

	for(i=0; i<totalBuckets; i++)
	{		
		//printf("%d, %p\n", i, MyTable[i].chain);

		struct LinkedList *temp4;

		if(MyTable[i].chain != NULL)
		{

			struct HashNode *temp3;
			//printf("LLValue 56 is: %d \n", head->LLvalue);
			temp3 = MyTable[i].chain;
			//printf("LLValue 57 is: %d \n", head->LLvalue);

			 while(1)
			 {		 	

			 	//printf(" Value is: %d  ", temp3->value);
			 	
			 	if(temp3->next == NULL)
			 		break;
			 	temp3 = temp3->next;
			 }
			 //printf(" \n");
		}		
	}
	//printf("Hash Address  is %p\n", MyTable[0].chain);
}

////////////////////////////////////////////////
///COUNTELEMENTS
///Counts the total number of elements in the Hash Table
///////////////////////////////////////////////
int	Hash_CountElements()	
{
	int i, count = 0; 
	printf("bluh and totalBuckets is: %d \n", totalBuckets);

	for(i=0; i<totalBuckets; i++)
	{		
		if(MyTable[i].chain != NULL);
		{
			count++;
			struct HashNode *temp3;
			temp3 =MyTable[i].chain;
			printf("2bluh\n");
			 while(1)
			 {
			 	printf("Im NOt in here\n");

			 	if(temp3->next == NULL)
			 		break;

			 	//printf("3bluh\n");
			 	count++;
			 	temp3 = temp3->next;
			 }
		}		
	}
	return count;
}
  
///////////////////////////////////////////////
///COUNTKETELEMENTS
///Counts the total number of elements in he specified Bucket
/////////////////////////////////////////////// 

int	Hash_CountBucketElements(int bucketNumber)
{
	pthread_mutex_lock(&lock);
	int count = 0;
	if(MyTable[bucketNumber].chain != NULL)
		{
			count++;
			struct HashNode *temp3;
			temp3 =MyTable[bucketNumber].chain;
			 while(1)
			 {
			 	if(temp3->next == NULL)
			 		break;
			 	count++;
			 	temp3 = temp3->next;
			 }
		}	
		else
		{
			pthread_mutex_unlock(&lock);
			return 0;
		}	
	pthread_mutex_unlock(&lock);
	return count;	
}

  
///////////////////////////////////////////////
///ALREADYEXISTS
///Called by Hash_Insert and Hash_Remove
////Checks that if a value already exists
/////////////////////////////////////////////// 

int Hash_AlreadyExists( int bucketNumber, int x)
{
	if(MyTable[bucketNumber].chain != NULL)
		{
			struct HashNode *temp3;
			temp3 =MyTable[bucketNumber].chain;
			while(1)
			 {			 	
			 	if(temp3->value == x)
			 	{
			 		AnotherPointer = temp3->Pointer;
			 		return -1;
			 	}
			 	if(temp3->next == NULL)
			 	{
			 		break;
			 	}
			 	temp3 = temp3->next;
			 }
		}	
		else
		{
			return 0;
		}	
		//pthread_mutex_unlock(&lock);
		return 0;
}
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////
//////////////////////////END
////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
