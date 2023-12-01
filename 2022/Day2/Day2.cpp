
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>


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

    std::vector<std::pair<char, char>> rounds;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            rounds.push_back({input[0] - 'A', input[2] - 'X'});

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (auto [opp, pla] : rounds) {
            // Win
            if (pla == (opp + 1) % 3)
                ans += pla + 1 + 6;

            // Draw
            else if (opp == pla)
                ans += pla + 1 + 3;

            // Lose
            else
                ans += pla + 1;
        }
    } else {
        for (auto [opp, end] : rounds) {
            // Win
            if (end == 2)
                ans += (opp + 1) % 3 + 6 + 1;

            // Draw
            else if (end == 1)
                ans += opp + 3 + 1;

            // Lose
            else
                ans += (opp + 2) % 3 + 1;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
