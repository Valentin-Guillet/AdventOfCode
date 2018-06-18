
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

int main(void)
{
	bool firstHalf = true;
	std::string input;
	std::vector<std::unordered_map<char, int>> tab;
	int ans = 0;

	std::ifstream myfile ("Day4Input");
	if (myfile.is_open())
	{
		while (getline(myfile, input)) {
			int i = 0;
			// std::cout << input << std::endl;

			std::unordered_map<char, int> word;

			tab.clear();
			input += '\n';
			bool okay = true;
			while (i < input.length()) {
				if (input[i] != ' ' && input[i] != '\n') {
					word[input[i]]++;
				} else {
					if (find(tab.begin(), tab.end(), word) != tab.end()) {
						okay = false;
					} else {
						tab.push_back(word);
						word.clear();
					}
					if (input[i] == '\n' && okay) {
						ans++;
					}
				}
				i++;
			}
		}
		myfile.close();
	}
	else std::cout << "Unable to open file"; 

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}