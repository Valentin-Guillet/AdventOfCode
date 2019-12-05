
#include "iostream"
#include "fstream"
#include <string>
#include <vector>

int main(void)
{
    bool firstHalf = false;
	std::string input;
	int ans = 0;

	std::ifstream myfile ("Day1Input");
	if (myfile.is_open())
	{
        while (getline(myfile, input)) {
            int fuel = std::stoi(input) / 3 - 2;
            if (firstHalf)
                ans += std::stoi(input) / 3 - 2;
            else {
                while (fuel > 0) {
                    ans += fuel;
                    fuel = fuel / 3 - 2;
                }
            }
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file";
        return 1;
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
