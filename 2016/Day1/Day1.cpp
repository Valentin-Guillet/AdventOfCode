
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;


int main()
{
    bool first_half = false;
	string input;
	int ans = 0;
	vector<string> tab;

	ifstream myfile ("Day1Input");
	if (myfile.is_open())
	{
		while (getline(myfile, input, ','))
            tab.push_back(input.substr(1));

		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    int x = 0, y = 0;
    pair<int, int> orientation = {0, 1};
    set<pair<int, int>> locations;
    locations.insert({x, y});

    for (string cmd : tab) {
        if (cmd[0] == 'L') {
            orientation = {-orientation.second, orientation.first};
        } else {
            orientation = {orientation.second, -orientation.first};
        }
        
        int dist = stoi(cmd.substr(1));
        for (int i=0; i<dist; ++i) {
            x += orientation.first;
            y += orientation.second;

            if (!first_half && locations.find({x, y}) != locations.end())
                goto break_out;

            locations.insert({x, y});
        }
    }

break_out:
    ans = abs(x) + abs(y);

	cout << "Answer : " << ans << endl;
	return 0;
}
