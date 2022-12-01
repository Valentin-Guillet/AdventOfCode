
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>


std::pair<bool, int> run_program(std::vector<std::string> const& instr) {
    int accumulator = 0;
    int ip = 0;
    std::unordered_set<int> seen;

    while (0 <= ip && ip < (int) instr.size()) {
        if (seen.find(ip) != seen.end())
            return {false, accumulator};
        seen.insert(ip);

        int value = std::stoi(instr[ip].substr(5));
        if (instr[ip].at(4) == '-')
            value = -value;
        std::string operation = instr[ip].substr(0, 3);
        if (operation == "nop") {
            ip++;
        } else if (operation == "acc") {
            accumulator += value;
            ip++;
        } else if (operation == "jmp") {
            ip += value;
        }
    }

    return {true, accumulator};
}


int main()
{
    bool first_half = false;
	int ans = 0;
    std::vector<std::string> instr;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            instr.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        ans = run_program(instr).second;
    } else {
        for (size_t i=0; i<instr.size(); ++i) {
            std::vector<std::string> copy_instr(instr);
            if (copy_instr[i].substr(0, 3) == "jmp")
                copy_instr[i] = "nop" + copy_instr[i].substr(3);
            else if (copy_instr[i].substr(0, 3) == "nop")
                copy_instr[i] = "jmp" + copy_instr[i].substr(3);

            auto [completed, acc] = run_program(copy_instr);
            if (completed) {
                ans = acc;
                break;
            }
        }
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
