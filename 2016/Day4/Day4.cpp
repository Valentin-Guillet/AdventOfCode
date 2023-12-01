
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;


tuple<string, int, string> parse(string line) {
    string name, chksum;
    int id_name, sector_id;

    id_name = line.find_first_of("0123456789");
    name = line.substr(0, id_name);

    line = line.substr(id_name);
    sector_id = stoi(line.substr(0, 3));

    chksum = line.substr(4, 5);

    return {name, sector_id, chksum};
}

string compute_chksum(string name) {
    string chksum;
    name.erase(remove(name.begin(), name.end(), '-'), name.end());
    sort(name.begin(), name.end());

    int nb_letters = name.size();
    while (nb_letters > 0) {
        for (char c : name) {
            if (chksum.find(c) == string::npos && count(name.begin(), name.end(), c) == nb_letters)
                chksum += c;

            if (chksum.size() == 5)
                goto break_out;
        }
        nb_letters--;
    }

break_out:
    return chksum;
}

string decrypt(string name, int id) {
    string ans;
    for (char c : name) {
        if (c == '-') {
            ans += ' ';
        } else {
            ans += ((c - 'a') + id) % 26 + 'a';
        }
    }
    return ans;
}


int main()
{
    bool first_half = false;
    string input;
    int ans = 0;
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

    for (string line : tab) {
        auto [name, id, chksum] = parse(line);

        if (compute_chksum(name) == chksum) {
            if (first_half) {
                ans += id;
            } else {
                name = decrypt(name, id);
                if (name.find("north") != string::npos)
                    ans = id;
            }
        }
    }

    cout << "Answer : " << ans << endl;
    return 0;
}
