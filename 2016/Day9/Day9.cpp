
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


string decompress(string input) {
    string ans;

    while (!input.empty()) {
        if (input[0] != '(') {
            ans += input[0];
            input.erase(0, 1);

        } else {
            int id_x = input.find('x');
            int id_end = input.find(')');

            int nb_letters = stoi(input.substr(1, id_x-1));
            int nb_repeat = stoi(input.substr(id_x+1, id_end-id_x-1));

            for (int j=0; j<nb_repeat; ++j) {
                ans += input.substr(id_end+1, nb_letters);
            }

            input.erase(0, id_end+nb_letters+1);
        }
    }

    return ans;
}

long get_decompress_size(string input) {
    long ans = 0;

    while (!input.empty()) {
        if (input[0] != '(') {
            ans++;
            input.erase(0, 1);

        } else {
            int id_x = input.find('x');
            int id_end = input.find(')');

            int nb_letters = stoi(input.substr(1, id_x-1));
            int nb_repeat = stoi(input.substr(id_x+1, id_end-id_x-1));

            string sub_input = input.substr(id_end+1, nb_letters);

            ans += nb_repeat * get_decompress_size(sub_input);

            input.erase(0, id_end+nb_letters+1);
        }
    }

    return ans;
}

int main()
{
    bool first_half = false;
	string input;
	long ans = 0;
	vector<int> tab;

	ifstream myfile ("input");
	if (myfile.is_open()) {
		getline(myfile, input);
		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    if (first_half) {
        ans = decompress(input).size();
    } else {
        ans = get_decompress_size(input);
    }

	cout << "Answer : " << ans << endl;
	return 0;
}
