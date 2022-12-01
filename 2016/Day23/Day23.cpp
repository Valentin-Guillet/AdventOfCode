
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;


unordered_map<string, string> TOGGLE =
    {{"inc", "dec"},
     {"dec", "inc"},
     {"tgl", "inc"},
     {"jnz", "cpy"},
     {"cpy", "jnz"},
    };

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

            } else if (cmd == "tgl") {
                int value_arg;
                if ('a' <= args[0] && args[0] <= 'z') {
                    value_arg = registers[args[0]];
                } else {
                    value_arg = stoi(args);
                }
                int instr_ptr = ptr + value_arg;
                cout << "Let's toggle line " << instr_ptr << endl;

                if (0 <= instr_ptr && instr_ptr < (int) program.size())
                    program[instr_ptr] = TOGGLE[program[instr_ptr].substr(0, 3)] + program[instr_ptr].substr(3);

            } else {
                cout << "Command invalid, something went wrong !" << endl;
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
    if (first_half) {
        computer.set('a', 7);
        computer.run();
    } else {
        // computer.set('a', 12);
        /* computer.run(); */
        /* By analysing the program, we can see that :
         * 1. First, a <- 12, b <- 11
         * 2. Then [l2 -> l9], a is multiplied by b
         * 3. Then [l10 -> l18], b is decr and the line 16+2*b is toggled
         * 4. The steps 2. and 3. are repeated while the toggle affect line 18
         *    leading to a <- 12*11*10*... i.e. a <- factorial(12)
         * 5. Then [l19 -> l25], c <- 93, d <- 80 and a <- a + d*c
         * So finally, a <- factorial(12) + 93 * 80 */
        computer.set('a', 12*11*10*9*8*7*6*5*4*3*2 + 93*80);
    }

    ans = computer.get('a');

	cout << "Answer : " << ans << endl;
	return 0;
}
