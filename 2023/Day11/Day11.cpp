
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <vector>


struct Pos {
    size_t row, col;
};

void localize_galaxies(const std::vector<std::string>& universe, std::vector<Pos>& galaxies,
        std::unordered_set<size_t>& empty_rows, std::unordered_set<size_t>& empty_cols) {
    std::unordered_set<size_t> non_empty_cols;
    for (size_t row = 0; row < universe.size(); ++row) {
        if (!universe[row].contains('#')) {
            empty_rows.insert(row);
            continue;
        }

        for (size_t col = 0; col < universe[row].size(); ++col) {
            if (universe[row][col] == '#') {
                galaxies.push_back({row, col});
                non_empty_cols.insert(col);
            }
        }
    }

    for (size_t col = 0; col < universe[0].size(); ++col) {
        if (!non_empty_cols.contains(col))
            empty_cols.insert(col);
    }
}

int main(int argc, char* argv[])
{
    bool first_half = true;
    long long ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<std::string> universe;
    if (myfile.is_open()) {
        std::istream_iterator<std::string> file_iter(myfile), end;
        universe.assign(file_iter, end);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::vector<Pos> galaxies;
    std::unordered_set<size_t> empty_rows, empty_cols;
    localize_galaxies(universe, galaxies, empty_rows, empty_cols);

    const int expansion_factor = (first_half ? 2 : 1000000);
    for (size_t ind1 = 0; ind1 < galaxies.size(); ++ind1) {
        for (size_t ind2 = ind1 + 1; ind2 < galaxies.size(); ++ind2) {
            auto [min_row, max_row] = std::minmax(galaxies[ind1].row, galaxies[ind2].row);
            auto [min_col, max_col] = std::minmax(galaxies[ind1].col, galaxies[ind2].col);

            size_t dist = (max_row - min_row) + (max_col - min_col);
            dist += std::count_if(empty_rows.begin(), empty_rows.end(),
                        [&](size_t row) { return min_row < row && row < max_row; }
                    ) * (expansion_factor - 1);
            dist += std::count_if(empty_cols.begin(), empty_cols.end(),
                        [&](size_t col) { return min_col < col && col < max_col; }
                    ) * (expansion_factor - 1);

            ans += dist;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
