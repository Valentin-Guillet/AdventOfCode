
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


bool support_tls(string line) {
    bool support = false;
    bool in_hypernet = false;

    for (unsigned int i=0; i<line.size()-3; ++i) {
        if (line[i] == '[') {
            in_hypernet = true;
        } else if (line[i] == ']') {
            in_hypernet = false;
        } else if (line[i] == line[i+3] && line[i+1] == line[i+2] && line[i] != line[i+1]) {
            if (in_hypernet) {
                return false;
            } else {
                support = true;
            }
        }
    }

    return support;
}

bool support_ssl(string line) {
    string hypernets;

    while (line.find('[') != string::npos) {
        int i = line.find('[');
        int j = line.find(']');

        hypernets += '/' + line.substr(i+1, j-i-1);
        line.erase(i, j-i);
        line[i] = '/';
    }

    for (unsigned int i=0; i<line.size()-2; ++i) {
        string bab;
        bab += line[i+1];
        bab += line[i];
        bab += line[i+1];
        if (line[i] != '/' && line[i+1] != '/' && line[i] != line[i+1]
                           && line[i] == line[i+2]
                           && hypernets.find(bab) != string::npos)
            return true;
    }
    return false;
}

int main()
{
    bool first_half = false;
	string input;
	int ans = 0;
	vector<string> tab;

	ifstream myfile ("Day7Input");
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
        for (string line : tab)
            ans += support_tls(line);
    } else {
        for (string line : tab)
            ans += support_ssl(line);
    }

	cout << "Answer : " << ans << endl;
	return 0;
}
