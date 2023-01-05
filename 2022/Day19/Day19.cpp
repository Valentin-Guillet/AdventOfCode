
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using Action = int;

struct Blueprint {
    int id;
    int ore_costs[4];

    int obsidian_robot_clay_cost;
    int geode_robot_obsidian_cost;

    int max_ore_robots;

    Blueprint(std::string line) {
        size_t index = line.find(':');
        id = std::stoi(line.substr(10, index - 10));

        line = line.substr(index + 1);
        ore_costs[0] = line[22] - '0';
        ore_costs[1] = line[51] - '0';
        ore_costs[2] = line[84] - '0';

        index = line.find(' ', 94);
        obsidian_robot_clay_cost = std::stoi(line.substr(94, index - 94));

        line = line.substr(index + 7);
        ore_costs[3] = line[23] - '0';

        index = line.find(' ', 33);
        geode_robot_obsidian_cost = std::stoi(line.substr(33, index - 33));

        max_ore_robots = 0;
        for (size_t i=0; i<4; i++) {
            if (ore_costs[i] > max_ore_robots)
                max_ore_robots = ore_costs[i];
        }
    }
};

int get_number_turns(int need, int production) {
    if (need <= 0)
        return 0;

    int required_turns = need / production;
    if (need % production != 0)
        required_turns++;

    return required_turns;
}

struct State {
    Blueprint const& blueprint;
    int time_step = 0;

    int resources[4] = {0, 0, 0, 0};
    int robots[4] = {1, 0, 0, 0};

    State(Blueprint const& blueprint_) : blueprint(blueprint_) { }

    int get_upper_bound(int time_limit) const {
        // There are some geode robots built
        if (robots[3] > 0) {
            int length = (time_limit - time_step);
            // Don't have enough obsidian to build a geode robot next step
            if (resources[2] < blueprint.geode_robot_obsidian_cost)
                return resources[3] + robots[3] + ((length - 1) / 2 + robots[3]) * length;

            return resources[3] + (length / 2 + robots[3]) * (length + 1);
        }

        int min_time_geode;
        // Can build a first geode robot next step
        if (blueprint.geode_robot_obsidian_cost < resources[2]) {
            min_time_geode = time_step + 1;

        // Must wait a certain amount of time steps to build the first geode robot
        } else {
            double delta = (2 * robots[2] + 1) * (2 * robots[2] + 1) + 8 * (blueprint.geode_robot_obsidian_cost - resources[2]);
            min_time_geode = time_step + 1 + (-(2 * robots[2] + 1) + sqrt(delta)) / 2;
        }

        // No time to build any geode robot
        if (min_time_geode >= time_limit)
            return 0;

        int length = (time_limit - min_time_geode);
        return resources[3] + (length / 2 + robots[3]) * (length + 1);
    }

    void produce_resources(int duration) {
        for (size_t i=0; i<4; i++)
            resources[i] += duration * robots[i];

        time_step += duration;
    }

    bool step(int time_limit, Action action) {
        if (action == 0) {
            produce_resources(time_limit - time_step);
            return false;
        }

        int required_turns = get_number_turns(blueprint.ore_costs[action - 1] - resources[0],
                                              robots[0]);

        int needed_clays = blueprint.obsidian_robot_clay_cost - resources[1];
        int needed_obsidians = blueprint.geode_robot_obsidian_cost - resources[2];
        if (action == 3 && needed_clays > 0) {
            int required_turns_clay = get_number_turns(needed_clays, robots[1]);
            if (required_turns_clay > required_turns)
                required_turns = required_turns_clay;

        } else if (action == 4 && needed_obsidians > 0) {
            int required_turns_obsidian = get_number_turns(needed_obsidians, robots[2]);
            if (required_turns_obsidian > required_turns)
                required_turns = required_turns_obsidian;
        }

        if (required_turns < 0 || time_step + required_turns >= time_limit)
            return false;

        produce_resources(required_turns + 1);
        resources[0] -= blueprint.ore_costs[action - 1];

        if (action == 3)
            resources[1] -= blueprint.obsidian_robot_clay_cost;
        else if (action == 4)
            resources[2] -= blueprint.geode_robot_obsidian_cost;

        robots[action - 1]++;

        return true;
    }
};

int _get_max_geodes(int time_limit, State state, int curr_max_geodes=0) {
    if (state.time_step >= time_limit)
        return state.resources[3];

    int max_possible_geodes = state.get_upper_bound(time_limit);
    if (max_possible_geodes <= curr_max_geodes)
        return 0;

    int max_geodes = curr_max_geodes;
    for (Action action=0; action<5; action++) {
        // Don't need to build more ore robots if the production is enough to build anything each turn
        if (action == 1 && state.robots[0] >= state.blueprint.max_ore_robots)
            continue;

        // Can't select actions 3 and 4 without corresponding resources production
        if ((action == 3 && state.robots[1] == 0) || (action == 4 && state.robots[2] == 0))
            continue;

        State new_state(state);
        int nb_geodes;
        if (new_state.step(time_limit, action))
            nb_geodes = _get_max_geodes(time_limit, new_state, max_geodes);
        else
            nb_geodes = new_state.resources[3];

        if (nb_geodes > max_geodes)
            max_geodes = nb_geodes;
    }

    return max_geodes;
}

int get_max_geodes(Blueprint blueprint, int time_limit) {
    State state(blueprint);
    int nb_geodes = _get_max_geodes(time_limit, state);

    return nb_geodes;
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

    std::vector<Blueprint> blueprints;
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            blueprints.push_back(Blueprint(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (Blueprint blueprint : blueprints)
            ans += get_max_geodes(blueprint, 24) * blueprint.id;
    } else {
        ans = 1;
        for (size_t i=0; i<3; i++)
            ans *= get_max_geodes(blueprints[i], 32);
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
