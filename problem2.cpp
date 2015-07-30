// this solution encodes configurations into ints of size at most 4mn 3^{k-2}

#include <iostream>
#include <queue>
#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

typedef vector< pair<int,int> > Snake;

const int maxm = 12;
const int maxn = 12;

int n, m, k;

// grid[i][j] is 0 if there's a wall, else it's 1
char grid[12][12];
pair<int, int> apple;

queue< pair<Snake, int> > Q;
// number of states is m*n*4*3^{k-2}
// m*n for the head, 4 for the next body piece relative to the head, then 3
// possibilities for every subsequent piece of the snake's body
const int N = 1259712;
char dist[N];

int dirs[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
// from any square you can go down, up, left, right
// encoding[][] maps these options to 0, 1, 2, 3
// encoding[dx+1][dy+1] does the mapping. the +1 is so that we don't give
// negative indices to the array. ignore 9's.
int encoding[3][3] = { {9,0,9}, {2,9,3}, {9,1,9} };

int encode(Snake snake) {
  int ret = 0;
  ret += snake[0].first*maxn + snake[0].second;
  if (snake.size() > 1) {
    ret <<= 2;
    ret |= encoding[snake[1].first-snake[0].first+1][snake[1].second-snake[0].second+1];
  }
  for (int i = 0; i < int(snake.size())-2; ++i) {
    ret *= 3;
    int at = 0;
    for (int j = 0; j < 4; ++j) {
      int dx = dirs[j][0];
      int dy = dirs[j][1];
      if (snake[i].first-snake[i+1].first==dx && snake[i].second-snake[i+1].second==dy)
	continue;
      if (snake[i+2].first-snake[i+1].first==dx && snake[i+2].second-snake[i+1].second==dy)
	break;
      ++at;
    }
    ret += at;
  }
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
}

Snake move(Snake snake, int dx, int dy) {
  Snake ret;
  ret.push_back(make_pair(snake[0].first+dx, snake[0].second+dy));
  for (int i = 0; i+1 < snake.size(); ++i)
    ret.push_back(snake[i]);
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
