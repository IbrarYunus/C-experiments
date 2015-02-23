/////////////////////////////////////////
/////////////HASHING FUNCTION
//////////*****IBRAR YUNUS*********
////////////v0.1 Multi-thread Safe
/////////////// v0.1 ~ Sun Nov 23 12:06 AM
///////////////////Compiled ~ Elementary OS Luna ~ GCC
////////////////////////////////////////


#include<stdio.h>
#include<stdlib.h>
////////////////////Definitions
int Hash_Insert(int numOfBuckets);
int Hash_Insert(int x);
void Hash_Dump();	
void MemCheck();
int	Hash_CountElements();	
int	Hash_CountBucketElements(int bucketNumber);



//////STRUCTS 
typedef struct HashNode {
		int value;
		struct HashNode *next;	//Pointer to the next LL Node
		struct HashNode *prev;
} HashNode;

typedef struct TableElement {
		struct HashNode *chain; //for chaining
		pthread_mutex_t lock;
}TableElement;	


///Globals
struct TableElement *MyTable;
int totalBuckets;




////////////////////////////////////////////////
///MAIN
///Used For debuggin purposes. Do not use
///
///
///////////////////////////////////////////////
/*int main (void)
{
Hash_Init(60);
Hash_Insert(230958);
Hash_Insert(2593);
Hash_Insert(342);
Hash_Insert(18);
Hash_Insert(2);
Hash_Insert(1);
Hash_Insert(78);
Hash_Insert(78);
Hash_Dump();	
//printf("Total Elements: %d\n", Hash_CountElements());
//printf("Number of Elements in bucket 18: %d \n", Hash_CountBucketElements(18));

if(Hash_Remove(2) == -1 )
// printf("DOES NOT EXIST IN THE HASH TABLE\n");  
if(Hash_Remove(18) == -1 )
// printf("DOES NOT EXIST IN THE HASH TABLE\n"); 
if(Hash_Remove(61) == -1 )
// printf("61  DOES NOT EXIST IN THE HASH TABLE\n"); 

Hash_Dump();	
//printf("Total Elements: %d\n", Hash_CountElements());
//printf("Number of Elements in bucket 18: %d \n", Hash_CountBucketElements(18));
}
*///main ending
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
	int i=0;
	//printf("Initializing your HashTable with %d elements\n", numOfBuckets);
	if( numOfBuckets < 1)
	 {
		//printf("Incorrect HashTable size set - EXITING");
		return 0;
	 }		
	MyTable = malloc(numOfBuckets * sizeof(struct TableElement));
	while(i<numOfBuckets){
		pthread_mutex_init(&MyTable[i].lock,NULL);//initilized all the locks successfullly
		i++;		
	}
	 if ( MyTable == NULL)
	 {
	 	//printf("Could not Resize Array");
	 	return 0;
	 }	
	 totalBuckets = numOfBuckets;
}


////////////////////////////////////////////////
///HASHINSERT
///Inserts a interger in the hash table. 
///If the number already exists, then doe not insert and returns -1 ( else returns 0)
///
///////////////////////////////////////////////
int Hash_Insert(int x)
{
	
	int index;
	//printf("Inserting %d into HashTable\n", x);
	index = x % totalBuckets;
	pthread_mutex_lock(&MyTable[index].lock);
	 if(Hash_AlreadyExists(index, x) == -1)
	 {
	 	//printf(" %d Already Exists\n", x);
		pthread_mutex_unlock(&MyTable[index].lock);
	 	return -1;						///////////If an elements already Exists, then do not reinsert it and Return -1
	 }
	///////printf("Index is: %d \n", index);
	/////////Temp Node creation
	struct HashNode *temp = malloc( 1 * sizeof(HashNode));
	struct HashNode *temp2 =  malloc( 1 * sizeof(HashNode));
	void *ptr;
	ptr = MyTable[index].chain;
	
	 if(MyTable[index].chain != NULL)
	 {
	 	//printf("Inserting at Old and chain pointer is %p\n", MyTable[index].chain);	 	
	 	(*temp2).value = x;
	 	MyTable[index].chain = &(*temp2); 
	 	(*temp2).next = ptr;
		(*temp2).prev = (HashNode*) &MyTable[index].chain;
		(*temp2).next->prev = &(*temp2);
	 	//printf("The next element is %p\n", temp.next);
	 	//printf("The current element is %d\n", MyTable[index].chain->next->prev->value);
	 }
	if(MyTable[index].chain == NULL)
	{
		//printf("Inserting at New and chain pointer is %p\n", MyTable[index].chain);
		(*temp).value = x;
		(*temp).next =NULL;
		(*temp).prev = (HashNode*) &MyTable[index].chain;

		MyTable[index].chain = &(*temp);
	}

	// free(temp);
	// free(temp2);
	//Debugger();
	pthread_mutex_unlock(&MyTable[index].lock);
	return 0;						///////////On successful insertion
}


////////////////////////////////////////////////
///HASHREMOVE
///Removes a number from the Hash Table
///If the number does not exist ~ Returns 0
///Else Returns 1
///////////////////////////////////////////////
int	Hash_Remove(int  x)
{
	int index = x % totalBuckets;
	pthread_mutex_lock(&MyTable[index].lock);
	//printf("Trying to remove %d from HashTable\n", x);
	if(MyTable[index].chain == NULL)
	{
		//printf("%d does not exist in the hashtable", x);
		pthread_mutex_unlock(&MyTable[index].lock);
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
						pthread_mutex_unlock(&MyTable[index].lock);
				 		return 0;			//////////////Return 0  If element EXISTS in the hash table
				 	}
				 	if(temp3->next == NULL)
				 		break;
				 	
				 	temp3 = temp3->next;
				 }
				pthread_mutex_unlock(&MyTable[index].lock);
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
		if(MyTable[i].chain != NULL)
		{
			struct HashNode *temp3;
			temp3 =MyTable[i].chain;
			//printf("Temp3 is %p", temp3);
			 while(1)
			 {
			 	//printf("  %d  ", temp3->value);
			 	if(temp3->next == NULL)
			 		break;
			 	temp3 = temp3->next;
			 }
			 //printf(" \n");
		}		
	}

}

////////////////////////////////////////////////
///COUNTELEMENTS
///Counts the total number of elements in the Hash Table
///////////////////////////////////////////////
int	Hash_CountElements()	
{
	int i, count = 0; 
	for(i=0; i<totalBuckets; i++)
	{		
		if(MyTable[i].chain != NULL)
		{
			count++;
			struct HashNode *temp3;
			temp3 =MyTable[i].chain;
			 while(1)
			 {
			 	if(temp3->next == NULL)
			 		break;
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
			return 0;
		}	
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
			 	if(temp3->next == NULL)
			 		break;
			 	if(temp3->value == x)
			 		return -1;
			 	temp3 = temp3->next;
			 }
		}	
		else
		{
			return 0;
		}	

}


////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////
//////////////////////////END
////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
