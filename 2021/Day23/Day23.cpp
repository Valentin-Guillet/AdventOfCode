
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <unordered_map>


int get_score(int amphipod) {
    int ans = 1;
    for (int i=0; i<amphipod; i++) ans *= 10;
    return ans;
}

struct State {
    public:
        int energy = 0;

        State(int nb_spaces, int hallway_spaces[11], int room_spaces[4][4]) : nb_spaces(nb_spaces) {
            for (int i=0; i<11; i++) hallway[i] = hallway_spaces[i];
            for (int i=0; i<4; i++) {
                for (int j=0; j<4; j++)
                    rooms[i][j] = room_spaces[i][j];
            }
        }

        bool is_finished() const {
            for (int room=0; room<4; room++) {
                for (int j=0; j<nb_spaces; j++) {
                    if (rooms[room][j] != room)
                        return false;
                }
            }
            return true;
        }

        bool is_valid() const {
            for (int i=0; i<11; i++) {
                if (hallway[i] == -1)
                    continue;

                int target = 2 + 2*hallway[i];
                if (i < target) {
                    for (int x=i+1; x<target; x++) {
                        if (hallway[x] != -1 && 2+2*hallway[x] <= i)
                            return false;
                    }
                } else if (i > target){
                    for (int x=target; x<i-1; x++) {
                        if (hallway[x] != -1 && 2+2*hallway[x] >= i)
                            return false;
                    }
                }
            }
            return true;
        }

        void forced_move() {
            bool moved;
            do {
                moved = false;
                moved = forced_hallway() || moved;
                moved = forced_rooms() || moved;
            } while (moved);
        }

        std::vector<State> generate_next_states() const {
            std::vector<State> next_states;

            for (int room=0; room<4; room++) {
                int last_space = get_last_occupied_space(room);
                if (last_space == -1 || is_room_valid(room) || hallway[2 + 2*room] != -1) continue;

                int min_accessible = 2 + 2*room;
                while (0 < min_accessible && hallway[min_accessible-1] == -1)
                    min_accessible--;

                int max_accessible = 2 + 2*room;
                while (max_accessible < 10 && hallway[max_accessible+1] == -1)
                    max_accessible++;

                if (max_accessible - min_accessible < 2) continue;
                for (int x=min_accessible; x<=max_accessible; x++) {
                    if (hallway[x] != -1 || x == 2+2*room) continue;
                    if ((x == 2 || x == 4 || x == 6 || x == 8)) continue;

                    State new_state(*this);
                    new_state.hallway[x] = new_state.rooms[room][last_space];
                    new_state.rooms[room][last_space] = -1;
                    int nb_steps = (nb_spaces - last_space) + std::abs(2+2*room - x);
                    new_state.energy += get_score(rooms[room][last_space]) * nb_steps;

                    if (new_state.is_valid())
                        next_states.push_back(new_state);
                }
            }

            return next_states;
        }

        int min_energy() const {
            int remaining_energy = 0;
            for (int i=0; i<11; i++) {
                if (hallway[i] == -1) continue;

                int target = 2 + 2*hallway[i];
                remaining_energy += get_score(hallway[i]) * (std::abs(target - i) + 1);
            }

            for (int room=0; room<4; room++) {
                for (int space=0; space<nb_spaces; space++) {
                    int amphi = rooms[room][space];
                    if (amphi == -1 || amphi == room) continue;

                    int target = 2 + 2*amphi;
                    remaining_energy += get_score(amphi) * (std::abs(target - amphi + 2));
                }
            }

            return remaining_energy;
        }

        size_t hash() const {
            std::string repr;
            for (int i=0; i<11; i++) {
                if (hallway[i] != -1)
                    repr += std::to_string(i) + (char)('A' + hallway[i]) + ".";
            }

            for (int room=0; room<4; room++) {
                for (int space=0; space<nb_spaces; space++) {
                    if (rooms[room][space] != -1) {
                        repr += (char)('0'+room);
                        repr += (char)('0'+space);
                        repr += (char)('A'+rooms[room][space]);
                    }
                }
            }

            return std::hash<std::string>{}(repr);
        }

        void debug() const {
            std::cout << "#############" << std::endl;
            std::cout << "#";
            for (int i=0; i<11; i++) {

                if (hallway[i] == -1) std::cout << ".";
                else std::cout << (char)('A' + hallway[i]);
            }
            std::cout << "#" << std::endl;
            std::cout << "###";
            for (int i=0; i<4; i++) {
                if (rooms[i][nb_spaces-1] == -1) std::cout << ".#";
                else std::cout << (char)('A' + rooms[i][nb_spaces-1]) << "#";
            }
            std::cout << "##" << std::endl;

            for (int space=nb_spaces-2; 0<=space; space--) {
                std::cout << "  #";
                for (int i=0; i<4; i++) {
                    if (rooms[i][space] == -1) std::cout << ".#";
                    else std::cout << (char)('A' + rooms[i][space]) << "#";
                }
                std::cout << std::endl;
            }
            std::cout << "  #########" << std::endl;
            std::cout << "Energy: " << energy << std::endl << std::endl;
        }

