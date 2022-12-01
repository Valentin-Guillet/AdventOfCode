
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>


bool has_multiple(std::string id, int value) {
    std::unordered_map<char, int> count;
    for (char c : id)
        count[c]++;

    for (auto kv : count) {
        if (kv.second == value)
            return true;
    }
    return false;
}

int one_dist(std::string id1, std::string id2) {
    int ans = 0;
    for (size_t i=0; i<id1.size(); ++i) {
        if (id1[i] != id2[i])
            ans++;
    }
    return ans;
}

std::string diff(std::string id1, std::string id2) {
    std::string ans;
    for (size_t i=0; i<id1.size(); ++i) {
        if (id1[i] == id2[i])
            ans += id1[i];
    }
    return ans;
}

int main(void)
{
    bool first_half = false;
    std::string ans;
	std::vector<std::string> tab;

	std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            tab.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        int two_letters = 0, three_letters = 0;

        for (std::string id : tab) {
            if (has_multiple(id, 2))
                two_letters++;

            if (has_multiple(id, 3))
                three_letters++;
        }
        ans = std::to_string(two_letters * three_letters);

    } else {
        for (size_t i=0; i<tab.size()-1; ++i) {
            for (size_t j=i+1; j<tab.size(); ++j) {
                if (one_dist(tab[i], tab[j]) == 1) {
                    ans = diff(tab[i], tab[j]);
                    goto break_out;
                }
            }
        }
    }

break_out:
	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
