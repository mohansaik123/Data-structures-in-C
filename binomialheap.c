#include<stdio.h>
#include<stdlib.h>
//define the node strucrure
typedef struct heapnode{
	struct heapnode *parent;
	struct heapnode *sibling;
	struct heapnode *child;
	int key;
	int visited;//to mark whetherthe node is visted or not in level order traversal 
	int degree;
}heapnode;
//define the tree structure
typedef struct bheap{
	heapnode *head;
}bheap;
//function to contsruct a node with given key value
heapnode *construct(int key){
	heapnode *temp=(heapnode*)malloc(sizeof(heapnode));
	temp->parent=NULL;
	temp->child=NULL;
	temp->sibling=NULL;
	temp->key=key;
	temp->visited=0;// for level order 0 for not visted 1 for visted 
	temp->degree=0;
	return temp;
}
//function to create a empty  binomialheap 
bheap makeheap(){
	bheap bh;
	bh.head=NULL;//make head as NULL
	return bh;
}

heapnode *bunion(bheap h1, bheap h2);
heapnode *merge(bheap h1, bheap h2);
heapnode *link(heapnode *y, heapnode *z);
heapnode *findmin(heapnode *head);
heapnode *removeroot(heapnode *root,heapnode *prev,heapnode *head);
heapnode *extractmin(heapnode *head);
heapnode *search(int key,heapnode *head);
heapnode *decreasekey(heapnode *head, int oldkey, int newkey);
void display(heapnode *head);

//function to insert a node into heap
bheap insert(int key, bheap bh){
	bheap temp=makeheap();//create a temporary heap
	temp.head=construct(key);//add the node to be inserted into the heap
	bh.head=bunion(bh,temp);//union main heap and temp heap
	return bh;//return main heap
}
//function to delete a key from the heap
bheap deletekey(bheap bh,int key){
	//return null if the heap is eampty
	if(bh.head==NULL){
		return;
	}
	//call the function decrease key wich replaces old key value new key which is less than old key 
	bh.head=decreasekey(bh.head,key,INT_MIN);//INT-MIN=-2147483648
	bh.head=extractmin(bh.head);//deletes the minimum node from the heap
	return bh;//return the main heap
	
}

//utility function to make visted to "zero" after executing level order traversal
void visted(heapnode *head){
	//traverse through the heap
	while (head!=NULL) 
    { 
        head->visited=0; //make visted as zero
        visted(head->child);//make children to zero
        head= head->sibling; //shift the head pointer to it's sibling
    } 
}

//function to create a queue
struct heapnode** createQueue(int *front, int *rear) 
{ 
	struct heapnode **queue =(struct heapnode **)malloc(sizeof(struct heapnode*)*500); 

	*front = *rear = 0; //intially the que is empty
	return queue; 
} 
//function to enque
void enQueue(struct heapnode **queue, int *rear, struct heapnode *new_node) 
{ 
	queue[*rear] = new_node;//add the node at the end of que
	(*rear)++; 
} 
//function to deque and to return the dequed node
struct heapnode *deQueue(struct heapnode **queue, int *front) 
{ 
	(*front)++; 
	return queue[*front - 1];//return the fist node 
} 


//function to print the tree in level order
void printLevelOrder(struct heapnode *root ) 
{ 
	int rear, front, size=0,count=0; //count is for keeping track of no of nodes enqued in each level
	//size is for keeping track of no of nodes present in the que for a particular level 
	struct heapnode **queue = createQueue(&front, &rear); 
	struct heapnode *temp, *head;
	head=root;
	// to keep track of root list level
	enQueue(queue, &rear, root);//enque the root
	count++;
	root->visited=1;
	while(root->sibling!=NULL)//enque the root list 
            	{
					enQueue(queue, &rear, root->sibling); 
					count++;
					
					root->sibling->visited=1;//make visted 1 as the node is visted
					root=root->sibling;
				}
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
        		if(temp->child!=NULL) {
					enQueue(queue,&rear,temp->child);
					 temp=temp->child;//make child node as temp
					temp->visited=1;
					count++;
				}
				while(temp->sibling!=NULL&&temp->sibling->visited==0) 
            	{
					enQueue(queue, &rear, temp->sibling); 
					count++;
					temp=temp->sibling;
					temp->visited=1;//make 1 as th node is visted
				}
		
			
			size--;//reduce the size after printing a node if size becomes zero then a level is completed
		}
		printf("\n");
 
	}
	visted(head);
	free(queue); 
}

