
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>


int get_priority(char c) {
    if ('A' <= c && c <= 'Z')
        return c - 'A' + 27;
    else
        return c - 'a' + 1;
}

std::string get_intersection(std::string s1, std::string s2) {
    std::string ans;
    for (char c : s1) {
        if (s2.find(c) != std::string::npos && ans.find(c) == std::string::npos)
            ans += c;
    }
    return ans;
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

    std::vector<std::string> rucksacks;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            rucksacks.push_back(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (std::string items : rucksacks) {
            std::string item1 = items.substr(0, items.size() / 2);
            std::string item2 = items.substr(items.size() / 2);

            char common_item = get_intersection(item1, item2)[0];
            ans += get_priority(common_item);
        }
    } else {
        for (size_t i=0; i<rucksacks.size(); i+=3) {
            char common_item = get_intersection(rucksacks[i],
                                                get_intersection(rucksacks[i+1], rucksacks[i+2]))[0];
            ans += get_priority(common_item);
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
