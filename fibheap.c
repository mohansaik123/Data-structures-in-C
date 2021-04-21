#include <math.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <limits.h>

//  structure to represent a node in the heap 
typedef struct fibnode { 
	struct fibnode* parent; // Parent pointer 
	struct fibnode* child; // Child pointer 
	struct fibnode* prev; // Pointer to the node on the prev 
	struct fibnode* next; // Pointer to the node on the next 
	int key; // Value of the node 
	int degree; // Degree of the node 
	int mark; // 1 for true or 0 for false mark of the node
	int visited; //flag for assisting  search and level order 1 for visited 0 for not visited
}fibnode; 

// structure to represent the heap
typedef struct fibheap{
	fibnode *mini;// mini which points to to minimum key in the root list
	int noOfNodes;// keeps tarck of no of nodes present in the heap
}fibheap;
//function to make a haep
fibheap makeheap(){
	fibheap fh;
	fh.mini=NULL;
	fh.noOfNodes=0;
	return fh;
}

// Function to build the node
fibnode *construct(int val) 
{ 
	fibnode* temp = (struct fibnode*)malloc(sizeof(struct fibnode)); 
	temp->key = val; 
	temp->degree = 0; 
	temp->mark = 0; 
	temp->visited=0;
	temp->parent = NULL; 
	temp->child = NULL; 
	temp->prev = temp; 
	temp->next = temp; 
	return temp;
	} 
// function to union two heaps 
fibheap fibunion(fibheap fh,fibheap fh1){
	//if both the heps are empty return
	if(fh.mini==NULL&&fh1.mini==NULL) return;
	//if 1st heap is emty return mini if 2nd heap
	else if(fh.mini==NULL) return fh1;
	//viceversa
	else if(fh1.mini==NULL) return fh;
	//if mini of fh is not null concatinate the root list
	if (fh.mini != NULL) { 
		// add the root list in circular doubly linked list manner
		(fh.mini->prev)->next = fh1.mini; //point next of previous node of fh min to fh1 min
		(fh1.mini->prev)->next = fh.mini; //point fh1 min's next to fh min
		fibnode *temp=fh1.mini->prev;// copy the fh1 min prev to to temp node 
		fh1.mini->prev = fh.mini->prev; // point fh1 min's prev to fh min' prev
		fh.mini->prev = temp; //point prev of fh min to temp 
		//if the value of fh1 min is less than fh min change the min pointer of fh to point the minimum value
		if (fh1.mini->key < fh.mini->key) 
			fh.mini = fh1.mini; 
	} 

	// add the node count  	 
	fh.noOfNodes=fh.noOfNodes+fh1.noOfNodes;
	return fh;
	
}
// function to insert a node into the heap;
fibheap insert(int key, fibheap fh){
	fibheap temp=makeheap();//create a temporary heap
	temp.mini=construct(key);//add the node to be inserted into the heap with given value
	temp.noOfNodes=1;// no of nodes are 1
	fh=fibunion(fh,temp);//union main heap and temp heap
	return fh;//return main heap
}


