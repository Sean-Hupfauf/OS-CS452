
#include <iostream>

using namespace std;

long f(long nonce) {
    const long A = 48271;
    const long M = 2147483647;
    const long Q = M/A;
    const long R = M%A;

	static long state = 1;
	long t = A * (state % Q) - R * (state / Q);
	
	if (t > 0)
		state = t;
	else
		state = t + M;
	return (long)(((double) state/M)* nonce);
}

int main( )
{
  const int MAX = 20;
  long nonce = 5647892341;
  for  (int i = 1; i <= MAX; i++) {

      cout << f(nonce) << endl;

  } 
  return (0);
}
