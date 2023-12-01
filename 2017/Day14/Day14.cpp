
#include "iostream"
#include <string>
#include <vector>
#include <queue>
#include <math.h>
#include <iomanip>
#include <algorithm>

void reverse(int tab[512], int beg, int len) {
    int tmp;
    for (int i = 0; i < floor(len/2); i++)
    {
        tmp = tab[beg+i];
        tab[beg+i] = tab[beg+len-i-1];
        tab[beg+len-i-1] = tmp;
    }
}

void update(int tab[512], int len) {
    for (int i = 0; i < len; i++)
        tab[i] = tab[256+i];
}

void reset(int tab[512]) {
    for (int i = 0; i < 256; i++)
        tab[i+256] = tab[i];
}

std::string knot_hash(std::string input) {
    std::vector<int> tab;
    for (int i = 0; i < input.size(); ++i)
        tab.push_back(input[i]);
    tab.push_back(17);
    tab.push_back(31);
    tab.push_back(73);
    tab.push_back(47);
    tab.push_back(23);

    int numbers[512] = {0};
    for (int i = 0; i < 256; ++i)
        numbers[i] = i;
    reset(numbers);

    int pos = 0;
    int skip = 0;
    for (int j = 0; j < 64; ++j) {
        for (int i = 0; i < tab.size(); ++i) {
            int len = tab[i];
            reverse(numbers, pos, len);
            if (pos + len > 256)
                update(numbers,pos + len - 256);
            reset(numbers);
            pos = (pos + len + skip) % 256;
            skip++;
            }
    }

    std::string dense_hash;
    for (int i = 0; i < 16; ++i) {
        int xor_ans = 0;
        for (int j = 0; j < 16; ++j)
            xor_ans ^= numbers[16*i + j];
        std::stringstream stream;
        stream << std::hex << xor_ans;
        if (stream.str().size() == 1)
            dense_hash += '0';
        dense_hash += stream.str();
    }
    return dense_hash;
}

int main(void)
{
    std::string input;
    int ans = 0;
    bool first_half = false;

    std::string grid[128];
    for (int i = 0; i < 128; ++i) {
        std::string hex_hash = knot_hash("ugkiagan-"+std::to_string(i));
        std::string bin_hash = "";
        for (char c : hex_hash) {
            switch (c) {
                case '0': bin_hash += "0000"; break;
                case '1': bin_hash += "0001"; break;
                case '2': bin_hash += "0010"; break;
                case '3': bin_hash += "0011"; break;
                case '4': bin_hash += "0100"; break;
                case '5': bin_hash += "0101"; break;
                case '6': bin_hash += "0110"; break;
                case '7': bin_hash += "0111"; break;
                case '8': bin_hash += "1000"; break;
                case '9': bin_hash += "1001"; break;
                case 'a': bin_hash += "1010"; break;
                case 'b': bin_hash += "1011"; break;
                case 'c': bin_hash += "1100"; break;
                case 'd': bin_hash += "1101"; break;
                case 'e': bin_hash += "1110"; break;
                case 'f': bin_hash += "1111"; break;
            }
        }
        if (first_half)
            ans += std::count(bin_hash.begin(), bin_hash.end(), '1');
        grid[i] = bin_hash;
    }

    if (first_half) {
        std::cout << "Answer : " << ans << std::endl;
        return 0;
    }

    bool seen[128][128] = {false};
    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 128; ++j) {
            if (!seen[i][j] && grid[i][j] == '1') {
                std::queue<std::pair<int, int>> pos;
                pos.push({i, j});
                while (pos.size() > 0) {
                    int x = pos.front().first;
                    int y = pos.front().second;
                    pos.pop();
                    if (seen[x][y])
                        continue;
                    seen[x][y] = true;
                    if (0 < x && grid[x-1][y] == '1' && !seen[x-1][y])
                        pos.push({x-1, y});
                    if (x < 127 && grid[x+1][y] == '1' && !seen[x+1][y])
                        pos.push({x+1, y});
                    if (0 < y && grid[x][y-1] == '1' && !seen[x][y-1])
                        pos.push({x, y-1});
                    if (y < 127 && grid[x][y+1] == '1' && !seen[x][y+1])
                        pos.push({x, y+1});
                }
                ans++;
            }
        }
    }
    std::cout << "Answer : " << ans << std::endl;

    return 0;
}
