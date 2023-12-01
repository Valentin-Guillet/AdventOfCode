
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>


struct Position {
    int x;
    int y;

    bool operator==(Position const& other) const {
        return (x == other.x) && (y == other.y);
    }
};

struct hashPosition {
    size_t operator()(Position const& pos) const {
        return 31 * pos.x ^ pos.y;
    }
};

using PosSet = std::unordered_set<Position, hashPosition>;


enum class Direction { Up, Down, Left, Right };

struct Movement {
    Direction direction;
    int length;
};


void move_head(Direction dir, Position& head) {
    if (dir == Direction::Up)
        head.x--;
    else if (dir == Direction::Down)
        head.x++;
    else if (dir == Direction::Left)
        head.y--;
    else if (dir == Direction::Right)
        head.y++;
}

void follow_knot(Position const& head, Position& tail) {
    // One step away
    int dx = std::abs(tail.x - head.x);
    int dy = std::abs(tail.y - head.y);
    if (dx + dy <= 1 || (dx == 1 && dy == 1))
        return;

    // Two straight steps away
    if (dx == 0 || dy == 0) {
        tail.x = (tail.x + head.x) / 2;
        tail.y = (tail.y + head.y) / 2;
        return;
    }

    // Three steps diagonally
    tail.x += (head.x > tail.x ? 1 : -1);
    tail.y += (head.y > tail.y ? 1 : -1);
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

    std::vector<Movement> movements;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            Direction dir;
            if (input[0] == 'U') dir = Direction::Up;
            else if (input[0] == 'D') dir = Direction::Down;
            else if (input[0] == 'R') dir = Direction::Right;
            else if (input[0] == 'L') dir = Direction::Left;
            int length = std::stoi(input.substr(2));

            movements.push_back({dir, length});
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int rope_length = (first_half ? 2 : 10);
    std::vector<Position> knots;
    for (int i=0; i<rope_length; i++)
        knots.push_back({0, 0});

    PosSet seen_positions;
    for (auto [dir, length]: movements) {
        for (int i=0; i<length; i++) {
            move_head(dir, knots[0]);
            for (int j=1; j<rope_length; j++)
                follow_knot(knots[j-1], knots[j]);
            seen_positions.insert(knots[rope_length - 1]);
        }
    }
    ans = seen_positions.size();

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
