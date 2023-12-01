
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


std::vector<size_t> get_separators(std::string input) {
    std::vector<size_t> separators = {0};
    int bracket_level = 0;
    for (size_t i=1; i<input.size()-1; i++) {
        if (bracket_level == 0 && input[i] == ',')
            separators.push_back(i);

        else if (input[i] == '[')
            bracket_level++;

        else if (input[i] == ']')
            bracket_level--;
    }
    separators.push_back(input.size()-1);
    return separators;
}

enum class ValueType { Int, List };

struct Value {
    ValueType type;
    int ivalue = -1;
    std::vector<Value> lvalue;

    Value() { }
    Value(std::string input) {
        if (input[0] == '[') {
            type = ValueType::List;
            std::vector<size_t> separators = get_separators(input);

            if (input[1] == ']')
                return;

            for (size_t i=0; i<separators.size()-1; i++) {
                size_t beg = separators[i] + 1;
                size_t end = separators[i+1] - 1;
                std::string value_str = input.substr(beg, end - beg + 1);
                lvalue.push_back(Value(value_str));
            }
        } else {
            type = ValueType::Int;
            size_t index = input.find(',');
            ivalue = std::stoi(input.substr(0, index));
        }
    }

    std::string to_string() const {
        if (type == ValueType::Int)
            return std::to_string(ivalue);

        std::string repr = "[";
        if (!lvalue.empty()) {
            for (size_t i=0; i<lvalue.size()-1; i++)
                repr += lvalue[i].to_string() + ',';

            repr += lvalue[lvalue.size()-1].to_string();
        }
        repr += ']';
        return repr;
    }
};

int _compare(Value const& left, Value const& right) {
    // Int vs Int
    if (left.type == ValueType::Int && right.type == ValueType::Int) {
        if (left.ivalue < right.ivalue)
            return 1;
        else if (left.ivalue > right.ivalue)
            return -1;
        return 0;
    }

    // List vs List
    if (left.type == ValueType::List && right.type == ValueType::List) {
        for (size_t i=0; i<left.lvalue.size(); i++) {
            // Right has less elements
            if (i == right.lvalue.size())
                return -1;

            // Two elements are not ordered
            int comparison = _compare(left.lvalue[i], right.lvalue[i]);
            if (comparison != 0)
                return comparison;
        }
        // Left run out of elements first
        if (left.lvalue.size() < right.lvalue.size())
            return 1;

        // Equal list, continue
        return 0;
    }

    // List vs Int
    Value new_value;
    new_value.type = ValueType::List;
    if (left.type == ValueType::List) {
        new_value.lvalue.push_back(right);
        return _compare(left, new_value);
    }

    // Int vs List
    new_value.lvalue.push_back(left);
    return _compare(new_value, right);
}

bool compare(Value const& left, Value const& right) {
    return _compare(left, right) == 1;
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

    std::vector<Value> packets;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            if (!input.empty())
                packets.push_back(Value(input));
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (size_t i=0; i<packets.size(); i+=2) {
            if (compare(packets[i], packets[i+1]))
                ans += i / 2 + 1;
        }
    } else {
        std::string divider1 = "[[2]]";
        std::string divider2 = "[[6]]";
        packets.push_back(Value(divider1));
        packets.push_back(Value(divider2));

        std::sort(packets.begin(), packets.end(), compare);

        ans = 1;
        for (size_t i=0; i<packets.size(); i++) {
            std::string repr = packets[i].to_string();
            if (repr == divider1 || repr == divider2)
                ans *= i + 1;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
