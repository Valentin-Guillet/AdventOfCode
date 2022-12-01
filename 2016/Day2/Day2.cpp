
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;


map<char, vector<char>> NEIGHBOURS =  {
    {'1', {'1', '1', '3', '1'}},
    {'2', {'2', '3', '6', '2'}},
    {'3', {'1', '4', '7', '2'}},
    {'4', {'4', '4', '8', '3'}},
    {'5', {'5', '6', '5', '5'}},
    {'6', {'2', '7', 'A', '5'}},
    {'7', {'3', '8', 'B', '6'}},
    {'8', {'4', '9', 'C', '7'}},
    {'9', {'9', '9', '9', '8'}},
    {'A', {'6', 'B', 'A', 'A'}},
    {'B', {'7', 'C', 'D', 'A'}},
    {'C', {'8', 'C', 'C', 'B'}},
    {'D', {'B', 'D', 'D', 'D'}}
};



int main()
{
    bool first_half = false;
	string input;
	string ans;
	vector<string> tab;

	ifstream myfile ("input");
	if (myfile.is_open())
	{
        while (getline(myfile, input))
            tab.push_back(input);

		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }


    if (first_half) {
        int value = 5;

        for (string line : tab) {
            for (char dir : line) {
                if (dir == 'U') {
                    if (value > 3)
                        value -= 3;
                } else if (dir == 'D') {
                    if (value < 7)
                        value += 3;
                } else if (dir == 'L') {
                    if (value % 3 != 1)
                        value--;
                } else {
                    if (value % 3 != 0)
                        value++;
                }
            }
            ans += to_string(value);
        }

    } else {
        char value = '5';
        for (string line : tab) {
            for (char dir : line) {
                int index = (dir == 'R') + 2*(dir == 'D') + 3*(dir == 'L');
                value = NEIGHBOURS[value][index];
            }
            ans += value;
        }
    }

	cout << "Answer : " << ans << endl;
	return 0;
}
