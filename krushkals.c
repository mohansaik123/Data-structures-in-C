#include<stdio.h>
#include<stdlib.h>

typedef struct AdjListNode { 
	int dest; 
	int weight; 
	int visited;
	struct AdjListNode* next; 
}AdjListNode; 

typedef struct Edge 
{ 
	int src, dest, weight; 
}Edge;
 
typedef struct visited{
	int dest1;
	int dest2;
}visited;

typedef struct Graph { 
	int V; 
	struct AdjListNode **array; 
	struct Edge edge[20];
}Graph; 


AdjListNode* newAdjListNode(int dest, int weight) 
{ 
	AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode)); 
	newNode->dest = dest; 
	newNode->weight = weight; 
	newNode->next = NULL; 
	newNode->visited=0;
	return newNode; 
} 



Graph* createGraph(int V) 
{ 
	 Graph* graph = ( struct Graph*)malloc(sizeof(struct Graph)); 
	graph->V = V; 


	graph->array = ( AdjListNode**)calloc(V , sizeof( AdjListNode*)); 
	
	

	return graph; 
} 
void addEdgeList(Graph *graph, int src, int dest, int i , int weight){
	graph->edge[i].src=src;
	graph->edge[i].dest=dest;
	graph->edge[i].weight=weight;
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
void addEdgeMst( Graph* graph, int src, int dest, int weight) 
{ 
	AdjListNode* Node = newAdjListNode(dest, weight); 
	Node->next = graph->array[src]; 
	graph->array[src] = Node; 


	Node = newAdjListNode(src, weight); 
	Node->next = graph->array[dest]; 
	graph->array[dest] = Node; 
}

int createEdge(Graph *graph){
	int v,i,j=0;
	v=graph->V;
	AdjListNode *temp;
	
	for(i=0;i<v;i++){
		temp=graph->array[i];
		while(temp){
			addEdgeList(graph,i,temp->dest,j,temp->weight);
			j++;
			temp=temp->next;
		}
	}
	return j;
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
void printEdges(Graph *graph, int totaledges){
	int i;
	for(i=0;i<totaledges;i++){
		printf("\n%d - %d",graph->edge[i].src, graph->edge[i].dest);
	}
	
}
	

int getMinimum(Graph *graph, int totalEdges, int *visitedEdges){
	int i, minidx=-100;
	for(i=0;i< totalEdges;i++){
		if(visitedEdges[i]==0){
			if(minidx==-100){
				minidx=i;
			}
			else {
				if(graph->edge[i].weight<graph->edge[minidx].weight)
					minidx=i;
			}
		}
	}
	return minidx;
}
int isCycle(visited *vertex, int u, int v){
	while(vertex[u].dest1!= -1) u = vertex[u].dest1;
	while(vertex[v].dest1!= -1) v= vertex[v].dest1;
 	if(u == v) return -100;
	return u;
}
Graph *kruskalsMst(Graph *graph, int totalEdges){
	int visitedEdges[totalEdges];
	int i;
	for(i=0;i<totalEdges;i++)
		visitedEdges[i]=0;
	int v=graph->V;
	visited vertex[v];
	//vertex = ( visited**)malloc(v* sizeof( visited*));
	Graph *MstGraph=createGraph(v);
	for(i=0;i<v;i++)
	{
	
		vertex[i].dest1=-1;
		vertex[i].dest2=-1;
	}

	int minidx=getMinimum(graph,totalEdges,visitedEdges);
	int minidx2;
	for(i=minidx+1;i<totalEdges;i++)
	{
		if(graph->edge[minidx].src==graph->edge[i].dest && graph->edge[minidx].dest==graph->edge[i].src) minidx2=i;
	}
	printf("\n\n The Mst edges are\n");

	do{
		visitedEdges[minidx]=1;
		visitedEdges[minidx2]=1;
		if((i = isCycle(vertex, graph->edge[minidx].src, graph->edge[minidx].dest)) != -100 ){
			vertex[i].dest1=graph->edge[minidx].dest;
			vertex[graph->edge[minidx].dest].dest2=i;
			
			addEdgeMst(MstGraph,graph->edge[minidx].src,graph->edge[minidx].dest,graph->edge[minidx].weight);
			
			printf(" %d - %d weight %d\n",graph->edge[minidx].src,graph->edge[minidx].dest,graph->edge[minidx].weight);
		}
		minidx = getMinimum(graph, totalEdges, visitedEdges);
	} while(minidx!= -100 );
	
	return MstGraph;
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


int main(){
	int V=4;
	struct Graph *graph = createGraph(V);
	addEdge(graph, 0, 1, 4); 
	addEdge(graph, 0, 2, 8); 
	addEdge(graph, 0, 3, 8); 
	addEdge(graph, 1, 2, 11); 
	addEdge(graph, 1, 3, 7); 
	addEdge(graph, 2, 3, 2); 

	/*addEdge(graph, 0, 1, 4); 
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
	*///printGraph(graph);
	int totaledges=createEdge(graph);
	int i;
	printf("\n the edges of the graph are ");
	printEdges(graph, totaledges);
	Graph *MstGraph;
	MstGraph=kruskalsMst(graph, totaledges);
	printGraph(MstGraph);
	printf("\n the Preorder traversal of above mst is");
	preorder(0,MstGraph);
	printf("\n%d",0);
	free(graph);
	free(MstGraph);
	
}

