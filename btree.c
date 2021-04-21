#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define T 2 //degree of Tree
//node structure
struct btreeNode{	  
	int keys[2*T-1];  //node have 2t-1 keys
	struct btreeNode *C[2*T]; // node has 2T children  
	int n; //no of keys in a node
	bool leaf; // to determine whether the node is leaf or non leaf node
	
};
//tree structure
struct btree{
	struct btreeNode *root; //root node of the tree
	
};

//function to construct a empty tree
struct btree construct()
{ struct btree b; 
	b.root=NULL;
	return b;
}
//funcion to create a node
struct btreeNode *create(bool leaf1) 
{ 	
	struct btreeNode* temp= (struct btreeNode*)malloc(sizeof(struct btreeNode)); 
	temp->leaf = leaf1;
	temp->n = 0;//initially no of keys are zero
	return temp;
}
//function to search the key
struct  btreeNode* search(int k,struct btreeNode *s) 
{ 
    // find the index of key greater than or equal to k 
    int i = 0; 
    while (i <s->n && k >s->keys[i]) 
        i++; 
  
    // if the found key is equal to k return the node 
    if (s->keys[i] == k) 
        return s; 
  
    // if key is not found and this is a leaf node return NULL 
    if (s->leaf == true) 
        return NULL; 
  
    // go to the appropriate child 
    return search(k,s->C[i]); 
} 
//function to split the child by taking its parent node
struct btreeNode *splitChild(int i, struct btreeNode *x) 
{ 	//copy the child into new node and create a sibiling node with y's leaf property
	struct btreeNode *y=x->C[i];
	struct btreeNode *z = create(y->leaf); 
	z->n = T - 1; // no of keys are minimum no of keys
	int j;
	//copy keys of y after T no of keys to z
	for (j = 0; j < T-1; j++) 
		z->keys[j] = y->keys[j+T]; 
	
	//if y is not a leaf shift the child pointers of y to z
	if (y->leaf == false) 
	{ 
		for (j = 0; j < T; j++) 
			z->C[j] = y->C[j+T]; 
	} 

	
	
	//shift the children of parent X by 1 position if index+1 is less than no of nodes of x 
	for (j =x->n; j>=i+1; j--) 
	{		
		x->C[j+1] = x->C[j];
			 
	}
	 
	x->C[i+1] = z; //point the index+1 to sibiling z
	//shift the keys of parent X by one position if index is equal or lest than n-1 of X
	for (j =x->n-1; j >=i; j--) 
	{
		x->keys[j+1] = x->keys[j];
			 
	}
	 //copy the median of children Y into index poistion of parent
	x->keys[i] = y->keys[T-1]; 
	y->n = T-1; // no of keys in y are T-1
	x->n = x->n + 1;// increase n of X 
	
	return x; //return the parent node
} 
//function to insert a key if node is not full
struct btreeNode *insertNonFull(int k, struct btreeNode *s) 
{ 
	
	int i = s->n-1; //index is no of keys (n-1 because array starts from 0)

	 //if node is leaf
	if (s->leaf == true) 
	{ 	//loop to shift the keys and decrease the index value if keys present are greater than given key
		while (i >= 0 && s->keys[i] > k) 
		{ 
			s->keys[i+1] = s->keys[i]; 
			i--; 
		} 

		
		s->keys[i+1] = k;//insert the key in the node 
		s->n = s->n+1; //increase no of keys
	} 
	//if node is not leaf
	else 
	{ 
		//loop  decrease the index value if keys present are greater than given key
		while (i >= 0 && s->keys[i] > k) 
			i--; 

		//if the child where key has to be inserted is full split it
		if (s->C[i+1]->n == 2*T-1) 
		{ 	s=splitChild(i+1,s); 
			//the value of key is greater than index+1 key increament the index value
			if (s->keys[i+1] < k) 
				i++; 
		} 
		//insert the key in child of index+1 child
		s->C[i+1]=insertNonFull(k,s->C[i+1]); 
	} 
	return s; //return the parent node
} 

