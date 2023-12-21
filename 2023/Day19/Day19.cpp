
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>


std::vector<std::string> extract(const std::string& input) {
    std::stringstream str_stream(input.substr(1, input.size() - 2));
    std::vector<std::string> res;
    for (std::string token; getline(str_stream, token, ','); )
        res.emplace_back(token);
    return res;
}

struct Interval {
    int min = 1;
    int max = 4000;
};

struct Part {
    int x, m, a, s;

    explicit Part(const std::string& input) {
        std::vector<int*> ratings = {&x, &m, &a, &s};
        std::vector<std::string> values = extract(input);
        for (size_t i = 0; i < 4; ++i)
            *ratings[i] = std::stoi(values[i].substr(2));
    }

    int get_value(char c) const {
        switch (c) {
            case 'x': return x;
            case 'm': return m;
            case 'a': return a;
            case 's': return s;
        }
        return '0';
    }

    int get_total_rating() const {
        return x + m + a + s;
    }
};

struct LimitPart {
    std::vector<Interval> x, m, a, s;

    std::vector<Interval>& get_interval(char c) {
        switch (c) {
            case 'x': return x;
            case 'm': return m;
            case 'a': return a;
            case 's': return s;
            default: __builtin_unreachable();
        }
    }

    bool empty() const {
        return (x.empty() && m.empty() && a.empty() && s.empty());
    }

    long long count_combinations() {
        long long count = 1;
        for (char c : std::string("xmas")) {
            long long count_combi = 0;
            for (Interval& interval : get_interval(c))
                count_combi += interval.max - interval.min + 1;
            count *= count_combi;
        }
        return count;
    }
};

struct Workflow;
using System = std::unordered_map<std::string, Workflow>;

struct Rule {
    bool force_redirect;
    std::string target;

    char rating;
    bool op_inf;
    int value;

    explicit Rule(const std::string& input) {
        size_t index = input.find(':');
        if (index == std::string::npos) {
            force_redirect = true;
            target = input;
            op_inf = false;
            return;
        }
        force_redirect = false;
        rating = input[0];
        op_inf = (input[1] == '<');
        value = std::stoi(input.substr(2, index - 2));
        target = input.substr(index + 1);
    }

    bool check(const Part& part) const {
        return force_redirect || (op_inf && part.get_value(rating) < value)
            || (!op_inf && part.get_value(rating) > value);
    }

    void apply(const Interval& interval, std::vector<Interval>& accepted, std::vector<Interval>& rejected) const {
        // The entire interval follows the rule
        if ((op_inf && interval.max < value) || (!op_inf && interval.min > value)) {
            accepted.push_back(interval);
            return;
        }

        // The entire interval doesn't follow the rule
        if ((op_inf && interval.min > value) || (!op_inf && interval.max < value)) {
            rejected.push_back(interval);
            return;
        }

        if (op_inf) {
            accepted.push_back({interval.min, value - 1});
            rejected.push_back({value, interval.max});
        } else {
            accepted.push_back({value + 1, interval.max});
            rejected.push_back({interval.min, value});
        }
    }

    LimitPart apply(LimitPart& part) const {
        if (force_redirect) {
            LimitPart accepted;
            std::swap(part, accepted);
            return accepted;
        }

        LimitPart accepted(part);
        std::vector<Interval>& intervals = part.get_interval(rating);
        std::vector<Interval> acc, rej;
        for (const Interval& interval : intervals)
            apply(interval, acc, rej);

        intervals = rej;
        accepted.get_interval(rating) = acc;
        return accepted;
    }
};

struct Workflow {
    std::string name;
    std::vector<Rule> rules;

    explicit Workflow(const std::string& input) {
        size_t index = input.find('{');
        name = input.substr(0, index);

        for (const std::string& rule : extract(input.substr(index)))
            rules.emplace_back(rule);
    }

    std::string get_target(const Part& part) const {
        for (const Rule& rule : rules) {
            if (rule.check(part))
                return rule.target;
        }
        return "";
    }

    long long count_combinations(const System& system, LimitPart part) const {
        long long count = 0;
        for (const Rule& rule : rules) {
            LimitPart accepted = rule.apply(part);
            if (accepted.empty())
                continue;

            if (rule.target == "A")
                count += accepted.count_combinations();
            else if (rule.target != "R")
                count += system.at(rule.target).count_combinations(system, accepted);
        }
        return count;
    }
};

bool accept(const System& system, const Part& part) {
    Workflow workflow = system.at("in");
    while (true) {
        std::string target = workflow.get_target(part);
        if (target == "A")
            return true;
        if (target == "R")
            return false;
        workflow = system.at(target);
    }
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

    System system;
    std::vector<Part> parts;
    if (myfile.is_open()) {
        std::string input;
        while (getline(myfile, input) && !input.empty()) {
            Workflow new_workflow(input);
            system.emplace(new_workflow.name, new_workflow);
        }

        while (getline(myfile, input))
            parts.emplace_back(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (const Part& part : parts) {
            if (accept(system, part))
                ans += part.get_total_rating();
        }
    } else {
        LimitPart limits;
        for (char c : std::string("xmas"))
            limits.get_interval(c) = {{1, 4000}};
        ans = system.at("in").count_combinations(system, limits);
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
