#include <stdio.h>
//#include <vector>
#include <string.h>
#include <stdlib.h>
//using std::vector;

struct Node
{
	int size;  
	int score;
	int parentScore;
	struct Node* parent;
	int rank;
	//Node():rank(0),parent(this),score(1),parentScore(1),size(1){}
};

struct Node* FindSet(struct Node *node)
{
	if(node->parent == node)
		return node;
	struct Node *newRoot = FindSet(node->parent);
	//if(node->parent->score != node->parentScore)
	node->score += ( node->parent->score - node->parentScore); 
	node->parent = newRoot;
	node->parentScore = newRoot->score; 
	return (newRoot);
}

void Union(struct Node *wnode,struct Node *lnode)
{
	wnode->score += lnode->size;
	if(wnode->rank > lnode->rank){
		lnode->parent = wnode;
		lnode->parentScore = wnode->parentScore = wnode->score;
		wnode->size += lnode->size;
	}
	else{
		wnode->parent = lnode;
		lnode->parentScore = wnode->parentScore = lnode->score;
		lnode->size += wnode->size;
		if(wnode->rank == lnode->rank)
			++(lnode->rank);
	}
}

int main()
{
	int N,L,M;
	scanf("%d%d%d",&N,&M,&L);
	struct Node DisjointSet[N];
	for(int i = 0; i < N; ++i)
	{
		//rank(0),parent(this),score(1),parentScore(1),size(1)
		DisjointSet[i].score = DisjointSet[i].parentScore = DisjointSet[i].size = 1;
		DisjointSet[i].parent = DisjointSet + i;
		DisjointSet[i].rank = 0;
	}
	getc(stdin);
	int sz = M + L;
	int input[2];
	char line[32];
	unsigned long result = 0;
	for(int i = 0; i < sz; ++i)
	{	
		gets(line);
		int j = 0;
		char *token = strtok(line," ");
		while( token != NULL ) 
        {
              input[j++] = atoi(token);
              token = strtok(NULL," ");
        }
		if(j == 2){
				struct Node* wnode = FindSet(DisjointSet + input[0]); 
				struct Node* lnode = FindSet(DisjointSet + input[1]);
				if(wnode != lnode)
				{
					Union(wnode,lnode);
				}
		}
		else{
			FindSet(DisjointSet + input[0]);
			result += (DisjointSet[input[0]]).score;
		}
	}
	printf("%lu",result);
	return 0;
}