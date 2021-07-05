
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <unordered_set>
#include <unordered_map>

using Pos = std::pair<int, int>;

bool is_letter(char c) {
    return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

std::vector<Pos> get_neighbours(int i, int j) {
    return {{i, j-1}, {i+1, j}, {i, j+1}, {i-1, j}};
}


struct Tree
{
    int length;
    char tag;
    Tree* parent;
    std::vector<Tree*> children;

    Tree() : length(0) { }
    Tree(int length) : length(length) { }
    ~Tree() {
        for (Tree *child : children)
            delete child;
    }

    unsigned int get_nb_keys() {
        int nb_key = ('a' <= tag && tag <= 'z') ? 1 : 0;
        for (Tree *child : children)
            nb_key += child->get_nb_keys();
        return nb_key;
    }

    void display(int depth=0) {
        std::cout << std::endl;
        for (int i=0; i<depth-1; ++i)
            std::cout << "    ";
        if (depth > 0)
            std::cout << "--> ";
        std::cout << tag << " / " << length;
        for (Tree *child : children)
            child->display(depth + 1);
    }
};


class DistMatrix
{
    private:
        int nb_keys = 0;
        std::unordered_map<char, Tree*> tree_map;
        int distances[26][26];

        int compute_dist(char cx, char cy) {
            Tree *parentX = tree_map[cx];
            Tree *parentY = tree_map[cy];
            while (parentX != parentY) {
                if (parentX->length < parentY->length) {
                    parentY = parentY->parent;
                } else {
                    parentX = parentX->parent;
                }
            }
            int distX = (tree_map[cx]->length - parentX->length);
            int distY = (tree_map[cy]->length - parentY->length);
            return distX + distY;
        }

    public:
        DistMatrix(std::unordered_map<char, Tree*>& tree_map) : tree_map(tree_map) {
            for (std::pair<char, Tree*> elt : tree_map) {
                if ('a' <= elt.first && elt.first <= 'z')
                    nb_keys++;
            }
            build_distance_matrix();
        }

        void build_distance_matrix() {
            for (char cx='a'; cx<'a'+nb_keys; ++cx) {
                for (char cy=cx+1; cy<'a'+nb_keys; ++cy) {
                    int dist = compute_dist(cx, cy);
                    distances[cx-'a'][cy-'a'] = dist;
                    distances[cy-'a'][cx-'a'] = dist;
                }
            }
        }

        int get_distance(char a, char b) {
            if (a == '@')
                return tree_map[b]->length;
            if (b == '@')
                return tree_map[a]->length;

            return distances[a-'a'][b-'a'];
        }
};


struct State
{
    DistMatrix *distances;
    State(DistMatrix *distances) : distances(distances) { }

    char tag;
    std::unordered_set<char> prev;
    std::vector<State*> parents;

    int length = -1;
    std::vector<char> path;

    void build_state() {
        if (tag == '@') {
            length = 0;
            path.push_back(tag);
        } else {
            int min_len = 100000;
            State *min_parent;
            for (unsigned int i=0; i<parents.size(); ++i) {
                int len = parents[i]->get_length() + distances->get_distance(parents[i]->tag, tag);
                if (len < min_len) {
                    min_len = len;
                    min_parent = parents[i];
                }
            }
            length = min_len;
            path = min_parent->get_path();
            path.push_back(tag);
        }
    }

    int get_length() {
        if (length < 0)
            build_state();
        return length;
    }

    std::vector<char> get_path() {
        if (length < 0)
            build_state();
        return path;
    }

    void print_path() {
        for (char c : get_path())
            std::cout << c << "->";
    }

    static State* find(std::vector<State*>& states, State *next_state) {
        for (State *state : states) {
            if (state->tag == next_state->tag && state->prev == next_state->prev)
                return state;
        }
        return nullptr;
    }
};


class Maze
{
    private:
        bool first_half;

        int origX, origY;
        int sizeX, sizeY;
        std::map<Pos, char> maze;

        Tree *tree;
        std::unordered_map<char, Tree*> tree_map;
        DistMatrix *distances = nullptr;
        int nb_int = 0;


        bool update(std::map<Pos, char>& grid, int i, int j) {
            if (grid[{i, j}] == '#' || ('a' <= grid[{i, j}] && grid[{i, j}] <= 'z')
                                    || grid[{i, j}] == '@')
                return false;

            int ways = 0;
            std::vector<Pos> neighbours = get_neighbours(i, j);
            for (Pos neigh : neighbours)
                ways += (grid[neigh] == '#' ? 1 : 0);

            if (ways < 3)
                return false;

            grid[{i, j}] = '#';
            for (Pos neigh : neighbours)
                update(grid, neigh.first, neigh.second);

            return true;
        }

        std::vector<Pos> get_ways(std::map<Pos, char>& grid, int i, int j) {
            std::vector<Pos> output;
            for (Pos neigh : get_neighbours(i, j)) {
                if (grid[neigh] != '#')
                    output.push_back(neigh);
            }
            return output;
        }

        Tree* build_tree(std::map<Pos, char>& grid, int i, int j, int length=0) {

            std::vector<Pos> ways = get_ways(grid, i, j);
            while (!is_letter(grid[{i, j}]) && ways.size() == 1) {
                grid[{i, j}] = '#';
                i = ways[0].first;
                j = ways[0].second;
                ways = get_ways(grid, i, j);
                length++;
            }

            Tree* subtree = new Tree(length);
            if (is_letter(grid[{i, j}]) || grid[{i, j}] == '@') {
                subtree->tag = grid[{i, j}];
            } else {
                subtree->tag = (nb_int++ + '0');
            }
            tree_map[subtree->tag] = subtree;

            for (Pos neigh : get_ways(grid, i, j)) {
                std::map<Pos, char> subgrid(grid);
                subgrid[{i, j}] = '#';
                subtree->children.push_back(build_tree(subgrid, neigh.first, neigh.second, length+1));
            }

            for (Tree *child : subtree->children)
                child->parent = subtree;

            return subtree;
        }

        std::queue<char> get_keys(Tree *tree, std::unordered_set<char> obtained_keys) {
            std::queue<char> keys;
            std::queue<Tree*> to_see;
            for (Tree *branches : tree->children)
                to_see.push(branches);

            while (!to_see.empty()) {
                Tree *subtree = to_see.front();
                to_see.pop();
                // Door and key not obtained
                if (first_half && 'A' <= subtree->tag && subtree->tag <= 'Z'
                        && obtained_keys.find(subtree->tag+' ') == obtained_keys.end())
                    continue;

                // Key
                if ('a' <= subtree->tag && subtree->tag <= 'z') {
                    // Not obtained
                    if (obtained_keys.find(subtree->tag) == obtained_keys.end()) {
                        keys.push(subtree->tag);
                    } else {
                        for (Tree *child : subtree->children)
                            to_see.push(child);
                    }
                } else {
                    for (Tree *child : subtree->children)
                        to_see.push(child);
                }
            }
            return keys;
        }

        State *get_next_state(State *state, char key) {
            State *next_state = new State(*state);
            next_state->length = -1;
            next_state->parents = {state};

            next_state->prev.insert(next_state->tag);
            next_state->tag = key;

            return next_state;
        }

        void explore_tree(Tree *tree, std::vector<State*>& states, std::queue<char> avail_keys, State *curr_state, int depth=0) {
            while (!avail_keys.empty()) {

                char key = avail_keys.front();
                avail_keys.pop();

                State *next_state = get_next_state(curr_state, key);

                State *previous_state = State::find(states, next_state);
                if (previous_state != nullptr) {
                    previous_state->parents.push_back(curr_state);
                    delete next_state;
                    continue;
                }

                std::unordered_set<char> obtained_keys(next_state->prev);
                obtained_keys.insert(next_state->tag);

                std::queue<char> new_keys = get_keys(tree, obtained_keys);
                states.push_back(next_state);

                explore_tree(tree, states, new_keys, next_state, depth+1);
            }
        }

        void fill() {
            bool acted = true;
            std::map<Pos, char> grid(maze);

            while (acted) {
                acted = false;
                for (int j=1; j<sizeY-1; ++j) {
                    for (int i=1; i<sizeX-1; ++i) {
                        if (update(grid, i, j))
                            acted = true;
                    }
                }
            }
            maze = grid;
        }

    public:
        Maze(std::vector<std::string> input) {
            sizeY = input.size();
            sizeX = input[0].size();

            for (int j=0; j<sizeY; ++j) {
                for (int i=0; i<sizeX; ++i) {
                    maze[{i, j}] = input[j][i];
                    if (input[j][i] == '@') {
                        origX = i;
                        origY = j;
                    }
                }
            }
        }

        ~Maze() {
            delete distances;
            delete tree;
        }

        void build(bool first_half) {
            this->first_half = first_half;

            maze[{origX-1, origY}] = '#';
            maze[{origX, origY+1}] = '#';
            maze[{origX, origY}] = '#';
            origX++;
            origY--;
            maze[{origX, origY}] = '@';

            fill();

            std::map<Pos, char> grid(maze);
            tree = build_tree(grid, origX, origY);

            distances = new DistMatrix(tree_map);
        }

        int get_shortest_path(Tree *tree) {

            std::vector<State*> states;

            State *first_state = new State(distances);
            first_state->tag = '@';
            first_state->length = 0;

            std::queue<char> avail_keys = get_keys(tree, {});
            explore_tree(tree, states, avail_keys, first_state);

            int is_first_key = ('a' <= tree->tag && tree->tag <= 'z') ? 1 : 0;
            int min_dist = 1000000;
            for (State *state : states) {
                if (state->prev.size() == tree->get_nb_keys()-is_first_key) {
                    int dist = state->get_length();
                    if (dist < min_dist)
                        min_dist = dist;
                }
            }

            delete first_state;
            for (State *state : states)
                delete state;

            return min_dist;
        }

        int get_shortest_path() {
            if (first_half) {
                return get_shortest_path(this->tree) + 2;
            } else {
                int length = 0;
                for (Tree *child : this->tree->children)
                    length += get_shortest_path(child);
                return length - 4;
            }
        }

        void display(std::map<Pos, char>& grid) const {
            for (int j=0; j<sizeY; ++j) {
                for (int i=0; i<sizeX; ++i) {
                    std::cout << grid[{i, j}];
                }
                std::cout << std::endl;
            }
        }

        void display() {
            display(maze);
        }

        void display_tree() {
            tree->display();
            std::cout << std::endl;
        }
};

int main(void)
{
    bool first_half = false;
	int ans = 0;
	std::vector<std::string> tab;

	std::ifstream myfile ("Day18Input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            tab.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }


    Maze maze(tab);
    maze.build(first_half);

    // maze.display();
    // maze.display_tree();

    ans = maze.get_shortest_path();

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
