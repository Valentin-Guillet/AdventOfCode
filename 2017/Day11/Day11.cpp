
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <algorithm>

int main(void)
{
    std::string input;
    std::string dir;
    int horizontal = 0, abs_horizontal = 0;
    int vertical = 0, abs_vertical = 0;
    int ans = 0;
    std::vector<int> tab;

    std::ifstream myfile ("input");
    if (myfile.is_open())
    {
        getline(myfile, input);
        myfile.close();
    }
    else std::cout << "Unable to open file";

    input += ',';
    int pos;
    while ((pos = input.find(',')) != std::string::npos) {
        dir = input.substr(0, pos);
        // std::cout << dir << std::endl;
        input.erase(0, pos+1);
        if (dir == "n")
            vertical += 2;
        else if (dir == "ne") {
            vertical++;
            horizontal++;
        } else if (dir == "nw") {
            vertical++;
            horizontal--;
        } else if (dir == "s") {
            vertical -= 2;
        } else if (dir == "se") {
            vertical--;
            horizontal++;
        } else if (dir == "sw") {
            vertical--;
            horizontal--;
        }

        abs_vertical = std::abs(vertical);
        abs_horizontal = 2*std::abs(horizontal);
        if (abs_horizontal > abs_vertical)
            ans = std::max(ans, abs_horizontal/2);
        else
            ans = std::max(ans, abs_horizontal/2 + (abs_vertical-abs_horizontal/2)/2);
    }
    std::cout << "Answer : " << ans << std::endl;
    return 0;
}