//function to insert node and key into the tree
void insert(int k,struct btree *b) 
{ 	//if root of the tree is null insert the key in root
	if (b->root == NULL) 
	{ 
		
		b->root = create(true); 
		b->root->keys[0] = k; 
		b->root->n = 1; 
	} 
	//if root is not empty
	else  
	{ 
		//check if the root is full or not 
		if (b->root->n == 2*T-1) 
		{ 
			//create a empty node as non leaf	
			struct btreeNode *s = create(false); 

			//make the root as first child of new node
			s->C[0] = b->root; 
			//split the root
			s=splitChild(0,s); 
			//insert the key 
			s=insertNonFull(k,s);
			
			
			//make this node as root
			b->root = s; 
		} 
		//if root is not full 
		else{
		b->root=insertNonFull(k,b->root);
		}
	}
} 
//function to perform inorder traversal
void traverse(struct btreeNode *temp) 
{ 	
	int i; 
	//traverse upto n keys and n children
	for (i = 0; i <temp->n; i++) 
	{ 	// If this is not leaf, then before printing key[i], traverse the subtree of child C[i]. 
		if (temp->leaf== false) 
			traverse(temp->C[i]); 
		printf("%d\n",temp->keys[i]); 
	} 

	//print the subtree of last child
	if (temp->leaf== false) 
		traverse(temp->C[i]); 
} 
//function to create a queue
struct btreeNode** createQueue(int *front, int *rear) 
{ 
	struct btreeNode **queue =(struct btreeNode **)malloc(sizeof(struct btreeNode*)*500); 

	*front = *rear = 0; //intially the que is empty
	return queue; 
} 
//function to enque
void enQueue(struct btreeNode **queue, int *rear, struct btreeNode *new_node) 
{ 
	queue[*rear] = new_node;//add the node at the end of que
	(*rear)++; 
} 
//function to deque and to return the dequed node
struct btreeNode *deQueue(struct btreeNode **queue, int *front) 
{ 
	(*front)++; 
	return queue[*front - 1];//return the fist node 
} 


//function to print the tree in level order
void printLevelOrder(struct btreeNode *root ) 
{ 
	int rear, front, size=0,count=0; //count is for keeping track of no of nodes enqued in each level
	//size is for keeping track of no of nodes present in the que for a particular level 
	struct btreeNode **queue = createQueue(&front, &rear); 
	struct btreeNode *temp;
	
	enQueue(queue, &rear, root);//enque the root
	count++;
	while(1) 
	{ 	size=count; //the size of ques is no of nodes enqued
		if(size==0){
			break;
		}		 
		while(size>0){
			temp = deQueue(queue, &front);//deque the node
			int i;
			//print all the keys present in the current node
			for(i=0;i<temp->n;i++){
			printf("%d ", temp->keys[i]);
			}
			count--;
			//enque the children of current node
			for(i=0;i<=temp->n;i++)
			{
			if (!temp->leaf)
			{		 
				enQueue(queue, &rear, temp->C[i]);
				count++;
			}
		
			}
		size--;//reduce the size after printing a node if size becomes zero then a level is completed

		}
		printf("\n");
 
} 
}

int findKey(int k,struct btreeNode* s);
void removekey(int k, struct btree *b);
struct btreeNode* removeKnode(int k,struct btreeNode* s);
struct btreeNode* removeFromLeaf (int idx, struct btreeNode* s);
struct btreeNode* removeFromNonLeaf(int idx, struct btreeNode*s);
struct btreeNode* merge(int idx,struct btreeNode*s);
int getPred(int idx, struct btreeNode*s);
int getSucc(int idx,struct btreeNode*s);
struct btreeNode* borrowFromPrev(int idx,struct btreeNode*s);
struct btreeNode* borrowFromNext(int idx,struct btreeNode*s);
struct btreeNode* merge(int idx,struct btreeNode*s);
struct btreeNode* fill(int idx,struct btreeNode*s);


int main(){
	struct btree b;
	int val,i;
	b=construct();
	struct btreeNode *temp;

	while(1){
	
		printf("\n1. Insertion of key\n2. search the key\n3. inorder traversal\n4. level order traversal\n5. deleting the node\n6. Exit ");
		printf("\nenter choice\n");
		scanf("%d",&i); 
		switch(i){
			case 1:	printf("Enter  key or enter -1 for no data\n");
					scanf("%d",&val);
					while(val!=-1){
		
		
						insert(val,&b);
		
			
						printf("\nenter next key or enter -1 for no data: ");
						scanf("%d",&val);
					}
					break;
			case 2: printf("\nenter the key to be searched\n");
					scanf("%d",&val);
					temp=search(val,b.root);
									
					if(temp==NULL){
						printf("\n Node not found");
					
					}
					else{
						printf("\nthe value enetred is in tree and the value is %d",val);
						 
					}
					break;
			case 3: printf("\n inorder traversal\n");
					traverse(b.root);
					break;
			case 4: printf("\n Level Order traversal\n");
					printLevelOrder(b.root);
					break;
			case 5: printf("\n enter the key to be deleted");
					scanf("%d",&val);
					removekey(val,&b);
					break;
			case 6:	exit(0);
		}
	}
	
	return 0; 
}

//function to return the index of key to be delted in current node or returns the index of last child if givn key is greater than last key of current node 
//return zero if key is less than first key of current node  
int findKey(int k,struct btreeNode* s) 
{ 
	int idx=0; 
	while (idx<s->n && s->keys[idx] < k) 
		idx++; 
	return idx; 
} 

