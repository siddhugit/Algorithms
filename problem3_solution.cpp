#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
const int maxn = 35000;

// the invariant we maintain is that if x is a root, max_score[x] is the maximum
// score of anyone in x's team. if x is not a root, max_score[x] is allowed to
// be anything. also, if x is a root then score_diff[x] is x's score; if not
// a root then score_diff[x] equals x's score minus p[x]'s score
int p[maxn], rank[maxn], n, m, l;
long long max_score[maxn], score_diff[maxn];

long long score(int x) {
  if (p[x] == x) return score_diff[x];
  else {
    long long ret = score_diff[x] + score(p[x]);
    p[x] = p[p[x]];
    score_diff[x] = ret - score_diff[p[x]];
    return ret;
  }
}

void compete(int x, int y) {
  // do score() on both x and y to cause path compression
  score(x), score(y);
  // a is the root of x's tree, and b is root of y's
  // after path compression, root of x's tree is either x or p[x]; same for y
  int a = p[x], b = p[y];

  // do union by rank, but also make sure max_score and score_diff continue to
  // satisfy the invariants above
  if (a != b) {
    if (rank[a] <= rank[b]) {
      p[a] = b;
      score_diff[a] = (score_diff[a] + max_score[b]) - score_diff[b];
      max_score[b] += max_score[a];
      if (rank[a] == rank[b])
	rank[b]++;
    } else {
      p[b] = a;
      score_diff[a] += max_score[b];
      max_score[a] += max_score[b];
      score_diff[b] -= score_diff[a];
    }
  }
}

int main() {
  long long sum = 0;
  cin >> n >> m >> l;
  getc(stdin);
  for (int i = 0; i < n; ++i)
    p[i] = i, max_score[i] = 1, score_diff[i] = 1;
  memset(rank, 0, sizeof(rank));
  for (int i = 0; i < m+l; ++i) {
    string s;
    int t[2], ctr = 0;
    getline(cin, s);
    istringstream iss(s);
    while (iss >> s)
      t[ctr++] = atoi(s.c_str());
    if (ctr == 2)
      compete(t[0], t[1]);
    else
      sum += score(t[0]);
  }
  cout << sum << endl;
  return 0;
}