// function to Link the heap nodA	es into  parent and child  
void fiblink(fibnode* y, fibnode* x, fibheap *fh) // y as child and x as parent
{ 	//remove y from the root list
	(y->prev)->next = y->next;// point next of previous node of y to y's next 
	(y->next)->prev = y->prev; // viceversa
	//if the root list has single node make it as min
	if (x->next == x) 
		fh->mini = x; 
	// point y to itself
	y->prev = y; 
	y->next = y; 
	y->parent = x; // make x as y's parent
	// if x child is null point y as x's child
	if (x->child == NULL) 
		x->child = y; 
	// or adjust the y node in x's child list
	else{
	
		y->next = x->child; 
		y->prev = (x->child)->prev; 
		((x->child)->prev)->next = y; 
		(x->child)->prev = y; 
		//if the key of y is less than x's current child key
		if (y->key < (x->child)->key) 
			x->child = y; //point x's child pointer to y
 	}
	x->degree++; // increase degree of x
	y->mark=0;// no children were cut from y mark is 0 
} 
// function to consolidate the heap
void Consolidate(fibheap* fh) 
{ 
	int d; 
	float temp= (log(fh->noOfNodes)) / (log(1.618)); //caliculate upperbound of no of nodes here 1.618 is the golden ratio 
	int D = temp;// take the int value of the temp 
	fibnode* arr[D];// array of nodes to point the the same degree nodes
	int i; 
	for ( i = 0; i <= D; i++) // make all the nodes as null
		arr[i] = NULL; 
	fibnode* x = fh->mini; 
	fibnode* y; 
	fibnode* swap; 
	fibnode* w = x; // w is the pointer to the root list
	do { 
		w = w->next; 
		d = x->degree;// d is the degree of x 
		while (arr[d] != NULL) { 
			y = arr[d]; //take a node of same degree 
			// if key of y is less than x then exchange the nodes
			if (x->key > y->key) { 
				swap = x; 
				x = y; 
				y = swap; 
			} 
			//if y is mini then chnage the mini to point to x
			if (y == fh->mini) 
				fh->mini = x; 
			//link the x and y into parent and child
			fiblink(y, x, fh); 
			//if the root list has single node then point mini to x
			if (x->next == x) 
				fh->mini = x; 
			// make arr[d] as NULL as x degrree has been increased
			arr[d] = NULL; 
			//increase d value
			d++; 
		} 
		arr[d] = x; // add x to the arr
		x = x->next; 
	} while (w != fh->mini); 
	fh->mini = NULL; // point the mini to NULL as the nodes have to be readjusted
	int j;
	//traverse throgh the whole array
	for (j = 0; j <= D; j++) { 
	//if array is not empty
		if (arr[j] != NULL) { 
			// point the array to itself
			arr[j]->prev = arr[j]; 
			arr[j]->next = arr[j]; 
			//if fh mini is not null add the node to the root list
			if (fh->mini != NULL) { 
				(fh->mini->prev)->next = arr[j]; 
				arr[j]->next = fh->mini; 
				arr[j]->prev = fh->mini->prev; 
				fh->mini->prev = arr[j]; 
				//if the array key is less than mini key then change the mini pointer 
				if (arr[j]->key < fh->mini->key) 
					fh->mini = arr[j]; 
			} 
			// point mini to array
			else { 
				fh->mini = arr[j]; 
			}  
		} 
	} 
} 

// function to find minimum node in the heap 

fibnode *findmin(fibnode *root){
	fibnode *min,*next1;
	//if heap is null return NULL
	if(root==NULL){
		return NULL;
	}
	
	else{
		min=root;//make root as minimum
		//if next of of root is not root
		next1=root;
		if(root->next!=root)
			next1=root->next;
		//traverse through the root list
		while(next1!=root){
			//if key of minimum is greater than next 
			if(min->key>next1->key){
				min=next1;//make minimum as next
			}
			next1=next1->next;
		}
		
		return min;//return minimum node
	}
}
//function to extract minimum from the heap
fibheap *extractmin(fibheap *fh) 
{ 	//if the heap is empty return 
	if (fh->mini == NULL){
	
		printf( "The heap is empty\n" );
		return; 
	}
	
	else { 
		fibnode* temp = fh->mini; // point temp to mini
		fibnode* temp1; 
		temp1 = temp; 
		fibnode* x = NULL;
 		// if the mini has child then re adjust the links
		if (temp->child != NULL) { 
			// take x as temp's child and bring it to the root list
			x = temp->child; 
			// traverse through the child list and bring all the children to root list
			do { 
				temp1 = x->next; 
				// add x to the root list
				((fh->mini)->prev)->next = x; 
				x->next = fh->mini; 
				x->prev = (fh->mini)->prev; 
				fh->mini->prev = x; 
				//if key of x is less than cuurent mini's key then change mini pointer
				if (x->key < (fh->mini)->key) 
					fh->mini = x; 
				// make parent pointer of x as NULL
				x->parent = NULL; 
				//go to the next child
				x = temp1; 
			} while (temp1 != temp->child);// until the whole child list is brought into the root list 
		} 
		//remove the node from the root list
		(temp->prev)->next = temp->next; 
		(temp->next)->prev = temp->prev; 
		fh->mini = temp->next; // point the mini to another node in the root list
		//if there is a single node in the list then make mini as NULL
		if (temp == temp->next && temp->child == NULL) 
			fh->mini = NULL; 
		else { 
			fh->mini = temp->next; 
			Consolidate(fh); // call the consolidate function to re arrange the heap
		} 
		fh->noOfNodes--; // decreas the no od nodes count
		free(temp);
	} 
	return fh;
} 

