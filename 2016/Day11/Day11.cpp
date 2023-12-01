
#include <iostream>
#include <string>
#include <array>
#include <queue>
#include <vector>
#include <unordered_set>

using namespace std;


class State
{
    private:
        int step;
        int floor;
        array<int, 7> chips;
        array<int, 7> generators;

        bool valid() {

            if (floor < 0 || 3 < floor)
                return false;

            for (int i=0; i<7; ++i) {
                if (chips[i] < 0 || 3 < chips[i])
                    return false;

                if (generators[i] < 0 || 3 < generators[i])
                    return false;

                bool irradiated = false;

                for (int j=0; j<7; ++j) {
                    if (i == j)
                        continue;

                    if (chips[i] == generators[j]) {
                        irradiated = true;
                        break;
                    }
                }

                if (chips[i] != generators[i] && irradiated)
                    return false;
            }

            return true;
        }

    public:
        State(int step, int floor, array<int, 7> init_chips, array<int, 7> init_generators) :
            step(step), floor(floor), chips(init_chips), generators(init_generators) { }

        State(State const& other, int id, int dir) {
            step = other.step + 1;
            floor = other.floor + dir;
            chips = other.chips;
            generators = other.generators;

            if (id < 7) {
                chips[id] += dir;
            } else {
                generators[id-7] += dir;
            }
        }

        State(State const& other, int id1, int id2, int dir) {
            step = other.step + 1;
            floor = other.floor + dir;
            chips = other.chips;
            generators = other.generators;

            if (id1 < 7) {
                chips[id1] += dir;
            } else {
                generators[id1-7] += dir;
            }

            if (id2 < 7) {
                chips[id2] += dir;
            } else {
                generators[id2-7] += dir;
            }
        }

        queue<State> next() {
            queue<State> next_states;

            vector<int> idx;
            for (int i=0; i<7; ++i) {
                if (floor == chips[i])
                    idx.push_back(i);

                if (floor == generators[i])
                    idx.push_back(i+7);
            }

            for (int id : idx) {
                State up(*this, id, 1);

                if (up.valid())
                    next_states.push(up);

                State down(*this, id, -1);

                if (down.valid())
                    next_states.push(down);
            }

            if (idx.size() > 1) {
                for (size_t i=0; i<idx.size()-1; ++i) {
                    for (size_t j=i+1; j<idx.size(); ++j) {
                        State up(*this, idx[i], idx[j], 1);
                        if (up.valid())
                            next_states.push(up);

                        State down(*this, idx[i], idx[j], -1);
                        if (down.valid())
                            next_states.push(down);
                    }
                }
            }

            return next_states;
        }

        int get_step() {
            return step;
        }

        bool finished() {
            if (floor < 3)
                return false;

            for (int i=0; i<7; ++i) {
                if (chips[i] < 3)
                    return false;
                if (generators[i] < 3)
                    return false;
            }
            return true;
        }

        size_t hash() const {
            size_t h = floor;
            for (int i=0; i<7; ++i) {
                h <<= 2;
                h += chips[i];
                h <<= 2;
                h += generators[i];
            }
            return h;
        }

        bool operator==(const State& other) const {
            return (other.floor == floor && other.chips == chips && other.generators == generators);
        }

        void debug() {
            /* char letters[2] = {'H', 'L'}; */
            char letters[7] = {'S', 'P', 'T', 'R', 'C', 'E', 'D'};

            for (int i=3; -1<i; --i) {
                cout << "F" << i << " ";
                cout << (floor == i ? 'E' : '.') << " ";
                for (int j=0; j<7; ++j)
                    cout << (chips[j] == i ? letters[j] : '.') << " ";

                for (int j=0; j<7; ++j)
                    cout << (generators[j] == i ? letters[j] : '.') << " ";
                cout << endl;
            }
        }

};

struct HashFunction {
    size_t operator()(State const& state) const {
        return state.hash();
    }
};


int main()
{
    bool first_half = true;
    int ans = 0;
    queue<State> states;
    unordered_set<State, HashFunction> seen;

    State init_state(0, 0, {0, 0, 2, 1, 1, 0, 0}, {0, 0, 1, 1, 1, 0, 0});
    states.push(init_state);
    seen.insert(init_state);

    int step_size = 0;
    while (true) {

        State curr_state = states.front();
        states.pop();

        queue<State> next_states = curr_state.next();
        while (!next_states.empty()) {
            State next_state = next_states.front();
            next_states.pop();

            if (next_state.finished()) {
                ans = next_state.get_step();
                goto break_out;
            }

            if (seen.find(next_state) == seen.end()) {
                states.push(next_state);
                seen.insert(next_state);
            }
        }

        if (states.size() == 0) {
            cout << "No solution !" << endl;
            goto break_out;
        }
    }

break_out:
    cout << "Answer : " << ans << endl;
    return 0;
}
