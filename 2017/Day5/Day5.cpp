
#include "iostream"
#include "fstream"
#include <string>
#include <vector>

int main(void)
{
	std::string input;
	int ans = 0;
	std::vector<int> tab;

	std::ifstream myfile ("input");
	if (myfile.is_open())
	{
		getline(myfile, input);
		myfile.close();
	}
	else std::cout << "Unable to open file";

	std::cout << "Input : " << input << std::endl;

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}