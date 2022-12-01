
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <algorithm>

int main(void)
{
	std::string input;
	bool first_half = false;
	int ans = 1000;
	int particules[1000][9];
	std::vector<int> indexes;
	for (int i = 0; i < 1000; ++i)
		indexes.push_back(i);
	std::unordered_map<std::string, std::vector<int>> collided;

	std::regex rgx("p=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, v=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, a=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>");
	std::smatch sm;

	std::ifstream myfile ("input");
	if (myfile.is_open())
	{
		int i = 0;
		while (getline(myfile, input)) {
			std::regex_match(input, sm, rgx);
			for (int j = 0; j < 9; ++j)
				particules[i][j] = std::stoi(sm[j+1]);
			++i;
		}
		myfile.close();
	}
	else std::cout << "Unable to open file";

	for (int i = 0; i < 1000; ++i)
	{
		// for (int i = 0; i < 9; ++i)
			// std::cout << particules[0][i] << "< ";
		// std::cout<<std::endl << std::endl;
		collided.clear();

		for (int i = indexes.size()-1; i >= 0; --i) {
			int id = indexes[i];
			particules[id][3] += particules[id][6];
			particules[id][4] += particules[id][7];
			particules[id][5] += particules[id][8];
			particules[id][0] += particules[id][3];
			particules[id][1] += particules[id][4];
			particules[id][2] += particules[id][5];
			std::string pos = std::to_string(particules[id][0]) + ","
							+ std::to_string(particules[id][1]) + ","
							+ std::to_string(particules[id][2]);
			collided[pos].push_back(id);
		}
		for (auto it : collided) {
			if (it.second.size() > 1) {
				for (int id : it.second) {
					indexes.erase(std::find(indexes.begin(), indexes.end(), id));
					--ans;
				}
			}
		}
	}

	if (first_half) {
		long min = 1000000000000;
		for (int i = 0; i < 1000; ++i) {
			long dist = abs(particules[i][0]) + abs(particules[i][1]) + abs(particules[i][2]);
			if (dist < min) {
				ans = i;
				min = dist;
			}
		}
	}

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}