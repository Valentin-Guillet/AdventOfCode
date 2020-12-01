
#include <iostream>
#include <vector>
#include <unordered_map>


int main()
{
    bool first_half = false;
	int ans = 0;
    std::vector<int> numbers = {16, 1, 0, 18, 12, 14, 19};
    std::unordered_map<int, size_t> indices;

    for (size_t i=0; i<numbers.size()-1; ++i)
        indices[numbers[i]] = i+1;

    size_t n;
    if (first_half)
        n = 2020;
    else
        n = 30000000;

    while (numbers.size() <= n) {
        size_t last_number = numbers[numbers.size()-1];
        if (indices.count(last_number) == 0) {
            indices[last_number] = numbers.size();
            numbers.push_back(0);
        } else {
            numbers.push_back(numbers.size() - indices[last_number]);
            indices[last_number] = numbers.size() - 1;
        }
    }

    ans = numbers[n-1];

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
