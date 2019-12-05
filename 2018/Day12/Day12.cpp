
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <queue>
#include <regex>

int main(void)
{
	std::string input;
	int ans = 0;
	std::vector<int> tab[2000];

	std::regex rgx("[0-9]+ <-> (.*)");
	std::smatch sm;

	std::ifstream myfile ("Day12Input");
	if (myfile.is_open())
	{
		int i = 0;
		while(getline(myfile, input)) {
			std::regex_match(input, sm, rgx);
			input = sm[1].str();
			input += ',';
			// std::cout << input << std::endl;
			int pos;
			while((pos = input.find(",")) != std::string::npos) {
				tab[i].push_back(std::stoi(input.substr(0, pos)));
				input.erase(0, pos+1);
			}
			++i;
		}
		myfile.close();
	}
	else std::cout << "Unable to open file"; 
	
	bool seen[2000] = {false};
	while(std::find(std::begin(seen), std::end(seen), false) != std::end(seen))
	{
		int init = 0;
		while(seen[init])
			++init;

		std::queue<int> queue;
		queue.push(init);
		while(queue.size() > 0) {
			int x = queue.front();
			queue.pop();
			seen[x] = true;
			for (int i = 0; i < tab[x].size(); ++i)
			{
				if(!seen[tab[x][i]]) {
					queue.push(tab[x][i]);
				}
			}
		}
		++ans;	
	}

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}