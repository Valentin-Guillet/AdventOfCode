
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>


std::regex INPUT_REGEX(R"((\d+) units each with (\d+) hit points (?:\((.*)\) )?with an attack that does (\d+) (\w+) damage at initiative (\d+))");

class Group {
    public:
        Group(std::string input, bool which_army) {
            std::smatch match;
            if (!std::regex_match(input, match, INPUT_REGEX))
                std::cout << "ERROR ! Input \"" << input << "\" invalid !" << std::endl;

            units = std::stoi(match[1]);
            units_save = units;
            hit_points = std::stoi(match[2]);
            set_weaknesses_immunities(match[3]);
            attack_damage = std::stoi(match[4]);
            attack_type = match[5];
            initiative = std::stoi(match[6]);
            army = which_army;
        }

        int get_effective_power() const {
            return units * (attack_damage + boost);
        }

        bool get_army() const {
            return army;
        }

        int get_units() const {
            return units;
        }

        bool is_immune(std::string type) {
            return std::find(immunities.begin(), immunities.end(), type) != immunities.end();
        }

        bool is_weak(std::string type) {
            return std::find(weaknesses.begin(), weaknesses.end(), type) != weaknesses.end();
        }

        void set_target(std::vector<Group*> groups) {
            int max_damage = 0;
            int max_effective_power = 0;
            int max_initiative = 0;

            for (Group *g : groups) {
                if (g->army == army || g->is_attacked || g->is_immune(attack_type))
                    continue;

                int dmg = get_effective_power();
                if (g->is_weak(attack_type))
                    dmg *= 2;
                int eff_power = g->get_effective_power();
                int initiative = g->initiative;

                if (dmg > max_damage || (dmg == max_damage &&
                        (eff_power > max_effective_power || (eff_power == max_effective_power &&
                                                             initiative > max_initiative)))) {
                    max_damage = dmg;
                    max_effective_power = eff_power;
                    max_initiative = initiative;
                    target = g;
                }
            }

            if (target)
                target->is_attacked = true;
        }

        void attack() {
            if (!target || units <= 0 || target->is_immune(attack_type))
                return;

            int dmg = get_effective_power();
            if (target->is_weak(attack_type))
                dmg *= 2;

            target->units -= (int) (dmg / target->hit_points);
        }

        bool update() {
            target = nullptr;
            is_attacked = false;
            return units <= 0;
        }

        void reset(int new_boost) {
            units = units_save;
            if (army)
                boost = new_boost;
        }

        friend std::ostream& operator<<(std::ostream& os, const Group& group);
        friend bool has_one_army(std::vector<Group*> groups);
        friend struct SortEffPower;
        friend struct SortInitiative;

    private:
        int units, units_save;
        int hit_points;
        std::vector<std::string> weaknesses, immunities;
        int attack_damage;
        std::string attack_type;
        int initiative;
        bool army;
        int boost = 0;

        Group* target = nullptr;
        bool is_attacked = false;

        void set_weaknesses_immunities(std::string input) {
            if (input.empty())
                return;

            size_t index = input.find("; ");
            if (index != std::string::npos) {
                set_weaknesses_immunities(input.substr(0, index));
                set_weaknesses_immunities(input.substr(index+2));
                return;
            }

            size_t n;
            std::vector<std::string> *vect;
            if (input[0] == 'w') {
                n = 8;
                vect = &weaknesses;
            } else {
                n = 10;
                vect = &immunities;
            }
            input = input.substr(n);
            size_t first_word_index = input.find(", ");
            while (first_word_index != std::string::npos) {
                vect->push_back(input.substr(0, first_word_index));
                input = input.substr(first_word_index+2);
                first_word_index = input.find(", ");
            }
            vect->push_back(input);
        }

};

std::ostream& operator<<(std::ostream& os, const Group& group) {
    os << "Init: " << group.initiative << ", units: " << group.units;
    os << ", hit points: " << group.hit_points;
    if (!group.weaknesses.empty()) {
        os << ", weaknesses: ";
        for (unsigned int i=0; i<group.weaknesses.size()-1; ++i) {
            os << group.weaknesses[i] << "/";
        }
        os << group.weaknesses[group.weaknesses.size()-1];
    }
    if (!group.immunities.empty()) {
        os << ", immunities: ";
        for (unsigned int i=0; i<group.immunities.size()-1; ++i) {
            os << group.immunities[i] << "/";
        }
        os << group.immunities[group.immunities.size()-1];
    }
    os << ", attack damage: " << group.attack_damage << ", attack type: " << group.attack_type;
    if (group.army) {
        os << ", Immune System";
    } else {
        os << ", Infection";
    }
    if (group.target)
        os << ", target = " << group.target->initiative;
    return os;
}


bool has_one_army(std::vector<Group*> groups) {
    if (groups.empty())
        return false;

    bool army = groups[0]->army;
    for (size_t i=1; i<groups.size(); ++i) {
        if (groups[i]->army != army)
            return false;
    }

    return true;
}

struct SortEffPower {
    bool operator()(const Group *g1, const Group *g2) const {
        return (g1->get_effective_power() > g2->get_effective_power() ||
                (g1->get_effective_power() == g2->get_effective_power() &&
                 g1->initiative > g2->initiative));
    }
};

struct SortInitiative {
    bool operator()(const Group *g1, const Group *g2) const {
        return (g1->initiative > g2->initiative);
    }
};


std::vector<Group*> run_fight(std::vector<Group*> groups, int boost) {
    for (Group *g : groups)
        g->reset(boost);

    int steps = 0;
    while (!has_one_army(groups) && steps < 10000) {
        // Sort by effective power
        std::sort(groups.begin(), groups.end(), SortEffPower());
        // Choose target
        for (Group *g : groups)
            g->set_target(groups);

        // Sort by initiative
        std::sort(groups.begin(), groups.end(), SortInitiative());
        // Attack phase
        for (Group *g : groups)
            g->attack();

        // Update deads
        for (std::vector<Group*>::iterator it=groups.begin(); it!=groups.end(); ) {
            if ((*it)->update()) {
                it = groups.erase(it);
            } else {
                ++it;
            }
        }
        steps++;
    }

    return groups;
}


int main()
{
    bool first_half = false;
    std::vector<Group*> groups;
    int ans = 0;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        // Immune system
        getline(myfile, input);
        getline(myfile, input);
        while (!input.empty()) {
            groups.push_back(new Group(input, true));
            getline(myfile, input);
        }

        // Infection
        getline(myfile, input);
        while (getline(myfile, input)) {
            groups.push_back(new Group(input, false));
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::vector<Group*> surviving_groups;
    if (first_half) {
        surviving_groups = run_fight(groups, 0);
    } else {
        int boost = 0;
        do {
            surviving_groups = run_fight(groups, boost);
            boost++;
        } while (!has_one_army(surviving_groups) || !surviving_groups[0]->get_army());
    }

    for (Group *g : surviving_groups)
        ans += g->get_units();

    for (Group *g : groups)
        delete g;

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
