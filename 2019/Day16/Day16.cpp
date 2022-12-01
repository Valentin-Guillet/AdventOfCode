
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<int> apply_phase1(std::vector<int> input)
{
    std::vector<int> output;
    for (unsigned int k=0; k<input.size(); ++k)
    {
        int value = 0, sgn = 1;
        int i = k;

        while ((unsigned) i < input.size()) {
            for (unsigned int j=0; j<k+1 && i+j < input.size(); ++j) {
                value += sgn * input[i+j];
            }
            sgn *= -1;
            i += 2*(k+1);
        }
        value %= 10;
        if (value < 0)
            value *= -1;

        output.push_back(value);
    }

    return output;
}

std::vector<int> apply_phase2(std::vector<int> input)
{
    std::vector<int> output(input.size());
    int value = 0;
    for (int i=input.size(); 0<=i; --i)
    {
        value = (value + input[i]) % 10;
        output[i] = value;
    }

    return output;
}

int main(void)
{
    bool first_half = false;
	std::string input;
    std::vector<int> tab;
    int ans = 0;

	std::ifstream myfile ("input");
	if (myfile.is_open()) {
		getline(myfile, input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (char c : input)
            tab.push_back(c - '0');

        for (int i=0; i<100; ++i)
            tab = apply_phase1(tab);

    } else {

        int offset = std::stoi(input.substr(0, 7));
        std::string tmp = input.substr(offset % input.size());

        while (tmp.size() < 10000 * input.size() - offset)
            tmp.append(input);
        input = tmp;

        for (char c : input)
            tab.push_back(c - '0');

        for (int i=0; i<100; ++i)
            tab = apply_phase2(tab);
    }

    for (int i=0; i<8; ++i) {
        ans = ans*10 + tab[i];
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
