
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>


struct Square {
    int id;
    bool on_off;
    int min[3];
    int max[3];
};

bool operator<(Square const& s1, Square const& s2) {
    return s1.id < s2.id;
}

bool operator==(Square const& s1, Square const& s2) {
    return s1.min[0] == s2.min[0] && s1.min[1] == s2.min[1] && s1.min[2] == s2.min[2] &&
        s1.max[0] == s2.max[0] && s1.max[1] == s2.max[1] && s1.max[2] == s2.max[2];
}

std::ostream& operator<<(std::ostream& os, Square const& square) {
    os << "(" << square.id << ",";
    os << " [" << square.min[0] << ", " << square.max[0] << "],";
    os << " [" << square.min[1] << ", " << square.max[1] << "],";
    os << " [" << square.min[2] << ", " << square.max[2] << "]";
    os << ", " << (square.on_off ? "on" : "off") << ")";
    return os;
}

struct Segment {
    int min =  100000000;
    int max = -100000000;
};

using EventMap = std::map<int, std::vector<std::pair<bool, Square>>>;


long compute_volume(std::vector<Square> squares, size_t dim=3) {
    std::sort(squares.begin(), squares.end());
    long volume = 0;
    if (dim == 1) {
        std::vector<Segment> segments;
        for (Square square : squares) {
            bool intersect = false;
            std::vector<Segment> new_segments;
            std::vector<Segment> intersected_segments;

            for (Segment seg : segments) {
                // No intersection
                if (square.min[0] > seg.max || square.max[0] < seg.min) {
                    new_segments.push_back(seg);
                    continue;
                }

                // Intersection
                if (square.on_off) {
                    intersected_segments.push_back(seg);
                    intersect = true;

                } else {
                    if (square.min[0] <= seg.min && seg.max <= square.max[0])
                        continue;

                    if (seg.min < square.min[0])
                        new_segments.push_back({seg.min, square.min[0]-1});

                    if (square.max[0] < seg.max)
                        new_segments.push_back({square.max[0]+1, seg.max});
                }
            }

            if (!intersected_segments.empty()) {
                Segment new_seg = {square.min[0], square.max[0]};
                for (Segment seg : intersected_segments) {
                    new_seg.min = std::min(seg.min, new_seg.min);
                    new_seg.max = std::max(seg.max, new_seg.max);
                }
                new_segments.push_back(new_seg);
            }

            if (square.on_off && !intersect)
                new_segments.push_back({square.min[0], square.max[0]});

            segments = new_segments;
        }

        for (Segment segment : segments)
            volume += segment.max - segment.min + 1;

    } else {
        EventMap events;
        for (Square square : squares) {
            events[square.min[dim-1]].push_back({true, square});
            events[square.max[dim-1]+1].push_back({false, square});
        }

        int prev_event = (*events.begin()).first;
        std::vector<Square> curr_squares;
        for (auto [event, square_list] : events) {

            long curr_vol = compute_volume(curr_squares, dim-1);
            volume += (event - prev_event) * curr_vol;

            for (auto [insert, square] : square_list) {
                if (insert)
                    curr_squares.push_back(square);
                else
                    curr_squares.erase(std::find(curr_squares.begin(),
                                                 curr_squares.end(),
                                                 square));
            }

            prev_event = event;
        }
    }

    return volume;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	long ans = 0;
    std::vector<Square> squares;

    std::ifstream myfile("Day22Input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("testInput");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
        std::string input;

        int id = 0;
        while (getline(myfile, input)) {
            bool on_off = (input[1] == 'n');
            size_t index1 = input.find('=');
            size_t index2 = input.find('.');
            size_t index3 = input.find(',');
            size_t index4 = input.find('.', index3);
            size_t index5 = input.find(',', index4);
            size_t index6 = input.find('.', index5);
            int min_x = std::stoi(input.substr(index1+1, index2-index1-1));
            int max_x = std::stoi(input.substr(index2+2, index3-index2-2));
            int min_y = std::stoi(input.substr(index3+3, index4-index3-3));
            int max_y = std::stoi(input.substr(index4+2, index5-index4-2));
            int min_z = std::stoi(input.substr(index5+3, index6-index5-3));
            int max_z = std::stoi(input.substr(index6+2));

            if (!first_half || (-50 <= min_x && min_x <= 50 && -50 <= max_x && max_x <= 50))
                squares.push_back(Square{id, on_off, {min_x, min_y, min_z}, {max_x, max_y, max_z}});
            id++;
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    ans = compute_volume(squares, 3);
    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