    private:
        int nb_spaces;
        int rooms[4][4];
        int hallway[11] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

        bool is_room_available(int room) const {
            if (rooms[room][nb_spaces-1] != -1)
                return false;

            for (int space=0; space<nb_spaces-1; space++) {
                if (rooms[room][space] != -1 && rooms[room][space] != room)
                    return false;
            }
            return true;
        }

        bool is_room_valid(int room) const {
            for (int space=0; space<nb_spaces; space++) {
                if (rooms[room][space] != room && rooms[room][space] != -1)
                    return false;
            }
            return true;
        }

        int get_first_free_space(int room) const {
            int space = 0;
            while (rooms[room][space] != -1)
                space++;
            return space;
        }

        int get_last_occupied_space(int room) const {
            for (int space=nb_spaces-1; 0<=space; space--) {
                if (rooms[room][space] != -1)
                    return space;
            }
            return -1;
        }

        bool forced_hallway() {
            bool moved = false;
            for (int i=0; i<11; i++) {
                int amphi = hallway[i];
                if (amphi == -1 || !is_room_available(amphi)) continue;

                int target = 2 + 2*amphi;
                bool path_free = true;
                for (int x=std::min(i, target); x<=std::max(i, target); x++) {
                    if (x != i && hallway[x] != -1) {
                        path_free = false;
                        break;
                    }
                }

                if (path_free) {
                    int target_space = get_first_free_space(amphi);
                    int nb_steps = std::abs(target - i) + (nb_spaces - target_space);

                    rooms[amphi][target_space] = amphi;
                    energy += get_score(amphi) * nb_steps;
                    hallway[i] = -1;
                    moved = true;
                }
            }
            return moved;
        }

        bool forced_rooms() {
            bool moved = false;

            bool intern_moved;
            do {
                intern_moved = false;
                for (int room=0; room<4; room++) {
                    int last_space = get_last_occupied_space(room);
                    if (last_space == -1) continue;

                    int amphi = rooms[room][last_space];
                    if (amphi == -1 || amphi == room || !is_room_available(amphi)) continue;

                    int curr_pos = 2 + 2*room;
                    int target = 2 + 2*amphi;
                    bool path_free = true;
                    for (int x=std::min(curr_pos, target); x<=std::max(curr_pos, target); x++) {
                        if (hallway[x] != -1) {
                            path_free = false;
                            break;
                        }
                    }
                    if (!path_free) continue;

                    int target_space = get_first_free_space(amphi);
                    int nb_steps = (nb_spaces - last_space) + std::abs(target - curr_pos) + (nb_spaces - target_space);

                    rooms[amphi][target_space] = amphi;
                    energy += get_score(amphi) * nb_steps;
                    rooms[room][last_space] = -1;
                    intern_moved = true;
                    moved = true;
                }
            } while (intern_moved);

            return moved;
        }
};

int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 10000000;
    int hallway[11];
    int rooms[4][4];

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
        std::string input;

		getline(myfile, input);
		getline(myfile, input);
        for (int i=1; i<12; i++) {
            if (input[i] == '.') hallway[i-1] = -1;
            else hallway[i-1] = input[i] - 'A';
        }

        for (int j=1; j>=0; j--) {
            getline(myfile, input);
            int line = (first_half ? j : 3*j);
            for (int i=0; i<4; i++) {
                if (input[3 + 2*i] == '.')
                    rooms[i][line] = -1;
                else
                    rooms[i][line] = input[3 + 2*i] - 'A';
            }
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (!first_half) {
        rooms[0][1] = 3;
        rooms[1][1] = 1;
        rooms[2][1] = 0;
        rooms[3][1] = 2;
        rooms[0][2] = 3;
        rooms[1][2] = 2;
        rooms[2][2] = 1;
        rooms[3][2] = 0;
    }

    State state(first_half ? 2 : 4, hallway, rooms);

    std::unordered_set<size_t> seen;
    std::queue<State> states;
    states.push(state);

    while (!states.empty()) {
        State curr_state = states.front();
        states.pop();

        for (State next_state : curr_state.generate_next_states()) {
            size_t next_state_hash = next_state.hash();
            if (seen.contains(next_state_hash)) continue;
            seen.insert(next_state_hash);

            next_state.forced_move();
            if (next_state.is_finished() && next_state.energy < ans)
                ans = next_state.energy;

            if (next_state.energy < ans && next_state.min_energy() < ans)
                states.push(next_state);
        }
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
