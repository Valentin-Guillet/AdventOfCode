
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>


using Engine = std::vector<std::string>;

struct Number {
    int value;
    size_t row, col;
    size_t length;
};

// Szudzik's hash function
inline size_t hash_coords(size_t a, size_t b) {
    return (a >= b ? a * a + a + b : a + b * b);
}

using GearMap = std::unordered_map<size_t, std::vector<Number>>;

std::vector<Number> build_numbers(const Engine& engine) {
    std::vector<Number> numbers;
    size_t nb_cols = engine[0].size();
    for (size_t y = 0; y < engine.size(); ++y) {
        size_t x = 0;
        while (x < nb_cols) {
            if (!std::isdigit(engine[y][x])) {
                x++;
                continue;
            }

            size_t dx = 0;
            while (x + dx < nb_cols && std::isdigit(engine[y][x + dx]))
                dx++;

            numbers.emplace_back(std::stoi(engine[y].substr(x, dx)), y, x, dx);
            x += dx;
        }
    }
    return numbers;
}

bool is_valid(const Engine& engine, const Number& number) {
    size_t first_row = (number.row > 0 ? number.row - 1 : 0);
    size_t last_row = std::min(number.row + 1, engine.size() - 1);
    size_t first_col = (number.col > 0 ? number.col - 1 : 0);
    size_t last_col = std::min(number.col + number.length, engine[0].size() - 1);

    for (size_t y = first_row; y <= last_row; ++y) {
        for (size_t x = first_col; x <= last_col; ++x) {
            char c = engine[y][x];
            if (!std::isdigit(c) && c != '.')
                return true;
        }
    }
    return false;
}

GearMap build_gears(const Engine& engine, const std::vector<Number>& numbers) {
    GearMap gears;

    for (const Number& number : numbers) {
        size_t first_row = (number.row > 0 ? number.row - 1 : 0);
        size_t last_row = number.row + (number.row < engine.size() - 1 ? 1 : 0);
        size_t first_col = (number.col > 0 ? number.col - 1 : 0);
        size_t last_col = std::min(number.col + number.length, engine[0].size() - 1);

        for (size_t y = first_row; y <= last_row; ++y) {
            for (size_t x = first_col; x <= last_col; ++x) {
                if (engine[y][x] == '*')
                    gears[hash_coords(y, x)].emplace_back(number);
            }
        }
    }

    return gears;
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

    Engine engine;
    if (myfile.is_open()) {
        std::string input;
        std::istream_iterator<std::string> file_iter(myfile), end;
        engine.assign(file_iter, end);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::vector<Number> numbers = build_numbers(engine);
    if (first_half) {
        for (const Number& number : numbers) {
            if (is_valid(engine, number))
                ans += number.value;
        }
    } else {
        GearMap gears = build_gears(engine, numbers);
        for (const auto& [_, gear_ratios] : gears) {
            if (gear_ratios.size() == 2)
                ans += gear_ratios[0].value * gear_ratios[1].value;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
