
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>


struct Pulse {
    const std::string& input, destination;
    bool is_high;
};

struct Module {
    std::string name;
    char type;
    std::vector<std::string> destinations;

    bool state = false;
    std::unordered_map<std::string, bool> input_states;

    explicit Module(const std::string& input) {
        size_t beg_ind = 0;
        if (input[0] == '%' || input[0] == '&')
            type = input[beg_ind++];
        else
            type = '>';

        size_t end_ind = input.find(' ');
        name = input.substr(beg_ind, end_ind - beg_ind);

        beg_ind = end_ind + 4;
        while (end_ind != std::string::npos) {
            end_ind = input.find(", ", beg_ind);
            destinations.push_back(input.substr(beg_ind, end_ind - beg_ind));
            beg_ind = end_ind + 2;
        }
    }

    void process(std::queue<Pulse>& pulses, const Pulse& pulse) {
        if (type == '%') {
            if (pulse.is_high)
                return;
            state = !state;

        } else if (type == '&') {
            input_states[pulse.input] = pulse.is_high;
            state = !std::all_of(input_states.begin(), input_states.end(),
                    [](const std::pair<std::string, bool>& input_state) { return input_state.second; });
        } else {
            state = pulse.is_high;
        }

        for (const std::string& dest : destinations)
            pulses.push({name, dest, state});
    }
};

using Machine = std::unordered_map<std::string, Module>;

long long count_period(const Machine& machine, std::string module_name) {
    long long period = 0;
    int base = 0;
    const Module* module;
    do {
        module = &machine.at(module_name);
        bool links_to_conjunction = false;
        module_name.clear();
        for (const std::string& dest : module->destinations) {
            char dest_type = machine.at(dest).type;
            if (dest_type == '&')
                links_to_conjunction = true;
            else if (dest_type == '%')
                module_name = dest;
        }
        period |= ((int)links_to_conjunction) << base;
        base++;
    } while (!module_name.empty());

    return period;
}

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

    Machine machine;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input); ) {
            Module new_module(input);
            machine.emplace(new_module.name, new_module);
        }
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (const auto& [module_name, module] : machine) {
        for (const std::string& dest : module.destinations) {
            // Output-only module
            if (!machine.contains(dest))
                machine.emplace(dest, dest);

            if (machine.at(dest).type == '&')
                machine.at(dest).input_states[module_name] = false;
        }
    }

    if (first_half) {
        int nb_pulses[2] = {0, 0};
        std::queue<Pulse> pulses;
        for (int button_push = 0; button_push < 1000; ++button_push) {
            pulses.push({"button", "broadcaster", false});
            while (!pulses.empty()) {
                Pulse pulse = pulses.front();
                pulses.pop();
                machine.at(pulse.destination).process(pulses, pulse);
                nb_pulses[pulse.is_high]++;
            }
        }
        ans = nb_pulses[0] * nb_pulses[1];
    } else {
        // The input has a specific form that must be analyzed: cf. notes.txt
        // To visualize the graph, generate it using GraphViz:
        // `dot -Tpng graph.dot graph.png`
        ans = 1;
        for (const std::string& name : machine.at("broadcaster").destinations)
            ans = std::lcm(ans, count_period(machine, name));
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
