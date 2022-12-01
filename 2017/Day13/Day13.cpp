
#include "iostream"
#include "fstream"
#include <string>
#include <unordered_map>
#include <utility>
#include <regex>

int main(void)
{
	std::string input;
	int ans = -1;
	std::unordered_map<int, std::tuple<int, int>> tab;

	std::regex rgx("([0-9]+): ([0-9]+)");
	std::smatch sm;

	int size = 0;
	std::ifstream myfile ("input");
	if (myfile.is_open())
	{
		while(getline(myfile, input)) {
			std::regex_match(input, sm, rgx);
			size = std::stoi(sm[1]);
			tab[size] = std::make_tuple(2*std::stoi(sm[2])-2, 0);
		}
		myfile.close();
	}
	else std::cout << "Unable to open file";

	bool caught = true;
	while (caught) {
		caught = false;
		for (auto& elt : tab) {
			--std::get<1>(elt.second);
			if (std::get<1>(elt.second) <= 0)
				std::get<1>(elt.second) = std::get<0>(elt.second);
			if ((elt.first - std::get<1>(elt.second)) % std::get<0>(elt.second) == 0)
				caught = true;
		}
		++ans;
	}
	std::cout << "Answer : " << ans << std::endl;
	return 0;
}