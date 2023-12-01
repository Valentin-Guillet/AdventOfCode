
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>


class Program
{
    private:
        std::unordered_map<long, long> tab;
        int pos = 0;
        int base_ref = 0;
        std::queue<char> input;

        int step(bool debug=false) {
            int instruction = tab[pos] % 100;
            int modes = tab[pos] / 100;
            long *arg1, *arg2, *arg3;

            if (instruction != 99) {
                if (modes % 10 == 0) {
                    arg1 = &tab[tab[pos+1]];
                } else if (modes % 10 == 1) {
                    arg1 = &tab[pos+1];
                } else if (modes % 10 == 2) {
                    arg1 = &tab[base_ref + tab[pos+1]];
                }

                if ((modes/10) % 10 == 0) {
                    arg2 = &tab[tab[pos+2]];
                } else if ((modes/10) % 10 == 1) {
                    arg2 = &tab[pos+2];
                } else if ((modes/10) % 10 == 2) {
                    arg2 = &tab[base_ref + tab[pos+2]];
                }

                if ((modes/100) % 10 == 0) {
                    arg3 = &tab[tab[pos+3]];
                } else if ((modes/100) % 10 == 2) {
                    arg3 = &tab[base_ref + tab[pos+3]];
                }
            }

            if (debug) {
                std::cout << "Instruction : " << instruction << std::endl;
                std::cout << "Position : " << pos << ", tab[pos] = " << tab[pos] << ", base_ref = " << base_ref << std::endl;
                std::cout << "Args : " << *arg1 << ", " << *arg2 << ", " << *arg3 << std::endl;
            }

            switch (instruction) {
                case 1:
                    *arg3 = *arg1 + *arg2;
                    pos += 4;
                    break;

                case 2:
                    *arg3 = *arg1 * *arg2;
                    pos += 4;
                    break;

                case 3:
                    if (input.empty())
                        return 1;
                    *arg1 = input.front();
                    input.pop();
                    pos += 2;
                    break;

                case 4:
                    std::cout << (char) *arg1;
                    pos += 2;
                    break;

                case 5:
                    if (*arg1 != 0) {
                        pos = *arg2;
                    } else {
                        pos += 3;
                    }
                    break;

                case 6:
                    if (*arg1 == 0) {
                        pos = *arg2;
                    } else {
                        pos += 3;
                    }
                    break;

                case 7:
                    *arg3 = (long) (*arg1 < *arg2);
                    pos += 4;
                    break;

                case 8:
                    *arg3 = (long) (*arg1 == *arg2);
                    pos += 4;
                    break;

                case 9:
                    base_ref += *arg1;
                    pos += 2;
                    break;

                case 99:
                    std::cout << "End of program" << std::endl;
                    return -1;

                default:
                    std::cout << "Invalid instruction !" << std::endl;
                    break;
            }

            if (debug)
            {
                /* for (auto elt : tab) */
                /*     std::cout << "A" << elt.first << "/V" << elt.second << ", "; */
                /* std::cout << std::endl; */
                std::cout << "Args : " << *arg1 << ", " << *arg2 << ", " << *arg3 << std::endl << std::endl;
            }

            return 0;
        }

    public:
        Program(std::unordered_map<long, long> tab) : tab(tab) { }

        void run() {

            int status = 0;
            while (status != -1) {

                if (status == 1) {
                    std::string cmd;
                    std::getline(std::cin, cmd);
                    for (char c : cmd)
                        input.push((long) c);

                    input.push(10);
                    status = 0;
                }

                while (status == 0) {
                    status = step();
                }
            }
        }


};


int main(void)
{
    int ans = 0;
    std::unordered_map<long, long> tab;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        int i = 0;
        while (getline(myfile, input, ','))
            tab[i++] = std::stol(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    Program program(tab);
    program.run();

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
