
#include <iostream>
#include <string>
#include <vector>

#include "md5.cpp"

using namespace std;


int main()
{
    bool first_half = true;
	string input = "ojvtpuvg";
	string ans = "________";
    cout << "Answer = " << ans << "\r" << flush;

    int index = 0;
    int nb_letters = 0;
    while (nb_letters < 8) {
        string hash = md5(input + to_string(index));
        if (hash.substr(0, 5) == "00000") {
            if ('0' <= hash[5] && hash[5] < '8' && ans[hash[5] - '0'] == '_') {
                ans[hash[5] - '0'] = hash[6];
                cout << "Answer = " << ans << "\r" << flush;
                nb_letters++;
            }
        }
        index++;
    }

	cout << "Answer : " << ans << endl;
	return 0;
}
