
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


struct Range {
    long long start, end;
    long long dest_start;
    long long length;

    Range(long long start_, long long len) : start(start_), length(len) {
        end = start + length - 1;
    }

    explicit Range(const std::string& input) {
        std::stringstream line_stream(input);
        line_stream >> dest_start >> start >> length;
        end = start + length - 1;
    }

    bool operator<(const Range& other) const {
        return start < other.start;
    }

    void apply(std::vector<Range>& sources, std::vector<Range>& dests) const {
        for (auto it = sources.begin(); it != sources.end(); ) {
            // Range before or after source: no intersection, do nothing
            if (end < it->start || start > it->end) {
                it++;
                continue;
            }

            // Source in range: remove from sources and add its map to dests
            if (start <= it->start && it->end <= end) {
                dests.emplace_back(dest_start + it->start - start, it->length);
                it = sources.erase(it);
                continue;
            }

            // Start of source in range: shorten source and add map of other part
            if (start <= it->start && end < it->end) {
                dests.emplace_back(dest_start + it->start - start, end - it->start + 1);
                it->start = end + 1;
                it->length = it->end - end;
                it->end = it->start + it->length - 1;
                it++;
                continue;
            }

            // End of source in range: same thing
            if (it->start < start && it->end <= end) {
                dests.emplace_back(dest_start, it->end - start);
                it->length = start - it->start + 1;
                it->end = it->start + it->length - 1;
                it++;
                continue;
            }

            // Range in source: split source and add entire map in dests
            if (it->start < start && end < it->end) {
                dests.emplace_back(dest_start, length);
                // Add source part after range
                sources.emplace_back(end + 1, it->end - end - 1);
                // Shorten source part before range
                it->length = start - it->start + 1;
                it->end = it->start + it->length - 1;
                break;
            }
        }
    }
};

struct Map {
    std::string name;
    std::vector<Range> ranges;

    explicit Map(const std::vector<std::string>& inputs) {
        size_t ind = inputs[0].find(' ');
        name = inputs[0].substr(0, ind);

        for (size_t i = 1; i < inputs.size(); ++i)
            ranges.emplace_back(inputs[i]);
    }

    void map(std::vector<Range>& source_ranges) const {
        std::vector<Range> dest_ranges;

        for (const Range& range : ranges)
            range.apply(source_ranges, dest_ranges);

        // Keep sources that have not been modified, and add destinations
        source_ranges.insert(source_ranges.end(), dest_ranges.begin(), dest_ranges.end());
    }
};

std::vector<Range> read_seeds(std::string input, bool first_half) {
    size_t ind = input.find(' ');
    std::stringstream seed_stream(input.substr(ind));

    std::vector<Range> seeds;
    if (first_half) {
        long long seed;
        while (seed_stream >> seed)
            seeds.emplace_back(seed, 1);
    } else {
        long long seed, length;
        while (seed_stream >> seed >> length)
            seeds.emplace_back(seed, length);
    }
    return seeds;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Range> seeds;
    std::vector<Map> maps;
    if (myfile.is_open()) {
        std::string line;
        std::vector<std::string> lines;

        // Seeds
        getline(myfile, line);
        seeds = read_seeds(line, first_half);

        // Blank line
        getline(myfile, line);

        // Maps
        while (getline(myfile, line)) {
            while (!line.empty()) {
                lines.push_back(line);
                getline(myfile, line);
            }

            maps.emplace_back(lines);
            lines.clear();
        }
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (const Map& map : maps)
        map.map(seeds);
    ans = std::min_element(seeds.begin(), seeds.end())->start;

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
