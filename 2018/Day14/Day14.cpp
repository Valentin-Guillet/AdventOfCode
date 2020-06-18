
#include <iostream>
#include <string>
#include <vector>


bool is_last_digits(const std::vector<int>& tab, std::string input) {
    for (size_t i=0; i<input.size(); ++i) {
        if (tab[tab.size()-input.size()+i] + '0' != input[i])
            return false;
    }
    return true;
}

int main(void)
{
    bool first_half = false;
	int input = 825401;
    long ans = 0;
	std::vector<int> tab = {3, 7};

    int id1 = 0, id2 = 1;

    if (first_half) {
        while (tab.size() < (unsigned int) input+10) {
            int sum = tab[id1] + tab[id2];
            if (sum > 9) {
                tab.push_back(sum / 10);
                sum %= 10;
            }
            tab.push_back(sum);
            id1 = (id1 + tab[id1] + 1) % tab.size();
            id2 = (id2 + tab[id2] + 1) % tab.size();
        }

        for (int i=input; i<input+10; ++i)
            ans = 10*ans + tab[i];

    } else {
        std::string str_input = std::to_string(input);
        while (true) {
            int sum = tab[id1] + tab[id2];
            if (sum > 9) {
                tab.push_back(sum / 10);
                sum %= 10;
                if (is_last_digits(tab, str_input))
                    break;
            }
            tab.push_back(sum);
            if (is_last_digits(tab, str_input))
                break;
            id1 = (id1 + tab[id1] + 1) % tab.size();
            id2 = (id2 + tab[id2] + 1) % tab.size();
        }

        ans = tab.size() - str_input.size();
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
