#include <iostream>
#include <string.h>

using namespace std;

int bp[1005];
int dp[1005][1005];
int next_break[1005][1005];

int go(int i, int j) {
  if (j == i+1) return 0;
  else if (j == i+2) {
    next_break[i][j] = i + 1;
    return bp[j] - bp[i];
  }
  int &ret = dp[i][j];
  if (ret != -1) return ret;
  ret = 1000000000;

  // do these two calls to make sure the next_break[][] values are filled in
  go(i, j-1);
  go(i+1, j);

  for (int k = next_break[i][j-1]; k <= next_break[i+1][j]; ++k)
    if (bp[j] - bp[i] + go(i, k) + go(k, j) < ret) {
      next_break[i][j] = k;
      ret = bp[j] - bp[i] + go(i, k) + go(k, j);
    }

  return ret;
}

int main() {
  int L, l;
  cin >> L >> l;
  for (int i = 1; i < L+1; ++i)
    cin >> bp[i];
  bp[0] = 0, bp[L+1] = l;
  memset(dp, -1, sizeof(dp));
  cout << go(0, L+1) << endl;
  return 0;
}
