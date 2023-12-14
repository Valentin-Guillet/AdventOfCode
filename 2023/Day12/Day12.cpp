
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>


inline size_t hash(size_t a, size_t b) {
    return (a >= b ? a * a + a + b : a + b * b);
}

struct Record {
    std::string pattern;
    std::vector<int> groups;
    std::unordered_map<size_t, long long> count_memory;

    explicit Record(const std::string& input) {
        size_t ind = input.find(' ');
        pattern = input.substr(0, ind);
        std::istringstream stream(input.substr(ind+1));
        for (std::string group; getline(stream, group, ','); )
            groups.push_back(std::stoi(group));
    }

    void extend() {
        std::string new_pattern(pattern);
        new_pattern.reserve((pattern.size() + 1) * 5 - 1);

        std::vector<int> new_groups(groups);
        new_groups.reserve(groups.size() * 5);
        for (int i = 0; i < 4; ++i) {
            new_pattern += "?" + pattern;
            new_groups.insert(new_groups.end(), groups.begin(), groups.end());
        }

        pattern = new_pattern;
        groups = new_groups;
    }

    bool is_valid(size_t group_id, size_t pos) const {
        if (group_id >= groups.size())
            return (pos >= pattern.size()) ||
                (std::all_of(pattern.begin() + pos, pattern.end(),
                             [](char c) { return c != '#'; }));

        size_t end_pos = pos + groups[group_id];
        if (end_pos > pattern.size())
            return false;

        for (size_t i = pos; i < end_pos; ++i) {
            if (pattern[i] == '.')
                return false;
        }

        return pattern[end_pos] != '#';
    }

    long long count_arrangements(size_t group_id, size_t min_pos) {
        if (group_id >= groups.size())
            return (long long)is_valid(group_id, min_pos);

        min_pos = pattern.find_first_not_of('.', min_pos);
        if (min_pos == std::string::npos)
            return 0;

        long long count = 0;
        for (size_t pos = min_pos; pos < pattern.size(); ++pos) {
            if (is_valid(group_id, pos))
                count += count_arrangements_memo(group_id + 1, pos + groups[group_id] + 1);

            if (pattern[pos] == '#')
                break;
        }
        return count;
    };

    long long count_arrangements_memo(size_t group_id, size_t min_pos) {
        size_t args_hash = hash(group_id, min_pos);
        if (!count_memory.contains(args_hash))
            count_memory[args_hash] = count_arrangements(group_id, min_pos);
        return count_memory.at(args_hash);
    }

    long long count_arrangements() {
        return count_arrangements_memo(0, 0);
    }
};

int main(int argc, char* argv[])
{
    bool first_half = true;
    long long ans = 0;

    std::ifstream myfile("input");
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Record> records;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input); )
            records.emplace_back(input);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (!first_half) {
        for (Record& record : records)
            record.extend();
    }

    for (Record& record : records)
        ans += record.count_arrangements();

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
