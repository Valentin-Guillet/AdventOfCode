
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int count_slope(std::vector<std::string> const& tab, int dx, int dy) {
    int ans = 0;
    int x = 0;
    for (size_t y=0; y<tab.size(); y+=dy) {
        ans += (tab[y][x] == '#');
        x = (x+dx) % tab[0].size();
    }
    return ans;
}

int main()
{
    bool first_half = false;
    long ans = 1;
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
        ans = count_slope(tab, 3, 1);
    } else {
        std::vector<std::pair<int, int>> slopes;
        slopes = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
        for (auto [dx, dy] : slopes)
            ans *= count_slope(tab, dx, dy);
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
