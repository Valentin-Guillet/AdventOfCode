
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

std::string replace(std::unordered_map<std::string, std::string> rules, std::string pattern, int div) {
	int i = 0;
	while (rules.find(pattern) == rules.end()) {
		if (i == 3) {
			for (int j = 0; j < div; ++j) {
				char tmp = pattern[j*(div+1)];
				pattern[j*(div+1)] = pattern[j*(div+1) + div-1];
				pattern[j*(div+1) + div-1] = tmp;
			}
		} else {
			char tmp = pattern[0];
			pattern[0] = pattern[div-1];
			pattern[div-1] = pattern[(div-1)*(div+2)];
			pattern[(div-1)*(div+2)] = pattern[(div-1)*(div+1)];
			pattern[(div-1)*(div+1)] = tmp;

			if (div == 3) {
				tmp = pattern[1];
				pattern[1] = pattern[6];
				pattern[6] = pattern[9];
				pattern[9] = pattern[4];
				pattern[4] = tmp;
			}
		}
		++i;
	}
	return rules[pattern];
}

int main(void)
{
	std::string input;
	int ans = 0;
	std::unordered_map<std::string, std::string> rules;
	std::vector<std::string> grid = {".#.", "..#", "###"};

	std::ifstream myfile ("Day21Input");
	if (myfile.is_open())
	{
		while (getline(myfile, input)) {
			int pos = input.find(" => ");
			rules[input.substr(0, pos)] = input.substr(pos + 4);
		}
		myfile.close();
	}
	else std::cout << "Unable to open file";

	std::vector<std::string> copy_grid;
	for (int step = 0; step < 18; ++step) {
		int div = 2;
		int size = grid[0].size();
		if (size % 2 == 1)
			++div;
	
		copy_grid.clear();	
		for (int i = 0; i < size/div*(div+1); ++i)
			copy_grid.push_back(std::string (size/div*(div+1), '.'));

		for (int x = 0; x < size/div; ++x) {
			for (int y = 0; y < size/div; ++y) {
				std::string pattern = "";
				for (int j = 0; j < div; ++j) {
					pattern += grid[x*div + j].substr(y*div, div);
					if (j < div-1)
						pattern += "/";
				}
				pattern = replace(rules, pattern, div);

				for (int i = 0; i < div+1; ++i) {
					for (int j = 0; j < div+1; ++j)
						copy_grid[x*(div+1) + i][y*(div+1) + j] = pattern[i * (div+2) + j];
				}
			}
		}
		grid = copy_grid;

		// for (int x = 0; x < grid.size(); ++x)
			// std::cout << grid[x] << std::endl;
		// std::cout << "----------------------------------------" << std::endl;
		std::cout << "Iteration " << step << std::endl;
	}

	for (std::string line : grid) {
		ans += std::count(line.begin(), line.end(), '#');
	}

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}