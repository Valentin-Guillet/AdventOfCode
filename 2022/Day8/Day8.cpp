
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Position {
    int x;
    int y;

    bool operator==(Position const& other) const {
        return (x == other.x) && (y == other.y);
    }
};

struct hashPosition {
    size_t operator()(Position const& pos) const {
        return 31 * pos.x ^ pos.y;
    }
};

using PosSet = std::unordered_set<Position, hashPosition>;

int get_scenic_score(std::vector<std::vector<int>>& trees, int x, int y) {
    int height = trees[x][y];
    int scores[4] = {0};

    // Towards North
    for (int dx=1; x-dx>=0; dx++) {
        scores[0]++;
        if (trees[x-dx][y] >= height)
            break;
    }

    // Towards East
    for (int dy=1; y+dy<(int)trees[0].size(); dy++) {
        scores[1]++;
        if (trees[x][y+dy] >= height)
            break;
    }

    // Towards South
    for (int dx=1; x+dx<(int)trees.size(); dx++) {
        scores[2]++;
        if (trees[x+dx][y] >= height)
            break;
    }

    // Towards West
    for (int dy=1; y-dy>=0; dy++) {
        scores[3]++;
        if (trees[x][y-dy] >= height)
            break;
    }

    return scores[0] * scores[1] * scores[2] * scores[3];
}


int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<std::vector<int>> trees;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            std::vector<int> tree_row;
            for (char c : input)
                tree_row.push_back(c - '0');
            trees.push_back(tree_row);
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        PosSet seen_trees;
        // North -> South
        for (int y=0; y<(int)trees[0].size(); y++) {
            int curr_height = -1;
            for (int x=0; x<(int)trees.size(); x++) {
                if (trees[x][y] <= curr_height)
                    continue;

                seen_trees.insert({x, y});
                curr_height = trees[x][y];
            }
        }

        // East -> West
        for (int x=0; x<(int)trees.size(); x++) {
            int curr_height = -1;
            for (int y=trees[0].size()-1; y>=0; y--) {
                if (trees[x][y] <= curr_height)
                    continue;

                seen_trees.insert({x, y});
                curr_height = trees[x][y];
            }
        }

        // South -> North
        for (int y=0; y<(int)trees[0].size(); y++) {
            int curr_height = -1;
            for (int x=trees.size()-1; x>=0; x--) {
                if (trees[x][y] <= curr_height)
                    continue;

                seen_trees.insert({x, y});
                curr_height = trees[x][y];
            }
        }

        // West -> East
        for (int x=0; x<(int)trees.size(); x++) {
            int curr_height = -1;
            for (int y=0; y<(int)trees[0].size(); y++) {
                if (trees[x][y] <= curr_height)
                    continue;

                seen_trees.insert({x, y});
                curr_height = trees[x][y];
            }
        }

        ans = seen_trees.size();
    } else {
        int max_scenic_score = 0;
        for (int x=0; x<(int)trees.size(); x++) {
            for (int y=0; y<(int)trees[0].size(); y++) {
                int scenic_score = get_scenic_score(trees, x, y);
                if (scenic_score > max_scenic_score)
                    max_scenic_score = scenic_score;
            }
        }
        ans = max_scenic_score;
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
