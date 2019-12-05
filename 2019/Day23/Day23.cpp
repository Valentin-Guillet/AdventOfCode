
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>


struct Packet {
    Packet(int dest, long x, long y) : dest(dest), x(x), y(y) { }
    int dest;
    long x;
    long y;
};


class Program
{
    private:
        std::unordered_map<long, long> tab;
        int pos = 0;
        int base_ref = 0;
        int packet_fill_id = 0;
        Packet curr_packet;
        int count_send;

    public:
        int id;
        std::queue<long> input;
        std::queue<Packet> output;

        Program(std::unordered_map<long, long> tab, int id) :
            tab(tab), curr_packet(0, 0, 0), id(id)
        {
            input.push(id);
        }

        long step(bool debug=false) {
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
                    if (!input.empty()) {
                        *arg1 = input.front();
                        input.pop();
                    } else {
                        *arg1 = -1;
                    }
                    pos += 2;
                    count_send++;
                    break;

                case 4:
                    if (packet_fill_id == 0) {
                        curr_packet.dest = *arg1;
                    } else if (packet_fill_id == 1) {
                        curr_packet.x = *arg1;
                    } else {
                        curr_packet.y = *arg1;
                        output.push(curr_packet);
                    }
                    packet_fill_id = (packet_fill_id + 1) % 3;
                    pos += 2;
                    count_send = 0;
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
                    return 1;

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

        bool idle() {
            return input.empty() && output.empty() && (count_send > 10);
        }

};


int main(void)
{
    bool firstHalf = false;
	std::string input;
    int ans = 0;
	std::unordered_map<long, long> tab;

	std::ifstream myfile ("Day23Input");
	if (myfile.is_open())
	{
        int i = 0;
		while (getline(myfile, input, ','))
            tab[i++] = std::stol(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file";
        return 1;
    }


    std::vector<Program> computers;
    for (long i=0; i<50; ++i)
        computers.push_back(Program(tab, i));

    Packet NAT(0, 0, 0);
    long last_y = -1;
    bool idle = false;
    int idle_countdown = 0;

    while (ans == 0) {
        idle = true;

        for (Program& computer : computers) {
            computer.step();

            while (!computer.output.empty()) {
                Packet packet = computer.output.front();

                if (packet.dest == 255) {

                    if (firstHalf) {
                        ans = packet.y;
                    } else {
                        NAT = packet;
                    }
                } else {
                    computers[packet.dest].input.push(packet.x);
                    computers[packet.dest].input.push(packet.y);
                }

                computer.output.pop();
            }

            if (!computer.idle())
                idle = false;
        }

        if (idle_countdown == 0 && idle) {
            computers[0].input.push(NAT.x);
            computers[0].input.push(NAT.y);
            idle_countdown = 100;

            if (NAT.y == last_y) {
                ans = NAT.y;
            } else {
                last_y = NAT.y;
            }
        }

        if (idle_countdown > 0)
            idle_countdown--;
    }
            
	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