// A function to remove the key  from the given node 
struct btreeNode* removeKnode(int k,struct btreeNode* s) 
{ 
	int idx = findKey(k, s); 

	// The key to be removed is present in given node 
	if (idx <s->n && s->keys[idx] == k) 
	{ 

		// if the node is a leaf node - removeFromLeaf is called 
		// else removeFromNonLeaf function is called 
		if (s->leaf) 
			s=removeFromLeaf(idx,s); 
		else
			s=removeFromNonLeaf(idx,s); 
	} 
	//key is not present in given node
	else
	{ 

		// If given node is a leaf node, then the key is not present in tree 
		if (s->leaf) 
		{ 
			printf("The key %d is does not exist in the tree\n",k); 
			return; 
		} 

		// key to be removed is present in the sub-tree of this node
		 
		// flag is to indicates whether the key is present in the sub-tree of last child of given node 
		bool flag = ( (idx==s->n)? true : false ); 

		// if the child where the key exist has less than t keys, we fill that child with sibilings keys 
		if (s->C[idx]->n < T) 
			s=fill(idx,s); 


		// if the last child has been merged, it must have merged with the previous child  
		if (flag && idx >s->n) 
			s->C[idx-1]=removeKnode(k,s->C[idx-1]);  //recurse on the (idx-1)th child
		//else recurse on the (idx)th child (has t keys after filling)
		else
			s->C[idx]=removeKnode(k,s->C[idx]); 
	} 
	return s; 
} 

// function to remove the key from give node which is a leaf node 
struct btreeNode* removeFromLeaf (int idx, struct btreeNode* s) 
{   int i; 

	// shift all the keys after the idx position one place backward 
	for (i=idx+1; i<s->n; ++i) 
		s->keys[i-1] = s->keys[i]; 

	// reduce the count of keys 
	s->n--; 

	return s; 
} 

// function to remove key from given node which is a non-leaf node 
struct btreeNode* removeFromNonLeaf(int idx, struct btreeNode*s) 
{ 
	int k = s->keys[idx]; 
	 

	// if the child that precedes key (C[idx]) has atleast t keys, find the predecessor  of k in the subtree at C[idx] 
	if (s->C[idx]->n >= T) 
	{ 	//call get predecessor
		int pred = getPred(idx,s); 
		s->keys[idx] = pred; //replace key by pred
		s->C[idx]=removeKnode(pred,s->C[idx]); //delete pred at present in subtree of C[idx]
	} 

	// if the child C[idx] has less than t keys, examine C[idx+1]. 
	// if the child C[idx+1] has atleast t keys, find the successor of key in the subtree at C[idx+1]
	else if (s->C[idx+1]->n >= T) 
	{ 
		int succ = getSucc(idx+1,s); 
		s->keys[idx] = succ; //replace key by succ
		s->C[idx+1]=removeKnode(succ,s->C[idx+1]);//recursively delete succ in C[idx+1] 
	} 

	// if both C[idx] and C[idx+1] has less than t keys,merge key and  C[idx+1] into C[idx] 
	// after merging C[idx] contains 2t-1 keys 
	 
	else
	{ 
		s=merge(idx,s); 
		s->C[idx]=removeKnode(k,s->C[idx]); //recursively delete k from C[idx]
	} 
	return s; 
} 

// function to get predecessor of key 
int getPred(int idx, struct btreeNode*s) 
{ 
	//  move to the right most node  until we reach a leaf 
	struct btreeNode* cur=s->C[idx]; 
	while (!cur->leaf) 
		cur = cur->C[cur->n]; 

	// Return the last key of the leaf 
	return cur->keys[cur->n-1]; 
} 
//sunction to get sucessor of key
int getSucc(int idx,struct btreeNode*s) 
{ 

	// move to the left most node until we reach a leaf 
	struct btreeNode* cur =s->C[idx]; 
	while (!cur->leaf) 
		cur = cur->C[0]; 

	// Return the first key of the leaf 
	return cur->keys[0]; 
} 

//function to fill child which has less than t-1 keys 
struct btreeNode *fill(int idx,struct btreeNode*s) 
{ 

	// if the previous sibiling has more than t-1 keys, borrow a key from prev sibiling 
	if (idx!=0 && s->C[idx-1]->n>=T) 
		s=borrowFromPrev(idx,s); 

	//if the next sibiling has more than t-1 keys, borrow a key from next sibiling
	else if (idx!=s->n && s->C[idx+1]->n>=T) 
		s=borrowFromNext(idx,s); 

	// if prev and next sibling doesnt have T keys merge the node with sibiling 
	
	
	else
	{ 	// if node is not last child merge it with next sibiling 
		if (idx != s->n) 
			s=merge(idx,s); 
		
		// if node is the last child merge it with with its previous sibling
		else
			s=merge(idx-1,s); 
	} 
	return s; 
} 

