
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


std::string hex_to_bin(std::string hex) {
    std::string bin;
    for (char c : hex) {
        int n;
        if ('0' <= c && c <= '9')
            n = c - '0';
        else
            n = c - 'A' + 10;
        for (int j=3; j>=0; j--)
            bin.push_back((n & (1 << j)) ? '1' : '0');
    }
    return bin;
}

long long int bin_to_long(std::string bin) {
    return std::stol(bin, nullptr, 2);
}

class Packet {
    public:
        Packet(std::string bin_packet) : bin_packet(bin_packet) { }
        ~Packet() {
            for (Packet* subpacket : subpackets)
                delete subpacket;
        }

        int parse() {
            std::string orig_bin_packet(bin_packet);
            int length;
            version = bin_to_long(bin_packet.substr(0, 3));
            id = bin_to_long(bin_packet.substr(3, 3));
            bin_packet = bin_packet.substr(6);
            length = 6;

            // Litteral value
            if (id == 4) {
                length += parse_litteral(bin_packet);

            } else {
                int type = bin_packet[0] - '0';
                length++;
                bin_packet = bin_packet.substr(1);
                if (type == 0)
                    length += parse_subpackets_length(bin_packet);
                else
                    length += parse_subpackets_number(bin_packet);
            }

            return length;
        }

        int add_versions() {
            int sum = version;
            for (Packet* packet : subpackets)
                sum += packet->add_versions();
            return sum;
        }

        long long int compute() {
            if (id == 4)
                return value;

            if (id < 4) {
                long long int ans = 0;
                if (id == 1) ans = 1;
                if (id == 2) ans = 1000000000000;
                for (Packet* packet : subpackets) {
                    long long int subpacket_value = packet->compute();
                    switch (id) {
                        case 0: ans += subpacket_value; break;
                        case 1: ans *= subpacket_value; break;
                        case 2: ans = std::min(ans, subpacket_value); break;
                        case 3: ans = std::max(ans, subpacket_value); break;
                    }
                }
                return ans;
            }

            long long int first = subpackets[0]->compute();
            long long int second = subpackets[1]->compute();
            switch (id) {
                case 5: return (long long int)(first > second);
                case 6: return (long long int)(first < second);
                case 7: return (long long int)(first == second);
                default: return 0;   // Unreachable
            }
        }

        void debug(int depth=0) {
            for (int i=0; i<depth; i++)
                std::cout << "  ";
            std::cout << "V" << version << " ID" << id << " ";
            std::string msg;
            switch (id) {
                case 0: msg = "+"; break;
                case 1: msg = "*"; break;
                case 2: msg = "min"; break;
                case 3: msg = "max"; break;
                case 4: msg = "[Litteral], value = " + std::to_string(value); break;
                case 5: msg = ">"; break;
                case 6: msg = "<"; break;
                case 7: msg = "=="; break;
            }
            std::cout << msg << std::endl;

            for (Packet* subpacket : subpackets)
                subpacket->debug(depth+1);
        }

    private:
        std::string bin_packet;
        int version;
        int id;
        long long int value;
        std::vector<Packet*> subpackets;

        int parse_litteral(std::string bin_packet) {
            std::string bin_value;
            size_t i = 0;
            do {
                bin_value += bin_packet.substr(i+1, 4);
                i += 5;
            } while (bin_packet[i-5] == '1');

            value = bin_to_long(bin_value);
            return i;
        }

        int parse_subpackets_length(std::string bin_packet) {
            long long int length = bin_to_long(bin_packet.substr(0, 15));
            bin_packet = bin_packet.substr(15);

            long long int curr_length = 0;
            while (curr_length < length) {
                Packet* new_subpacket = new Packet(bin_packet);
                long long int new_length = new_subpacket->parse();
                subpackets.push_back(new_subpacket);

                curr_length += new_length;
                bin_packet = bin_packet.substr(new_length);
            }

            return 15 + length;
        }

        int parse_subpackets_number(std::string bin_packet) {
            long long int nb_subpackets = bin_to_long(bin_packet.substr(0, 11));
            bin_packet = bin_packet.substr(11);

            long long int length = 0;
            for (int i=0; i<nb_subpackets; i++) {
                Packet* new_subpacket = new Packet(bin_packet);
                long long int new_length = new_subpacket->parse();
                subpackets.push_back(new_subpacket);

                length += new_length;
                bin_packet = bin_packet.substr(new_length);
            }

            return 11 + length;
        }
};

int main(int argc, char* argv[])
{
    bool first_half = true;
	long long int ans = 0;
    std::string hex_packet;

    std::ifstream myfile("Day16Input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("testInput");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
		getline(myfile, hex_packet);
		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    Packet packet(hex_to_bin(hex_packet));
    packet.parse();
    /* packet.debug(); */

    if (first_half)
        ans = packet.add_versions();
    else
        ans = packet.compute();

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
