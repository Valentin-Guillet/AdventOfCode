
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>


struct Hex {
    int x;
    int y;

    bool operator==(Hex const& other) const {
        return x == other.x && y == other.y;
    }
};

struct HexHash {
    size_t operator()(const Hex& hex) const {
        std::string repr = std::to_string(hex.x) + ',' + std::to_string(hex.y);
        return std::hash<std::string>()(repr);
    }
};

using HexGrid = std::unordered_set<Hex, HexHash>;

void add_hex(HexGrid &hexes, std::string coords) {
    Hex new_hex{0, 0};
    size_t index = 0;
    while (index < coords.size()) {
        if (coords[index] == 'w') {
            new_hex.x--;
        } else if (coords[index] == 'e') {
            new_hex.x++;
        } else if (coords[index] == 's') {
            if ((new_hex.y & 1) && coords[index+1] == 'w')
                new_hex.x--;
            if (!(new_hex.y & 1) && coords[index+1] == 'e')
                new_hex.x++;
            new_hex.y++;
            index++;
        } else if (coords[index] == 'n') {
            if ((new_hex.y & 1) && coords[index+1] == 'w')
                new_hex.x--;
            if (!(new_hex.y & 1) && coords[index+1] == 'e')
                new_hex.x++;
            new_hex.y--;
            index++;
        }
        index++;
    }
    HexGrid::iterator it = hexes.find(new_hex);
    if (it == hexes.end())
        hexes.insert(new_hex);
    else
        hexes.erase(it);
}

std::vector<Hex> get_neighbours(Hex hex) {
    int side = ((hex.y & 1) ? -1 : 1);
    std::vector<Hex> neighbours = {Hex{hex.x-1, hex.y}, Hex{hex.x+1, hex.y},
                                   Hex{hex.x, hex.y-1}, Hex{hex.x, hex.y+1},
                                   Hex{hex.x+side, hex.y-1}, Hex{hex.x+side, hex.y+1}};
    return neighbours;
}

HexGrid next_gen(HexGrid const& hexes) {
    HexGrid new_hexes;
    std::unordered_map<Hex, int, HexHash> hex_count;

    for (Hex hex : hexes) {
        for (Hex neighbour : get_neighbours(hex))
            hex_count[neighbour]++;
    }

    for (auto [hex, count] : hex_count) {
        bool is_black = hexes.find(hex) != hexes.end();
        if ((is_black && (count == 1 || count == 2)) || (!is_black && count == 2))
            new_hexes.insert(hex);
    }

    return new_hexes;
}

int main()
{
    bool first_half = false;
    int ans = 0;
    HexGrid hexes;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            add_hex(hexes, input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (!first_half) {
        for (int i=0; i<100; ++i)
            hexes = next_gen(hexes);
    }
    ans = hexes.size();

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
