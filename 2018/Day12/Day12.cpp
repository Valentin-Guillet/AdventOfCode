
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>


void next(std::unordered_set<std::string>& rules, std::string& state, long& offset) {
    std::string next_state;

    state = "...." + state + "....";
    for (size_t i=0; i<state.size()-5; ++i)
        next_state += (rules.find(state.substr(i, 5)) != rules.end() ? '#' : '.');

    int id1 = next_state.find('#');
    int id2 = next_state.rfind('#');
    state = next_state.substr(id1, id2-id1+1);
    offset += id1-2;
}

long compute(std::string state, long offset) {
    long ans = 0;
    for (size_t i=0; i<state.size(); ++i) {
        if (state[i] == '#')
            ans += (i + offset);
    }
    return ans;
}

int main(void)
{
    bool first_half = false;
    std::string state;
    long ans = 0;
    std::unordered_set<std::string> rules;

    std::ifstream myfile ("input");
    if (myfile.is_open())
    {
        std::string input;
        getline(myfile, input);
        state = input.substr(15);

        getline(myfile, input);
        while (getline(myfile, input)) {
            if (input[9] == '#')
                rules.insert(input.substr(0, 5));
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    long offset = 0;

    if (first_half) {
        for (int i=0; i<20; ++i)
            next(rules, state, offset);

    } else {
        int step = 0;
        std::string prev_state;

        while (prev_state != state) {
            prev_state = state;
            next(rules, state, offset);
            step++;
        }

        offset += 50000000000 - step;
    }

    ans = compute(state, offset);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
