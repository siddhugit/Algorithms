#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
using namespace std;

void merge(vector<int> &a, vector<int> &b, vector<int> &c) {
  int at1 = 0, at2 = 0, at = 0;
  while (at1<a.size() || at2<b.size())
    if (at1==a.size())
      c[at++] = b[at2++];
    else if (at2==b.size())
      c[at++] = a[at1++];
    else if (a[at1] < b[at2])
      c[at++] = a[at1++];
    else
      c[at++] = b[at2++];
}

long long go(vector<int> &fl, vector<int> &fr) {
  int n = fl.size();
  long long ans = 0;
  if (n == 1) 
    return 0;
  else {
    int mid = n/2;
    vector<int> fl_left, fl_right, fr_left, fr_right;
    for (int i = 0; i < mid; ++i)
      fl_left.push_back(fl[i]), fr_left.push_back(fr[i]);
    for (int i = mid; i < n; ++i)
      fl_right.push_back(fl[i]), fr_right.push_back(fr[i]);
    ans = go(fl_left, fr_left) + go(fl_right, fr_right);
    int at = 0;
    for (int i = 0; i < fl_left.size(); ++i) {
      while (at < fr_right.size() && fl_left[i] > fr_right[at])
	++at;
      ans += at;  
    }
    merge(fl_left, fl_right, fl);
    merge(fr_left, fr_right, fr);
    return ans;
  }
}

int main() {
  int n;
  cin >> n;
  int seen[n], a[n];
  for (int i = 0; i < n; ++i)
    cin >> a[i];

  // make it so that all the species IDs are in the set {0,...,n-1}
  vector< pair<int,int> > b;
  for (int i = 0; i < n; ++i)
    b.push_back(make_pair(a[i], i));
  sort(b.begin(), b.end());
  int val = 0;
  for (int i = 0; i < n; ++i) {
    if (i>0 && b[i].first!=b[i-1].first)
      ++val;
    a[b[i].second] = val;
  }

  // from_left[i] is the number of animals with species a[i] amongst the animals
  // a[0],a[1],...,a[i]
  // from_right[i] is the number of animals with species a[i] amongst the animals
  // a[i],a[i+1],...,a[n-1]
  vector<int> from_left = vector<int>(n, 0);
  vector<int> from_right = vector<int>(n, 0);

  memset(seen, 0, sizeof(seen));
  for (int i = 0; i < n; ++i) {
    seen[a[i]]++;
    from_left[i] = seen[a[i]];
  }

  memset(seen, 0, sizeof(seen));
  for (int i = n-1; i >= 0; --i) {
    seen[a[i]]++;
    from_right[i] = seen[a[i]];
  }

  cout << go(from_left, from_right) << endl;

  return 0;
}
