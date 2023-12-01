
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <math.h>
#include <iomanip>

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

int main(void)
{
    std::string input;
    int ans = 0;
    std::vector<int> tab;

    std::ifstream myfile ("input");
    if (myfile.is_open())
    {
        getline(myfile, input);
        myfile.close();
    }
    else
        std::cout << "Unable to open file";

    int i = 0;
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

    for (int j = 0; j < 64; ++j)
    {
        for (int i = 0; i < tab.size(); ++i)
        {
            int len = tab[i];
            reverse(numbers, pos, len);
            if (pos + len > 256)
                update(numbers,pos + len - 256);
            reset(numbers);
            pos = (pos + len + skip) % 256;
            skip++;
            }
    }

    std::string dense_hash[16];
    for (int i = 0; i < 16; ++i)
    {
        int xor_ans = 0;
        for (int j = 0; j < 16; ++j)
            xor_ans ^= numbers[16*i + j];
        std::stringstream stream;
        stream << std::hex << xor_ans;
        dense_hash[i] = stream.str();
    }

    for (int i = 0; i < 16; ++i)
        std::cout << dense_hash[i];
    std::cout << std::endl;

    return 0;
}