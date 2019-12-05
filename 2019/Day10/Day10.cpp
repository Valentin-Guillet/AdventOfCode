
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>

const int SIZE = 36;


using Coord = std::pair<int, int>;
using Angle = std::pair<int, int>;

struct Map {
    std::vector<std::pair<Angle, std::vector<Coord>>> map;
    void insert(Angle a, Coord pos) {
        for (unsigned int i=0; i<map.size(); ++i) {
            std::pair<Angle, std::vector<Coord>> *elt = &map[i];
            if (elt->first == a) {
                elt->second.push_back(pos);
                return;
            }
        }
        map.push_back({a, {pos}});
    }
    int size() { return map.size(); }
    std::pair<Angle, std::vector<Coord>>& operator[](int pos) { return map[pos]; }
};

int pgcd(int a, int b)
{
    while (b != 0) {
        int c = a % b;
        a = b;
        b = c;
    }
    return a;
}

Angle reduce_frac(int a, int b)
{
    int g = abs(pgcd(a, b));
    return {a/g, b/g};
}

int compute_sight(int tab[SIZE][SIZE], int y, int x)
{
    std::set<Angle> angles;
    int count = 0;
    for (int j=0; j<SIZE; ++j) {
        for (int i=0; i<SIZE; ++i) {
            if ((i != x || j != y) && tab[j][i] == 1) {
                auto [a, b] = reduce_frac(x-i, y-j);
                if (angles.find({a, b}) == angles.end()) {
                    count++;
                    angles.insert({a, b});
                }
            }
        }
    }
    return count;
}

bool dist_comp(int x, int y, Coord a, Coord b)
{
    int distA = (x-a.first)*(x-a.first) + (y-a.second)*(y-a.second);
    int distB = (x-b.first)*(x-b.first) + (y-b.second)*(y-b.second);
    return distA < distB;
}

bool angle_comp(std::pair<Angle, std::vector<Coord>> elt1, std::pair<Angle, std::vector<Coord>> elt2)
{
    Angle a = elt1.first, b = elt2.first;
    if (a.first == 0 && a.second < 0)
        return true;
    if (a.first == 0)
        return b.first < 0;
    if (b.first == 0 && b.second < 0)
        return false;
    if (b.first == 0)
        return a.first > 0;
    if ((a.first < 0) != (b.first < 0))
        return a.first > 0;

    double angleA = (double) a.second / (double) a.first;
    double angleB = (double) b.second / (double) b.first;
    return angleA < angleB;
}

void disp_map(const Map world)
{
    for (auto elt : world.map) {
        std::cout << "Angle = (" << elt.first.first << ',' << elt.first.second << ") ; [";
        for (auto pos : elt.second)
            std::cout << '(' << pos.first << ',' << pos.second << "), ";
        std::cout << ']' << std::endl;
    }
}


int main(void)
{
    using namespace std::placeholders;

    bool firstHalf = false;
	std::string input;
	int ans = 0;
    int tab[SIZE][SIZE];

	std::ifstream myfile ("Day10Input");
	if (myfile.is_open())
	{
        int j = 0;
		while (getline(myfile, input)) {
            for (int i=0; i<SIZE; ++i)
                tab[j][i] = (int) (input[i] == '#');
            j++;
        }
		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (firstHalf)
    {
        int value, posx, posy;
        for (int j=0; j<SIZE; ++j) {
            for (int i=0; i<SIZE; ++i) {
                if (tab[j][i] == 1) {
                    value = compute_sight(tab, j, i);
                    if (value > ans) {
                        ans = value;
                        posx = i;
                        posy = j;
                    }
                }
            }
        }
        std::cout << "Position station : (" << posx << ", " << posy << ')' << std::endl;
    } else {
        int posx = 25, posy = 31;
        
        // Map creation
        Map world;
        for (int j=0; j<SIZE; ++j) {
            for (int i=0; i<SIZE; ++i) {
                if ((i != posx || j != posy) && tab[j][i] == 1) {
                    Angle angle = reduce_frac(i-posx, j-posy);
                    world.insert(angle, {i, j});
                }
            }
        }

        // Map sorting
        for (int i=0; i<world.size(); ++i)
            std::sort(world[i].second.begin(), world[i].second.end(), std::bind(dist_comp, posx, posy, _1, _2));

        std::sort(world.map.begin(), world.map.end(), angle_comp);

        // disp_map(world);

        // Computation of 200th element
        int nb_angles = world.size(), index = 0, count = 0;
        int line, col;
        while (count < 200) {
            line = index % nb_angles;
            col = index / nb_angles;
            if (col <= (int) world[line].second.size())
                count++;
            index++;
        }
        auto [ansx, ansy] = world[line].second[col];
        ans = 100 * ansx + ansy;
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
