
#include <algorithm>
#include <bit>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


bool has_smudge(size_t hash1, size_t hash2) {
    // Has smudge iff Hamming distance == 1
    //            iff Hamming weight (nb of bits) of h1 XOR h2 == 1
    //            iff h1 XOR h2 is a power of 2
    return std::has_single_bit(hash1 ^ hash2);
}

int get_reflection_axis(const std::vector<size_t>& hashes, bool with_smudge) {
    for (size_t ind = 0; ind < hashes.size() - 1; ++ind) {
        size_t max_nb_inds = std::min(hashes.size() - ind - 2, ind);

        bool valid = true;
        bool found_smudge = false;
        for (size_t i = 0; i <= max_nb_inds && valid; ++i) {
            if (with_smudge && has_smudge(hashes[ind - i], hashes[ind + i + 1])) {
                valid = !found_smudge;
                found_smudge = true;
                continue;
            }
            valid &= (hashes[ind - i] == hashes[ind + i + 1]);
        }

        if (valid && (!with_smudge || found_smudge))
            return ind + 1;
    }

    return 0;
}

struct Pattern {
    std::vector<std::string> pattern;
    std::vector<size_t> vertical_hashes, horizontal_hashes;

    explicit Pattern(const std::vector<std::string>& pat) : pattern(pat) {
        for (size_t col = 0; col < pattern[0].size(); ++col) {
            size_t hash = 0;
            for (size_t row = 0; row < pattern.size(); ++row)
                hash = (hash << 1) + (pattern[row][col] == '#' ? 1 : 0);
            vertical_hashes.push_back(hash);
        }

        for (size_t row = 0; row < pattern.size(); ++row) {
            size_t hash = 0;
            for (size_t col = 0; col < pattern[0].size(); ++col)
                hash = (hash << 1) + (pattern[row][col] == '#' ? 1 : 0);
            horizontal_hashes.push_back(hash);
        }
    }

    int summarize(bool with_smudge) const {
        int ans = get_reflection_axis(vertical_hashes, with_smudge);
        if (ans == 0)
            ans = 100 * get_reflection_axis(horizontal_hashes, with_smudge);
        return ans;
    }
};

int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;

    std::ifstream myfile("input");
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Pattern> patterns;
    if (myfile.is_open()) {
        std::vector<std::string> lines;
        std::string input;
        while (getline(myfile, input)) {
            if (input.empty()) {
                patterns.emplace_back(lines);
                lines.clear();
            } else {
                lines.emplace_back(input);
            }
        }
        patterns.emplace_back(lines);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (const Pattern& pattern : patterns)
        ans += pattern.summarize(!first_half);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
