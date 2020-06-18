
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;


regex RECT("rect (\\d+)x(\\d+)");
regex ROT_ROW("rotate row y=(\\d+) by (\\d+)");
regex ROT_COL("rotate column x=(\\d+) by (\\d+)");

void apply(string instr, bool (&screen)[6][50]) {
    smatch match;

    if (regex_search(instr, match, RECT)) {
        int x = stoi(match[1]);
        int y = stoi(match[2]);

        for (int i=0; i<y; ++i) {
            for (int j=0; j<x; ++j)
                screen[i][j] = true;
        }

    } else if (regex_search(instr, match, ROT_ROW)) {
        int id = stoi(match[1]);
        int value = stoi(match[2]);

        bool row[50];
        for (int j=0; j<50; ++j)
            row[j] = screen[id][j];

        for (int j=0; j<50; ++j)
            screen[id][j] = row[(j-value+50) % 50];

    } else if (regex_search(instr, match, ROT_COL)) {
        int id = stoi(match[1]);
        int value = stoi(match[2]);

        bool col[6];
        for (int i=0; i<6; ++i)
            col[i] = screen[i][id];

        for (int i=0; i<6; ++i)
            screen[i][id] = col[(i-value+6) % 6];
    }
}

void display(bool const (&screen)[6][50]) {
    for (int i=0; i<6; ++i) {
        for (int j=0; j<50; ++j)
            cout << (screen[i][j] ? '#' : '.');
        cout << endl;
    }
}

int main()
{
	string input;
	int ans = 0;
	vector<string> tab;

	ifstream myfile ("Day8Input");
	if (myfile.is_open())
	{
		while (getline(myfile, input))
            tab.push_back(input);

		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    bool screen[6][50] = {false};

    for (string instr : tab) {
        apply(instr, screen);
    }

    for (int i=0; i<6; ++i) {
        for (int j=0; j<50; ++j) {
            ans += screen[i][j];
        }
    }
    
    display(screen);

	cout << "Answer : " << ans << endl;
	return 0;
}
