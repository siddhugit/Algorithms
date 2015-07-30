// this solution encodes configurations into ints of size at most mn 4^{k-1},
// which is fast enough to get full credit and has a slightly simpler encode()
// function

#include <iostream>
#include <queue>
#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int maxm = 12;
const int maxn = 12;

int n, m, k;

typedef vector< pair<int,int> > Snake;

// grid[i][j] is 0 if there's a wall, else it's 1
char grid[12][12];
pair<int, int> apple;

// An element of the queue is a state (where the snake is on the board) paired
// with the shortest path distance to that state from the start state
queue< pair<Snake, int> > Q;

// number of states is m*n*4^{k-1}
// m*n for the head, 4 for every subsequence body piece (up, down, left, right)
const int N = 9437184;
char dist[N];

int dirs[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
int encoding[3][3];

inline int encode(Snake snake) {
  int ret = 0;
  ret += snake[0].first*maxn + snake[0].second;
  for (int i = 0; i < k-1; ++i)
    ret = (ret<<2) | encoding[snake[i+1].first-snake[i].first+1][snake[i+1].second-snake[i].second+1];
  return ret;
}

inline void enqueue(Snake snake, int d) {
  int E = encode(snake);
  if (dist[E] == -1) {
    Q.push(make_pair(snake, d));
    dist[E] = d;
  }
}

void init() {
  cin >> m >> n >> k;
  getc(stdin);
  memset(grid, 0, sizeof(grid));
  Q = queue< pair<Snake, int> >();
  for (int i = 0; i < N; ++i)
    dist[i] = -1;
  // from any square you can go down, up, left, right
  // encoding[][] maps these options to 0, 1, 2, 3
  // encoding[dx+1][dy+1] does the mapping. the +1 is so that we don't give
  // negative indices to the array
  encoding[-1+1][0+1] = 0;
  encoding[1+1][0+1] = 1;
  encoding[0+1][-1+1] = 2;
  encoding[0+1][1+1] = 3;
}

Snake move(Snake snake, int dx, int dy) {
  Snake ret = Snake(k);
  ret[0] = make_pair(snake[0].first+dx, snake[0].second+dy);
  for (int i = 0; i+1 < snake.size(); ++i)
    ret[i+1] = snake[i];
  return ret;
}

int main() {
  init();
  Snake snake;
  // snake[i] gives the coordinates of ith part of body of snake
  snake = Snake(k);
  for (int i = 0; i < m; ++i) {
    string s;
    getline(cin, s);
    for (int j = 0; j < n; ++j)
      if (s[j] != 'X') {
	grid[i+1][j+1] = 1;
	if (s[j] == 'A')
	  apple = make_pair(i+1, j+1);
	else if (s[j]>='1' && s[j]<='9')
	  snake[s[j]-'1'] = make_pair(i+1, j+1);
      }
  }
 
  // BFS
  enqueue(snake, 0);
  while (Q.size()) {
    snake = Q.front().first;
    int D = Q.front().second;
    Q.pop();
    if (snake[0].first == apple.first && snake[0].second == apple.second) {
      cout << D << endl;
      return 0;
    }
    for (int d = 0; d < 4; ++d) {
      int nx = snake[0].first + dirs[d][0];
      int ny = snake[0].second + dirs[d][1];

      // snake shouldn't move into an obstacle
      if (!grid[nx][ny])
	continue;

      // also shouldn't move into its own body
      bool bad = false;
      for (int i = 1; i+1 < k; ++i)
	if (nx==snake[i].first && ny==snake[i].second) {
	  bad = true;
	  break;
	}
      if (bad)
	continue;

      enqueue(move(snake, dirs[d][0], dirs[d][1]), D + 1);
    }
  }
  cout << -1 << endl;
  return 0;
}
