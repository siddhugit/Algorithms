#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
using  std::vector;
using  std::map;

void merge(vector <int> &A,int left,int mid,int right)
{
     int i = left;    
     int j = mid + 1;
     int k = 0;
	 vector<int> C(right - left + 1);
     while((i <= mid) && (j <= right))
     {
          if (A[i] < A[j])
          {
                 C[k] = A[i];
                 ++i;    
          }
         else
         {
                 C[k] = A[j];
                 ++j;      
         }
        ++k;     
     }
     while (i <= mid)
     {
           C[k] = A[i];
           ++i;
           ++k;
     }
     while (j <= right)
     {
           C[k] = A[j];
           ++j;
           ++k;
     }
	 std::copy(C.begin(),C.end(),A.begin() + left);
}

int position(const vector<int>& A,int index,int base,int sz,int key)
{
	while(index < sz && key > A[index + base])
		++index;
	return index;
}

int compare(vector<int>& west,vector<int>& east,int left,int right)
{
	if(left == right ){
		return 0;
	}
	int mid = (left + right)/2;
	int result = 0;
	result += compare(west,east,left,mid);
	result += compare(west,east,mid + 1,right);
	int pos = 0,wsz = mid - left + 1;
	for(int k = 0; k < wsz; ++k){
		pos = position(east,pos,mid + 1,right - mid,west[k + left]);
		result += pos;
	}
	merge(west,left,mid,right);
	merge(east,left,mid,right);
	return result;
}

int main()
{
	int N;
	scanf("%d",&N);
	vector<int> A(N);
	vector<int> east(N);
	vector<int> west(N);
	map<int,int> eastM;
	map<int,int> westM;
	getc(stdin);
	for(int i = 0; i < N; ++i){
		scanf("%d",&A[i]);
		west[i] = ++westM[A[i]];
	}
	for(int i = N; i > 0; --i){
		east[i - 1] = ++eastM[A[i - 1]];
	}
	int result = compare(west,east,0,N - 1);
	printf("%d",result);
	return 0;
}