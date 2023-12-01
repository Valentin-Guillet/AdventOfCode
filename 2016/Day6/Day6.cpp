
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;


int main()
{
    bool first_half = false;
    string input;
    string ans = "________";
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

    unordered_map<char, int> freq[8];

    for (string line : tab) {
        for (int i=0; i<8; ++i) {
            freq[i][line[i]]++;
        }
    }

    for (int i=0; i<8; ++i) {
        int nb_occ = (first_half ? 0 : tab.size());
        for (auto [c, id] : freq[i]) {
            if (first_half ? (id > nb_occ) : (id < nb_occ)) {
                nb_occ = id;
                ans[i] = c;
            }
        }
    }

    cout << "Answer : " << ans << endl;
    return 0;
}
