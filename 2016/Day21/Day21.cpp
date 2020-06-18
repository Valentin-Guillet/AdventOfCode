
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;


regex SWAP_IND("swap position (\\d+) with position (\\d+)");
regex SWAP_LET("swap letter ([a-z]) with letter ([a-z])");
regex ROTATE("rotate (left|right) (\\d+) steps?");
regex ROTATE_POS("rotate based on position of letter ([a-z])");
regex REVERSE("reverse positions (\\d+) through (\\d+)");
regex MOVE("move position (\\d+) to position (\\d+)");


string scramble(string instr, string password, bool unsramble) {
    string ans = password;
    smatch match;

    if (regex_search(instr, match, SWAP_IND)) {
        int id1 = stoi(match[1]), id2 = stoi(match[2]);
        ans[id1] = password[id2];
        ans[id2] = password[id1];

    } else if (regex_search(instr, match, SWAP_LET)) {
        char c1 = match[1].str()[0], c2 = match[2].str()[0];
        for (size_t i=0; i<ans.size(); ++i) {
            if (ans[i] == c1) {
                ans[i] = c2;
            } else if (ans[i] == c2) {
                ans[i] = c1;
            }
        }

    } else if (regex_search(instr, match, ROTATE)) {
        int n = stoi(match[2]);
        if ((!unsramble && match[1] == "left") || (unsramble && match[1] == "right")) {
            ans.erase(0, n);
            ans += password.substr(0, n);
        } else {
            ans.erase(ans.size() - n);
            ans = password.substr(password.size() - n) + ans;
        }

    } else if (regex_search(instr, match, ROTATE_POS)) {
        int id = ans.find(match[1].str()[0]);

        if (!unsramble) {
            size_t n = id+1;
            if (id >= 4)
                n++;

            if (n > password.size())
                n -= password.size();

            ans.erase(ans.size() - n);
            ans = password.substr(password.size() - n) + ans;
        } else {
            int n;
            if (id == 0 || id == 1) {
                n = 1;
            } else if (id == 2) {
                n = 6;
            } else if (id == 3) {
                n = 2;
            } else if (id == 4) {
                n = 7;
            } else if (id == 5) {
                n = 3;
            } else if (id == 7) {
                n = 4;
            }
            ans.erase(0, n);
            ans += password.substr(0, n);
        }
            

    } else if (regex_search(instr, match, REVERSE)) {
        int id1 = stoi(match[1]), id2 = stoi(match[2]);
        reverse(ans.begin()+id1, ans.begin()+id2+1);

    } else if (regex_search(instr, match, MOVE)) {
        int id1, id2;

        if (!unsramble) {
            id1 = stoi(match[1]);
            id2 = stoi(match[2]);
        } else {
            id1 = stoi(match[2]);
            id2 = stoi(match[1]);
        }
        ans.erase(id1, 1);

        ans.insert(ans.begin()+id2, password[id1]);

    } else {
        cout << "ERROR instr = " << instr << endl;
    }

    return ans;
}

int main()
{
    bool first_half = false;
	string input;
	vector<string> tab;

	ifstream myfile ("Day21Input");
	if (myfile.is_open())
	{
		while (getline(myfile, input))
            tab.push_back(input);

		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    string password = (first_half ? "abcdefgh" : "fbgdceah");
    if (!first_half)
        reverse(tab.begin(), tab.end());

    for (string instr : tab)
        password = scramble(instr, password, !first_half);

    cout << "Answer : " << password << endl;
	return 0;
}
