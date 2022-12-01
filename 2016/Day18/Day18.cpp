
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;


string next_row(string input) {
    string row;
    input = '.' + input + '.';
    for (size_t i=1; i<input.size()-1; ++i) {
        string pat = input.substr(i-1, 3);
        if (pat == "^^." || pat == ".^^" || pat == "^.." || pat == "..^") {
            row += '^';
        } else {
            row += '.';
        }
    }

    return row;
}

int main()
{
    bool first_half = false;
	string input;
	int ans = 0;
    int nb_rows = (first_half ? 40 : 400000);

	ifstream myfile ("input");
	if (myfile.is_open())
	{
		getline(myfile, input);
		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    ans = count(input.begin(), input.end(), '.');

    for (int i=1; i<nb_rows; ++i) {
        input = next_row(input);
        ans += count(input.begin(), input.end(), '.');
    }

	cout << "Answer : " << ans << endl;
	return 0;
}
