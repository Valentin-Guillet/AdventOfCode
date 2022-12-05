
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>


struct Instruction {
    int nb_crates, from, to;

    Instruction(std::string input) {
        size_t ind1 = 5;
        size_t ind2 = input.find(' ', ind1);
        nb_crates = std::stoi(input.substr(ind1, ind2 - ind1 + 1));

        ind1 = ind2 + 6;
        ind2 = input.find(' ', ind1);
        from = std::stoi(input.substr(ind1, ind2 - ind1 + 1));
        to = std::stoi(input.substr(ind2 + 4));
    }

};

std::vector<std::stack<char>> create_stacks(std::stack<std::string> input) {
    std::vector<std::stack<char>> stacks;

    int nb_stacks = (input.top().size() + 1) / 4;
    input.pop();
    for (int i=0; i<nb_stacks; i++)
        stacks.push_back({});

    while (!input.empty()) {
        std::string line = input.top();
        input.pop();
        for (int i=0; i<nb_stacks; i++) {
            size_t index = 4 * i + 1;
            if (line[index] != ' ')
                stacks[i].push(line[index]);
        }
    }

    return stacks;
}

void move_from_to(std::stack<char>& from, std::stack<char>& to, int nb_crates) {
    for (int i=0; i<nb_crates; i++) {
        to.push(from.top());
        from.pop();
    }
}

void move(std::vector<std::stack<char>>& stacks, Instruction const& instr, bool first_half) {
    if (first_half) {
        move_from_to(stacks[instr.from - 1], stacks[instr.to - 1], instr.nb_crates);
    } else {
        std::stack<char> tmp_stack;
        move_from_to(stacks[instr.from - 1], tmp_stack, instr.nb_crates);
        move_from_to(tmp_stack, stacks[instr.to - 1], instr.nb_crates);
    }
}


int main(int argc, char* argv[])
{
    bool first_half = true;
    std::string ans;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::stack<std::string> stacks_repr;
    std::vector<Instruction> instructions;
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            if (input.empty())
                break;
            stacks_repr.push(input);
        }

        while (getline(myfile, input))
            instructions.push_back(Instruction(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::vector<std::stack<char>> stacks = create_stacks(stacks_repr);
    for (Instruction instr : instructions)
        move(stacks, instr, first_half);

    for (auto stack : stacks)
        ans += stack.top();

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
