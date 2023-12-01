
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


std::pair<int, int> find_indexes(std::vector<int> const& tab, int sum) {
    int i = 0;
    int j = tab.size()-1;
    while (i < j && tab[i] + tab[j] != sum) {
        if (tab[i] + tab[j] < sum)
            i++;
        else
            j--;
    }
    if (i < j)
        return {i, j};
    else
        return {-1, -1};
}

int main()
{
    bool first_half = false;
    int ans = 0;
    std::vector<int> tab;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            tab.push_back(std::stoi(input));

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::sort(tab.begin(), tab.end());
    if (first_half) {
        auto [i, j] = find_indexes(tab, 2020);
        ans = tab[i] * tab[j];

    } else {
        for (size_t k=0; k<tab.size(); ++k) {
            auto [i, j] = find_indexes(tab, 2020-tab[k]);
            if (i > -1) {
                ans = tab[i] * tab[j] * tab[k];
                break;
            }
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
