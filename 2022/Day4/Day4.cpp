
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


struct Pair {
    int beg1, end1;
    int beg2, end2;
};

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

    std::vector<Pair> pairs;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            size_t index = input.find(',');
            std::string pair1 = input.substr(0, index);
            std::string pair2 = input.substr(index+1);

            Pair new_pair;
            index = pair1.find('-');
            new_pair.beg1 = std::stoi(pair1.substr(0, index));
            new_pair.end1 = std::stoi(pair1.substr(index+1));

            index = pair2.find('-');
            new_pair.beg2 = std::stoi(pair2.substr(0, index));
            new_pair.end2 = std::stoi(pair2.substr(index+1));
            pairs.push_back(new_pair);
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (Pair pair : pairs) {
        if ((pair.beg1 <= pair.beg2 && pair.end2 <= pair.end1) ||
                (pair.beg2 <= pair.beg1 && pair.end1 <= pair.end2)) {
            ans++;
            continue;
        }

        if (!first_half && !(pair.end1 < pair.beg2 || pair.end2 < pair.beg1))
            ans++;
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
