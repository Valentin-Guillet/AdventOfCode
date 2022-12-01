
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


struct Range {
    long low;
    long high;

    Range(long low, long high) : low(low), high(high) { }

    Range(string input) {
        int id = input.find('-');
        low = stol(input.substr(0, id));
        high = stol(input.substr(id+1));
    }

    vector<Range> remove(Range blacklist) {
        vector<Range> new_ranges;

        if (blacklist.high <= low || blacklist.low >= high) {
            new_ranges.push_back(Range(low, high));;

        } else if (blacklist.low <= low && blacklist.high < high) {
            new_ranges.push_back(Range(blacklist.high+1, high));

        } else if (low < blacklist.low && high <= blacklist.high) {
            new_ranges.push_back(Range(low, blacklist.low-1));

        } else if (low < blacklist.low && blacklist.high < high) {
            new_ranges.push_back(Range(low, blacklist.low-1));
            new_ranges.push_back(Range(blacklist.high+1, high));
        }

        return new_ranges;
    }

    long size() {
        return high - low + 1;
    }
};

ostream& operator<<(ostream& os, Range const& range) {
    os << "[" << range.low << "-" << range.high << "]";
    return os;
}


int main()
{
    bool first_half = false;
	string input;
	long ans = 0;
	vector<Range> tab;

	ifstream myfile ("input");
	if (myfile.is_open())
	{
		while (getline(myfile, input))
            tab.push_back(Range(input));

		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    vector<Range> valid_ranges;
    valid_ranges.push_back(Range(0, 4294967295));

    for (Range range : tab) {
        vector<Range> new_ranges;

        for (Range valid_range : valid_ranges) {
            vector<Range> new_blacklist = valid_range.remove(range);
            new_ranges.insert(new_ranges.end(), new_blacklist.begin(), new_blacklist.end());

        }

        valid_ranges = new_ranges;
    }

    if (first_half) {
        long min = 4294967295;
        for (Range range : valid_ranges) {
            if (range.low < min)
                min = range.low;
        }
        ans = min;
    } else {
        for (Range range : valid_ranges)
            ans += range.size();
    }

	cout << "Answer : " << ans << endl;
	return 0;
}
