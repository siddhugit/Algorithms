#include <iostream>
#include <vector>
#include <deque>
#include <cstring>
#include <cstdlib>
using namespace std;
 
void printResult(const vector<vector<int> >& result,const vector<vector<int> >& A,int N)
{
	for(int i = 0; i < N; ++i){
		bool first = true;
		for(int j = 0; j < N; ++j){
			if(result[i][j] == 0 && A[i][j] > 0){
				if(first){
					cout<<j;
					first = false;
				}
				else{
					cout<<" "<<j;
				}
			}
		}
		cout<<endl;
	}
}
 
vector<vector<int> > multiply(const vector<vector<int> >& A,const vector<vector<int> >& B,int N)
{
	vector<vector<int> > temp(N,vector<int>(N,0));
	for(int i = 0; i < N; ++i){
		for(int j = 0; j < N; ++j){
			for(int k = 0; k < N; ++k){
				temp[i][j] += A[i][k]*B[k][j];
			}
		}
	}
	return temp;
}
 
vector<vector<int> > add(const vector<vector<int> >& A,const vector<vector<int> >& B,int N)
{
	vector<vector<int> > temp(N,vector<int>(N,0));
	for(int i = 0; i < N; ++i){
		for(int j = 0; j < N; ++j){
			temp[i][j] = A[i][j] + B[i][j];
		}
	}
	return temp;
}
 
vector<bool> ConvertToBinary(int n)
{
	vector<bool> bits;
	while(n) {
		bits.push_back(n & 1);
		n = n >> 1;
	}
	return bits;
}

void removeRedundancyImpl(const vector<vector<int> >& A,vector<vector<int> >& B,int N,const vector<bool>& bits,int n,vector<vector<int> >& result)
{
	if(n == 0){
		B = A;
	}
	else if(n == 1)
	{
		B = multiply(A,A,N);
	}
	else
	{
		vector<vector<int> > temp = add(B,A,N);
		B = multiply(temp,temp,N);	
	}
	if(bits[n])
		result = multiply(result,B,N);
	if(n == bits.size() - 1){
		result = add(result,B,N);
		return;
	}
	removeRedundancyImpl(A,B,N,bits,++n,result);
}

void removeRedundancy(const vector<vector<int> >& A,int N)
{
	if(N < 3){
		printResult(vector<vector<int> >(N,vector<int>(N,0)),A,N);
		return;
	}
	vector<vector<int> > B(N,vector<int>(N,0));
	vector<vector<int> > result(N,vector<int>(N,0));
	for(int i = 0; i < N; ++i)
		result[i][i] = 1;
	int n = 0;
	vector<bool> bits = ConvertToBinary(N - 1);
	removeRedundancyImpl(A,B,N,bits,n,result);
	printResult(result,A,N);
}

int main()
{
	char line[1024];
	cin.getline(line,1024);
	int N = atoi(line);
	vector<vector<int> > A(N,vector<int>(N,0));
	vector<bool> nodes(N,true);
	for(int i = 0; i < N; ++i){
		cin.getline(line,1024);
		char *token = strtok(line," ");
		while( token != NULL )
		{
			int num=atoi(token);
			A[i][num] = 1;
			token = strtok(NULL," ");
		}
	}
	removeRedundancy(A,N);
	return 0;
}