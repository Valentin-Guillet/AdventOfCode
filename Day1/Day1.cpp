
#include "iostream"
#include "fstream"
#include <string>

int main(void)
{
	std::string input;
	int ans = 0;

	std::ifstream myfile ("Day1Input");
	if (myfile.is_open())
	{
		getline(myfile, input);
		myfile.close();
	}
	else std::cout << "Unable to open file"; 

	std::cout << "Input : " << input << std::endl;

	int away = input.length() / 2;

	for (int i = 0; i < input.length(); ++i)
	{
		int index = i+away;
		if (index >= input.length()) {index -= input.length();}
		if (input[i] == input[index]) {
			ans += input[i] - '0';
		}
	}
	std::cout << "Answer : " << ans << std::endl;
	return 0;
}