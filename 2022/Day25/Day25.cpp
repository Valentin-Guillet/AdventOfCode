
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


long snafu2long(std::string line) {
    long ans = 0;
    long power = 1;
    std::reverse(line.begin(), line.end());

    for (char c : line) {
        if (c == '=')
            ans += -2 * power;
        else if (c == '-')
            ans += -1 * power;
        else if (c == '1')
            ans += power;
        else if (c == '2')
            ans += 2 * power;
        power *= 5;
    }

    return ans;
}

std::string long2snafu(long n) {
    long base = 1;
    while (base < n / 5)
        base *= 5;

    if (n > (base * 5 - 1) / 2)
        base *= 5;

    std::string snafu;
    while (base != 0) {
        long coef = 2;
        while (std::abs(n - coef * base) > (base - 1) / 2)
            coef--;

        if (coef >= 0)
            snafu += coef + '0';
        else if (coef == -1)
            snafu += '-';
        else if (coef == -2)
            snafu += '=';

        n -= coef * base;
        base /= 5;
    }

    return snafu;
}

int main(int argc, char* argv[])
{
    long ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");
    }

    std::vector<std::string> lines;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            lines.push_back(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (std::string line : lines)
        ans += snafu2long(line);

    std::cout << "Answer : " << long2snafu(ans) << std::endl;
    return 0;
}
