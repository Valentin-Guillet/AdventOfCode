
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool has_double(int number, double first_half)
{
    std::string num = std::to_string(number);
    for (int i=0; i<5; ++i) {
        if (first_half) {
            if (num[i] == num[i+1])
                return true;
        } else {
            if (num[i] == num[i+1] && (i == 4 || num[i+1] != num[i+2]))
                return true;
            for (int j=i+1; j<5 && num[j] == num[i]; ++j && ++i) {}
        }
    }
    return false;
}

bool increase(int number)
{
    std::string num = std::to_string(number);
    for (int i=0; i<5; ++i) {
        if (num[i] > num[i+1])
            return false;
    }
    return true;
}


int main(void)
{
    bool first_half = false;
	std::string input;
	int ans = 0;
	std::vector<int> tab;

	std::ifstream myfile ("input");
	if (myfile.is_open()) {
        getline(myfile, input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int min, max;
    int pos = input.find('-');
    min = std::stoi(input.substr(0, pos));
    max = std::stoi(input.substr(pos+1));

    for (int number=min; number<max; ++number) {
        if (has_double(number, first_half) && increase(number))
            ++ans;
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
