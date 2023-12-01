
#include "iostream"
#include "fstream"
#include <unordered_set>

struct state
{
    int write[2];
    int move[2];
    int next_state[2];
};

int main(void)
{
    std::unordered_set<int> ones;
    state A = {{1, 0}, {1, -1}, {1, 2}};
    state B = {{1, 1}, {-1, 1}, {0, 3}};
    state C = {{0, 0}, {-1, -1}, {1, 4}};
    state D = {{1, 0}, {1, 1}, {0, 1}};
    state E = {{1, 1}, {-1, -1}, {5, 2}};
    state F = {{1, 1}, {1, 1}, {3, 0}};
    state states[6] = {A, B, C, D, E, F};

    state current_state = A;
    int pos = 0;

    for (int i = 0; i < 12481997; ++i)
    {
        bool value = (ones.find(pos) != ones.end());
        if (current_state.write[value])
            ones.insert(pos);
        else
            ones.erase(pos);

        pos += current_state.move[value];

        current_state = states[current_state.next_state[value]];
    }

    std::cout << "Answer : " << ones.size() << std::endl;
    return 0;
}