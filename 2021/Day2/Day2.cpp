
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;
    std::vector<std::string> tab;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            tab.push_back(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int pos = 0, depth = 0, aim = 0;
    if (first_half) {
        for (std::string command : tab) {
            int index = command.find(' ');
            std::string dir = command.substr(0, index);
            int value = std::stoi(command.substr(index+1));

            if (dir == "forward")
                pos += value;
            else if (dir == "up")
                depth -= value;
            else if (dir == "down")
                depth += value;
        }

    } else {
        for (std::string command : tab) {
            int index = command.find(' ');
            std::string dir = command.substr(0, index);
            int value = std::stoi(command.substr(index+1));

            if (dir == "forward") {
                pos += value;
                depth += aim * value;
            } else if (dir == "up") {
                aim -= value;
            } else if (dir == "down") {
                aim += value;
            }
        }
    }

    ans = pos * depth;
    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
