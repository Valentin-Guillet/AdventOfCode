
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <bitset>


struct Range {
    std::string range;
    long value;
    std::unordered_set<std::string> overwritten;

    Range(std::string mask, int address, long value) : value(value) {
        std::string binary = std::bitset<36>(address).to_string();
        for (size_t i=0; i<36; ++i) {
            if (mask[i] == '0')
                range += binary[i];
            else
                range += mask[i];
        }
    }

    void update(std::string other_range) {
        for (size_t i=0; i<36; ++i) {
            if (other_range[i] != 'X' && range[i] != 'X' && other_range[i] != range[i])
                return;
        }

        for (size_t i=0; i<36; ++i) {
            if (other_range[i] == 'X' && range[i] != 'X')
                other_range[i] = range[i];
        }

        int nb_float = std::count(other_range.begin(), other_range.end(), 'X');
        size_t limit = std::pow(2, nb_float);
        for (size_t i=0; i<limit; ++i) {
            std::string bitmask = std::bitset<36>(i).to_string().substr(36-nb_float);
            std::string copy_range(other_range);
            size_t ind = 0;
            for (size_t j=0; j<36; ++j) {
                if (copy_range[j] == 'X')
                    copy_range[j] = bitmask[ind++];
            }
            overwritten.insert(copy_range);
        }
    }

    long count() {
        int nb_float = std::pow(2, std::count(range.begin(), range.end(), 'X'));
        return (nb_float - overwritten.size()) * value;
    }

};

long apply_bitmask(long value, std::string mask) {
    std::string binary = std::bitset<36>(value).to_string();
    std::string masked_value;
    for (int i=0; i<36; ++i) {
        if (mask[i] == '0')
            masked_value += '0';
        else if (mask[i] == '1')
            masked_value += '1';
        else
            masked_value += binary[i];
    }
    return std::stol(masked_value, 0, 2);
}


int main()
{
    bool first_half = false;
	long ans = 0;
    std::vector<std::string> instructions;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            instructions.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::unordered_map<int, long> memory;
    std::string mask;

    if (first_half) {
        for (std::string instr : instructions) {
            if (instr.substr(0, 4) == "mask") {
                mask = instr.substr(7);
            } else {
                size_t index = instr.find(']');
                int addr = std::stoi(instr.substr(4, index-4));
                long value = std::stol(instr.substr(index+4));
                memory[addr] = apply_bitmask(value, mask);
            }
        }

        for (auto [addr, value] : memory)
            ans += value;

    } else {
        std::vector<Range> ranges;
        for (std::string instr : instructions) {
            if (instr.substr(0, 4) == "mask") {
                mask = instr.substr(7);
            } else {
                size_t index = instr.find(']');
                int addr = std::stoi(instr.substr(4, index-4));
                long value = std::stol(instr.substr(index+4));
                Range new_range(mask, addr, value);
                for (Range &prev_range : ranges)
                    prev_range.update(new_range.range);
                ranges.push_back(new_range);
            }
        }

        for (Range range : ranges)
            ans += range.count();
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
