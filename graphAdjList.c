#include<stdio.h>
#include<stdlib.h>

typedef struct adjNode { 
	int dest; 
	int weight; 
	struct adjNode* next; 
}adjNode; 


typedef struct adjList { 
	struct AdjListNode* head; 
}adjList; 

typedef struct graph { 
	int V; 
	struct adjList* array; 
}graph; 


adjNode* newNode(int dest, int weight) {
	adjNode* temp = (struct adjNode*)malloc(sizeof(struct adjNode)); 
	temp->dest = dest; 
	temp->weight = weight; 
	temp->next = NULL; 
	return temp; 
} 


graph* createGraph(int V) {
	graph* graph1 = (struct graph*)malloc(sizeof(struct graph)); 
	graph1->V = V; 


	graph1->array = (struct adjList*)malloc(V * sizeof(struct adjList)); 

	int i;
	for (i = 0; i < V; ++i) 
		graph1->array[i].head = NULL; 

	return graph1; 
} 


void addEdge(graph* graph1, int src, int dest, int weight) {

	adjNode *temp;
	temp = newNode(dest, weight); 
	temp->next = graph1->array[src].head; 
	graph1->array[src].head = temp; 


	temp = newNode(src, weight); 
	temp->next = graph1->array[dest].head; 
	graph1->array[dest].head = temp; 

 } 
void printGraph(graph* graph1) 
{ 
    int v; 
    for (v = 0; v < graph1->V; ++v) 
    { 
        adjNode* temp = graph1->array[v].head; 
        printf("\n Adjacency list of vertex %d\n head ", v); 
        while (temp) 
        { 
            printf("-> %d", temp->dest); 
            temp = temp->next; 
        } 
        printf("\n"); 
    } 
} 
int *VertexCover(graph *graph1){
 
    int V=graph1->V; 
	static int C[100];
	int i;
	for (i=0;i<V;i++){
		C[i]=0;
	}
  	adjList *E;
  	E=graph1->array;
        int u,v;
	for ( u=0; u<V; u++) 
    { 
        if (C[u] == 0) {
			
            adjNode *temp=E[u].head;
			 
            while(temp!=NULL){
         			v=temp->dest;
                if (C[v] == 0) 
                {
                     C[v] = 1;
					 C[u]=1; 
					break;
                } 
                temp=temp->next;
            } 
        } 
    }
	printCover(C,V); 
  return C;
    // Print the vertex cover 
}
void printCover(int *C,int V){

int i;
	for ( i=0; i<V; i++) 
        if (C[i]) 
          printf("%d ", i); 
}
int main(){
	printf("\nenter the no of vertices in the graph");
	int V,i,src,dest,weight;
	scanf("%d",&V);
	int *C;	
	graph *graph1;
	graph1 = createGraph(V);
	
	
	while(1){
	
		printf("\n\n1. Insert an edge in the graph\n2. Print the Graph\n3. Vertex cover of the graph\n4. Print the vertex Cover\n5. exit\n ");
		printf("\nenter choice\n");
		scanf("%d",&i); 
		switch(i){
			case 1:	while(1){
						printf("\nenter source if no edge enter -1 ");
						scanf("%d",&src);
						if(src==-1) break;
						printf("\nenter the deatination ");
						scanf("%d",&dest);
						printf("\nenter the weight ");
						scanf("%d",&weight);
						addEdge(graph1, src, dest, weight); 
					}
					break;
			case 2: printGraph(graph1);
					break;
			case 3: C=VertexCover(graph1);
					break;
			case 4: printCover(C,V);
					break;
			case 5: exit(0);
		}
	}
	free(graph1);
	return 0;
}
