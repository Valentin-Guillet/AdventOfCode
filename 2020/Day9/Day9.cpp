
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


bool is_valid(std::vector<long> const& numbers, int index) {
    std::vector<long> prev_numbers(numbers.begin()+index-26, numbers.begin()+index-1);
    std::sort(prev_numbers.begin(), prev_numbers.end());
    long target = numbers[index-1];

    int i = 0;
    int j = 24;
    while (i < j) {
        if (prev_numbers[i] + prev_numbers[j] == target)
            return true;
        else if (prev_numbers[i] + prev_numbers[j] < target)
            i++;
        else
            j--;
    }
    return false;
}

int main()
{
    bool first_half = false;
    long ans = 0;
    std::vector<long> numbers;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            numbers.push_back(std::stol(input));

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int index = 26;
    while (is_valid(numbers, index))
        ++index;
    int invalid_number = numbers[index-1];

    if (first_half) {
        ans = invalid_number;
    } else {
        for (int i=0; i<(int) numbers.size(); ++i) {
            int sum = numbers[i];
            int j = i+1;
            while (sum < invalid_number)
                sum += numbers[j++];

            if (sum == invalid_number) {
                auto min_max = std::minmax_element(numbers.begin()+i, numbers.begin()+j);
                ans = *min_max.first + *min_max.second;
                break;
            }
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
