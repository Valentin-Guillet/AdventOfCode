
#include "iostream"
#include "fstream"
#include <string>
#include <regex>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cctype>


int get(std::unordered_map<char, long long int> registers, std::string s) {
	if (std::isalpha(s[0]))
		return registers[s[0]];
	else
		return std::stoi(s);
}

int main(void)
{
	std::string input;
	int ans = 0;
	std::vector<std::array<std::string, 3>> instr;

	std::regex rgx("([a-z]{3}) ([0-9a-z]) ?(.*)");
	std::smatch sm;

	std::ifstream myfile ("Day23Input");
	if (myfile.is_open())
	{
		while(getline(myfile, input)) {
			std::regex_match(input, sm, rgx);
			std::cout << sm[1] << ", " << sm[2] << ", " << sm[3] << std::endl;
			instr.push_back({sm[1], sm[2], sm[3]});
		}
		myfile.close();
	}
	else std::cout << "Unable to open file";

	int pos = 0;
	std::unordered_map<char, long long int> registers;
	registers['a'] = 1;
	while (0 <= pos && pos < instr.size()) {
		std::cout << registers['b'] << std::endl;
		auto s = instr[pos];
		if (pos == 10) {
			registers['f'] = 0;
			registers['d'] = registers['b'];
			registers['e'] = registers['b'];
			pos = 24;
			continue;
		}

		if (s[0] == "set") {
			registers[s[1][0]] = get(registers, s[2]);

		} else if (s[0] == "sub") {
			registers[s[1][0]] -= get(registers, s[2]);

		} else if (s[0] == "mul") {
			registers[s[1][0]] *= get(registers, s[2]);
		}

		if (s[0] == "jnz") {
			if (std::isalpha(s[1][0])) {
				if (registers[s[1][0]] != 0)
					pos += get(registers, s[2]);
				else
					++pos;
			} else {
				if (std::stoi(s[1]) != 0)
					pos += get(registers, s[2]);
				else
					++pos;
			}
		} else {
			++pos;
		}
	}

	std::cout << "Answer : " << registers['h'] << std::endl;

	return 0;
}