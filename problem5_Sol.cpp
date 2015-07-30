#include <iostream>

#define USE_N2_SOL 0

/*
 *  Based on Joy's Python solution.
 */

using namespace std;

int main() {

  int L, length;
  cin >> L >> length;

  int breakpoints[L + 2];
  breakpoints[0] = 0;
  for (int i = 1; i < L+1; i++) 
    cin >> breakpoints[i];
  breakpoints[L+1] = length;
  
  int sols[L+2][L+2];
  int sols_break[L+2][L+2];
  
  for (int i = 0; i < L+1; i++)
    sols[i][i+1] = 0;
  
  for (int i = 0; i < L; i++) {
    sols[i][i+2] = breakpoints[i+2] - breakpoints[i];
    sols_break[i][i+2] = i+1;
  }
  
  for (int d = 3; d < L + 2; d++) {
    for (int i = 0; i < L + 2 - d; i++) {
      int distance = breakpoints[i+d] - breakpoints[i];
      int range_l = USE_N2_SOL ? sols_break[i][i+d-1] : i + 1;
      int range_r = USE_N2_SOL ? sols_break[i+1][i+d] + 1 : i + d;
      int min_recur = 1000000000;
      int next_break;
      while (range_l < range_r) {
	int candidate = sols[i][range_l] + sols[range_l][i+d];
	if (candidate < min_recur) {
	  min_recur = candidate;
	  next_break = range_l;
	}
	range_l++;
      }
      sols[i][i+d] = distance + min_recur;
      sols_break[i][i+d] = next_break;
    }
  }
  
  cout << sols[0][L+1] << endl;
}
