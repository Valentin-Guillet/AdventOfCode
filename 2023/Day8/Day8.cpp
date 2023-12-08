
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>


struct Node {
    std::string name;
    std::string left, right;

    Node(const std::string& input) {
        name = input.substr(0, 3);
        left = input.substr(7, 3);
        right = input.substr(12, 3);
    }
};

using NodeMap = std::unordered_map<std::string, Node>;

int compute_period(const NodeMap& node_map, const std::string& instr, const Node* curr_node, bool first_half) {
    int period = 0;
    while (!((first_half && curr_node->name == "ZZZ") || curr_node->name[2] == 'Z')) {
        if (instr[period % instr.size()] == 'R')
            curr_node = &node_map.at(curr_node->right);
        else
            curr_node = &node_map.at(curr_node->left);
        period++;
    }
    return period;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
    long long ans = 1;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::string instructions;
    NodeMap node_map;
    if (myfile.is_open()) {
        getline(myfile, instructions);

        std::string input;
        getline(myfile, input);

        while (getline(myfile, input))
            node_map.emplace(input.substr(0, 3), input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        ans = compute_period(node_map, instructions, &node_map.at("AAA"), true);

    } else {
        // The input has a very specific structure:
        // - Each node that ends with A follows a loop containing one and only one
        //   node that ends in Z
        // - The node right after the Z one is the same as the one after the A one
        // - The length of such a loop is necessarily a multiple of the instructions length
        // Only below these three conditions can we guaranty that the answer is indeed
        // the least common divisor of all these loop lengths
        for (const auto& [name, node] : node_map) {
            if (name[2] == 'A')
                ans = std::lcm(ans, compute_period(node_map, instructions, &node, false));
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
