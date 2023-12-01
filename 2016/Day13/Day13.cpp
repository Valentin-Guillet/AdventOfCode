
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;


struct Position {
    int first = 0;
    int second = 0;
    int step = 0;
};

struct hash_position {
    size_t operator()(Position const& p) const {
        size_t seed = hash<int>{}(p.first) + 0x9e3779b9;
        return seed ^ (hash<int>{}(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2));
    }
};

using Grid = unordered_map<Position, bool, hash_position>;

bool operator==(Position const& p1, Position const& p2) {
    return (p1.first == p2.first && p1.second == p2.second);
}

Position operator+(Position const& p1, Position const& p2) {
    return {p1.first + p2.first, p1.second + p2.second};
}

ostream& operator<<(ostream& os, Position const& pos) {
    os << "(" << pos.first << ", " << pos.second << ")";
    return os;
}


class Maze
{
    private:
       Grid grid;
       int input;

       unordered_set<Position, hash_position> seen;

       bool is_valid(Position pos) {
           return (pos.first >= 0 && pos.second >= 0);
       }

       bool is_openspace(Position pos) {
           int x = pos.first, y = pos.second;
           long z = x*x + 3*x + 2*x*y + y + y*y + input;

           unsigned int count = 0;
           while (z) {
               count += z & 1;
               z >>= 1;
           }
           return (count % 2 == 0);
       }

       vector<Position> get_neighbours(Position pos) {
           vector<Position> neighbours;

           vector<Position> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
           for (Position dir : dirs) {
               Position neigh = pos + dir;
               if (is_valid(neigh) && is_openspace(neigh) && seen.find(neigh) == seen.end()) {
                   neigh.step = pos.step+1;
                   neighbours.push_back(neigh);
                   seen.insert(neigh);
               }
           }
           return neighbours;
       }

    public:
        Maze(int input) : input(input) { }

        int go_to(Position target) {
            queue<Position> positions;
            positions.push({1, 1});
            seen.insert({1, 1});

            while (true) {
                Position curr_pos = positions.front();
                positions.pop();

                if (curr_pos == target)
                    return curr_pos.step;

                for (Position neigh : get_neighbours(curr_pos))
                    positions.push(neigh);
            }
        }

        int nb_cells(int value) {
            queue<Position> positions;
            positions.push({1, 1});
            seen.insert({1, 1});

            int ans = 0;
            while (true) {
                Position curr_pos = positions.front();
                positions.pop();

                if (curr_pos.step > value)
                    return ans;
                ans++;

                for (Position neigh : get_neighbours(curr_pos))
                    positions.push(neigh);
            }
            return ans;
        }

        void debug() {
            for (int i=0; i<35; ++i) {
                for (int j=0; j<35; ++j)
                    cout << (is_openspace({i, j}) ? '.' : '#');
                cout << endl;
            }
        }
};

int main()
{
    bool first_half = false;
    int input = 1352;
    int ans = 0;

    Maze maze(input);

    if (first_half) {
        ans = maze.go_to({31, 39});
    } else {
        ans = maze.nb_cells(50);
    }

    cout << "Answer : " << ans << endl;
    return 0;
}
