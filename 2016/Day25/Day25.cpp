
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
        string output;

        void reset() {
            registers.clear();
            output.clear();
            ptr = 0;
        }

        bool step() {
            string instr = program[ptr];
            string cmd = instr.substr(0, 3);
            string args = instr.substr(4);

            bool out = true;

            if (cmd == "cpy") {
                int id = args.find(' ');
                string arg1 = args.substr(0, id);
                string arg2 = args.substr(id+1);

                // Execute `cpy` only if valid instr (due to `tgl`)
                if ('a' <= arg2[0] && arg2[0] <= 'z') {
                    int value_arg1 = 0;
                    if (arg1.size() == 1 && 'a' <= arg1[0] && arg1[0] <= 'z') {
                        value_arg1 = registers[arg1[0]];
                    } else {
                        value_arg1 = stoi(arg1);
                    }
                    registers[arg2[0]] = value_arg1;
                }

            } else if (cmd == "inc") {
                if ('a' <= args[0] && args[0] <= 'z')
                    registers[args[0]]++;

            } else if (cmd == "dec") {
                if ('a' <= args[0] && args[0] <= 'z')
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

                int value_arg2 = 0;
                if (arg2.size() == 1 && 'a' <= arg2[0] && arg2[0] <= 'z') {
                    value_arg2 = registers[arg2[0]];
                } else {
                    value_arg2 = stoi(arg2);
                }

                if (value_arg1 != 0)
                    ptr += value_arg2 - 1;

            } else if (cmd == "out") {
                int value_arg;
                if ('a' <= args[0] && args[0] <= 'z') {
                    value_arg = registers[args[0]];
                } else {
                    value_arg = stoi(args);
                }

                if ((value_arg == 0 && output.size() % 2 == 1) ||
                        (value_arg == 1 && output.size() % 2 == 0))
                    out = false;

                output += to_string(value_arg);

            } else {
                cout << "Command invalid, something went wrong !" << endl;
            }
            ptr++;

            return out;
        }

        bool valid() {
            for (size_t i=0; i<output.size(); i+=2) {
                if (output[i] != '0' || (i+1 < output.size() && output[i+1] != '1'))
                    return false;
            }
            return true;
        }

    public:
        Computer(vector<string> program) : program(program) {
            /* First 8 instructions of the program just takes `a` as input and
             * set `d` to 643*4 + a, so we're manually doing it */
            program.erase(program.begin(), program.begin()+8);
        }

        int solve() {
            int index = 0;
            while (true) {
                reset();
                registers['a'] = index;
                registers['d'] = index + 2572;
                while (output.size() < 50 && step()) {  }

                if (output.size() == 50)
                    return index;

                index++;
            }
        }
};

int main()
{
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
    ans = computer.solve();

    cout << "Answer : " << ans << endl;
    return 0;
}
