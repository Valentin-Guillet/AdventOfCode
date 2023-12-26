
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>


struct Brick {
    int id;
    int min_x, max_x;
    int min_y, max_y;
    int min_z, max_z;
    std::vector<Brick*> supports, supported_by;

    explicit Brick(const std::string& input) {
        size_t beg_index = 0, end_index;
        for (int* ref : {&min_x, &min_y, &min_z, &max_x, &max_y, &max_z}) {
            end_index = std::min(input.find(',', beg_index), input.find('~', beg_index));
            *ref = std::stoi(input.substr(beg_index, end_index - beg_index));
            beg_index = end_index + 1;
        }
    }

    bool cross(const Brick& other) const {
        return (((min_x <= other.min_x && other.min_x <= max_x) || (min_x <= other.max_x && other.max_x <= max_x) ||
                 (other.min_x <= min_x && min_x <= other.max_x) || (other.min_x <= max_x && max_x <= other.max_x)) &&
                ((min_y <= other.min_y && other.min_y <= max_y) || (min_y <= other.max_y && other.max_y <= max_y) ||
                 (other.min_y <= min_y && min_y <= other.max_y) || (other.min_y <= max_y && max_y <= other.max_y)));
    }

    void fall(std::vector<Brick>& bricks) {
        int target_z = 1;
        std::vector<int> indices_bricks_below;
        for (int index = id - 1; index >= 0; --index) {
            if (cross(bricks[index])) {
                target_z = std::max(target_z, bricks[index].max_z + 1);
                indices_bricks_below.push_back(index);
            }
        }
        for (int index : indices_bricks_below) {
            if (bricks[index].max_z + 1 == target_z) {
                bricks[index].supports.push_back(this);
                supported_by.push_back(&bricks[index]);
            }
        }
        int z_length = max_z - min_z;
        min_z = target_z;
        max_z = target_z + z_length;
    }

    bool can_desintegrate() const {
        return std::all_of(supports.begin(), supports.end(),
                [](Brick* brick) { return brick->supported_by.size() != 1; });
    }

    void mark_unstable(std::unordered_set<int>& unstable_indices) const {
        bool unstable = std::all_of(supported_by.begin(), supported_by.end(),
                [&](Brick* brick) { return unstable_indices.contains(brick->id); });
        if (!unstable)
            return;

        unstable_indices.insert(id);
        for (Brick* brick : supports)
            brick->mark_unstable(unstable_indices);
    }

    int count_unstables() {
        std::unordered_set<int> unstable_indices;
        unstable_indices.insert(id);
        for (Brick* brick : supports)
            brick->mark_unstable(unstable_indices);
        return unstable_indices.size() - 1;
    }

    bool operator<(const Brick& other) const {
        return min_z < other.min_z;
    }
};

int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;

    std::ifstream myfile("input");
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Brick> bricks;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input); )
            bricks.emplace_back(input);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::sort(bricks.begin(), bricks.end());

    for (size_t index = 0; index < bricks.size(); ++index)
        bricks[index].id = index;

    for (Brick& brick : bricks)
        brick.fall(bricks);

    for (Brick& brick : bricks)
        ans += (first_half ? brick.can_desintegrate() : brick.count_unstables());

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
