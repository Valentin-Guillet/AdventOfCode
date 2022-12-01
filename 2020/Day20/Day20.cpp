
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>


struct Tile;
using TileIdx = std::unordered_map<int, Tile>;
using TileMap = std::unordered_map<int, std::vector<int>>;
const int N = 12;
const int W = 10;
const std::set<std::pair<int, int>> SEA_MONSTER = {
        {0, 18},
        {1, 0}, {1, 5}, {1, 6}, {1, 11}, {1, 12}, {1, 17}, {1, 18}, {1, 19},
        {2, 1}, {2, 4}, {2, 7}, {2, 10}, {2, 13}, {2, 16}
};
/*                     #
     #    ##    ##    ###
      #  #  #  #  #  #      */


std::pair<size_t, size_t> transform_coords(size_t length, size_t i, size_t j, int rotation, bool flip) {
    size_t x, y;
    if (rotation == 0) {
        x = i;
        y = j;
    } else if (rotation == 1) {
        x = j;
        y = length - i;
    } else if (rotation == 2) {
        x = length - i;
        y = length - j;
    } else if (rotation == 3) {
        x = length - j;
        y = i;
    }
    if (flip)
        y = length - y;

    return {x, y};
}

struct Tile {
    int id;
    char tile[W][W];
    int values[4] = {0};
    int rotation = 0;
    bool flip = false;

    void compute_values(TileMap &edge_values) {
        for (size_t i=0; i<4; ++i) {
            std::array<char, W> edge = get_edge(i);
            int val_normal = 0;
            int val_reverse = 0;
            for (size_t j=0; j<W; ++j) {
                val_normal = (val_normal << 1) + (edge[j] == '#');
                val_reverse = (val_reverse << 1) + (edge[W-1-j] == '#');
            }
            values[i] = std::min(val_normal, val_reverse);
            edge_values[values[i]].push_back(id);
        }
    }

    int get_face(int face) {
        int orientation = (face + rotation) % 4;
        if (flip && orientation % 2 == 1)
            orientation = 4 - orientation;

        return orientation;
    }

    std::array<char, W> get_edge(int face, bool reverse=false) {
        std::array<char, W> edge;
        int orientation = get_face(face);

        for (size_t i=0; i<W; ++i) {
            if (orientation == 0)
                edge[i] = tile[0][i];
            else if (orientation == 1)
                edge[i] = tile[i][W-1];
            else if (orientation == 2)
                edge[i] = tile[W-1][W-1-i];
            else if (orientation == 3)
                edge[i] = tile[W-1-i][0];
        }
        if (flip != reverse)
            std::reverse(edge.begin(), edge.end());
        return edge;
    }

    char get_cell(size_t i, size_t j) {
        auto [x, y] = transform_coords(W-1, i, j, rotation, flip);
        return tile[x][y];
    }

    bool is_corner(TileMap const& edge_values) {
        int cant_be_linked = 0;
        for (size_t i=0; i<4; ++i) {
            if (edge_values.at(values[i]).size() == 1)
                cant_be_linked++;
        }
        return cant_be_linked == 2;
    }

    void set_corner(TileMap const& edge_values) {
        while (edge_values.at(values[rotation]).size() == 2)
            rotation = (rotation + 1) % 4;
        if (edge_values.at(values[(rotation+3)%4]).size() == 2)
            rotation = (rotation + 1) % 4;
    }

