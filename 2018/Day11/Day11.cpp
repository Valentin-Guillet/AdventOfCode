
#include <iostream>
#include <string>
#include <vector>


long get_power(int x, int y, int input) {
    int id = x + 10;
    long pow = id * (id * y + input);
    pow = (pow % 1000) / 100;
    return pow - 5;
}

int main(void)
{
    bool first_half = false;
    int input = 7989;
    std::string ans;

    std::vector<std::vector<long>> power;

    for (int y=0; y<300; ++y) {
        power.push_back({});

        for (int x=0; x<300; ++x)
            power[y].push_back(get_power(x+1, y+1, input));
    }


    int max_power = 0;
    int max_x, max_y, max_size;

    if (first_half) {
        for (int y=0; y<298; ++y) {
            for (int x=0; x<298; ++x) {
                int pow = power[y][x] + power[y][x+1] + power[y][x+2]
                          + power[y+1][x] + power[y+1][x+1] + power[y+1][x+2]
                          + power[y+2][x] + power[y+2][x+1] + power[y+2][x+2];

                if (pow > max_power) {
                    max_power = pow;
                    max_x = x+1;
                    max_y = y+1;
                }
            }
        }

    } else {
        std::vector<std::vector<long>> sum_power = power;

        for (int size=1; size<301; ++size) {
            std::vector<std::vector<long>> next_sum_power;

            for (int y=0; y<301-size; ++y) {
                if (y < 300 - size)
                    next_sum_power.push_back({});

                for (int x=0; x<301-size; ++x) {

                    if (y < 300 - size && x < 300 - size) {
                        int sum = sum_power[y][x];
                        for (int i=0; i<size; ++i) {
                            sum += power[y+size][x+i];
                            sum += power[y+i][x+size];
                        }
                        sum += power[y+size][x+size];

                        next_sum_power[y].push_back(sum);
                    }

                    if (sum_power[y][x] > max_power) {
                        max_power = sum_power[y][x];
                        max_x = x+1;
                        max_y = y+1;
                        max_size = size;
                    }
                }
            }

            sum_power = next_sum_power;
        }
    }

    ans = std::to_string(max_x) + "," + std::to_string(max_y);

    if (!first_half)
        ans += "," + std::to_string(max_size);

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
