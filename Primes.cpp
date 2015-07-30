#include<cstdio>
#include<cstring>
#include <cstdlib>

int main()
{
	char* pattern = (char*)malloc(8388608);
	char* text = (char*)malloc(8388608);
	gets(pattern);
	gets(text);
	int M = strlen(pattern);
	int N = strlen(text);
	long prime = 9576890767;
	long base = 1;
	for (int i = 0; i < M-1; ++i)
		base = (base << 8)%prime;
	long hashp = 0,hasht = 0;
	for (int i = 0; i < M; i++)
	{
		hashp = ((hashp << 8) + pattern[i])%prime;
		hasht = ((hasht << 8) + text[i])%prime;
	}
	int result = 0;
	for (int i = 0; i <= N - M; i++)
	{
		if ( hasht == hashp )
		{
			++result;
		}
		if(i < N - M)
		{
			hasht = (((hasht - text[i]*base) << 8) + text[i+M])%prime;
			if(hasht < 0) 
				hasht += prime;
		}
	}
	printf("%d",result);
	return 0;
}