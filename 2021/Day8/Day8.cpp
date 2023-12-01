
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>


using Digit = std::unordered_set<char>;

Digit operator+(Digit const& d1, Digit const& d2) {
    Digit ans(d1);
    for (char c : d2)
        ans.insert(c);
    return ans;
}

Digit operator-(Digit const& d1, Digit const& d2) {
    Digit ans(d1);
    for (char c : d2)
        ans.erase(c);
    return ans;
}

Digit operator&(Digit const& d1, Digit const& d2) {
    Digit ans(d1);
    for (char c : d1) {
        if (d2.find(c) == d2.end())
            ans.erase(c);
    }
    return ans;
}

std::ostream& operator<<(std::ostream& os, Digit const& d) {
    os << "(";
    for (char c : d)
        os << c << ", ";
    os << ")";
    return os;
}

struct Display {
    std::array<Digit, 10> digits;
    std::array<Digit, 4> outputs;

    int count_simples() const {
        int count = 0;
        for (int i=0; i<4; i++) {
            size_t size = outputs[i].size();
            count += (size == 2 || size == 3 || size == 4 || size == 7);
        }
        return count;
    }

    int decode() const {
        int one, four, seven, eight;
        Digit fives = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
        Digit sixes(fives);
        for (int i=0; i<10; i++) {
            if (digits[i].size() == 2)
                one = i;
            else if (digits[i].size() == 3)
                seven = i;
            else if (digits[i].size() == 4)
                four = i;
            else if (digits[i].size() == 5)
                fives = fives & digits[i];
            else if (digits[i].size() == 6)
                sixes = sixes & digits[i];
            else if (digits[i].size() == 7)
                eight = i;
        }

        Digit a = digits[seven] - digits[one];
        Digit g = sixes - digits[four] - a;
        Digit d = fives - a - g;
        Digit b = digits[four] - digits[one] - d;
        Digit f = sixes - a - b - g;
        Digit c = digits[one] - f;
        Digit e = digits[eight] - a - b - c - d - f - g;

        std::vector<Digit> ordered_digits;
        ordered_digits.push_back(a + b + c + e + f + g);       // 0
        ordered_digits.push_back(c + f);                       // 1
        ordered_digits.push_back(a + c + d + e + g);           // 2
        ordered_digits.push_back(a + c + d + f + g);           // 3
        ordered_digits.push_back(b + c + d + f);               // 4
        ordered_digits.push_back(a + b + d + f + g);           // 5
        ordered_digits.push_back(a + b + d + e + f + g);       // 6
        ordered_digits.push_back(a + c + f);                   // 7
        ordered_digits.push_back(a + b + c + d + e + f + g);   // 8
        ordered_digits.push_back(a + b + c + d + f + g);       // 9

        int ans = 0;
        for (Digit digit : outputs) {
            for (int i=0; i<10; i++) {
                if (digit == ordered_digits[i]) {
                    ans = 10 * ans + i;
                    break;
                }
            }
        }

        return ans;
    }
};


int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;
    std::vector<Display> displays;

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

        while (getline(myfile, input)) {
            std::array<Digit, 10> digits;
            std::array<Digit, 4> outputs;

            int index = input.find(" | ");
            std::istringstream iss(input.substr(0, index));
            std::string s;
            int i = 0;
            while (getline(iss, s, ' ')) {
                Digit d;
                for (char c : s)
                    d.insert(c);
                /* std::cout << "Digit: " << d << std::endl; */
                digits[i++] = d;
            }

            iss = std::istringstream(input.substr(index+3));
            i = 0;
            while (getline(iss, s, ' ')) {
                Digit d;
                for (char c : s)
                    d.insert(c);
                outputs[i++] = d;
            }

            displays.push_back(Display{digits, outputs});
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (Display const& display : displays) {
        if (first_half)
            ans += display.count_simples();
        else
            ans += display.decode();
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
