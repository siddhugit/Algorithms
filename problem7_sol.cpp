#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>

#define MAXN 5010 // 50*50*2
#define MAXM 3000 
#define INF 9999

using namespace std;

const int dx[] = {0, 1, -1, 0};
const int dy[] = {1, 0, 0, -1};
int n,m;
int tests;

// Represent a vertex of the grid as 2*(n*x+y) + {0,1}
// where 0 is the in-vertex, 1 is the out-vertex

// source, sink
int S = 5000;
int T = 5001;

// Stores the locations of the caterpillars
int loc[MAXM];

// adjacency lists
int adj[MAXN][MAXN]; // largest will be for S, T
int adj_size[MAXN];

int maxflow = 0;
int cap[MAXN][MAXN]; // capacity is always 1
int flow[MAXN][MAXN];
int par[MAXN];

// augment a path
int augment(int v, int f) {
  if (v == S) {
    maxflow += f;
    return f;
  }

  int u = par[v];
  bool backflow = false;
  if (flow[v][u] > 0) {
    backflow = true;
  }
  if (backflow) {
    f = min(f, flow[v][u]);
  } else {
    f = min(f, cap[u][v] - flow[u][v]);
  }
  f = augment(u, f);
  if (backflow) {
    flow[v][u] -= f;
  } else {
    flow[u][v] += f;
  }
  return f;
}

void add_adj(int u, int v) {
  cap[u][v] = 1;
  adj[u][adj_size[u]] = v;
  ++adj_size[u];
  adj[v][adj_size[v]] = u;
  ++adj_size[v];
}

void solve() {
  cin >> n >> m;
  memset(adj, 0, sizeof(adj));
  memset(adj_size, 0, sizeof(adj_size));
  memset(cap, 0, sizeof(cap));
  memset(flow, 0, sizeof(flow));
  memset(par, -1, sizeof(par));
  maxflow = 0;

  for (int j=0;j<m;++j) {
    int x,y;
    cin >> x >> y;
    --x; --y;
    loc[j] = 2*(n*x + y);
    add_adj(S, loc[j]);
    cap[S][loc[j]] = 1;
  }

  // set capacities and adjacent vertices
  for (int x=0;x<n;++x) {
    for (int y=0;y<n;++y) {
      int u = 2*(n*x + y);
      add_adj(u, u+1);
      for (int k=0;k<4;++k) {
        int v = 2*(n*(x + dx[k]) + (y+dy[k]));
        if (v < 0 || v >= 2*n*n)
          continue;
        add_adj(u+1, v);
        add_adj(v+1, u);
      }
    }
  }

  // sink
  // edges of grid
  for (int x=0;x<n;++x) {
    int v = 2*(n*x) + 1;
    add_adj(v, T);
    v = 2*(n*x + n-1) + 1;
    add_adj(v,T);
  }
  for (int y=1;y<n-1;++y) {
    int v = 2*(y) + 1;
    add_adj(v,T);
    v = 2*(n*(n-1) + y) + 1;
    add_adj(v,T);
  }

  // run ford-fulkerson
  while (1) {
    queue<int> q;
    memset(par, -1, sizeof(par));
    q.push(S);
    par[S] = S;
    bool augmented = false;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int i=0;i<adj_size[u];++i) {
        int v = adj[u][i];
        if (par[v] != -1) continue;
        if (cap[u][v] - flow[u][v] > 0 || flow[v][u] > 0) {
          par[v] = u;
          if (v == T) {
            augment(T, INF);
            augmented = true;
            break;
          } else {
            q.push(v);
          }
        }
      }
      if (augmented) break;
    }
    if (!augmented) break; // no more augmenting paths
  }

  if (maxflow == m)
    printf("possible\n");
  else if (maxflow < m) {
    printf("not possible\n");
    printf("%d\n", maxflow);
  }
}

int main() {
  cin >> tests;
  for (int t=0;t<tests;++t)
    solve();

  return 0;
}