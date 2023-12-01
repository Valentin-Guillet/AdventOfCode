
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


int compute_fuel(std::vector<int> const& crabs, int pos, bool increase) {
    int ans = 0;
    for (int crab : crabs) {
        if (increase) {
            int diff = std::abs(crab - pos);
            ans += diff * (diff+1) / 2;
        } else {
            ans += std::abs(crab - pos);
        }
    }
    return ans;
}


int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;
    std::vector<int> crabs;

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

        getline(myfile, input);
        std::istringstream iss(input);
        std::string s;
        while (getline(iss, s, ','))
            crabs.push_back(std::stoi(s));

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int fuel_min = 100000000;
    int x_max = *std::max_element(crabs.begin(), crabs.end());
    for (int pos=0; pos<x_max; pos++) {
        int fuel = compute_fuel(crabs, pos, !first_half);
        if (fuel < fuel_min)
            fuel_min = fuel;
    }
    ans = fuel_min;

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
