
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>


int main()
{
    bool first_half = false;
	int ans = 0;
    std::vector<std::unordered_set<char>> groups;

    std::ifstream myfile ("Day6Input");
	if (myfile.is_open()) {
        std::string input;

        std::unordered_set<char> group;
        if (first_half) {
            while (getline(myfile, input)) {
                if (input == "") {
                    groups.push_back(group);
                    group.clear();
                    continue;
                }

                for (char c : input)
                    group.insert(c);
            }

        } else {
            getline(myfile, input);
            for (char c : input)
                group.insert(c);

            while (getline(myfile, input)) {
                if (input == "") {
                    groups.push_back(group);
                    group.clear();
                    getline(myfile, input);
                    for (char c : input)
                        group.insert(c);
                    continue;
                }

                for (auto it=group.begin(); it!=group.end(); ) {
                    if (input.find(*it) == std::string::npos)
                        it = group.erase(it);
                    else
                        ++it;
                }
            }
        }
        groups.push_back(group);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (std::unordered_set<char> group : groups)
        ans += group.size();

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
