#include"fibheap.h"

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


Graph* PrimMST(struct Graph* graph, int *min) 
{ 
	int V = graph->V;
	Graph* mstgraph = createGraph(V);
	fibheap fh = makeheap(); 
	int v;
	int i=INT_MAX;
	for ( v = 1; v < V; ++v) { 
		min[v] =i; 
		fh=insert(i,v,fh);
		i--;
	} 

	min[0] = 0; 
	fh=insert(0,0,fh);
		 

	while (fh.noOfNodes!=0){
		int u = fh.mini->v;
		extractmin(&fh);

		struct AdjListNode* temp = graph->array[u]; 
		while (temp != NULL) { 
			int v = temp->dest; 
			fibnode *found=search(fh.mini,v);
			if ( found !=NULL&& temp->weight < min[v]) { 
				min[v] = temp->weight; 
				graph->array[v]->parent = u;
				decreasekey(min[v],v,&fh); 
			} 
			temp = temp->next; 
		} 
	} 
		for(i=1;i<V;i++){
			addEdge(mstgraph,graph->array[i]->parent, i,min[i]);
		}
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
    while(temp!=NULL)
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
	printf("\n");
    printGraph(mstgraph);
	int i;
	printf("\nthe edges of MST are\n");
	for(i=1;i<V;i++)
	{	printf("%d - %d : weight %d\n", graph->array[i]->parent, i,min[i]);
	}
	printf("\n the Preorder traversal of above mst is");
	preorder(0, mstgraph);
	printf("\n%d",0);
	free(graph);
	free(mstgraph);
	free(min);
	return 0; 
} 