int main(){
	bheap bh;
	heapnode *temp;
	bh=makeheap();//create the main heap
	int key,i, oldkey;
	/*bh=insert(10,bh);
	bh=insert(9,bh);
	bh=insert(8,bh);
	bh=insert(12,bh);
	bh=insert(15,bh);
	bh=insert(20,bh);
	bh=insert(1,bh);
	bh=insert(21,bh);
	bh=insert(5,bh);
	*/
	while(1){
	
		printf("\n\n1. Insertion of key\n2. search the key\n3. find minimum\n4. extract minimum\n5. decrease key\n6. delete key\n7. display keys\n8. Level order traversal\n9. exit\n ");
		printf("\nenter choice\n");
		scanf("%d",&i); 
		switch(i){
			case 1:	printf("Enter  key or enter -1 for no data\n");
					scanf("%d",&key);
					while(key!=-1){
		
		
						bh=insert(key,bh);
		
			
						printf("\nenter next key or enter -1 for no data: ");
						scanf("%d",&key);
						
					}
					break;
			case 2: printf("\nenter the key to be searched\n");
					scanf("%d",&key);
					temp=search(key,bh.head);
									
					if(temp==NULL){
						printf("\n key not found");
					
					}
					else{
						printf("\nthe value enetred is in the heap and the value is %d",key);
						 
					}
					break;
			case 3: printf("\n minimum key is\n");
					temp=findmin(bh.head);
					printf("%d",temp->key);
	
					break;
			case 4: printf("\n Extracted minimum and removed\n");
						bh.head=extractmin(bh.head);
					
					break;
			case 5: printf("\n enter the key to be decreased\n");
					scanf("%d",&oldkey);
					printf("\n enter the new value of the key\n");
					scanf("%d",&key);
					bh.head=decreasekey(bh.head,oldkey,key);
					break;
			case 6: printf("\n enter the key to be deleted\n");
					scanf("%d",&key);
					bh=deletekey(bh,key);
					break;
			case 7: printf("\n displaying the keys present in heap\n");
					display(bh.head);
					break;
			case 9:	exit(0);
			
			case 8: printLevelOrder(bh.head);
		}
	}
	
return 0;
}
//function to union two heaps
heapnode *bunion(bheap h1, bheap h2){
	heapnode *head;
	head=merge(h1,h2);//call the merge functon and point it to the head
	heapnode *curr,*next,*prev=NULL;
	curr=head;//current value as head
	next=head->sibling;//head's sibling
	while(next!=NULL){
		//case 1 when curr and next degree's are not euqal & case 2 current, next to next's sibling degres's are equal
		if(curr->degree!=next->degree||(next->sibling!=NULL&&curr->degree==next->sibling->degree)){
			prev=curr;//make current as previous
			curr=next;//shift the current pointer to next
		}
		//case 3 & 4 two nodes of same degree 
		//case 3 if current's key is less than or euqal to next's key 
		else if(curr->key<=next->key){
			curr->sibling=next->sibling;//make next's sibiling  as curr's sibiling 
			curr=link(next,curr);//link next to current as child
		}
		//case 4 link current to next as child
		else{
			
			if(prev!=NULL){
				prev->sibling=next;//make next as sibling of prev
			}
			
			else{
				head=next;//make head to point to next
			}
			next=link(curr,next);//make next as parent to curr
			curr=next;
			}
			next=curr->sibling;
	}
	return head;//retun the head pointer to the heap
}

heapnode *merge(bheap h1, bheap h2){
	//if both the heaps are empty return NULL
	if(h1.head==NULL&&h2.head==NULL){
		return NULL;
	}
	//if head of heap 1 is empty return head of heap 2
	
	else if(h1.head==NULL){
		
		return h2.head;
	}
	//if head of heap 2 is empty return head of heap 1
	else if(h2.head==NULL){
		return h1.head;
	}
		heapnode *head,*tail;//create a head and tail pointer for linked list 
		heapnode *h1next=h1.head;//take a next pointer to point sibling of head and intilize with head
		heapnode *h2next=h2.head;//take a next pointer to point sibling of head and intilize with head
				//if degree of h1 head is less than or euqal to degree of h2 head
	if(h1.head->degree<=h2.head->degree){
		head=h1.head;//point newhead to heap 1 head
		h1next=h1.head->sibling;//point h1next to heap 1 head's sibling
	}
	//if degree of h1 head is greater than degree of h2 head
	else{
		head=h2.head;//point new head to head of heap 2
		h2next=h2.head->sibling;//pooint h2 next to heap 2 head's sibling
	}
	tail=head;//first node of single linked list is pointed by tail and head
	// traverse till if any of heap gets empty and make a single linked list based on degree of nodes 
	while(h1next!=NULL&&h2next!=NULL){
		// if degree of h1 is smaller and euqal 
		if(h1next->degree<=h2next->degree){
			tail->sibling=h1next;//make h1 next as sibling to tail
			h1next=h1next->sibling;//shift h1next to its sibling
		}
		//if degree oh h2 is greater than h1
		else{
			tail->sibling=h2next;//make h2 next as sibling to tail
			h2next=h2next->sibling;//shift h2 to its sibling
		}
		tail=tail->sibling;
	}
	tail->sibling=(h1next!=NULL)?h1next:h2next;//if h1next is not empty then h1next is tail's sibling else h2next is 
	return head;//return head of the list 
	}
	
