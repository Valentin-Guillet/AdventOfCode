
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <algorithm>


int incr(int size, int index) {
	index++;
	if (index >= size) {
		index -= size;
	}
	return index;
}


int main(void)
{
	std::string input;
	int ans = 0;
	std::vector<int> tab;

	std::ifstream myfile ("input");
	if (myfile.is_open())
	{
		getline(myfile, input);
		int i = 0;
		std::string number;
		input += '\n';
		while (i < input.length()) {
			if (input[i] != '\t' && input[i] != '\n') {
				number += input[i];
			} else {
				tab.push_back(std::stoi(number, nullptr));
				number = "";
			}
			i++;
		}
		myfile.close();
	}
	else std::cout << "Unable to open file";

	std::vector<std::vector<int>> set;
	set.push_back(tab);

	bool found = false;
	while (!found) {
		int max = *std::max_element(tab.begin(), tab.end());
		int index = 0;
		while (tab[index] != max) {
			index++;
		}
		tab[index] = 0;
		while (max > 0) {
			index = incr(tab.size(), index);
			tab[index]++;
			max--;
		}
		auto x = std::find(set.begin(), set.end(), tab);
		found = (x != set.end());
		if (!found) {
			set.push_back(tab);
		} else {
			while (set[ans] != *x) {
				ans++;
			}
			ans = set.size() -  ans;
		}
	}

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}