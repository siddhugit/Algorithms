#include <cstdio>
#include <vector>
#include <climits>
using namespace std;

int main()
{
	int M;
		scanf("%d",&M);
	int N;
	scanf("%d",&N);
	getc(stdin);
	int temp;
	vector<int> cuts(M);
	for(int i = 0; i < M; ++i){
		scanf("%d",&temp);
		cuts[i] = temp - 1;
	}
	vector<vector<int> > f(M,vector<int>(M,0));
	f[0][0] = cuts[1] + 1;
	for(int i = 1; i < M - 1; ++i){
		f[i][i] = cuts[i + 1] - cuts[i - 1];
	}
	f[M - 1][M - 1] = N - 1 - cuts[M - 2];
	vector<vector<int> > root(M,vector<int>(M,0));
	for(int i = 0; i < M; ++i){
		root[i][i] = i;
	}
	for(int len = 1;len < M; ++len){
		for(int i = 0;i < M - len; ++i){
			int j = i + len;
			int lower_bound = -1,upper_bound = N - 1,minSoFar = INT_MAX;
			if(i > 0)
			lower_bound = cuts[i - 1];
			if(j < M - 1)
			upper_bound = cuts[j + 1];
			int r1 = root[i][j - 1],r2 = root[i + 1][j];
			for(int k = r1;k <= r2; ++k){
				int cost = (upper_bound - lower_bound);
				if(k == i)
					cost += f[k + 1][j];
				else if(k == j)
					cost += f[i][k - 1];
				else
					cost += (f[i][k - 1] + f[k + 1][j]);
				if(minSoFar > cost){
					root[i][j] = k;
					minSoFar = cost;
				}
			}
		f[i][j] = minSoFar;
		}
	}
	printf("%d",f[0][M - 1]);
	return 0;
}