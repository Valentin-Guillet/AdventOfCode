
#include "iostream"
#include <vector>
#include <algorithm>


int main(void)
{
	int ans = 0;
	bool first_half = false;
	std::vector<int> tab;

	if (first_half) {
		tab.push_back(0);
		int pos = 0;
	
		for (int i = 0; i < 2017; ++i)
		{
			pos = (pos + 3) % tab.size() + 1;
			tab.insert(tab.begin() + pos, i+1);
		}
	
		ans = tab[std::find(tab.begin(), tab.end(), 2017) - tab.begin() + 1];
		std::cout << "Answer : " << ans << std::endl;

	} else {
		int pos = 0;
		int size = 1;
		for (int i = 0; i < 50000000; ++i)
		{
			pos = (pos + 328) % size + 1;
			if (pos == 1)
				std::cout << i+1 << std::endl;
			++size;
		}
	}

	return 0;
}