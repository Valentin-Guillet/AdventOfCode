
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


using Sequence = std::vector<int>;

int complete_sequence(const Sequence& seq, bool forward) {
    if (std::all_of(seq.begin(), seq.end(), [](int i){ return i == 0; }))
        return 0;

    Sequence diff_seq;
    diff_seq.reserve(seq.size() - 1);
    for (size_t i = 0; i < seq.size() - 1; ++i)
        diff_seq.push_back(seq[i+1] - seq[i]);

    if (forward)
        return seq.back() + complete_sequence(diff_seq, forward);
    return seq.front() - complete_sequence(diff_seq, forward);
}

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

    std::vector<Sequence> sequences;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input); ) {
            Sequence new_seq;
            std::istringstream sstream(input);
            for (int num; sstream >> num; )
                new_seq.push_back(num);
            sequences.emplace_back(std::move(new_seq));
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (const Sequence& seq : sequences)
        ans += complete_sequence(seq, first_half);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
