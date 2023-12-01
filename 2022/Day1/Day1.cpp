
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
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

    std::vector<int> elves;
    if (myfile.is_open()) {
        std::string input;

        int elf = 0;
        while (getline(myfile, input)) {
            if (input.empty()) {
                elves.push_back(elf);
                elf = 0;
                continue;
            }

            elf += std::stoi(input);
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::sort(elves.begin(), elves.end());
    std::reverse(elves.begin(), elves.end());

    if (first_half)
        ans = elves[0];
    else
        ans = elves[0] + elves[1] + elves[2];

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
