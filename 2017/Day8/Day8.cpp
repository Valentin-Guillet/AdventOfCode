
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <algorithm>

bool comp(int a, int b, std::string op) {
    if (op == "==") {
        return a == b;
    } else if (op == "!=") {
        return a != b;
    } else if (op == "<") {
        return a < b;
    } else if (op == "<=") {
        return a <= b;
    } else if (op == ">") {
        return a > b;
    } else if (op == ">=") {
        return a >= b;
    } else {
        std::cout << "Nope" << std::endl;
        return false;
    }
}

int main(void)
{
    std::string input;
    int ans = 0;
    std::vector<int> tab;

    std::regex rgx("([a-z]+) (inc|dec) (-?[0-9]+) if ([a-z]+) (..?) (-?[0-9]+)");
    std::smatch cm;
    std::unordered_map<std::string, int> dico;

    std::ifstream myfile ("input");
    if (myfile.is_open())
    {
        while (getline(myfile, input)) {
            std::regex_match(input, cm, rgx);
            if (comp(dico[cm[4]], std::stoi(cm[6]), cm[5])) {
                int sign = 1;
                if (cm[2] == "dec") sign = -1;
                dico[cm[1]] += sign * std::stoi(cm[3]);
                if (dico[cm[1]] > ans) {
                    ans = dico[cm[1]];
                }
            }
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";

    auto test = *std::max_element(dico.begin(), dico.end(),
        [](const std::pair<std::string, int>& p1, const std::pair<std::string, int>& p2) {return p1.second < p2.second;});
    std::cout << test.first << ", " << test.second << std::endl;

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}