// function to borrow a key from previous sibiling and insert it into current node
struct btreeNode* borrowFromPrev(int idx,struct btreeNode*s) 
{ 

	struct btreeNode *child=s->C[idx]; //child node of parent
	struct btreeNode *sibling=s->C[idx-1]; //prev sibiling of child

	// the last key from sibiling goes up to the parent and key[idx-1] from parent is inserted at the first key in child 
	int i;
	// shift all key in child one step ahead 
	for (i=child->n-1; i>=0; --i) 
		child->keys[i+1] = child->keys[i]; 

	// If child is not a leaf shift all its child pointers one step ahead 
	if (!child->leaf) 
	{ 
		for(i=child->n; i>=0; --i) 
			child->C[i+1] = child->C[i]; 
	} 

	// copy parent's keys[idx-1] in childs first key 
	child->keys[0] = s->keys[idx-1]; 

	//if child is not a leaf copy sibling's last child as childs first child 
	if(!child->leaf) 
		child->C[0] = sibling->C[sibling->n]; 

	// move the last key from the sibling to the parent  
	s->keys[idx-1] = sibling->keys[sibling->n-1]; 

	child->n += 1; //increase child n
	sibling->n -= 1; //decrease sibiling n

	return s; //return parent
} 

// function to borrow a key from the next sibiling and place it in current node 

struct btreeNode* borrowFromNext(int idx,struct btreeNode*s) 
{ 

	struct btreeNode *child=s->C[idx]; //child of parent
	struct btreeNode *sibling=s->C[idx+1]; //next sibiling of child
	// the first key from sibiling goes up to the parent and key[idx] from parent is inserted at the last key in child
	
	// parents's keys[idx] is inserted as the last key in child 
	child->keys[(child->n)] = s->keys[idx]; 

	//if sibiling is not leaf sibling's first child is inserted as the last child into child 
	if (!(child->leaf)) 
		child->C[(child->n)+1] = sibling->C[0]; 

	//first key from sibling is inserted into parent's keys[idx] 
	s->keys[idx] = sibling->keys[0]; 
	int i;
	// move all keys in sibling one step behind 
	for (i=1; i<sibling->n; ++i) 
		sibling->keys[i-1] = sibling->keys[i]; 

	//if sibiling is not leaf move the child pointers one step behind 
	if (!sibling->leaf) 
	{ 
		for(i=1; i<=sibling->n; ++i) 
			sibling->C[i-1] = sibling->C[i]; 
	} 

	
	child->n += 1; //increase child n
	sibling->n -= 1; //decrease sibiling n

	return s; //return parent
} 

// A function to merge child with sibiling 
// sibiling is deleted after merging 
struct btreeNode* merge(int idx,struct btreeNode*s) 
{ 
	struct btreeNode *child = s->C[idx];//child of parent 
	struct btreeNode *sibling =s->C[idx+1]; //sibiling of child

	// move the keys[idx] from the parent node and insert it into (t-1)th position of child 
	child->keys[T-1] = s->keys[idx]; 
	int i;
	// copy the keys from sibiling to child at the end 
	for (i=0; i<sibling->n; ++i) 
		child->keys[i+T] = sibling->keys[i]; 

	//if child is not leaf copy the child pointers from sibiling to child 
	if (!child->leaf) 
	{ 
		for( i=0; i<=sibling->n; ++i) 
			child->C[i+T] = sibling->C[i]; 
	} 

	// Move all keys after idx in the parent node one step before to fill the gap created by moving keys[idx] to child 
	for (i=idx+1; i<s->n; ++i) 
		s->keys[i-1] = s->keys[i]; 

	// Move the child pointers of parent after (idx+1)th positon one step before 
	for (i=idx+2; i<=s->n; ++i) 
		s->C[i-1] = s->C[i]; 

	// Update the key count of child and parent node 
	child->n += sibling->n+1; 
	s->n--; 

	// delete sibiling 
	free(sibling); 
	return s; //return parent
} 


//function to remove key from the tree
void removekey(int k, struct btree *b) 
{ 	
	if (b->root==NULL) 
	{ 
		printf("The tree is empty\n"); 
		return; 
	} 

	// Call the remove function for root 
	b->root=removeKnode(k,b->root); 

	// after deleting the key if the root node has 0 keys, make its first child as the new root 
	if (b->root->n==0) 
	{ 
		struct btreeNode* tmp = b->root; 
		//if root doesnt have any children then make it NULL
		if (b->root->leaf) 
			b->root = NULL; 
		//else make root's first child as root
		else
			b->root = b->root->C[0]; 

		//delete the old root 
		free(tmp); 
	} 
} 

