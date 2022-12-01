
#include <fstream>
#include <iostream>
#include <string>


int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

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
