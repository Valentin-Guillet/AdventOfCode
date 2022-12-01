
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_set>


using AllergenTable = std::map<std::string, std::unordered_set<std::string>>;

int main()
{
    bool first_half = false;
    AllergenTable allergens;
    std::map<std::string, int> ingredients;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            size_t bracket_index = input.find('(');
            std::string ingrs = input.substr(0, bracket_index-1);
            std::string allers = input.substr(bracket_index+10, input.size()-bracket_index-11);

            std::unordered_set<std::string> pres_ingr;
            size_t space_index;
            do {
                space_index = ingrs.find(' ');
                ingredients[ingrs.substr(0, space_index)]++;
                pres_ingr.insert(ingrs.substr(0, space_index));
                ingrs = ingrs.substr(space_index+1);
            } while (space_index != std::string::npos);

            size_t comma_index;
            do {
                comma_index = allers.find(", ");
                std::string aller = allers.substr(0, comma_index);
                if (allergens[aller].empty()) {
                    allergens[aller].insert(pres_ingr.begin(), pres_ingr.end());
                } else {
                    std::unordered_set<std::string> new_allers;
                    for (std::string ingr : pres_ingr) {
                        if (allergens[aller].find(ingr) != allergens[aller].end())
                            new_allers.insert(ingr);
                    }
                    allergens[aller] = new_allers;
                }
                allers = allers.substr(comma_index+2);
            } while (comma_index != std::string::npos);
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        int ans = 0;
        for (auto [ingr, count] : ingredients) {
            bool found = false;
            for (auto [name, list] : allergens) {
                if (list.find(ingr) != list.end()) {
                    found = true;
                    break;
                }
            }

            if (!found)
                ans += count;
        }
        std::cout << "Answer : " << ans << std::endl;

    } else {
        std::unordered_set<std::string> done;

        size_t n = allergens.size();
        for (size_t step=0; step<n; ++step) {
            for (auto& [aller_name, ingr_list] : allergens) {
                if (done.find(aller_name) == done.end() && ingr_list.size() == 1) {
                    done.insert(aller_name);
                    std::string ingr_name = *ingr_list.begin();
                    for (auto& [other_aller, other_list] : allergens) {
                        if (aller_name == other_aller)
                            continue;

                        if (other_list.find(ingr_name) != other_list.end())
                            other_list.erase(ingr_name);
                    }
                }
            }
        }

        std::cout << "Answer: ";
        for (auto [name, list] : allergens)
            std::cout << *list.begin() << ",";
        std::cout << std::endl;

    }

	return 0;
}
