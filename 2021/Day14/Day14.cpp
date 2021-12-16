
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


using PairCounter = std::unordered_map<std::string, long>;

void grow(std::unordered_map<std::string, char> const& rules, std::string& polymer) {
    int new_size = 2*polymer.size() - 1;
    std::string new_polymer(new_size, '0');
    for (size_t i=0; i<polymer.size()-1; i++) {
        char new_element = rules.at(polymer.substr(i, 2));
        new_polymer[2*i] = polymer[i];
        new_polymer[2*i+1] = new_element;
    }
    new_polymer[new_size-1] = polymer[polymer.size()-1];
    polymer = new_polymer;
}

void grow_count(std::unordered_map<std::string, char> const& rules, PairCounter& counter) {
    PairCounter new_counter;
    for (auto [pair, value] : counter) {
        char new_element = rules.at(pair);
        std::string new_pair(2, '.');
        new_pair[0] = pair[0];
        new_pair[1] = new_element;
        new_counter[new_pair] += value;

        new_pair[0] = new_element;
        new_pair[1] = pair[1];
        new_counter[new_pair] += value;
    }
    counter = new_counter;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	long ans = 0;
    std::string polymer;
    std::unordered_map<std::string, char> rules;

    std::ifstream myfile("Day14Input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("testInput");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
        std::string input;

        getline(myfile, polymer);
        getline(myfile, input);
        while (getline(myfile, input))
            rules[input.substr(0, 2)] = input[6];

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::unordered_map<char, long> counts;
    if (first_half) {
        for (int step=0; step<10; step++)
            grow(rules, polymer);

        for (char c : polymer)
            counts[c]++;

    } else {
        PairCounter pairs;
        for (size_t i=0; i<polymer.size()-1; i++)
            pairs[polymer.substr(i, 2)]++;

        for (int step=0; step<40; step++)
            grow_count(rules, pairs);

        for (auto [pair, count] : pairs)
            counts[pair[0]] += count;
        counts[polymer[polymer.size()-1]]++;
    }

    char most_c = polymer[0], least_c = polymer[0];
    for (auto [c, freq] : counts) {
        if (freq < counts[least_c])
            least_c = c;
        if (freq > counts[most_c])
            most_c = c;
    }
    ans = counts[most_c] - counts[least_c];

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
