
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

std::unordered_map<long, long> memoization;

long compute_lanterfish(long n) {
    if (n < 1) return 1;
    if (n < 7) return 2;

    if (memoization.find(n) != memoization.end())
        return memoization[n];

    int q = n / 7, r = n % 7;
    long result = compute_lanterfish(r);
    for (int i=0; i<q; i++)
        result += compute_lanterfish(7*i + r-2);
    memoization[n] = result;

    return result;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	long ans = 0;
    std::vector<int> lanternfishes;

    std::ifstream myfile("Day6Input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("testInput");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
        std::string input;

		getline(myfile, input);
        std::istringstream iss(input);
        std::string s;
        while (getline(iss, s, ','))
            lanternfishes.push_back(std::stoi(s));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        int n_days = 80;
        for (int day=0; day<n_days; day++) {
            size_t size = lanternfishes.size();
            for (size_t i=0; i<size; i++) {
                lanternfishes[i]--;
                if (lanternfishes[i] == -1) {
                    lanternfishes[i] = 6;
                    lanternfishes.push_back(8);
                }
            }
        }
        ans = lanternfishes.size();

    } else {
        int n_days = 256;
        for (int lanternfish : lanternfishes) {
            long start = n_days - lanternfish;
            ans += compute_lanterfish(start);
        }
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
