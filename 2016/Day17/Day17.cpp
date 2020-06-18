
#include <iostream>
#include <string>
#include <queue>
#include <vector>

#include "md5.cpp"

using namespace std;


struct Position {
    int x;
    int y;
    string key;
    string path;
    string hash;

    Position(string input) :
        x(0), y(0), key(input), hash(md5(input)) { }

    Position(Position parent, char dir) {
        x = parent.x;
        y = parent.y;
        key = parent.key;
        path = parent.path + dir;

        if (dir == 'U') {
            y--;
        } else if (dir == 'D') {
            y++;
        } else if (dir == 'L') {
            x--;
        } else if (dir == 'R') {
            x++;
        }

        hash = md5(key + path);
    }
};

int main()
{
    bool first_half = true;
	string input = "mmsxrhfx";

    queue<Position> positions;
    Position curr_pos(input);
    positions.push(curr_pos);

    int max_length = 0;
    while (!positions.empty()) {
        curr_pos = positions.front();
        positions.pop();

        if (curr_pos.x == 3 && curr_pos.y == 3) {
            if (first_half) {
                break;
            } else {
                max_length = curr_pos.path.size();
                continue;
            }
        }

        if (0 < curr_pos.y && 'b' <= curr_pos.hash[0] && curr_pos.hash[0] <= 'f')
            positions.push(Position(curr_pos, 'U'));

        if (curr_pos.y < 3 && 'b' <= curr_pos.hash[1] && curr_pos.hash[1] <= 'f')
            positions.push(Position(curr_pos, 'D'));

        if (0 < curr_pos.x && 'b' <= curr_pos.hash[2] && curr_pos.hash[2] <= 'f')
            positions.push(Position(curr_pos, 'L'));

        if (curr_pos.x < 3 && 'b' <= curr_pos.hash[3] && curr_pos.hash[3] <= 'f')
            positions.push(Position(curr_pos, 'R'));
    }

    if (first_half) {
        cout << "Answer : " << curr_pos.path << endl;
    } else {
        cout << "Answer : " << max_length << endl;
    }

	return 0;
}
