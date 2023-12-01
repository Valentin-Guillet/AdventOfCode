
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;


class Computer {
    private:
        vector<string> program;
        int ptr = 0;
        unordered_map<char, int> registers;

        void step() {
            string instr = program[ptr];
            string cmd = instr.substr(0, 3);
            string args = instr.substr(4);

            if (cmd == "cpy") {
                int id = args.find(' ');
                string arg1 = args.substr(0, id);
                string arg2 = args.substr(id+1);

                int value_arg1 = 0;
                if (arg1.size() == 1 && 'a' <= arg1[0] && arg1[0] <= 'z') {
                    value_arg1 = registers[arg1[0]];
                } else {
                    value_arg1 = stoi(arg1);
                }
                registers[arg2[0]] = value_arg1;

            } else if (cmd == "inc") {
                registers[args[0]]++;

            } else if (cmd == "dec") {
                registers[args[0]]--;

            } else if (cmd == "jnz") {
                int id = args.find(' ');
                string arg1 = args.substr(0, id);
                string arg2 = args.substr(id+1);

                int value_arg1 = 0;
                if (arg1.size() == 1 && 'a' <= arg1[0] && arg1[0] <= 'z') {
                    value_arg1 = registers[arg1[0]];
                } else {
                    value_arg1 = stoi(arg1);
                }

                if (value_arg1 != 0)
                    ptr += stoi(arg2) - 1;

            }
            ptr++;
        }

    public:
        Computer(vector<string> program) : program(program) { }

        void run() {
            while (ptr < (int) program.size())
                step();
        }

        void set(char c, int value) {
            registers[c] = value;
        }

        int get(char c) {
            return registers[c];
        }
};

int main()
{
    bool first_half = false;
    string input;
    int ans = 0;
    vector<string> tab;

    ifstream myfile ("input");
    if (myfile.is_open())
    {
        while (getline(myfile, input))
            tab.push_back(input);

        myfile.close();
    } else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    Computer computer(tab);

    if (!first_half)
        computer.set('c', 1);

    computer.run();
    ans = computer.get('a');

    cout << "Answer : " << ans << endl;
    return 0;
}
