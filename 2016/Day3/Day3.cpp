
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;


int main()
{
    bool first_half = false;
	string input;
	int ans = 0;
	vector<tuple<int, int, int>> tab;

	ifstream myfile ("input");
	if (myfile.is_open())
	{
        int a, b, c;
        while (myfile >> a >> b >> c) {
            tab.push_back({a, b, c});
        }
		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    if (first_half) {
        for (auto const& [a, b, c] : tab) {
            if (a + b > c && a + c > b && b + c > a)
                ans++;
        }
    } else {
        for (unsigned int i=0; i<tab.size(); i+=3) {
            auto [a1, a2, a3] = tab[i];
            auto [b1, b2, b3] = tab[i+1];
            auto [c1, c2, c3] = tab[i+2];

            if (a1 + b1 > c1 && a1 + c1 > b1 && b1 + c1 > a1)
                ans++;

            if (a2 + b2 > c2 && a2 + c2 > b2 && b2 + c2 > a2)
                ans++;

            if (a3 + b3 > c3 && a3 + c3 > b3 && b3 + c3 > a3)
                ans++;
        }
    }

	cout << "Answer : " << ans << endl;
	return 0;
}
