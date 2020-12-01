
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using Interval = std::pair<int, int>;
using FieldMap = std::unordered_map<std::string, std::pair<Interval, Interval>>;

std::pair<Interval, Interval> get_intervals(std::string input) {
    int or_index = input.find(" or ");
    std::string range = input.substr(0, or_index);
    int dash_index = range.find('-');
    int low = std::stoi(range.substr(0, dash_index));
    int high = std::stoi(range.substr(dash_index+1));
    Interval low_interval = {low, high};

    range = input.substr(or_index+4);
    dash_index = range.find('-');
    low = std::stoi(range.substr(0, dash_index));
    high = std::stoi(range.substr(dash_index+1));
    Interval high_interval = {low, high};

    return {low_interval, high_interval};
}

std::vector<int> parse_values(std::string input) {
    std::vector<int> values;
    size_t index = input.find(',');
    while (index != std::string::npos) {
        values.push_back(std::stoi(input.substr(0, index)));
        input = input.substr(index+1);
        index = input.find(',');
    }
    values.push_back(std::stoi(input));
    return values;
}

bool in_range(std::pair<Interval, Interval> intervals, int value) {
    return ((intervals.first.first <= value && value <= intervals.first.second) ||
            (intervals.second.first <= value && value <= intervals.second.second));
}

bool non_valid(FieldMap const& fields, int value) {
    for (auto &field : fields) {
        if (in_range(field.second, value))
            return false;
    }
    return true;
}

std::unordered_set<std::string> filter_valid(FieldMap const& fields, std::vector<std::vector<int>> const& nearby_tickets, size_t i, std::unordered_set<std::string> const& assigned_field) {
    std::unordered_set<std::string> valid_fields;
    for (auto &field_range : fields) {

        std::string field_name = field_range.first;
        if (assigned_field.find(field_name) != assigned_field.end())
            continue;

        bool valid = true;
        for (auto &ticket : nearby_tickets) {
            int value = ticket[i];
            if (!in_range(field_range.second, value)) {
                valid = false;
                break;
            }
        }

        if (valid)
            valid_fields.insert(field_name);
    }

    return valid_fields;
}


int main()
{
    bool first_half = false;
	long ans = 0;

    FieldMap fields;
    std::vector<int> my_ticket;
    std::vector<std::vector<int>> nearby_tickets;

    std::ifstream myfile ("Day16Input");
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            if (input == "")
                break;
            int col_index = input.find(':');
            std::string field_name = input.substr(0, col_index);
            input = input.substr(col_index+2);

            fields[field_name] = get_intervals(input);
        }

        getline(myfile, input);
        getline(myfile, input);
        my_ticket = parse_values(input);

        getline(myfile, input);
        getline(myfile, input);
        while (getline(myfile, input))
            nearby_tickets.push_back(parse_values(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (std::vector<int> ticket : nearby_tickets) {
            for (int value : ticket) {
                if (non_valid(fields, value))
                    ans += value;
            }
        }
    } else {
        for (auto it=nearby_tickets.begin(); it!=nearby_tickets.end(); ) {
            bool valid = true;
            for (int value : *it) {
                if (non_valid(fields, value)) {
                    valid = false;
                    break;
                }
            }
            if (valid)
                ++it;
            else
                it = nearby_tickets.erase(it);
        }

        std::unordered_map<std::string, size_t> field_indices;
        std::unordered_set<size_t> assigned_col;
        std::unordered_set<std::string> assigned_field;
        while (assigned_field.size() < fields.size()) {
            size_t nb_assigned = assigned_field.size();
            for (size_t i=0; i<my_ticket.size(); ++i) {
                if (assigned_col.find(i) != assigned_col.end())
                    continue;

                std::unordered_set<std::string> valid_fields = filter_valid(fields, nearby_tickets, i, assigned_field);
                if (valid_fields.size() == 0) {
                    std::cout << "ERROR Invalid ticket !" << std::endl;
                } else if (valid_fields.size() == 1) {
                    std::string field = *valid_fields.begin();
                    field_indices[field] = i;
                    assigned_col.insert(i);
                    assigned_field.insert(field);
                }
            }
            if (assigned_field.size() <= nb_assigned) {
                std::cout << "ERROR Stall: no field has been assigned." << std::endl;
                break;
            }
        }

        ans = 1;
        for (auto field_range : fields) {
            std::string field_name = field_range.first;
            if (field_name.substr(0, 9) == "departure")
                ans *= my_ticket[field_indices[field_name]];
        }
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
