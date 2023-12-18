
#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

enum Direction { NORTH = 0, EAST = 1, WEST = 2, SOUTH = 3 };
using SeenState = unsigned int;
const unsigned int SEEN_START = (1 << 0);
const unsigned int SEEN_END = (1 << 1);

using HeatMap = std::vector<std::vector<int>>;

struct Block;
using Grid = std::vector<std::vector<std::vector<Block>>>;

struct Block {
    int nb_rows, nb_cols;
    int min_straight, max_straight;
    int row, col, state;
    int heat_loss, total_loss;
    std::array<Block*, 4> neighbors;

    void update_neighbors(Grid& grid) {
        size_t index = 0;

        if (state == -1) {
            if (row == 0) {
                neighbors[index++] = &grid[1][0][SOUTH];
                neighbors[index++] = &grid[0][1][EAST];
            } else {
                neighbors[index++] = &grid[row - 1][col][NORTH];
                neighbors[index++] = &grid[row][col - 1][WEST];
            }
            neighbors[index++] = nullptr;
            return;
        }

        int nb_straight = (state >> 2);
        int dir = (state & 3);
        if (nb_straight < min_straight) {
            int new_state = ((nb_straight + 1) << 2) | dir;
            if (dir == NORTH && row > 0)
                neighbors[index++] = &grid[row - 1][col][new_state];
            if (dir == SOUTH && row < nb_rows - 1)
                neighbors[index++] = &grid[row + 1][col][new_state];
            if (dir == WEST && col > 0)
                neighbors[index++] = &grid[row][col - 1][new_state];
            if (dir == EAST && col < nb_cols - 1)
                neighbors[index++] = &grid[row][col + 1][new_state];
            neighbors[index++] = nullptr;
            return;
        }

        if (row > 0 && dir != SOUTH && !(dir == NORTH && nb_straight == max_straight - 1)) {
            int new_straight = (dir == NORTH ? (nb_straight + 1) : 0);
            neighbors[index++] = &grid[row - 1][col][(new_straight << 2) | NORTH];
        }

        if (row < nb_rows - 1 && dir != NORTH && !(dir == SOUTH && nb_straight == max_straight - 1)) {
            int new_straight = (dir == SOUTH ? (nb_straight + 1) : 0);
            neighbors[index++] = &grid[row + 1][col][(new_straight << 2) | SOUTH];
        }

        if (col > 0 && dir != EAST && !(dir == WEST && nb_straight == max_straight - 1)) {
            int new_straight = (dir == WEST ? (nb_straight + 1) : 0);
            neighbors[index++] = &grid[row][col - 1][(new_straight << 2) | WEST];
        }

        if (col < nb_cols - 1 && dir != WEST && !(dir == EAST && nb_straight == max_straight - 1)) {
            int new_straight = (dir == EAST ? (nb_straight + 1) : 0);
            neighbors[index++] = &grid[row][col + 1][(new_straight << 2) | EAST];
        }

        neighbors[index++] = nullptr;
    }
};

struct BlockComp {
    inline bool operator()(const Block* b1, const Block* b2) const {
        return b1->total_loss > b2->total_loss;
    }
};

using BlockQueue = std::priority_queue<Block*, std::vector<Block*>, BlockComp>;

Grid create_grid(const HeatMap& heat_map, int min_straight, int max_straight) {
    // The 3 dimension of the grid represents the "state" of the node,
    // i.e. in which combination of (direction + nb_straight) it is in
    // -> 4 directions * max_straight possibilities for nb_straight
    int nb_rows = heat_map.size();
    int nb_cols = heat_map[0].size();

    Grid grid;
    grid.reserve(nb_rows);
    for (int row = 0; row < nb_rows; ++row) {
        std::vector<std::vector<Block>> line;
        line.reserve(nb_cols);
        for (int col = 0; col < nb_cols; ++col) {
            std::vector<Block> depth;
            depth.reserve(4 * max_straight);
            for (int state = 0; state < 4 * max_straight; ++state)
                depth.emplace_back(nb_rows, nb_cols, min_straight, max_straight, row, col, state, heat_map[row][col], 100000);
            line.emplace_back(depth);
        }
        grid.push_back(line);
    }
    return grid;
}