// funtion to cut a node in the heap to be placed in the root list 
void Cut(fibnode* found, fibnode* parnt, fibheap *fh) // parnt is found's parent
{	//if there is a single node in the list make temp child as NULLL
	if (found == found->next) 
		parnt->child = NULL; 
//readjust the list
	(found->prev)->next = found->next; 
	(found->next)->prev = found->prev; 
	// if found is child pointed by parnt make found's next to be pointed
	if (found == parnt->child) 
		parnt->child = found->next; 
//decrease degree of of parent 
	parnt->degree = parnt->degree - 1; 
	//remove found from the list
	found->next = found; 
	found->prev = found; 
	//bring the found node to root list and readjust the list
	(fh->mini->prev)->next = found; 
	found->next = fh->mini; 
	found->prev = (fh->mini)->prev; 
	(fh->mini)->prev = found; 
	found->parent = NULL; // point found's parent to NULL
	found->mark = 0; // 0 as no child was cut from found
} 

//  cascade cutting function 
void Cascade_cut(fibnode* y,fibheap *fh) 
{ 
	fibnode *z = y->parent; 
	if (z != NULL) { 
		// mark the field as a child was cut from it
		if (y->mark == 0) { 
			y->mark = 1; 
		} 
		//if not perform a cut and mark it 
		else { 
			Cut(y, z,fh); 
			Cascade_cut(z,fh); 
		} 
	} 
} 
// function to search a node with given value
fibnode* search(fibnode *mini, int val) 
{ 
    fibnode* found = NULL;  // pointer to point the required node
    // if the node is NULL return NULL
    if(mini==NULL){
    	return NULL;
	}
	mini->visited = 1; // visted is 1 when a node is visited
    if (mini->key == val) { 
        found= mini; 
        mini->visited = 0;  // if found make the visted to zero
        return found; 
    } 
    // if the node is not found traverse through the heap recursively
    if (found == NULL) { 
    	// if the node has child search the child  
        if (mini->child != NULL) 
            found=search(mini->child, val); 
        // if the node is not found in the child node then search the list
        if ((mini->next)->visited != 1&&found==NULL) 
            found=search(mini->next, val);
    } 
    mini->visited = 0;// at the end make visited as zero if the nodes traversed doesnt have the key at 
    return found;
} 

// function to decarese key of a node in the heap
fibheap *decreasekey(int val,int oldval, fibheap *fh) 
{ 
	// check if the node is empty or not
	if (fh->mini == NULL){
	
		printf("The Heap is Empty\n" );
		return NULL; 
	}

	fibnode *found,*temp1;
	temp1=fh->mini;
	// search for the value to be decreased in the heap 
	found=search(temp1,oldval);

	// if the node is not found then return
	if (found == NULL) {
		printf("Node not found in the Heap\n");
		return fh; 
	}
	// update the key of found node
	found->key = val; 
	printf("key updated \n");
	fibnode* temp = found->parent; // point temp to found's parent
	// if the new key is less than parents key then readjust the heap
	if (temp != NULL && found->key < temp->key) { 
		Cut(found, temp,fh); 
		Cascade_cut(temp,fh); 
	} 
	// if the key is less than current mini's key then change mini
	if (found->key < (fh->mini)->key) 
		fh->mini = found;
	return fh; 
} 


// function to delete a node from the 
void delete(int val, fibheap *fh) 
{ // check if the heap is empty
	if (fh->mini == NULL) 
		printf("The heap is empty\n"); 
	else { 

		// decrease the value of the node to INT_MIN 
		decreasekey(INT_MIN,val,fh); 
		//if min value is INT_min extract it
		
		if(fh->mini->key==INT_MIN){
		
			extractmin(fh); 
			printf("Key Deleted\n");
			
		}
	} 
} 

// function to display rootlist the heap 
void displayRoot(fibheap *fh) 
{ 
	fibnode* ptr = fh->mini; 
	if (ptr == NULL) 
		printf("The Heap is Empty\n"); 

	else { 
		printf("The root list of the heap \n"); 
		
		do { 
			printf(" %d",ptr->key); 
			ptr = ptr->next; 
			if (ptr != fh->mini) { 
				printf ("-->"); 
			} 
		} while (ptr != fh->mini && ptr->next != NULL); 
		printf("\n"); 
			printf("The heap has %d nodes \n",fh->noOfNodes);
			printf("\n");
	} 
} 
void display(fibnode *root){
	//traverse through the heap
	fibnode *temp=root;
	if(temp!=NULL){
	
		do{
	        printf("%d ",temp->key); 
	        display(temp->child);//display children of head 
	        temp= temp->next; //shift the head pointer to it's sibling
	    }while(temp!=root); 
}
}
//utility function for level order to make viisted as zero after completion of traversal
void visted(fibnode *mini){
	//traverse through the heap
	fibnode *temp=mini;
	if(temp!=NULL){
	
		do 
	    { 
	        temp->visited=0; //make visted as zero
	        if(temp->child!=NULL)
				visted(temp->child);//make children to zero
	        temp= temp->next; //shift the head pointer to it's sibling
	    }while (temp!=mini); 
	}	
}

