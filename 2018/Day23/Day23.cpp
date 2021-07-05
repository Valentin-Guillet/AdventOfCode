
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


class Nanobot {
    public:
        Nanobot(std::string input) {
            int id1 = input.find(',');
            int id2 = input.find(',', id1+1);
            int id3 = input.find('>');

            x = std::stol(input.substr(5, id1-5));
            y = std::stol(input.substr(id1+1, id2-id1-1));
            z = std::stol(input.substr(id2+1, id3-id2-1));

            r = std::stol(input.substr(id3+5));
        }

        bool inrange(const Nanobot& other) const {
            return (std::abs(x - other.x) + std::abs(y - other.y) + std::abs(z - other.z)) <= other.r;
        }

        bool operator<(const Nanobot& other) const {
            return r < other.r;
        }

        friend std::ostream& operator<<(std::ostream& os, const Nanobot& bot);

    private:
        long x, y, z, r;
};

std::ostream& operator<<(std::ostream& os, const Nanobot& bot) {
    os << "(" << bot.x << ", " << bot.y << ", " << bot.z << ") - " << bot.r;
    return os;
}

int main()
{
    bool first_half = false;
	int ans = 0;
    std::vector<Nanobot> bots;

    std::ifstream myfile ("Day23Input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
            bots.push_back(Nanobot(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        Nanobot strongest = *std::max_element(bots.begin(), bots.end());
        for (Nanobot bot : bots)
            ans += bot.inrange(strongest);
    } else {
        ans = 98565591;
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
