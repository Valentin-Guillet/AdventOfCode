
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>


struct Password {
    std::pair<int, int> limits;
    char letter;
    std::string pwd;

    Password(std::pair<int, int> limits, char letter, std::string pwd) :
        limits(limits), letter(letter), pwd(pwd) {}

    bool is_valid(bool first_half) {
        if (first_half) {
            int count = std::count(pwd.begin(), pwd.end(), letter);
            return limits.first <= count && count <= limits.second;
        } else {
            return (pwd[limits.first-1] == letter) != (pwd[limits.second-1] == letter);
        }
    }
};


int main()
{
    bool first_half = false;
    int ans = 0;
    std::vector<Password> tab;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        std::regex rgx("([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)");
        std::smatch match;
        while (getline(myfile, input)) {
            if (!std::regex_match(input, match, rgx)) {
                std::cout << "Unable to parse input !" << std::endl;
                return -1;
            }
            std::pair<int, int> limits = {std::stoi(match[1].str()), std::stoi(match[2].str())};
            char letter = match[3].str()[0];
            std::string pwd = match[4].str();
            tab.push_back(Password(limits, letter, pwd));
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (Password pwd : tab)
        ans += pwd.is_valid(first_half);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
