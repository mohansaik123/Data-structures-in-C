#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct AdjListNode { 
	int dest; 
	int weight; 
	int parent;
	int visited;
	struct AdjListNode* next; 
}AdjListNode; 



typedef struct Graph { 
	int V; 
	struct AdjListNode **array; 
}Graph; 


AdjListNode* newAdjListNode(int dest, int weight) 
{ 
	AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode)); 
	newNode->dest = dest; 
	newNode->weight = weight; 
	newNode->next = NULL; 
	newNode->parent=-1;
	newNode->visited=0;
	return newNode; 
} 



Graph* createGraph(int V) 
{ 
	 Graph* graph = ( struct Graph*)malloc(sizeof(struct Graph)); 
	graph->V = V; 


	graph->array = ( AdjListNode**)malloc(V * sizeof( AdjListNode*)); 

	int i;
	for (i = 0; i < V; ++i) 
		graph->array[i] = NULL; 

	return graph; 
} 


void addEdge( Graph* graph, int src, int dest, int weight) 
{ 
	 AdjListNode* Node = newAdjListNode(dest, weight); 
	Node->next = graph->array[src]; 
	graph->array[src] = Node; 


	Node = newAdjListNode(src, weight); 
	Node->next = graph->array[dest]; 
	graph->array[dest] = Node; 
}
void printGraph( Graph* graph) 
{ 
    int v; 
    for (v = 0; v < graph->V; ++v) 
    { 
        AdjListNode* temp = graph->array[v]; 
        printf("\n Adjacency list of vertex %d\n ", v); 
        while (temp) 
        { 
            printf("  %d", temp->dest); 
            temp = temp->next; 
        } 
        printf("\n"); 
    } 
}

//  structure to represent a node in the heap 
typedef struct fibnode { 
	struct fibnode* parent; // Parent pointer 
	struct fibnode* child; // Child pointer 
	struct fibnode* prev; // Pointer to the node on the prev 
	struct fibnode* next; // Pointer to the node on the next 
	int key; // Value of the node 
	int degree; // Degree of the node 
	int mark; // 1 for true or 0 for false mark of the node
	int v; 
	int visited;
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
fibnode *construct(int val, int v) 
{ 
	fibnode* temp = (struct fibnode*)malloc(sizeof(struct fibnode)); 
	temp->key = val; 
	temp->degree = 0; 
	temp->mark = 0; 
	temp->v=v;
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
fibheap insert(int key,int v, fibheap fh){
	fibheap temp=makeheap();//create a temporary heap
	temp.mini=construct(key,v);//add the node to be inserted into the heap with given value
	temp.noOfNodes=1;// no of nodes are 1
	fh=fibunion(fh,temp);//union main heap and temp heap
	return fh;//return main heap
}


// function to Link the heap nodes into  parent and child  
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
	y->next = x->child; 
	y->prev = (x->child)->prev; 
	((x->child)->prev)->next = y; 
	(x->child)->prev = y; 
	//if the key of y is less than x's current child key
	if (y->key < (x->child)->key) 
		x->child = y; //point x's child pointer to y
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
fibnode *extractmin(fibheap *fh) 
{ 	//if the heap is empty return 
	if (fh->mini == NULL){
	
		printf( "The heap is empty\n" );
		return NULL; 
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
				//i f key of x is less than cuurent mini's key then change mini pointer
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
	return fh->mini;
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
void Cascade_cut(fibnode* found,fibheap *fh) 
{ 
	fibnode *z = found->parent; 
	if (z != NULL) { 
		// mark the field as the node has a child cut
		if (found->mark == 0) { 
			found->mark = 1; 
		} 
		// if the node didnt had any cut perdorm the cut 
		else { 
			Cut(found, z,fh); 
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
    if (mini->v == val) { 
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
fibheap *decreasekey(int val,int v, fibheap *fh) 
{ 
	// check if the node is empty or not
	if (fh->mini == NULL){
	
		printf("The Heap is Empty\n" );
		return NULL; 
	}

	fibnode *found,*temp1;
	temp1=fh->mini;
	// search for the value to be decreased in the heap 
	found=search(temp1,v);

	// if the node is not found then return
	if (found == NULL) {
		printf("Node not found in the Heap\n");
		return fh; 
	}
	// update the key of found node
	found->key = val; 
	//printf("key updated \n");
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


Graph* PrimMST(struct Graph* graph, int *key) 
{ 
	int V = graph->V; 
  // 	int *parent; 
	//parent = (int*)malloc(V* sizeof(int));
	Graph* mstgraph = createGraph(V);
	fibheap fh = makeheap(); 
	int v;
	int i=INT_MAX;
	for ( v = 1; v < V; ++v) { 
		//parent[v] = -1; 
		key[v] =i; 
		fh=insert(i,v,fh);
		i--;
	} 

	key[0] = 0; 
	fh=insert(0,0,fh);
		 

	while (fh.noOfNodes!=0){
		int u = fh.mini->v;
		extractmin(&fh);

		struct AdjListNode* temp = graph->array[u]; 
		while (temp != NULL) { 
			int v = temp->dest; 
			fibnode *found=search(fh.mini,v);
			if ( found !=NULL&& temp->weight < key[v]) { 
				key[v] = temp->weight; 
				graph->array[v]->parent = u;
				addEdge(mstgraph,u,v,temp->weight); 
				decreasekey(key[v],v,&fh); 
			} 
			temp = temp->next; 
		} 
	} 


//	printArr(parent, V);
       return mstgraph;	
} 

void preorder(int i, Graph *mstgraph)
{
    AdjListNode *temp;
   	int V=mstgraph->V;
   	int j;
	printf("\n%d",i);
    temp=mstgraph->array[i];
    temp->visited=1;
    while(p!=NULL)
    {
       i=temp->dest;
       
	   if(mstgraph->array[i]->visited==0)
        {
			
		    preorder(i, mstgraph);
    	}
		temp=temp->next;
    }
}


int main() 
{ 
	
	int V = 4;
	int *min;
	min=(int*)malloc(V* sizeof(int));
	struct Graph *graph = createGraph(V);
        struct Graph *mstgraph; 
	addEdge(graph, 0, 1, 4); 
	addEdge(graph, 0, 2, 8); 
	addEdge(graph, 0, 3, 8); 
	addEdge(graph, 1, 2, 11); 
	addEdge(graph, 1, 3, 7); 
	addEdge(graph, 2, 3, 2); 
/*	addEdge(graph, 0, 1, 4); 
	addEdge(graph, 0, 7, 8); 
	addEdge(graph, 1, 2, 8); 
	addEdge(graph, 1, 7, 11); 
	addEdge(graph, 2, 3, 7); 
	addEdge(graph, 2, 8, 2); 
	addEdge(graph, 2, 5, 4); 
	addEdge(graph, 3, 4, 9); 
	addEdge(graph, 3, 5, 14); 
	addEdge(graph, 4, 5, 10); 
	addEdge(graph, 5, 6, 2); 
	addEdge(graph, 6, 7, 1); 
	addEdge(graph, 6, 8, 6); 
	addEdge(graph, 7, 8, 7); 
*/	printf("\nthe created graph is \n");
	printGraph(graph);
	mstgraph=PrimMST(graph, min);
    
	int i;
	printf("\nthe edges of MST are\n");
	for(i=1;i<V;i++)
	{	printf("%d - %d : weight %d\n", graph->array[i]->parent, i,min[i]);
	}
	
	DFS(0, mstgraph);
	free(graph);
	free(mstgraph);
	free(min);
	return 0; 
} 

