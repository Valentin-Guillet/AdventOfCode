
#include "iostream"
#include "fstream"
#include <string>
#include <vector>

std::vector<std::pair<int, int>> components;
const int SIZE = 57;

std::pair<int, int> rec(int size, int length, int port, bool seen[SIZE])
{
	int max_s = size;
	int max_l = length;
	for (int i = 0; i < components.size(); ++i) {
		if (!seen[i] && (components[i].first == port || components[i].second == port)) {
			bool copy_seen[SIZE];
			for (int j = 0; j < SIZE; ++j)
				copy_seen[j] = seen[j];
			copy_seen[i] = true;
			int sum = length + components[i].first + components[i].second;
			std::pair<int, int> truc;
			if (components[i].first == port)
				truc = rec(size+1, sum, components[i].second, copy_seen);
			else
				truc = rec(size+1, sum, components[i].first, copy_seen);
			if (truc.first > max_s && truc.second > max_l) {
				max_s = truc.first;
				max_l = truc.second;
			}
		}
	}
	return {max_s, max_l};
}


int main(void)
{
	std::string input;
	std::vector<int> tab;

	std::ifstream myfile ("input");
	if (myfile.is_open())
	{
		while(getline(myfile, input)) {
			int pos = input.find('/');
			int n1 = std::stoi(input.substr(0, pos));
			int n2 = std::stoi(input.substr(pos+1, std::string::npos));
			components.push_back({n1, n2});
		}
		myfile.close();
	}
	else std::cout << "Unable to open file";

	bool seen[SIZE] = {false};
	std::pair<int, int> ans = rec(0, 0, 0, seen);

	std::cout << "Answer : " << ans.second << std::endl;
	return 0;
}