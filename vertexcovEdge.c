#include<stdlib.h>
#include<stdio.h>

typedef struct edge{
	int src;
	int dest;
	int weight;
}edge;

typedef struct list{
	struct edge *E;
}list;

list *createlist(int N){
	list *temp=(list*)calloc(2*N,sizeof(list));
	return temp;
}
edge *node(int source, int weight,int dest)
{	edge *temp=(edge *)malloc(sizeof(edge));
	temp->dest=dest;
	temp->src=source;
	temp->weight = weight;
	return temp;
}
void addedge(int source, int weight,int dest, int i, list* L){
	L[i].E = node(source, weight, dest);
	i++;
	L[i].E= node(dest,weight,source);
}
void printList(list*L, int N){
	int i;
	for(i=0;i<(2*N);i++){
		printf("\n%d - %d- %d",L[i].E->src,L[i].E->dest,L[i].E->weight);
	}
}
int *vertexcover(list *L, int N,int V){
	
	int *C;// to keep track of if the vertex is visted or not
	C = (int*)malloc(V* sizeof(int)); 
	int i;
	for (i=0;i<V;i++){
		C[i]=0;
	}
  	int u,v;
  	i=0;
	int j;
  	for(i=0;i<2*N;i++){
  		
  		u=L[i].E->src;
	  	v=L[i].E->dest;
	  	if(C[u]==0){
	  	  	
		
			  if(C[v]==0){
				C[u]=1;
				C[v]=1;
				}
				
		}
	}
	printf("\nthe cover of the vertex is\n ");
	for( i=0; i<V; i++){
        if (C[i]==1) 
          printf("%d ", i); 
	}
}
int main(){
	int src, dest, weight, i=0, N,V;
	list *L;
	N=8;
	V=7;
	L=createlist(N);
	addedge(0,1,2,i,L);
	i=i+2;
	addedge(0,1,2,i,L);
	i=i+2;
	addedge(2,1,4,i,L);
	i=i+2;
	addedge(2,1,3,i,L);
	i=i+2;
	addedge(3,1,4,i,L);
	i=i+2;
	addedge(3,1,5,i,L);
	i=i+2;
	addedge(3,1,6,i,L);
	i=i+2;
	addedge(4,1,5,i,L);
	i=i+2;
	printList(L,N);
	vertexcover(L,N,V);
	return 0;
}
	
	
	
	
	
