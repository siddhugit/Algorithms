using namespace std;
#include <iostream>

int main(){
  int answer = 0;

  // get input
  string a, b;
  cin >> a >> b;

  // choose primes to use for fingerprints
  int num_primes = 2;
  int p[num_primes];
  p[1] = 20000003;
  p[0] = 20000023;
  
  // initialize the fingerprints
  long a_fingerprint[num_primes];
  long b_fingerprint[num_primes];

  for (int i=0; i<num_primes; i++){
    a_fingerprint[i] = 0;
    b_fingerprint[i] = 0;
  }

  long position[num_primes];
  for (int j=0; j<num_primes; j++){
  position[j] = 1;
    for (int i = a.length() - 1; i >= 0; i--){
      a_fingerprint[j] = (a_fingerprint[j] + (a.at(i) - 'a') * position[j]) % p[j];
      b_fingerprint[j] = (b_fingerprint[j] + (b.at(i) - 'a') * position[j]) % p[j];
      if (i > 0) position[j] = (position[j] * 26) % p[j];
    }
  }

  // check for a match and adjust B's fingerprint
  for (int i = 0; i < b.length() - a.length(); i++){
    int match = 1;
    for (int j=0; j<num_primes; j++){
      if (a_fingerprint[j] != b_fingerprint[j]) match = 0;
    }
    if (match) answer += 1;

    for (int j=0; j<num_primes; j++){
      b_fingerprint[j] = (b_fingerprint[j] - (b.at(i) - 'a') * position[j]) * 26 + (b.at(i+a.length()) - 'a');
      b_fingerprint[j] %= p[j];
      if (b_fingerprint[j] < 0) b_fingerprint[j] += p[j];
    }
  }

  // check for a final match
  int match = 1;
  for (int j=0; j<num_primes; j++){
    if (a_fingerprint[j] != b_fingerprint[j]) match = 0;
  }
  if (match) answer += 1;

  cout << answer << endl;
}
