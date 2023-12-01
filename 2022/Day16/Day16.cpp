
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


bool line_sorter(std::string const& line1, std::string const& line2) {
    size_t index = line1.find(';');
    int flow_rate_1 = std::stoi(line1.substr(23, index-23));

    index = line2.find(';');
    int flow_rate_2 = std::stoi(line2.substr(23, index-23));

    return flow_rate_2 < flow_rate_1;
}

struct Valve {
    int flow_rate;
    std::vector<size_t> links;
};

using DistanceMap = std::vector<std::vector<int>>;
using HashMap = std::unordered_map<size_t, int>;

void build_valves(std::vector<std::string> const& lines, std::vector<Valve>& valves,
        std::unordered_map<std::string, size_t> const& name_to_id) {
    for (std::string line : lines) {
        Valve new_valve;

        size_t index = line.find(';');
        new_valve.flow_rate = std::stoi(line.substr(23, index-23));

        line = line.substr(index + 24 + (line[index + 23] == 's'));
        for (size_t i=0; i<line.size(); i+=4)
            new_valve.links.push_back(name_to_id.at(line.substr(i, 2)));

        valves.push_back(new_valve);
    }
}

void compute_distances(std::vector<Valve> const& valves, DistanceMap& distances) {
    for (size_t valve_id=0; valve_id<valves.size(); valve_id++) {
        int dist = 1;
        std::vector<size_t> neighbors(valves.at(valve_id).links);
        while (!neighbors.empty()) {
            std::vector<size_t> new_neighbors;
            for (size_t neighbor : neighbors) {
                if (valve_id == neighbor || distances[valve_id][neighbor] != -1)
                    continue;

                distances[valve_id][neighbor] = dist;
                for (size_t next_neigh : valves.at(neighbor).links)
                    new_neighbors.push_back(next_neigh);
            }
            neighbors = new_neighbors;
            dist++;
        }
    }
}

int _get_max_pressure(std::vector<Valve> const& valves, DistanceMap const& distances,
        size_t curr_valve, unsigned int avail_valves, int remaining_time,
        HashMap& memory_state) {
    // If this situation has already been seen (i.e. same seen valves, same current valve and
    // same remaining time), look in the table
    size_t state_hash = (avail_valves << 12) + (curr_valve << 6) + remaining_time;
    if (memory_state.contains(state_hash))
        return memory_state[state_hash];

    int max_pressure = 0;
    for (size_t next_valve=0; next_valve<valves.size(); next_valve++) {
        // Valve already seen
        if (!(avail_valves & (1 << next_valve)))
            continue;

        int new_remaining_time = remaining_time - distances.at(curr_valve).at(next_valve) - 1;
        if (new_remaining_time <= 0)
            continue;

        unsigned int new_avail_valves = avail_valves;
        new_avail_valves ^= (1 << next_valve);

        int pressure = valves[next_valve].flow_rate * new_remaining_time;
        if (new_avail_valves != 0)
            pressure += _get_max_pressure(valves, distances, next_valve, new_avail_valves,
                                          new_remaining_time, memory_state);

        if (pressure > max_pressure)
            max_pressure = pressure;
    }

    memory_state[state_hash] = max_pressure;
    return max_pressure;
}

int get_max_pressure(std::vector<Valve> const& valves, DistanceMap const& distances, size_t id_start, bool first_half) {
    HashMap memory_state;

    unsigned int valve_mask = 0;
    for (size_t i=0; i<valves.size(); i++)
        valve_mask = (valve_mask << 1) + 1;

    if (first_half)
        return _get_max_pressure(valves, distances, id_start, valve_mask, 30, memory_state);

    size_t nb_set_split = 1;
    for (size_t i=0; i<valves.size()-1; i++)
        nb_set_split *= 2;

    int max_single_pressure = _get_max_pressure(valves, distances, id_start, valve_mask, 26, memory_state);
    int max_pressure = 0;
    for (size_t i=0; i<nb_set_split; i++) {
        unsigned int avail_valves_1 = i;
        unsigned int avail_valves_2 = valve_mask ^ i;

        int pressure_1 = _get_max_pressure(valves, distances, id_start, avail_valves_1, 26, memory_state);
        // Abort if the best possible while being single is not enough to get more than the current max
        if (pressure_1 + max_single_pressure <= max_pressure)
            continue;

        int pressure_2 = _get_max_pressure(valves, distances, id_start, avail_valves_2, 26, memory_state);

        if (pressure_1 + pressure_2 > max_pressure)
            max_pressure = pressure_1 + pressure_2;
    }
    return max_pressure;
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

    std::vector<std::string> lines;
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            lines.push_back(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::sort(lines.begin(), lines.end(), line_sorter);
    std::unordered_map<std::string, size_t> name_to_id;
    for (size_t valve_id=0; valve_id<lines.size(); valve_id++)
        name_to_id[lines[valve_id].substr(6, 2)] = valve_id;

    std::vector<Valve> all_valves;
    build_valves(lines, all_valves, name_to_id);

    std::vector<int> line_distances(all_valves.size(), -1);
    DistanceMap distances(all_valves.size(), line_distances);
    compute_distances(all_valves, distances);

    std::vector<Valve> non_empty_valves;
    for (Valve valve : all_valves) {
        if (valve.flow_rate == 0)
            break;
        non_empty_valves.push_back(valve);
    }

    ans = get_max_pressure(non_empty_valves, distances, name_to_id["AA"], first_half);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
