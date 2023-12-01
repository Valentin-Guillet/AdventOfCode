
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>


struct Monkey;
using MonkeySet = std::unordered_map<std::string, Monkey>;

struct Monkey {
    std::string line;
    std::string name;

    bool is_leaf = false;
    long value;

    std::string parent_name, left_child, right_child;
    char op;
    bool has_human_child = false;

    Monkey() { }
    Monkey(std::string line_, std::string name_) : line(line_), name(name_) { }

    void build_tree(MonkeySet& monkeys) {
        if ('0' <= line[0] && line[0] <= '9') {
            is_leaf = true;
            value = std::stoi(line);
            has_human_child = (name == "humn");
            return;
        }

        op = line[5];
        left_child = line.substr(0, 4);
        right_child = line.substr(7, 4);
        Monkey& monkey1 = monkeys.at(left_child);
        Monkey& monkey2 = monkeys.at(right_child);

        monkey1.parent_name = name;
        monkey2.parent_name = name;

        monkey1.build_tree(monkeys);
        monkey2.build_tree(monkeys);

        has_human_child = monkey1.has_human_child || monkey2.has_human_child;
    }

    long get_number(MonkeySet const& monkeys) const {
        if (is_leaf)
            return value;

        Monkey monkey1 = monkeys.at(left_child);
        Monkey monkey2 = monkeys.at(right_child);
        if (op == '+')
            return monkey1.get_number(monkeys) + monkey2.get_number(monkeys);
        else if (op == '-')
            return monkey1.get_number(monkeys) - monkey2.get_number(monkeys);
        else if (op == '*')
            return monkey1.get_number(monkeys) * monkey2.get_number(monkeys);
        else
            return monkey1.get_number(monkeys) / monkey2.get_number(monkeys);
    }

    long get_child_value(MonkeySet const& monkeys) const {
        if (monkeys.at(left_child).has_human_child)
            return monkeys.at(right_child).get_number(monkeys);

        return monkeys.at(left_child).get_number(monkeys);
    }

    long get_value(MonkeySet const& monkeys) const {
        if (name == "root")
            return get_child_value(monkeys);

        Monkey parent = monkeys.at(parent_name);
        long parent_value = parent.get_value(monkeys);
        if (parent_name == "root")
            return parent_value;

        long child_value = parent.get_child_value(monkeys);
        bool is_left = monkeys.at(parent.left_child).has_human_child;

        if (parent.op == '+')
            return parent_value - child_value;

        if (parent.op == '-')
            return child_value + (is_left ? 1 : -1) * parent_value;

        if (parent.op == '*')
            return parent_value / child_value;

        if (parent.op == '/') {
            if (is_left)
                return parent_value * child_value;
            return child_value / parent_value;
        }

        return -1;  // never reached
    }
};


int main(int argc, char* argv[])
{
    bool first_half = true;
    long ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    MonkeySet monkeys;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            std::string name = input.substr(0, 4);
            Monkey new_monkey(input.substr(6), name);
            monkeys[name] = new_monkey;
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    monkeys["root"].build_tree(monkeys);
    if (first_half)
        ans = monkeys["root"].get_number(monkeys);
    else
        ans = monkeys["humn"].get_value(monkeys);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
