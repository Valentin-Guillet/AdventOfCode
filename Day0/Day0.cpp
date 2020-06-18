
#include <iostream>
#include <fstream>
#include <string>


int main()
{
    bool first_half = true;
	int ans = 0;

    std::ifstream myfile ("Day0Input");
	if (myfile.is_open()) {
        std::string input;

		getline(myfile, input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
