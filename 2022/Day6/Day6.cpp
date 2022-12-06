
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>


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

    std::string input;
	if (myfile.is_open()) {

		getline(myfile, input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    size_t nb_chars = (first_half ? 4 : 14);
    for (size_t i=0; i<input.size()-nb_chars; i++) {
        std::unordered_set<char> set;
        for (char c : input.substr(i, nb_chars))
            set.insert(c);

        if (set.size() == nb_chars) {
            ans = i + nb_chars;
            break;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
