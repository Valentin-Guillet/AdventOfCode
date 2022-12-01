
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

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

bool operator==(Position const& p1, Position const& p2) {
    return (p1.first == p2.first && p1.second == p2.second);
}


class Maze
{
    private:
       vector<string> grid;
       unordered_map<int, Position> nodes;
       vector<vector<int>> distances;
       bool first_half;

       bool is_valid(int i, int j) {
           return (0 <= i && i < (int) grid.size() && 0 <= j && j < (int) grid[0].size());
       }

       vector<Position> get_neighbours(Position pos) {
           vector<Position> neighbours;

           vector<pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
           for (auto [dx, dy] : dirs) {
               int x = pos.first+dx, y = pos.second+dy;

               if (is_valid(x, y) && grid[x][y] != '#')
                   neighbours.push_back({x, y, pos.step+1});
           }
           return neighbours;
       }

       vector<int> build_distance(int index) {
           vector<int> distance;
           for (size_t i=0; i<nodes.size(); ++i)
               distance.push_back(0);

           queue<Position> positions;
           unordered_set<Position, hash_position> seen;

           positions.push(nodes[index]);
           seen.insert(nodes[index]);

           size_t nb_node_met = 0;
           while (nb_node_met < nodes.size()) {
               Position curr_pos = positions.front();
               positions.pop();

               char cell = grid[curr_pos.first][curr_pos.second];
               if (cell != '.' && cell != '#') {
                   distance[cell - '0'] = curr_pos.step;
                   nb_node_met++;
               }

               for (Position neigh : get_neighbours(curr_pos)) {
                   if (seen.find(neigh) == seen.end()) {
                       positions.push(neigh);
                       seen.insert(neigh);
                   }
               }
           }

           return distance;
       }

       void build_distances() {
           for (size_t i=0; i<nodes.size(); ++i)
               distances.push_back(build_distance(i));
       }

       int get_dist(vector<int> path) {
           int ans = distances[0][path[0]];

           for (size_t i=0; i<path.size()-1; ++i)
               ans += distances[path[i]][path[i+1]];

           if (!first_half)
               ans += distances[path[path.size()-1]][0];

           return ans;
       }

    public:
       Maze(const vector<string>& tab, bool first_half) : grid(tab), first_half(first_half) {
           for (int i=0; i<(int)grid.size(); ++i) {
               for (int j=0; j<(int)grid[0].size(); ++j) {
                   if (grid[i][j] != '#' && grid[i][j] != '.')
                       nodes[grid[i][j] - '0'] = {i, j};
               }
           }

           build_distances();
       }

       int solve() {
           int min_dist = 100000;

           vector<int> path;
           for (size_t i=0; i<nodes.size()-1; ++i)
               path.push_back(i+1);

           do {
               int dist = get_dist(path);
               if (dist < min_dist)
                   min_dist = dist;
           } while (next_permutation(path.begin(), path.end()));

           return min_dist;

       }

        void debug() {
            for (size_t i=0; i<grid.size(); ++i) {
                for (size_t j=0; j<grid[0].size(); ++j)
                    cout << grid[i][j];
                cout << endl;
            }
        }
};

int main()
{
    bool first_half = false;
    string input;
	int ans = 0;
    vector<string> tab;

	ifstream myfile ("input");
	if (myfile.is_open())
	{
		while (getline(myfile, input))
            tab.push_back(input);

		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    Maze maze(tab, first_half);
    ans = maze.solve();

	cout << "Answer : " << ans << endl;
	return 0;
}
