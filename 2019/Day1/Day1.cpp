
#include <iostream>
#include <fstream>
#include <string>

int main(void)
{
    bool first_half = false;
	int ans = 0;

	std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            int fuel = std::stoi(input) / 3 - 2;
            if (first_half)
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