int get_min_heat_loss(const HeatMap& heat_map, int min_straight, int max_straight) {
    Grid forward_grid = create_grid(heat_map, min_straight, max_straight);
    Grid backward_grid = create_grid(heat_map, min_straight, max_straight);

    Block* start = &forward_grid[0][0][0];
    start->state = -1;
    start->total_loss = 0;

    Block* end = &backward_grid[heat_map.size() - 1][heat_map[0].size() - 1][0];
    end->state = -1;
    end->total_loss = end->heat_loss;

    std::vector<std::vector<SeenState>> seen_states(heat_map.size(), std::vector<SeenState>(heat_map[0].size(), 0));
    BlockQueue start_queue, end_queue;
    start_queue.push(start);
    end_queue.push(end);

    bool expand_forward = false;
    while (!start_queue.empty() && !end_queue.empty()) {
        expand_forward = !expand_forward;

        Grid& grid = (expand_forward ? forward_grid : backward_grid);
        BlockQueue& block_queue = (expand_forward ? start_queue : end_queue);
        Block* curr_block = block_queue.top();
        block_queue.pop();

        SeenState& curr_seen = seen_states[curr_block->row][curr_block->col];
        curr_seen |= (expand_forward ? SEEN_START : SEEN_END);

        if (curr_seen == (SEEN_START | SEEN_END)) {
            // Forward and backward search met, but we don't know in which state (dir + nb_straight),
            // so we must find it in the other grid
            int nb_straight = (curr_block->state >> 2);
            int dir = (curr_block->state & 3);

            // We go through the opposite grid and get the block which is seen and that has the minimal
            // total heat loss, and only if it has not opposite direction and cumulative nb_straights
            // larger than max_straight (in this case, the merge is invalid)
            Grid& opposite_grid = (expand_forward ? backward_grid : forward_grid);
            Block* min_other = nullptr;
            int min_dist = 100000;
            for (int state = 0; state < 4 * max_straight; ++state) {
                Block* other = &opposite_grid[curr_block->row][curr_block->col][state];
                int opp_nb_straight = (other->state >> 2);
                int opp_dir = (other->state & 3);
                // Check opposite direction + too many straight (the -2 comes from the fact
                // that we don't count both starting positions)
                if (dir == 3 - opp_dir && nb_straight + opp_nb_straight + 1 > max_straight - 1)
                    continue;

                // Check not opposite direction + too few straight
                if (dir != 3 - opp_dir && (nb_straight < min_straight || opp_nb_straight < min_straight))
                    continue;

                if (other->total_loss < min_dist) {
                    min_other = other;
                    min_dist = other->total_loss;
                }
            }

            // We count the meeting block heat loss twice, so we remove it once
            if (min_other)
                return curr_block->total_loss + min_other->total_loss - curr_block->heat_loss;
        }

        curr_block->update_neighbors(grid);
        for (Block* neigh : curr_block->neighbors) {
            if (!neigh)
                break;

            if (curr_block->total_loss + neigh->heat_loss >= neigh->total_loss)
                continue;

            neigh->total_loss = curr_block->total_loss + neigh->heat_loss;
            block_queue.push(neigh);
        }
    }

    return -1;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;

    std::ifstream myfile("input");
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    HeatMap heat_map;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input); ) {
            std::vector<int> line;
            for (char c : input)
                line.push_back(c - '0');
            heat_map.emplace_back(line);
        }
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int min_straight = (first_half ? 0 : 3);
    int max_straight = (first_half ? 3 : 10);
    ans = get_min_heat_loss(heat_map, min_straight, max_straight);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
