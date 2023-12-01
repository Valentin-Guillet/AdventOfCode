
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


struct Bag {
    std::unordered_map<std::string, Bag*> const& bags;
    std::string name;
    std::vector<std::pair<std::string, int>> children;
    std::vector<std::string> parents;
    int count = -1;

    Bag(std::unordered_map<std::string, Bag*> const& bags, std::string name) :
        bags(bags), name(name) { }

    int count_parents() {
        std::unordered_set<std::string> holders;
        _count_parents(holders, name);
        return holders.size();
    }

    void _count_parents(std::unordered_set<std::string> &holders, std::string name) {
        for (std::string parent : parents) {
            if (holders.find(parent) == holders.end()) {
                holders.insert(parent);
                bags.at(parent)->_count_parents(holders, name);
            }
        }
    }

    int count_bags() {
        if (count < 0) {
            count = 1;
            for (auto [bag_name, bag_count] : children)
                count += bag_count * bags.at(bag_name)->count_bags();
        }
        return count;
    }
};

int main()
{
    bool first_half = false;
    int ans = 0;
    std::vector<std::string> rules;
    std::unordered_map<std::string, Bag*> bags;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            rules.push_back(input);
            int index = input.find("bags contain");
            std::string name = input.substr(0, index-1);
            bags[name] = new Bag(bags, name);
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (std::string rule : rules) {
        int index = rule.find(" bags contain");
        std::string parent_name = rule.substr(0, index);
        std::string children = rule.substr(index+14);

        if (children == "no other bags.")
            continue;

        while (true) {
            size_t comma_index = children.find(',');
            if (comma_index == std::string::npos)
                comma_index = children.size()-1;

            std::string child;
            child = children.substr(0, comma_index);

            int count = child[0] - '0';
            int name_index = child.find(" bag");
            std::string child_name = child.substr(2, name_index-2);
            bags[parent_name]->children.push_back({child_name, count});
            bags[child_name]->parents.push_back(parent_name);

            if (children[comma_index] == '.')
                break;
            children = children.substr(comma_index+2);
        }
    }

    Bag *shiny_gold_bag = bags["shiny gold"];
    if (first_half)
        ans = shiny_gold_bag->count_parents();
    else
        ans = shiny_gold_bag->count_bags() - 1;

    for (auto [name, bag] : bags)
        delete bag;

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
