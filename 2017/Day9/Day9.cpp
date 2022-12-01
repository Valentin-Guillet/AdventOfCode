
#include "iostream"
#include "fstream"
#include <string>
#include <vector>


int main(void)
{
	std::string input;
	int ans = 0;
	std::vector<int> tab;
	bool first_half = false;

	std::ifstream myfile ("input");
	if (myfile.is_open())
	{
		getline(myfile, input);
		myfile.close();
	}
	else std::cout << "Unable to open file";

	int pos = 0;
	bool in_garbage = false;
	int level = 0;

	while (pos < input.size()) {
		if (!in_garbage) {
			if (input[pos] == '{') {
				level++;
			} else if (input[pos] == '}') {
				if (first_half) ans += level;
				level--;
			} else if (input[pos] == '<') {
				in_garbage = true;
			}
		} else {
			if (input[pos] == '!') {
				pos++;
			} else if (input[pos] == '>') {
				in_garbage = false;
			} else if (!first_half) {
				ans++;
			}
		}
		pos++;
	}

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}