
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


long count_arrangements(std::vector<int> adapters, size_t index=1) {
    if (index == adapters.size()-1)
        return 1;

    int ans = count_arrangements(adapters, index+1);
    if (adapters[index+1] - adapters[index-1] <= 3) {
        adapters.erase(adapters.begin()+index);
        ans += count_arrangements(adapters, index);
    }
    return ans;
}

long count_set(std::vector<int> set) {
    if (set.size() < 3)
        return 1;
    return count_arrangements(set);
}

int main()
{
    bool first_half = false;
	long ans = 0;
    std::vector<int> adapters = {0};

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            adapters.push_back(std::stoi(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::sort(adapters.begin(), adapters.end());
    adapters.push_back(*std::max_element(adapters.begin(), adapters.end()) + 3);

    if (first_half) {
        int count_1 = 0;
        int count_3 = 0;
        for (size_t i=0; i<adapters.size()-1; ++i) {
            if (adapters[i+1] - adapters[i] == 1)
                count_1++;
            else if (adapters[i+1] - adapters[i] == 3)
                count_3++;
        }
        ans = count_1 * count_3;
    } else {
        std::vector<std::vector<int>> sets;
        size_t set_beg = 0;
        for (size_t set_end=0; set_end<adapters.size()-1; ++set_end) {
            if (adapters[set_end+1] - adapters[set_end] == 3) {
                std::vector<int> subset(adapters.begin()+set_beg, adapters.begin()+set_end+1);
                sets.push_back(subset);
                set_beg = set_end+1;
            }
        }

        ans = 1;
        for (std::vector<int> set : sets)
            ans *= count_set(set);
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
