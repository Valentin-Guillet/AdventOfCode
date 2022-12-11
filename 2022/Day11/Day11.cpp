
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>


struct Monkey {
    bool first_half;

    std::queue<long> items;
    bool operation_add;
    bool operation_operand_is_item;
    long operation_operand = -1;
    int divisibility_test;
    int target_true;
    int target_false;
    long modulo;

    void set_modulo(long value) {
        modulo = value;
    }

    void apply_operation(long& item) {
        long operand = (operation_operand_is_item ? item : operation_operand);
        if (!first_half)
            operand %= modulo;

        if (operation_add)
            item += operand;
        else
            item *= operand;

        if (!first_half)
            item %= modulo;
    }
};

Monkey build_monkey(std::vector<std::string> lines, bool first_half) {
    Monkey new_monkey;
    new_monkey.first_half = first_half;

    std::string item_string = lines[1].substr(18);
    while (true) {
        size_t index = item_string.find(", ");
        if (index == std::string::npos) {
            new_monkey.items.push(std::stoi(item_string));
            break;
        }
        long item = std::stoi(item_string.substr(0, index));
        new_monkey.items.push(item);
        item_string = item_string.substr(index+2);
    }

    new_monkey.operation_add = (lines[2][23] == '+');
    if (lines[2].substr(25) == "old") {
        new_monkey.operation_operand_is_item = true;
    } else {
        new_monkey.operation_operand_is_item = false;
        new_monkey.operation_operand = std::stol(lines[2].substr(25));
    }

    new_monkey.divisibility_test = std::stoi(lines[3].substr(21));
    new_monkey.target_true = std::stoi(lines[4].substr(29));
    new_monkey.target_false = std::stoi(lines[5].substr(30));
    return new_monkey;
}


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

    std::vector<Monkey> monkeys;
	if (myfile.is_open()) {
        std::string input;

        std::vector<std::string> lines;
        while (getline(myfile, input)) {
            if (input.empty()) {
                monkeys.push_back(build_monkey(lines, first_half));
                lines.clear();
            } else {
                lines.push_back(input);
            }
        }
        monkeys.push_back(build_monkey(lines, first_half));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    long modulo = 1;
    for (Monkey monkey : monkeys)
        modulo *= monkey.divisibility_test;
    for (Monkey& monkey : monkeys)
        monkey.set_modulo(modulo);

    std::vector<long> nb_inspections(monkeys.size(), 0);
    int nb_rounds = (first_half ? 20 : 10000);
    for (int round=0; round<nb_rounds; round++) {
        for (int id_monkey=0; id_monkey<(int)monkeys.size(); id_monkey++) {
            Monkey& monkey = monkeys[id_monkey];
            while (!monkey.items.empty()) {
                nb_inspections[id_monkey]++;

                long item = monkey.items.front();
                monkey.items.pop();
                monkey.apply_operation(item);
                if (first_half)
                    item /= 3;
                int target = (item % monkey.divisibility_test == 0 ? monkey.target_true : monkey.target_false);
                monkeys[target].items.push(item);
            }
        }
    }

    std::sort(nb_inspections.begin(), nb_inspections.end());
    std::reverse(nb_inspections.begin(), nb_inspections.end());
    ans = nb_inspections[0] * nb_inspections[1];

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
