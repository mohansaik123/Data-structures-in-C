
#include <stdio.h>
#include<stdlib.h>

#define maxSetNum 20


int **initializeSets(int noOfSets)
{
	int i, j;
	int **superset = NULL, *set = NULL;
	superset = (int **)calloc(noOfSets, sizeof(int *));
	printf("\n 0 set is the universal set\n");
	int key;
	for(i = 0; i < noOfSets; i++)
	{
		printf("No. of elements in set [ %d ]  ", i);
		scanf("%d", &key);

		set = (int *)calloc(maxSetNum, sizeof(int));
		set[0] = key;// store the no of elements in each set in the 0th index
		superset[i] = set;// point the superset to the set
		
		printf("Enter the %d elements of set [ %d ]\n", key, i);
		for(j = 1; j <= set[0]; j++)
		{
			scanf("%d", &key);
			set[key] = 1;//the input key insdex of the set is present in the set
		}
		printf("\n");
	}
// print the sets
	for(i = 0; i < noOfSets; i++)
	{
		set = superset[i];
		printf("\n Elements in Set [ %d ] ", i);
		for(j = 1; j < maxSetNum; j++)
			if(set[j] == 1) printf("%d ", j);

	}
	printf("\n");
	return superset;
}
// to find the set which contains max elements
int findMaxSet(int **superset, int noOfSets)
{
	int maxSetIndex = 1;
	int i;
	// find the set with max elements in the superset and the no of elements were stored in 0th index of every set
	for(i = 1; i < noOfSets; i++)
		if(superset[i][0] >= superset[maxSetIndex][0])
			maxSetIndex = i;

	return maxSetIndex;
}
// function to remove elements that are covered from the universal set
void removeelement(int **superset, int noOfSets, int maxSetIndex)
{
	int i,j;
	int *maxSet = superset[maxSetIndex];
	for(i = 1; i < maxSetNum; i++)
	{	// if the i th index element of of max set is present 
		if(maxSet[i] == 1)
		{	// remove the element from remaining sets
			for(j = 0; j < noOfSets; j++)
			{	  
				if(superset[j][i] == 1)
				{
					superset[j][i] = 0;// mark it as covered 
					superset[j][0] = superset[j][0] - 1;// reduce one of element from total no of elements of that set 
				}
			}
		}
	}
}

void setcover(int **superset, int noOfSets)
{
	int *answerSet = (int *)calloc(noOfSets, sizeof(int));
	int maxSetIndex;
	int i;
	
	while(superset[0][0] != 0)// all the elements of the universal set are covered
	{
		maxSetIndex = findMaxSet(superset, noOfSets);// find the set with max no of elements
		answerSet[maxSetIndex] = 1;// mark the index of  covered set 
		removeelement(superset, noOfSets, maxSetIndex);// remove the elements of that set from universal set and corresponding sets
	}

	printf("\nApprox set cover\n");
	for(i = 1; i < noOfSets; i++)
		if(answerSet[i] == 1) printf("SET [ %d ]\n", i);

	free(answerSet);

}

int main()
{
	int i;
	int noOfSets = 0, **superset = NULL;
	printf("\nEnter No. of Sets including the universal set");
	scanf("%d", &noOfSets);
	
	superset = initializeSets(noOfSets);

	setcover(superset, noOfSets);

	for(i = 0; i < noOfSets; i++) free(superset[i]);
	free(superset);
	printf("\n");
	return 0;
}
