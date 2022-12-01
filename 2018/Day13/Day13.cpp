
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>


std::unordered_map<char, int> DIRECTIONS = {{'^', 0}, {'>', 1}, {'v', 2}, {'<', 3}};
std::unordered_map<int, int> DIRECTIONS_SLASH = {{0, 1}, {1, 0}, {2, 3}, {3, 2}};
std::unordered_map<int, int> DIRECTIONS_ANTISLASH = {{0, 3}, {1, 2}, {2, 1}, {3, 0}};
std::unordered_map<int, int> DIRECTIONS_INTERSECT[3] = {
    {{0, 3}, {1, 0}, {2, 1}, {3, 2}},
    {{0, 0}, {1, 1}, {2, 2}, {3, 3}},
    {{0, 1}, {1, 2}, {2, 3}, {3, 0}}
};

using Position = std::pair<int, int>;
class Cart;

struct hash_position {
    size_t operator()(const Position& p) const {
        size_t seed = std::hash<int>{}(p.first) + 0x9e3779b9;
        return seed ^ (std::hash<int>{}(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2));
    }
};

struct compare_position {
    bool operator()(const Position& p1, const Position& p2) const {
        return (p1.second < p2.second) || (p1.second == p2.second && p1.first < p2.first);
    }
};

using Grid = std::unordered_map<Position, char, hash_position>;
using CartMap = std::map<Position, Cart*, compare_position>;

class Cart {
    private:
        Grid& grid;
        CartMap& cart_map;
        int x, y;
        int dir = 0;
        int dir_intersect = 0;

    public:
        bool crashed = false;

        Cart(Grid& grid, CartMap& cart_map, int x, int y, int dir) :
            grid(grid), cart_map(cart_map), x(x), y(y), dir(dir) { }

        bool step(bool remove=false) {
            int tmp_x = x, tmp_y = y;

            if (dir == 0) {  // North
                y--;
            } else if (dir == 1) {  // East
                x++;
            } else if (dir == 2) {  // South
                y++;
            } else if (dir == 3) {  // West
                x--;
            }

            if (cart_map.find({x, y}) != cart_map.end() && !cart_map[{x, y}]->crashed) {
                if (remove) {
                    crashed = true;
                    cart_map[{x, y}]->crashed = true;
                }
                return true;
            }

            cart_map.erase({tmp_x, tmp_y});
            cart_map[{x, y}] = this;

            if (grid[{x, y}] == '/') {
                dir = DIRECTIONS_SLASH[dir];
            } else if (grid[{x, y}] == '\\') {
                dir = DIRECTIONS_ANTISLASH[dir];
            } else if (grid[{x, y}] == '+') {
                dir = DIRECTIONS_INTERSECT[dir_intersect][dir];
                dir_intersect = (dir_intersect+1) % 3;
            }

            return false;
        }

        std::string get_position() {
            return std::to_string(x) + "," + std::to_string(y);
        }
};

void debug(const Grid& grid, const CartMap& cart_map) {
    for (int y=0; y<150; ++y) {
        for (int x=0; x<150; ++x) {
            if (cart_map.find({x, y}) != cart_map.end()) {
                std::cout << "C";
            } else if (grid.find({x, y}) != grid.end()) {
                std::cout << grid.at({x, y});
            } else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }
}

int main(void)
{
    bool first_half = false;
    std::string ans;

    Grid grid;
    CartMap cart_map;

	std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

        int y = 0;
        while (getline(myfile, input)) {
            for (int x=0; x<(int)input.size(); ++x) {
                char c = input[x];
                if (c == ' ')
                    continue;

                if (DIRECTIONS.find(c) != DIRECTIONS.end()) {
                    Cart *new_cart = new Cart(grid, cart_map, x, y, DIRECTIONS[c]);
                    cart_map[{x, y}] = new_cart;
                    c = ((c == '<' || c == '>') ? '-' : '|');
                }
                grid[{x, y}] = c;
            }
            y++;
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        bool collision = false;
        while (!collision) {
            for (auto pos_cart : cart_map) {
                Cart *cart = pos_cart.second;
                collision = cart->step();
                if (collision) {
                    ans = cart->get_position();
                    break;
                }
            }
        }

    } else {
        while (cart_map.size() > 1) {
            CartMap tmp_map(cart_map);
            for (auto& pos_cart : tmp_map) {
                Cart *cart = pos_cart.second;
                if (cart->crashed)
                    continue;

                cart->step(true);
            }

            auto it = cart_map.begin();
            while (it != cart_map.end()) {
                if (it->second->crashed) {
                    it = cart_map.erase(it);
                } else {
                    it++;
                }
            }
        }

        ans = (*cart_map.begin()).second->get_position();
    }

    for (auto pos_cart : cart_map)
        delete pos_cart.second;

    cart_map.clear();

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
