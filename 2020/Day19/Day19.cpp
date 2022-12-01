
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


std::unordered_set<std::string> process(std::unordered_map<int, std::string> const& rules, int id, std::string input) {
    std::string rule = rules.at(id);
    if (rule.find('"') != std::string::npos) {
        if (input[0] == rule[1])
            return {input.substr(1)};
        else
            return {};
    }

    std::unordered_set<std::string> ans;

    std::vector<std::string> possibilities;
    size_t pipe_index = rule.find('|');
    if (pipe_index == std::string::npos) {
        possibilities.push_back(rule);
    } else {
        possibilities.push_back(rule.substr(0, pipe_index-1));
        possibilities.push_back(rule.substr(pipe_index+2));
    }

    for (std::string possibility : possibilities) {
        std::unordered_set<std::string> outputs = {input};
        std::istringstream iss(possibility);
        for (std::string token; iss >> token; ) {

            std::unordered_set<std::string> tmp_outputs;
            for (std::string output : outputs) {
                std::unordered_set<std::string> next_set = process(rules, std::stoi(token), output);
                tmp_outputs.insert(next_set.begin(), next_set.end());
            }
            outputs = tmp_outputs;
        }
        ans.insert(outputs.begin(), outputs.end());
    }

    return ans;
}

int main()
{
    bool first_half = false;
	int ans = 0;
    std::unordered_map<int, std::string> rules;
    std::vector<std::string> messages;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            if (input == "")
                break;

            size_t colon_index = input.find(':');
            int id = std::stoi(input.substr(0, colon_index));
            std::string rule = input.substr(colon_index+2);
            rules[id] = rule;
        }

        while (getline(myfile, input))
            messages.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (!first_half) {
        rules[8] = "42 | 42 8";
        rules[11] = "42 31 | 42 11 31";
    }

    for (std::string message : messages) {
        std::unordered_set<std::string> results = process(rules, 0, message);
        if (results.find("") != results.end())
            ans++;
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