    void orientate(TileIdx &tiles, TileMap const& edge_values, int grid[N][N],
            int value=-1, int face=-1, std::array<char, W> edge={}, int x=0, int y=0) {

        if (value > 0) {
            while (values[(face+rotation)%4] != value)
                rotation = (rotation + 1) % 4;

            std::array<char, W> curr_edge = get_edge(face);
            if (curr_edge != edge) {
                flip = true;
                if ((face == 0 && rotation % 2 == 1) || (face == 3 && rotation % 2 == 0))
                    rotation = (rotation + 2) % 4;
            }
        }

        int south_value = values[get_face(2)];
        if (edge_values.at(south_value).size() == 2) {
            int neighbor_id = edge_values.at(south_value)[0];
            if (neighbor_id == id)
                neighbor_id = edge_values.at(south_value)[1];

            grid[x+1][y] = neighbor_id;
            tiles[neighbor_id].orientate(tiles, edge_values, grid, south_value, 0, get_edge(2, true), x+1, y);
        }

        int east_value = values[get_face(1)];
        if (x == 0 && edge_values.at(east_value).size() == 2) {
            int neighbor_id = edge_values.at(east_value)[0];
            if (neighbor_id == id)
                neighbor_id = edge_values.at(east_value)[1];

            grid[x][y+1] = neighbor_id;
            tiles[neighbor_id].orientate(tiles, edge_values, grid, east_value, 3, get_edge(1, true), x, y+1);
        }
    }

    void debug() {
        std::cout << "Tile " << id << " (rotation " << rotation << ", flip=" << flip << ")" << std::endl;
        for (size_t i=0; i<W; ++i) {
            for (size_t j=0; j<W; ++j)
                std::cout << get_cell(i, j);
            std::cout << std::endl;
        }

        std::cout << "Values: ";
        for (size_t i=0; i<4; ++i) {
            std::cout << values[get_face(i)] << ", ";
        }
        std::cout << std::endl << std::endl;
    }
};

int count_sea_monsters(char image[N*(W-2)][N*(W-2)], int rotation, bool flip) {
    int count = 0;
    for (size_t i=0; i<N*(W-2)-2; ++i) {
        for (size_t j=0; j<N*(W-2)-19; ++j) {
            bool is_monster = true;
            for (auto [di, dj] : SEA_MONSTER) {
                auto [x, y] = transform_coords(N*(W-2), i+di, j+dj, rotation, flip);
                if (image[x][y] != '#') {
                    is_monster = false;
                    break;
                }
            }
            if (is_monster)
                count++;
        }
    }
    return count;
}

int main()
{
    bool first_half = false;
	long ans = 0;
    TileIdx tiles;
    TileMap edge_values;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            Tile new_tile;
            new_tile.id = std::stoi(input.substr(5, 4));
            for (size_t i=0; i<W; ++i) {
                getline(myfile, input);
                for (size_t j=0; j<W; ++j)
                    new_tile.tile[i][j] = input[j];
            }
            new_tile.compute_values(edge_values);
            tiles[new_tile.id] = new_tile;
            getline(myfile, input);
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int id_corner;
    for (auto [id, tile] : tiles) {
        if (tile.is_corner(edge_values)) {
            id_corner = id;
            break;
        }
    }
    tiles[id_corner].set_corner(edge_values);

    int grid[N][N];
    grid[0][0] = id_corner;
    tiles[id_corner].orientate(tiles, edge_values, grid);

    if (first_half) {
        ans = (long) grid[0][0] * grid[N-1][0] * grid[0][N-1] * grid[N-1][N-1];

    } else {
        char image[N*(W-2)][N*(W-2)] = {' '};
        for (size_t i=0; i<N; ++i) {
            for (size_t j=0; j<N; ++j) {
                for (size_t x=1; x<W-1; ++x) {
                    for (size_t y=1; y<W-1; ++y)
                        image[(W-2)*i+x-1][(W-2)*j+y-1] = tiles[grid[i][j]].get_cell(x, y);
                }
            }
        }

        int count_tag = 0;
        for (size_t i=0; i<N*(W-2); ++i) {
            for (size_t j=0; j<N*(W-2); ++j) {
                if (image[i][j] == '#')
                    count_tag++;
            }
        }

        int rotation = 0;
        bool flip = false;
        int nb_monster = 0;
        while (nb_monster == 0) {
            nb_monster = count_sea_monsters(image, rotation++, flip);
            if (rotation == 4) {
                rotation = 0;
                flip = true;
            }
        }
        ans = count_tag - (nb_monster * SEA_MONSTER.size());
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
