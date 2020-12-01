
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::pair<int, int> DIRECTIONS[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

struct Ship {
    int x = 0;
    int y = 0;
    int direction = 0;
    int x_waypoint = 10;
    int y_waypoint = -1;

    void move(std::string instr) {
        char action = instr[0];
        int value = std::stoi(instr.substr(1));

        if (action == 'N') {
            y -= value;
        } else if (action == 'S') {
            y += value;
        } else if (action == 'E') {
            x += value;
        } else if (action == 'W') {
            x -= value;
        } else if (action == 'L') {
            direction = (direction - (value / 90) + 4) % 4;
        } else if (action == 'R') {
            direction = (direction + (value / 90)) % 4;
        } else if (action == 'F') {
            x += value * DIRECTIONS[direction].first;
            y += value * DIRECTIONS[direction].second;
        }
    }

    void move_with_waypoint(std::string instr) {
        char action = instr[0];
        int value = std::stoi(instr.substr(1));

        if (action == 'N') {
            y_waypoint -= value;
        } else if (action == 'S') {
            y_waypoint += value;
        } else if (action == 'E') {
            x_waypoint += value;
        } else if (action == 'W') {
            x_waypoint -= value;
        } else if (action == 'L') {
            for (int i=0; i<(value/90); ++i) {
                int tmp_val = x_waypoint;
                x_waypoint = y_waypoint;
                y_waypoint = -tmp_val;
            }
        } else if (action == 'R') {
            for (int i=0; i<(value/90); ++i) {
                int tmp_val = x_waypoint;
                x_waypoint = -y_waypoint;
                y_waypoint = tmp_val;
            }
        } else if (action == 'F') {
            x += x_waypoint * value;
            y += y_waypoint * value;
        }
    }
};

int main()
{
    bool first_half = false;
	int ans = 0;
    std::vector<std::string> instructions;

    std::ifstream myfile ("Day12Input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            instructions.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    Ship ship;
    for (std::string instr : instructions) {
        if (first_half)
            ship.move(instr);
        else
            ship.move_with_waypoint(instr);
    }
    ans = std::abs(ship.x) + std::abs(ship.y);

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
