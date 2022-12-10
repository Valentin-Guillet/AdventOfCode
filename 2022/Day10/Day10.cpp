
#include <cmath>
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

    std::vector<std::string> instructions;
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            instructions.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int x = 1;
    int cycles = 0;
    for (std::string instruction : instructions) {
        int nb_steps = (instruction == "noop" ? 1 : 2);
        for (int step=0; step<nb_steps; step++) {
            if (!first_half) {
                std::cout << (std::abs((cycles % 40) - x) < 2 ? "#" : " ");
                if ((cycles + 1) % 40 == 0)
                    std::cout << std::endl;
            }

            ans += x * (++cycles % 40 == 20 ? cycles : 0);

        }
        if (instruction != "noop")
            x += std::stoi(instruction.substr(5));

        if (first_half && cycles > 220) {
            std::cout << "Answer : " << ans << std::endl;
            return 0;
        }
    }

	return 0;
}
