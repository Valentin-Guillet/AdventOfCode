
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>


std::pair<int, int> compute_range_speed_x(int min_x, int max_x) {
    int min_speed;
    bool valid = false;
    for (int speed=0; ; speed++) {
        int x = speed * (speed+1) / 2;
        if (!valid && min_x <= x && x <= max_x) {
            min_speed = speed;
            valid = true;
        }
        if (valid && x > max_x)
            return {min_speed, speed-1};
    }
}

int compute_max_height(int min_y, int max_y) {
    int max_speed_y = -min_y -1;
    for (int speed_y=max_speed_y; speed_y>0; speed_y--) {
        int y = 0;
        for (int curr_speed=speed_y; ; curr_speed++) {
            y -= curr_speed;
            if (min_y <= y && y <= max_y)
                return speed_y;
            if (y < min_y)
                break;
        }
    }
    return -1;
}

std::pair<int, int> compute_steps(int min_x, int max_x, int speed_x) {
    int min_steps;
    int x = 0;
    bool valid = false;
    for (int step=1; ; step++) {
        x += speed_x--;
        if (!valid && min_x <= x && x <= max_x) {
            min_steps = step;
            valid = true;
        }
        if (valid && x > max_x)
            return {min_steps, step-1};
        if (speed_x == 0) {
            if (valid)
                return {min_steps, -1};
            else
                return {-1, -1};
        }
    }
}

int count_speeds(int min_y, int max_y, int min_steps, int max_steps) {
    std::unordered_set<int> valid_speeds;
    for (int steps=min_steps; max_steps==-1 || steps<=max_steps; steps++) {
        int upper = std::floor((max_y + steps * (steps-1) / 2.0) / steps);
        int lower = std::floor((min_y + steps * (steps-1) / 2.0) / steps);

        for (int valid_speed=lower+1; valid_speed<=upper; valid_speed++)
            valid_speeds.insert(valid_speed);

        if ((min_y + steps * (steps-1) / 2) % steps == 0)
            valid_speeds.insert(lower);

        if (max_steps == -1 && (upper > -min_y + 1))
            break;
    }
    return valid_speeds.size();
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
    int min_x, max_x;
    int min_y, max_y;

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
        int index_eq = input.find("x=");
        int index_comma = input.find(",");

        std::string range_x = input.substr(index_eq+2, index_comma - index_eq - 2);
        std::string range_y = input.substr(index_comma+4);

        int index_dots = range_x.find("..");
        min_x = std::stoi(range_x.substr(0, index_dots));
        max_x = std::stoi(range_x.substr(index_dots+2));

        index_dots = range_y.find("..");
        min_y = std::stoi(range_y.substr(0, index_dots));
        max_y = std::stoi(range_y.substr(index_dots+2));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        auto [min_speed_x, max_speed_x] = compute_range_speed_x(min_x, max_x);

        int highest_speed_y = 0;
        for (int speed_x=min_speed_x; speed_x<=max_speed_x; speed_x++)
            highest_speed_y = std::max(highest_speed_y, compute_max_height(min_y, max_y));
        ans = highest_speed_y * (highest_speed_y+1) / 2;

    } else {
        int speed_x = max_x;
        do {
            auto [min_steps, max_steps] = compute_steps(min_x, max_x, speed_x);
            if (!(min_steps == -1 && max_steps == -1))
                ans += count_speeds(min_y, max_y, min_steps, max_steps);
            speed_x--;
        } while (speed_x * (speed_x+1) / 2 > min_x);
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
