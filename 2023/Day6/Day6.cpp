
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>


struct Race {
    long long time;
    long long dist;
};

Race merge_races(const Race& r1, const Race& r2) {
    long long time = std::stoll(std::to_string(r1.time) + std::to_string(r2.time));
    long long dist = std::stoll(std::to_string(r1.dist) + std::to_string(r2.dist));
    return {time, dist};
}

int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 1;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Race> races;
    if (myfile.is_open()) {
        std::string time_input, dist_input;
        getline(myfile, time_input);
        getline(myfile, dist_input);

        size_t time_ind = time_input.find(':');
        size_t dist_ind = dist_input.find(':');
        std::stringstream time_stream(time_input.substr(time_ind + 1));
        std::stringstream dist_stream(dist_input.substr(dist_ind + 1));
        long long time, dist;
        while (time_stream >> time && dist_stream >> dist)
            races.emplace_back(time, dist);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (!first_half) {
        Race merged_race = std::accumulate(races.begin() + 1, races.end(),
                                           races[0], merge_races);
        races.clear();
        races.push_back(merged_race);
    }

    for (auto [time, dist] : races) {
        // Add epsilon because if min_press is an int, we need strictly
        // superior speed to beat it, and same for max_press
        long long min_press = std::ceil((time - std::sqrt(time * time - 4.0 * dist)) / 2.0 + 1e-9);
        long long max_press = std::floor((time + std::sqrt(time * time - 4.0 * dist)) / 2.0 - 1e-9);

        ans *= (max_press - min_press + 1);
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
