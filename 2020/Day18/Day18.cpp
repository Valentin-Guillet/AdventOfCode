
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


size_t get_index_bracket(std::string line, size_t index) {
    int level = 0;
    while (index < line.size()) {
        if (line[index] == '(')
            level++;
        else if (line[index] == ')')
            level--;

        if (level == 0)
            return index;
        index++;
    }
    std::cout << "No closing bracket in " << line << std::endl;
    return -1;
}

long evaluate(std::string line, bool first_half) {
    std::vector<char> operators;
    std::vector<long> values;

    size_t index = 0;
    while (true) {
        if (line[index] == '(') {
            size_t bracket_index = get_index_bracket(line, index);
            long value = evaluate(line.substr(index+1, bracket_index-index-1), first_half);
            values.push_back(value);
            index = bracket_index + 2;
        } else {
            values.push_back(line[index] - '0');
            index += 2;
        }

        if (index >= line.size())
            break;

        operators.push_back(line[index]);
        index += 2;
    }

    long ans;
    if (first_half) {
        ans = values[0];
        for (size_t i=0; i<operators.size(); ++i) {
            if (operators[i] == '+')
                ans += values[i+1];
            else
                ans *= values[i+1];
        }
    } else {
        ans = 1;
        long tmp_value = values[0];

        size_t i = 0;
        while (i < values.size()) {
            while (i < operators.size() && operators[i] == '+')
                tmp_value += values[++i];
            ans *= tmp_value;
            tmp_value = values[++i];
        }
    }
    return ans;
}


int main()
{
    bool first_half = false;
	long ans = 0;
    std::vector<std::string> lines;

    std::ifstream myfile ("Day18Input");
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            lines.push_back(input);
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (std::string line : lines)
        ans += evaluate(line, first_half);

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
