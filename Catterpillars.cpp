#include <iostream>
#include <cstdio>
#include <limits.h>
#include <string.h>
#include <queue>
using namespace std;

bool bfs(int** rGraph, int s, int t, int parent[],int V)
{
    bool* visited = new bool[V];
    for(int i = 0; i < V; ++i)
            visited[i] = false;
    queue <int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v=0; v<V; v++)
        {
            if (visited[v]==false && rGraph[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return (visited[t] == true);
}
int fordFulkerson(int** graph, int s, int t,int V)
{
    int u, v;
      int** rGraph = new int*[V];
      for(int i = 0; i < V; ++i)
            rGraph[i] = new int[V]; 
    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
             rGraph[u][v] = graph[u][v];
    int* parent = new int[V]; 
    int max_flow = 0; 
    while (bfs(rGraph, s, t, parent,V))
    {
        int path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        max_flow += path_flow;
    }
    return max_flow;
}
 
vector<vector<int> > createGraph(int n,vector<bool> nodePositions)
{
      int num = (n*n) + 2;
	  int sz = 2*num - 2;
      vector<vector<int> > graph(sz,vector<int>(sz,0));
      //first source
      for(int i = 1;i < num - 1; ++i){
            if(nodePositions[i - 1])
                  graph[0][2*i - 1] = 1;
      }
      //nodes
      for(int i = 1;i <= n; ++i){
            for(int j = 1;j <= n; ++j){
                  int p,q,pos,nodePos = (i - 1)*(n) + j;
				  graph[2*nodePos - 1][2*nodePos] = 1;
                  //edge nodes
                  if(i == 1 || j == 1 || i == n || j == n){
                        graph[2*nodePos][sz - 1] = 1;
						
                       //graph[num - 1][nodePos] = 1;
                        //i + 1,j
                        p = i + 1;q = j;
                        if(p <= n){
                              pos = (p - 1)*(n) + q;
                              graph[2*nodePos][2*pos - 1] = 1;
                        }
                        //i - 1,j
                        p = i - 1;q = j;
                        if(p > 0){
                              pos = (p - 1)*(n) + q;
                              graph[2*nodePos][2*pos - 1] = 1;
                        }
                        //i,j + 1
                        p = i;q = j + 1;
                        if(q <= n){
                              pos = (p - 1)*(n) + q;
                              graph[2*nodePos][2*pos - 1] = 1;
                        }
                        //i,j - 1
                        p = i;q = j - 1;
                        if(q > 0){
                              pos = (p - 1)*(n) + q;
                              graph[2*nodePos][2*pos - 1] = 1;
                        }
                  }
                  //non-edge nodes
                  else
                  {
                        //i + 1,j
                        p = i + 1;q = j;
                        pos = (p - 1)*(n) + q;
                        graph[2*nodePos][2*pos - 1] = 1;
                        //i - 1,j
                        p = i - 1;q = j;
                        pos = (p - 1)*(n) + q;
                        graph[2*nodePos][2*pos - 1] = 1;
                        //i,j + 1
                        p = i;q = j + 1;
                        pos = (p - 1)*(n) + q;
                        graph[2*nodePos][2*pos - 1] = 1;
                        //i,j - 1
                        p = i;q = j - 1;
                        pos = (p - 1)*(n) + q;
                        graph[2*nodePos][2*pos - 1] = 1;
                  }
            }
      }
      return graph;
}
 
int main()
{
      int T;
	  scanf("%d",&T);
	  for(int i = 0;i < T;++i)
	  {
		  int m,n;
		  scanf("%d",&n);
		  scanf("%d",&m);
		  int num = (n*n);
		  vector<bool> nodePositions(num,false);
		  int x,y;
		  for(int i = 0;i < m;++i)
		  {
			scanf("%d",&x);
			scanf("%d",&y);
			int nodePos = (x - 1)*(n) + y - 1; 
			nodePositions[nodePos] = true;
		  }
		  vector<vector<int> > tempgraph = createGraph(n,nodePositions);
	      
		  
		  int V = 2*num + 2;
		  int** graph = new int*[V];
		  for(int i = 0; i < V; ++i)
				graph[i] = new int[V];
		  for (int u = 0; u < V; u++)
			for (int v = 0; v < V; v++)
				 graph[u][v] = tempgraph[u][v];
		  int maxFlowRet = fordFulkerson(graph, 0, V - 1,V);
		  if(maxFlowRet >= m){
			  printf("possible\n");
		  }
		  else{
			  printf("not possible\n");
			  printf("%d\n",maxFlowRet);
		  }
	  }
    return 0;
}