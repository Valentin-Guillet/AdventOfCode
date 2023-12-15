
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>


struct Lens {
    std::string label;
    int focal_length;
};

struct Box {
    std::vector<Lens> lenses;

    void insert(const std::string& label, int value) {
        auto it = std::find_if(lenses.begin(), lenses.end(),
                [&](const Lens& lens) { return lens.label == label; } );

        if (it != lenses.end())
            it->focal_length = value;
        else
            lenses.push_back({label, value});
    }

    void remove(const std::string& label) {
        auto it = std::find_if(lenses.begin(), lenses.end(),
                [&](const Lens& lens) { return lens.label == label; } );

        if (it != lenses.end())
            lenses.erase(it);
    }

    int get_focusing_power() const {
        int focusing_power = 0;
        for (size_t i = 0; i < lenses.size(); ++i)
            focusing_power += (i + 1) * lenses[i].focal_length;
        return focusing_power;
    }
};

int HASH(const std::string& input) {
    int value = 0;
    for (char c : input) {
        value += c;
        value *= 17;
        value %= 256;
    }
    return value;
}

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

    std::vector<std::string> steps;
    if (myfile.is_open()) {
        std::string input;
        getline(myfile, input);
        std::stringstream seq_stream(input);
        for (std::string step; getline(seq_stream, step, ','); )
            steps.emplace_back(step);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (const std::string& step : steps)
            ans += HASH(step);

    } else {
        std::vector<Box> boxes(256);
        for (const std::string& step : steps) {
            size_t ind = std::min(step.find('='), step.find('-'));
            std::string label = step.substr(0, ind);

            if (step[ind] == '=')
                boxes[HASH(label)].insert(label, step[ind + 1] - '0');
            else
                boxes[HASH(label)].remove(label);
        }

        for (size_t i = 0; i < boxes.size(); ++i)
            ans += (i + 1) * boxes[i].get_focusing_power();
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
