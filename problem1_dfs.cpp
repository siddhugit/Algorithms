#include <iostream>
#include <vector>
#include <sstream>
#include <string.h>
using namespace std;

// adjacency list representation
// adj[i] is a vector<int> of all vertices j such that (i,j) is an edge
vector< vector<int> > adj;

// adjacency matrix
char A[250][250];

// reachable[i][j] is 1 if i has a path to j (of length at least one),
// and is 0 otherwise
char reachable[250][250];

char visited[250];
void dfs(int v, int start) {
  if (v != start)
    reachable[start][v] = 1;
  visited[v] = 1;
  for (int i = 0; i < adj[v].size(); ++i)
    if (!visited[adj[v][i]])
      dfs(adj[v][i], start);
}

int main() {
  int n;
  cin >> n;
  cin.ignore(1, '\n');
  
  memset(A, 0, sizeof(A));
  memset(reachable, 0, sizeof(reachable));
    
  // Read input and fill in adjacency list and matrix
  for (int i = 0; i < n; ++i) {
    int vertex;
    string line;
    vector<int> V = vector<int>();
    getline(cin, line);
    istringstream iss(line);
    while (iss >> vertex) {
      V.push_back(vertex);
      A[i][vertex] = 1;	    
    }
    adj.push_back(V);       
  }
  
  for (int i = 0; i < n; ++i) {
    // figure out what is reachable from i and put it in reachable[i]
    memset(visited, 0, sizeof(visited));
    dfs(i, i);
  }
  
  // the new adjacency list representation after removing edges
  vector< vector<int> > new_adj_list;
  for (int i = 0; i < n; ++i)
    new_adj_list.push_back(vector<int>());

  for (int i = 0; i < n; ++i) {
    // mark all vertices k such that i has a path of length at least 2 to k
    char mark[n];
    memset(mark, 0, sizeof(mark));
    for (int j = 0; j < adj[i].size(); ++j)
      for (int k = 0; k < n; ++k)
	if (reachable[adj[i][j]][k])
	  mark[k] = 1;
    // copy over the adjacency list to new_adj_list, removing marked neighbors
    for (int j = 0; j < adj[i].size(); ++j)
      if (!mark[adj[i][j]])
	new_adj_list[i].push_back(adj[i][j]);
  }
  
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < new_adj_list[i].size(); ++j) {
      if (j)
	cout << " ";
      cout << new_adj_list[i][j];
    }
    cout << endl;
  }
  
  return 0;
}    
