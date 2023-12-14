
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


struct Set {
    int red = 0;
    int green = 0;
    int blue = 0;

    Set(int r, int g, int b) : red(r), green(g), blue(b) {}

    Set(const std::string& input) {
        size_t start = 0;
        do {
            size_t ind = input.find(' ', start);
            int nb = std::stoi(input.substr(start, ind - start));

            char color = input[ind + 1];
            if (color == 'r') {             // red
                red = nb;
                start = ind + 6;
            } else if (color == 'g') {      // green
                green = nb;
                start = ind + 8;
            } else {                        // blue
                blue = nb;
                start = ind + 7;
            }
        } while (start < input.size());
    }
};

struct Game {
    int id;
    std::vector<Set> sets;

    Game(const std::string& input) {
        size_t ind = input.find(':');
        id = std::stoi(input.substr(5, ind + 5));

        size_t start = ind + 2;
        size_t stop;
        do {
            stop = input.find(';', start);
            sets.emplace_back(input.substr(start, stop - start));
            start = stop + 2;
        } while (stop != std::string::npos);
    }

    bool is_possible(const Set& limits) const {
        for (const Set& set : sets) {
            if (set.red > limits.red || set.green > limits.green || set.blue > limits.blue)
                return false;
        }
        return true;
    }

    int compute_power() const {
        int max_red = 0;
        int max_green = 0;
        int max_blue = 0;

        for (const Set& set : sets) {
            max_red = std::max(max_red, set.red);
            max_green = std::max(max_green, set.green);
            max_blue = std::max(max_blue, set.blue);
        }
        return max_red * max_green * max_blue;
    }
};

int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;

    std::ifstream myfile("input");
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Game> games;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            games.emplace_back(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        Set limits(12, 13, 14);
        for (const Game& game : games) {
            if (game.is_possible(limits))
                ans += game.id;
        }
    } else {
        for (const Game& game : games)
            ans += game.compute_power();
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
