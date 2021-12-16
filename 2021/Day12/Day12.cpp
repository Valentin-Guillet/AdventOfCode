
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


using Links = std::unordered_map<std::string, std::vector<std::string>>;
using Seen = std::unordered_set<std::string>;

bool is_small(std::string cave) {
    return ('a' <= cave[0] && cave[0] <= 'z');
}

int count_paths(Links const& links, std::string curr_cave, Seen seen, bool twice) {
    if (curr_cave == "end")
        return 1;

    if (is_small(curr_cave))
        seen.insert(curr_cave);

    int count = 0;
    for (std::string cave : links.at(curr_cave)) {
        bool new_twice = twice;
        if (is_small(cave) && seen.contains(cave)) {
            if (twice)
                continue;
            new_twice = true;
        }

        count += count_paths(links, cave, seen, new_twice);
    }
    return count;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
    Links links;

    std::ifstream myfile("Day12Input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("testInput");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            int index = input.find('-');
            std::string start = input.substr(0, index);
            std::string end = input.substr(index+1);
            if (start != "end" && end != "start")
                links[start].push_back(end);
            if (end != "end" && start != "start")
                links[end].push_back(start);
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    ans = count_paths(links, "start", {}, first_half);

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
