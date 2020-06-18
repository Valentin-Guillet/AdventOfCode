/* This problem is actually the Josephus Problem */

#include <iostream>
#include <cmath>

using namespace std;


int main()
{
    bool first_half = false;
	int input = 3004953;
	int ans;

    if (first_half) {
        int pwr = 1;
        while (pwr < input)
            pwr <<= 1;

        ans = 2 * (input - (pwr >> 1)) + 1;

    } else {
        ans = 1;
        for (int n=2; n<input+1; ++n) {

            ans++;
            if (ans >= ceil((n+1) / 2.0))
                ans++;

            if (ans == n+1)
                ans -= n;
        }
    }

	cout << "Answer : " << ans << endl;
	return 0;
}
