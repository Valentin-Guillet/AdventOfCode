
#include "iostream"
#include <string>
#include <bitset>
#include <queue>

int main(void)
{
	std::string input;
	int ans = 0;

	unsigned long long int a = 618;
	unsigned long long int b = 814;
	std::queue<std::bitset<16>> aa, bb;

	int pair = 0;
	while (pair < 5000000)
	{
		a = 16807 * a % 2147483647;
		b = 48271 * b % 2147483647;
		if (a % 4 == 0)
			aa.push(std::bitset<16>(a));
		if (b % 8 == 0)
			bb.push(std::bitset<16>(b));
		if (aa.size() > 0 && bb.size() > 0) {
			if (aa.front() == bb.front())
				++ans;
			aa.pop();
			bb.pop();
			++pair;
		}

	}

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}