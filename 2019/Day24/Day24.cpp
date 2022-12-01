
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <algorithm>


std::unordered_map<int, std::vector<std::pair<int, int>>> NEIGHBOURS =
    { { 0, {{ 1,  7}, { 0,  1}, { 0,  5}, { 1, 11}}},
      { 1, {{ 1,  7}, { 0,  2}, { 0,  6}, { 0,  0}}},
      { 2, {{ 1,  7}, { 0,  3}, { 0,  7}, { 0,  1}}},
      { 3, {{ 1,  7}, { 0,  4}, { 0,  8}, { 0,  2}}},
      { 4, {{ 1,  7}, { 1, 13}, { 0,  9}, { 0,  3}}},
      { 5, {{ 0,  0}, { 0,  6}, { 0, 10}, { 1, 11}}},
      { 6, {{ 0,  1}, { 0,  7}, { 0, 11}, { 0,  5}}},
      { 7, {{ 0,  2}, { 0,  8}, {-1,  0}, {-1,  1}, {-1,  2}, {-1,  3}, {-1,  4}, { 0,  6}}},
      { 8, {{ 0,  3}, { 0,  9}, { 0, 13}, { 0,  7}}},
      { 9, {{ 0,  4}, { 1, 13}, { 0, 14}, { 0,  8}}},
      {10, {{ 0,  5}, { 0, 11}, { 0, 15}, { 1, 11}}},
      {11, {{ 0,  6}, {-1,  0}, {-1,  5}, {-1, 10}, {-1, 15}, {-1, 20}, { 0, 16}, { 0, 10}}},
      // Center tile
      {13, {{ 0,  8}, { 0, 14}, { 0, 18}, {-1,  4}, {-1,  9}, {-1, 14}, {-1, 19}, {-1, 24}}},
      {14, {{ 0,  9}, { 1, 13}, { 0, 19}, { 0, 13}}},
      {15, {{ 0, 10}, { 0, 16}, { 0, 20}, { 1, 11}}},
      {16, {{ 0, 11}, { 0, 17}, { 0, 21}, { 0, 15}}},
      {17, {{-1, 20}, {-1, 21}, {-1, 22}, {-1, 23}, {-1, 24}, { 0, 18}, { 0, 22}, { 0, 16}}},
      {18, {{ 0, 13}, { 0, 19}, { 0, 23}, { 0, 17}}},
      {19, {{ 0, 14}, { 1, 13}, { 0, 24}, { 0, 18}}},
      {20, {{ 0, 15}, { 0, 21}, { 1, 17}, { 1, 11}}},
      {21, {{ 0, 16}, { 0, 22}, { 1, 17}, { 0, 20}}},
      {22, {{ 0, 17}, { 0, 23}, { 1, 17}, { 0, 21}}},
      {23, {{ 0, 18}, { 0, 24}, { 1, 17}, { 0, 22}}},
      {24, {{ 0, 19}, { 1, 13}, { 1, 17}, { 0, 23}}}
    };


bool live_or_die(std::map<int, bool[25]>& tab, int i, int j) {
    int nb_neighbours = 0;

    // First Half case
    if (tab.size() == 1) {
        int x = j / 5, y = j % 5;

        // North
        if (x > 0 && tab[i][(x-1)*5 + y])
            nb_neighbours++;

        // South
        if (x < 4 && tab[i][(x+1)*5 + y])
            nb_neighbours++;

        // West
        if (y > 0 && tab[i][x*5 + y - 1])
            nb_neighbours++;

        // East
        if (y < 4 && tab[i][x*5 + y + 1])
            nb_neighbours++;
    } else {
        if (j == 12)
            return false;       // No bugs in center cell

        for (std::pair<int, int> neighbour : NEIGHBOURS[j]) {
            if (tab[i+neighbour.first][neighbour.second])
                nb_neighbours++;
        }
    }

    // Living cell with one neighbour
    if (tab[i][j] && nb_neighbours == 1)
        return true;

    // Dead cell with one or two neighbours
    if (!tab[i][j] && (nb_neighbours == 1 || nb_neighbours == 2))
        return true;

    return false;
}

void expand(std::map<int, bool[25]>& tab) {

    int min_id = tab.begin()->first;
    int max_id = tab.rbegin()->first;

    /* std::cout << "Expanding !" << std::endl; */
    /* std::cout << "Lowest layer : " << min_id << ", highest layer : " << max_id << std::endl << std::endl; */

    if (std::any_of(tab[min_id], tab[min_id]+25, [](bool b) {return b;}))  {
        for (int i=0; i<25; ++i)
            tab[min_id - 1][i] = false;
    }

    if (std::any_of(tab[max_id], tab[max_id]+25, [](bool b) {return b;})) {
        for (int i=0; i<25; ++i)
            tab[max_id + 1][i] = false;
    }
}

void step(std::map<int, bool[25]>& tab) {
    std::map<int, bool[25]> frozen_tab(tab);

    int min_id = tab.begin()->first;
    int max_id = tab.rbegin()->first;

    for (int i=min_id; i<=max_id; ++i) {
        for (int j=0; j<25; ++j)
            tab[i][j] = live_or_die(frozen_tab, i, j);
    }
}

int compute_biodiversity(std::map<int, bool[25]>& tab) {
    int ans = 0;
    if (tab.size() == 1) {
        for (int i=0; i<25; ++i)
            ans += tab[0][i] ? (1 << i) : 0;
    } else {
        for (std::pair<const int, bool[25]> const& level : tab) {
            for (int i=0; i<25; ++i)
                ans += level.second[i];
        }
    }
    return ans;
}


void debug(std::map<int, bool[25]>& tab) {
    std::cout << "Number of levels : " << tab.size() << std::endl;
    for (std::pair<const int, bool[25]>& level : tab) {
        std::cout << "Level " << level.first << " : " << std::endl;
        for (int i=0; i<25; ++i) {
            if (i == 12 && tab.size() > 1) {
                std::cout << '?';
            } else if (level.second[i]) {
                std::cout << '#';
            } else {
                std::cout << '.';
            }
            if (i % 5 == 4)
                std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

int main(void)
{
    bool first_half = false;
	int ans = 0;
    std::map<int, bool[25]> tab;

	std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

        int i = 0;
        while (getline(myfile, input)) {
            for (char c : input) {
                tab[0][i] = (c == '#');
                i++;
            }
        }
		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        std::unordered_set<int> seen;
        ans = compute_biodiversity(tab);

        while (seen.find(ans) == seen.end()) {
            seen.insert(ans);
            step(tab);
            ans = compute_biodiversity(tab);
        }
    } else {
        for (int i=0; i<200; ++i) {
            expand(tab);
            step(tab);
        }
        ans = compute_biodiversity(tab);
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
