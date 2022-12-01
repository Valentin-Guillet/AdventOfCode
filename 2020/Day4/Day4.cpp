
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>


std::regex HGT_REGEX("^([0-9]+)(cm|in)$");
std::regex HCL_REGEX("^#[0-9a-f]{6}$");
std::regex ECL_REGEX("^amb|blu|brn|gry|grn|hzl|oth$");
std::regex PID_REGEX("^[0-9]{9}$");

struct Passport {
    int byr = -1;
    int iyr = -1;
    int eyr = -1;
    std::string hgt = "";
    std::string hcl = "";
    std::string ecl = "";
    std::string pid = "";
    std::string cid = "";

    bool is_valid(bool first_half) {
        if (!(byr > 0 && iyr > 0 && eyr > 0 && hgt != "" && hcl != "" && ecl != "" && pid != ""))
            return false;
        if (first_half)
            return true;

        // Years
        if (!(1920 <= byr && byr <= 2002) || !(2010 <= iyr && iyr <= 2020) || !(2020 <= eyr && eyr <= 2030))
            return false;

        // Height
        std::smatch match;
        if (!std::regex_match(hgt, match, HGT_REGEX))
            return false;
        int height = std::stoi(match[1].str());
        if ((match[2].str() == "in" && !(59 <= height && height <= 76)) ||
                (match[2].str() == "cm" && !(150 <= height && height <= 193)))
            return false;

        // Others
        if (!std::regex_search(hcl, HCL_REGEX) || !std::regex_search(ecl, ECL_REGEX) ||
                !std::regex_search(pid, PID_REGEX))
            return false;

        return true;
    }
};

int main()
{
    bool first_half = false;
	int ans = 0;
    std::vector<Passport> tab;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

        Passport passport;
        while (getline(myfile, input)) {
            if (input == "") {
                tab.push_back(passport);
                passport = Passport();
                continue;
            }
            int index;
            do {
                index = input.find(' ');
                std::string field = input.substr(0, index);

                int colon_index = field.find(':');
                std::string key = field.substr(0, colon_index);
                std::string value = field.substr(colon_index+1);

                if (key == "byr") {
                    passport.byr = std::stoi(value);
                } else if (key == "iyr") {
                    passport.iyr = std::stoi(value);
                } else if (key == "eyr") {
                    passport.eyr = std::stoi(value);
                } else if (key == "hgt") {
                    passport.hgt = value;
                } else if (key == "hcl") {
                    passport.hcl = value;
                } else if (key == "ecl") {
                    passport.ecl = value;
                } else if (key == "pid") {
                    passport.pid = value;
                } else if (key == "cid") {
                    passport.cid = value;
                }

                /* std::cout << "Field: " << field << std::endl; */
                input = input.substr(index+1);
                /* std::cout << "Input: " << input << std::endl; */
            } while (index != -1);
        }
        tab.push_back(passport);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (Passport passport : tab) {
        if (passport.is_valid(first_half))
            ans++;
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
