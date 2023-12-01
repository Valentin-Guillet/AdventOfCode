
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>


const std::vector<std::string> digit_names = {
    "zero", "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine",
};

int find_digit(const std::string& line, bool only_digits, bool reverse = false) {
    for (size_t ind = 0; ind < line.size(); ++ind) {
        const int index = (reverse ? (line.size() - 1 - ind) : ind);

        // Digit
        if (std::isdigit(line[index]))
            return line[index] - '0';

        if (only_digits)
            continue;

        // String
        for (int digit = 0; digit < 10; ++digit) {
            const std::string& word = digit_names[digit];
            if ((index + word.size() <= line.size()) &&
                    line.substr(index, word.size()) == word)
                return digit;
        }
    }
    return -1;
}

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

    std::vector<std::string> lines;
    if (myfile.is_open()) {
        std::istream_iterator<std::string> file_iter(myfile), end;
        lines.assign(file_iter, end);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (const std::string& line : lines) {
        int d1 = find_digit(line, first_half);
        int d2 = find_digit(line, first_half, true);
        ans += 10 * d1 + d2;
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
