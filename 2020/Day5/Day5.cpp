
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


int compute_id(std::string boarding_pass) {
    int row = 0, column = 0;
    for (int i=0; i<7; ++i)
        row = (row << 1) + (boarding_pass[i] == 'B');
    for (int i=7; i<10; ++i)
        column = (column << 1) + (boarding_pass[i] == 'R');
    return row * 8 + column;
}

int main()
{
    bool first_half = false;
	int ans = 0;
    std::vector<std::string> tab;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            tab.push_back(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::vector<int> id;
    for (std::string pass : tab)
        id.push_back(compute_id(pass));
    int max_id = *std::max_element(id.begin(), id.end());

    if (first_half) {
        ans = max_id;
    } else {
        for (int i=8*8; i<max_id; ++i) {
            if (std::find(id.begin(), id.end(), i) == id.end())
                ans = i;
        }

    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