struct fibnode** createQueue(int *front, int *rear) 
{ 
	struct fibnode **queue =(struct fibnode **)malloc(sizeof(struct fibnode*)*500); 

	*front = *rear = 0; //intially the que is empty
	return queue; 
} 
//function to enque
void enQueue(struct fibnode **queue, int *rear, struct fibnode *new_node) 
{ 
	queue[*rear] = new_node;//add the node at the end of que
	(*rear)++; 
} 
//function to deque and to return the dequed node
struct fibnode *deQueue(struct fibnode **queue, int *front) 
{ 
	(*front)++; 
	return queue[*front - 1];//return the fist node 
} 


//function to print the tree in level order
void printLevelOrder(struct fibnode *mini ) 
{ 	if(mini==NULL){
		printf("\nthe heap is empty\n");
		return;
}
	int rear, front, size=0,count=0; //count is for keeping track of no of nodes enqued in each level
	//size is for keeping track of no of nodes present in the que for a particular level 
	struct fibnode **queue = createQueue(&front, &rear); 
	struct fibnode *temp, *head;
	head=mini;
	// to keep track of root list level
	
	do//enque the root list 
    {	enQueue(queue, &rear, mini);//enque the root
		count++;
		mini->visited=1;
	
	mini=mini->next;
	}while(mini!=head);
	while(1) 
	{ 	size=count; //the size of ques is no of nodes enqued
		if(size==0){
			break;
		}		 
		while(size>0){
			temp = deQueue(queue, &front);//deque the node
			int i;
			//print  the key present in the current node
			
				printf("  %d ", temp->key);
				if(temp->parent!=NULL){
					printf("  and its parent is %d    ", temp->parent->key);
				}

			count--;
			//enque the children of current node
        		if(temp->child!=NULL&&temp->child->visited==0) {
					enQueue(queue,&rear,temp->child);
					 
					 temp=temp->child;//make child node as temp
					 temp->visited=1;
					count++;
				}
				while(temp->next->visited==0) 
            	{
					enQueue(queue, &rear, temp->next); 
					count++;
					temp=temp->next;
					temp->visited=1;//make 1 as th node is visted
				}
		
			
			size--;//reduce the size after printing a node if size becomes zero then a level is completed
			//make flag as zero when the root list level is completed
		}
		printf("\n");
 
	}
	visted(head);
	free(queue); 
}

// Driver code 
int main() 
{ 	fibheap fh;
	fh=makeheap();
	int key,i, oldkey;
	fibnode *temp;
	while(1){
	
		printf("\n\n1. Insertion of key\n2. search the key\n3. find minimum\n4. extract minimum\n5. decrease key\n6. delete key\n7. display keys\n8. Level order traversal\n9. Display Rootlist\n10. exit\n ");
		printf("\nenter choice\n");
		scanf("%d",&i); 
		switch(i){
			case 1:	printf("Enter  key or enter -1 for no data\n");
					scanf("%d",&key);
					while(key!=-1){
		
		
						fh=insert(key,fh);
		
			
						printf("\nenter next key or enter -1 for no data: ");
						scanf("%d",&key);
						
					}
					break;
			case 2: printf("\nenter the key to be searched\n");
					scanf("%d",&key);
					temp=search(fh.mini,key);
							
					if(temp==NULL){
						printf("\n key not found");
					
					}
					else{
						printf("\nthe value enetred is in the heap and the value is %d",temp->key);
						 
					}
					break;
			case 3: printf("\n minimum key is\n");
					temp=findmin(fh.mini);
					printf("%d",temp->key);
	
					break;
			case 4: extractmin(&fh);
					
					break;
			case 5: printf("\n enter the key to be decreased\n");
					scanf("%d",&oldkey);
					printf("\n enter the new value of the key\n");
					scanf("%d",&key);
					decreasekey(key,oldkey,&fh);
					break;
			case 6: printf("\n enter the key to be deleted\n");
					scanf("%d",&key);
					delete(key,&fh);
					break;
			case 7: printf("\n displaying the keys present in heap\n");
					display(fh.mini);
					break;
			
			case 8: printLevelOrder(fh.mini);
					break;
			case 9: printf("\n displaying the root list\n");
					displayRoot(&fh);
					break;
			case 10:	exit(0);
			
		}
	}
	
return 0;
}
	

