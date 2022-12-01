
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <regex>

using namespace std;


struct Node {
    int x;
    int y;
    int size;
    int used;
    int avail;
    int percent;

    void set(string input) {
        regex re("/dev/grid/node-x(\\d+)-y(\\d+)\\s*(\\d+)T\\s*(\\d+)T\\s*(\\d+)T\\s*(\\d+)%");
        smatch match;

        if (!regex_search(input, match, re)) {
            cout << "Error ! Node could not be set properly" << endl;
            return;
        }

        x = stoi(match[1]);
        y = stoi(match[2]);
        size = stoi(match[3]);
        used = stoi(match[4]);
        avail = stoi(match[5]);
        percent = stoi(match[6]);
    }
};


ostream& operator<<(ostream& os, const Node& node) {
    os << "((" << node.x << ", " << node.y << "), " << node.size << ", " << node.used << ", " << node.avail << ")";
    return os;
}

int main()
{
    bool first_half = false;
	string input;
	int ans = 0;
	array<array<Node, 30>, 33> nodes;

	ifstream myfile ("input");
	if (myfile.is_open())
	{
		getline(myfile, input);
		getline(myfile, input);
        int x = 0;
        int y = 0;
        while (getline(myfile, input)) {
            nodes[x][y].set(input);
            y++;
            if (y == 30) {
                x++;
                y = 0;
            }
        }

		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    if (first_half) {
        for (int i=0; i<33*30; ++i) {
            int x1 = i/30, y1 = i%30;

            for (int j=i+1; j<33*30; ++j) {
                int x2 = j/30, y2 = j%30;

                if (nodes[x1][y1].used > 0 && nodes[x1][y1].used <= nodes[x2][y2].avail)
                    ans++;

                if (nodes[x2][y2].used > 0 && nodes[x2][y2].used <= nodes[x1][y1].avail)
                    ans++;
            }
        }
    } else {
        for (int y=0; y<30; ++y) {
            for (int x=0; x<33; ++x) {
                if (x == 32 && y == 0) {
                    cout << "G";
                } else if (nodes[x][y].size > 500) {
                    cout << "#";
                } else if (nodes[x][y].avail > 50) {
                    cout << "_";
                } else {
                    cout << ".";
                }
            }
            cout << endl;
        }
        ans = 69; // Nb of steps to bring hole to (31, 0)

        // It takes 5 steps to move {hole + goal} one node to the left
        // So it takes 5*31 steps to bring hole to (0, 0) and goal to (1, 0)
        ans += 5*31;

        ans++; // Last movement to bring goal to (0, 0)
    }

	cout << "Answer : " << ans << endl;
	return 0;
}
