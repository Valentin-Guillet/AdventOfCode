
#include <iostream>
#include <fstream>
#include <string>


std::string reduce(std::string polymer) {
    size_t index = 0;
    while (index < polymer.size()-1) {
        if (std::abs(polymer[index] - polymer[index+1]) == 32) {
            polymer.erase(index, 2);
            index = 0;
        } else {
            index++;
        }
    }
    return polymer;
}

std::string remove(std::string input, char rem) {
    std::string polymer;
    for (char c : input) {
        if (c != rem && c != rem+32)
            polymer += c;
    }
    return polymer;
}

int main(void)
{
    bool first_half = false;
	std::string input;
	int ans = 0;

	std::ifstream myfile ("input");
	if (myfile.is_open())
	{
		getline(myfile, input);
		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::string polymer = reduce(input);

    if (first_half) {
        ans = polymer.size();

    } else {
        int min_size = polymer.size();

        for (char c='A'; c<='Z'; ++c) {
            int size = reduce(remove(polymer, c)).size();

            if (size < min_size) {
                min_size = size;
            }
        }

        ans = min_size;
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
