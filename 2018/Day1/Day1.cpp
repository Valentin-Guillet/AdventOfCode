
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>


int main(void)
{
    bool first_half = false;
    int ans = 0;
    std::vector<int> tab;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            tab.push_back(stoi(input));

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (int x : tab)
            ans += x;
    } else {
        std::unordered_set<int> seen;
        int index = 0;

        while (seen.find(ans) == seen.end()) {
            seen.insert(ans);
            ans += tab[index];
            index = (index + 1) % tab.size();
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
