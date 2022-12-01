
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


struct ALU {
    std::unordered_map<char, long> registers;
    std::vector<std::string> instructions;

    ALU(std::vector<std::string> instructions) : instructions(instructions) {
        reset();
    }

    void reset() {
        registers['w'] = 0;
        registers['x'] = 0;
        registers['y'] = 0;
        registers['z'] = 0;
    }

    bool run(int input[14]) {
        reset();

        int count = -1;
        size_t index = 0;
        for (std::string instr : instructions) {
            std::string op = instr.substr(0, 3);
            char dst = instr[4];

            if (op == "inp") {
                /* print(count); */
                count++;
                registers[dst] = input[index++];
                continue;
            }

            int value;
            if ('w' <= instr[6] && instr[6] <= 'z')
                value = registers[instr[6]];
            else
                value = std::stoi(instr.substr(6));

            if (op == "add")
                registers[dst] += value;
            else if (op == "mul")
                registers[dst] *= value;
            else if (op == "div")
                registers[dst] /= value;
            else if (op == "mod")
                registers[dst] %= value;
            else if (op == "eql")
                registers[dst] = (registers[dst] == value ? 1 : 0);
        }

        /* print(); */
        return (registers['z'] == 0);
    }

    void print(int count=-1) {
        std::cout << "w=" << registers['w'] << ", ";
        std::cout << "x=" << registers['x'] << ", ";
        std::cout << "y=" << registers['y'] << ", ";
        if (count >= 0)
            std::cout << "z" << count << "=" << registers['z'] << std::endl;
        else
            std::cout << "z" << "=" << registers['z'] << std::endl;
    }
};

void next(int input[14]) {
    int index = 13;
    input[index]--;
    while (index > 0 && input[index] == 0) {
        input[index] = 9;
        if (index > 1)
            input[index-1]--;
        index--;
    }
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	long ans = 0;
    std::vector<std::string> instructions;

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

		while (getline(myfile, input))
            instructions.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    ALU alu(instructions);
    int input[14];
    if (first_half) {
        input[0] = 9;
        input[1] = 9;
        input[2] = 4;
        input[3] = 2;
        input[4] = 9;
        input[5] = 7;
        input[6] = 9;
        input[7] = 5;
        input[8] = 9;
        input[9] = 9;
        input[10] = 3;
        input[11] = 9;
        input[12] = 2;
        input[13] = 9;
    } else {
        input[0] = 1;
        input[1] = 8;
        input[2] = 1;
        input[3] = 1;
        input[4] = 3;
        input[5] = 1;
        input[6] = 8;
        input[7] = 1;
        input[8] = 5;
        input[9] = 7;
        input[10] = 1;
        input[11] = 6;
        input[12] = 1;
        input[13] = 1;
    }
    alu.run(input);

    for (int x : input)
        ans = 10*ans + x;
    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
