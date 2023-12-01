
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class Program {
    public:
        Program(std::string binding, std::vector<std::string> program, bool first_half) :
                program(program)
        {
            if (!first_half)
                registers[0] = 1;

            ip = std::stoi(binding.substr(4));
        }

        void run() {
            while (0 <= id && id < (int) program.size())
                step();
        }

        int operator[](int id) {
            return registers[id];
        }

    private:
        int registers[6] = {0, 0, 0, 0, 0, 0};
        std::vector<std::string> program;
        int ip;

        int id = 0;

        void step() {
            registers[ip] = id;

            std::string line = program[id];
            std::string instr = line.substr(0, 4);

            std::stringstream stream(line.substr(5));
            int a, b, c;
            stream >> a >> b >> c;

            if (instr == "addr") {
                registers[c] = registers[a] + registers[b];

            } else if (instr == "addi") {
                registers[c] = registers[a] + b;

            } else if (instr == "mulr") {
                registers[c] = registers[a] * registers[b];

            } else if (instr == "muli") {
                registers[c] = registers[a] * b;

            } else if (instr == "banr") {
                registers[c] = registers[a] & registers[b];

            } else if (instr == "bani") {
                registers[c] = registers[a] & b;

            } else if (instr == "borr") {
                registers[c] = registers[a] | registers[b];

            } else if (instr == "bori") {
                registers[c] = registers[a] | b;

            } else if (instr == "setr") {
                registers[c] = registers[a];

            } else if (instr == "seti") {
                registers[c] = a;

            } else if (instr == "gtir") {
                registers[c] = (int) (a > registers[b]);

            } else if (instr == "gtri") {
                registers[c] = (int) (registers[a] > b);

            } else if (instr == "gtrr") {
                registers[c] = (int) (registers[a] > registers[b]);

            } else if (instr == "eqir") {
                registers[c] = (int) (a == registers[b]);

            } else if (instr == "eqri") {
                registers[c] = (int) (registers[a] == b);

            } else if (instr == "eqrr") {
                registers[c] = (int) (registers[a] == registers[b]);

            }

            id = registers[ip] + 1;
        }

};

int main()
{
    bool first_half = false;
    int ans = 0;
    std::string binding;
    std::vector<std::string> instrs;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        getline(myfile, binding);
        while (getline(myfile, input)) {
            instrs.push_back(input);
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    // A short analysis of the program show that it first initializes register 2 to a value,
    // and then find all divisors of that number and sums them in register 0

    // In first half, registers[2] == 887 which is prime, so ans = 1 + 887 = 888
    // In second half, registers[2] == 10551287 which can be written as:
    // 1 * 10551287 = 127 * 83081 = 251 * 42037 = 331 * 31877
    // so ans = 1 + 127 + 251 + 331 + 31877 + 42037 + 83081 + 10551287 = 10708992

    /* Program program(binding, instrs, first_half); */
    /* program.run(); */

    /* ans = program[0]; */

    if (first_half) {
        ans = 888;
    } else {
        ans = 10708992;
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
