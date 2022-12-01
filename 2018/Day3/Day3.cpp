
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


struct Claim {
    int id;
    int shift_left, shift_up;
    int width, height;
    std::string line;

    Claim(std::string input) {
        line = input;
        int id_at = input.find('@');
        int id_comma = input.find(',');
        int id_column = input.find(':');
        int id_times = input.find('x');

        id = std::stoi(input.substr(1, id_at-2));
        shift_left = std::stoi(input.substr(id_at+2, id_comma-id_at-2));
        shift_up = std::stoi(input.substr(id_comma+1, id_column-id_comma-1));
        width = std::stoi(input.substr(id_column+2, id_times-id_column-2));
        height = std::stoi(input.substr(id_times+1));
    }
};

int main(void)
{
    bool first_half = false;
	int ans = 0;
	std::vector<Claim> tab;

	std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            tab.push_back(Claim(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int fabric[1000][1000] = {0};

    for (Claim claim : tab) {
        for (int i=claim.shift_left; i<claim.shift_left+claim.width; ++i) {
            for (int j=claim.shift_up; j<claim.shift_up+claim.height; ++j)
                fabric[i][j]++;
        }
    }

    if (first_half) {
        for (int i=0; i<1000; ++i) {
            for (int j=0; j<1000; ++j) {
                ans += (fabric[i][j] > 1);
            }
        }
    } else {
        for (Claim claim : tab) {
            bool overlapped = false;
            for (int i=claim.shift_left; i<claim.shift_left+claim.width; ++i) {
                for (int j=claim.shift_up; j<claim.shift_up+claim.height; ++j) {
                    if (fabric[i][j] > 1) {
                        overlapped = true;
                        goto end_claim;
                    }
                }
            }
end_claim:
            if (!overlapped) {
                ans = claim.id;
                break;
            }
        }
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
