
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <algorithm>

int main(void)
{
    bool first_half = false;
    std::string input;
    std::vector<int> tab;
    int ans = 0;

    std::ifstream myfile ("input");
    if (myfile.is_open())
    {
        while (getline(myfile, input)) {
            int i = 0;
            // std::cout << input << std::endl;
            std::string number;
            tab.clear();
            input += '\n';
            while (i < input.length()) {
                if (input[i] != '\t' && input[i] != '\n') {
                    number += input[i];
                } else {
                    // std::cout << number << std::endl;
                    tab.push_back(std::stoi(number, nullptr));
                    number = "";
                }
                i++;
            }
            if (first_half) {
                int min = 0, max = 0;
                min = *std::min_element(tab.begin(), tab.end());
                max = *std::max_element(tab.begin(), tab.end());
                std::cout << min << " < " << max << std::endl;
                ans += (max - min);
            } else {
                for (int i = 0; i < tab.size(); ++i)
                {
                    for (int j = 0; j < tab.size(); ++j)
                    {
                        if (i != j && tab[i] % tab[j] == 0)
                        {
                            ans += tab[i] / tab[j];
                        }
                    }
                }
            }
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}