//function to link children and parent	
heapnode *link(heapnode *y, heapnode *z){
	y->parent=z;//make z as parent to y
	y->sibling=z->child;//make z's childrean as sibling of y
	z->child=y;//make y as z'child
	z->degree++;//increase degree of z
	return z;//
}

//function to find minimum in the root list
heapnode *findmin(heapnode *head){
	heapnode *min,*next;
	//if head is null return NULL
	if(head==NULL){
		return NULL;
	}
	
	else{
		min=head;//make head as minimum
		next=head->sibling;//sibling of head as next
		//traverse through the heap
		while(next!=NULL){
			//if key of minimum is greater than next 
			if(min->key>next->key){
				min=next;//make minimum as next
			}
			next=next->sibling;
		}
		
		return min;//return minimum node
	}
}
//function to extract minimum and delete it
heapnode *extractmin(heapnode *head){
	heapnode *prev,*min,*next;
	//if head is null return NULL
	if(head==NULL){
		return NULL;
	}
	else{
		min=head;//make head as minimum
		next=head->sibling;//sibling of head as next
		prev=NULL;
		//traverse through the heap
		while(next!=NULL){
			//if key of minimum is greater than next
			if(min->key>next->key){
				prev=min;//make previous as min
				min=next;//make minimum as next
			}
			next=next->sibling;
		}
		//remove the minimum from root list
		head=removeroot(min,prev,head);
		return head;
	}
}
//function to remove node from root list
heapnode *removeroot(heapnode *root, heapnode *prev,heapnode *head){
	//if root to be deleted is head  
	if(root==head){
		head=root->sibling;//make head sibiling as head
	}
	
	else{
		prev->sibling=root->sibling;//make prev's sibiling as root's sibiling
	}
	//create a newhead  child and next pointers
	heapnode *newhead=NULL,*child,*next;
	
	child=root->child;//make child as root's child
	

	//traverse through the child list of root and re arrange the list by removing the parent link
	while(child!=NULL){
		next=child->sibling;// make next as child's sibling
		child->sibling=newhead;// make child's sibilng as new head
		child->parent =NULL; //   
		newhead=child;//make newhead as child
		child=next;//make child as next
		
	}
	bheap temp,temp1;//make new two temporary heaps
	//store the values newhead and head in temporary heaps
	temp.head=newhead;
	temp1.head=head;
	temp1.head=bunion(temp1,temp);//union two heaps and
	
	return temp1.head;
}
//function to search a key in the heap
heapnode *search(int key, heapnode *head){
	//if heap is NULL
	if(head==NULL){
		return NULL;
	}
	//if key is present in the node return the node
	if(head->key==key){
		return head;
	}
	heapnode *child,*sibling;//take child and and sibiling pointers
	child=search(key,head->child);//search for the key in child and return the node to child
	//if child is not NULL return child
	if(child!=NULL){
		return child;
	}
	sibling=search(key,head->sibling);//search for the key in sibling and return the node to sibling
	//if sibling is not NULL, return sibling
	if(sibling!=NULL){
		return sibling;
	}
}
//function to decrease a key value with new value
heapnode *decreasekey(heapnode *head,int oldkey,int newkey){
	//take parent node and temp node
	heapnode *parent,*temp;
	temp=search(oldkey,head);//search for the node to be decreased and return it to temp
	//if temp is null return NULL
	if(temp==NULL){
		return temp;
	}
	//replace key value with new key value
	temp->key=newkey;
	parent=temp->parent;//make parent to point to temp'parent
	int swapVar;//take a variable to swap the keys
	//re adjust the node in the heap if the newvalue is less than parent's key  
	while(parent!=NULL&&temp->key<parent->key){
		//swap the key value among temp and its parent
		swapVar=parent->key;
		parent->key=temp->key;
		temp->key=swapVar;
		//make temp as parent
		temp=parent;
		parent=parent->parent;//point parent to parent's parent
	}
	return head;//return head
}
// function display the nodes in heap
void display(heapnode *head){
	//traverse through the heap
	while (head!=NULL) 
    { //print head's key
        printf("%d ",head->key); 
        display(head->child);//display children of head 
        head= head->sibling; //shift the head pointer to it's sibling
    } 
}
	





