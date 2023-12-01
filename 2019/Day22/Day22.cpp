
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

const int M = 100007;
const __int128 N = 119315717514047;
const __int128 N_shuffle = 101741582076661;


std::vector<int> increment(std::vector<int> cards, int value) {
    std::vector<int> new_cards;
    for (unsigned int i=0; i<cards.size(); ++i)
        new_cards.push_back(0);

    int pos = 0;
    for (unsigned int i=0; i<cards.size(); ++i) {
        new_cards[pos] = cards[i];
        pos = (pos + value) % cards.size();
    }

    return new_cards;
}


__int128 modular_pow(int value) {
    __int128 result = 1;
    __int128 exponent = N - 2;
    __int128 base = value;

    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % N;
        exponent = exponent >> 1;
        base = (base * base) % N;
        if (base < 0)
            std::cout << "ERROR / ABORT" << std::endl;
    }
    return result;
}


int main(void)
{
    bool first_half = false;
    long long int ans = 0;
    std::vector<int> cards;
    std::vector<std::string> techniques;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input))
            techniques.push_back(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {

        for (int i=0; i<M; ++i) {
            cards.push_back(i);
        }

        for (std::string technique : techniques) {
            if (technique == "deal into new stack") {
                std::reverse(cards.begin(), cards.end());
            } else if (technique.substr(0, 3) == "cut") {
                int value = std::stoi(technique.substr(4));
                if (value > 0) {
                    std::rotate(cards.begin(), cards.begin()+value, cards.end());
                } else {
                    std::rotate(cards.begin(), cards.begin()+cards.size()+value, cards.end());
                }
            } else {
                int value = std::stoi(technique.substr(20));
                cards = increment(cards, value);
            }
        }

        for (int i=0; i<M; ++i) {
            if (cards[i] == 2019)
                ans = i;
        }

    } else {
        std::reverse(techniques.begin(), techniques.end());

        std::unordered_map<int, __int128> inverses;
        for (std::string technique : techniques) {
            if (technique.substr(0, 9) == "deal with") {
                int value = std::stoi(technique.substr(20));
                inverses[value] = modular_pow(value);
            }
        }

        __int128 a = 1;
        __int128 b = 0;

        for (std::string technique : techniques) {
            if (technique == "deal into new stack") {
                a *= -1;
                b = N - 1 - b;
                if (a < 0)
                    a += N;
            } else if (technique.substr(0, 3) == "cut") {
                int value = std::stoi(technique.substr(4));
                b = (b + value) % N;
                if (b < 0)
                    b += N;
            } else {
                int value = std::stoi(technique.substr(20));
                a = (a * inverses[value]) % N;
                b = (b * inverses[value]) % N;
            }
        }

        __int128 res_a = 1;
        __int128 res_b = 0;
        __int128 number = N_shuffle;
        __int128 tmp_a;

        while (number > 0) {
            if (number % 2 == 1) {
                res_a = (a * res_a    ) % N;
                res_b = (a * res_b + b) % N;
            }
            number = number >> 1;
            tmp_a = a;
            a = (a * a) % N;
            b = (tmp_a * b + b) % N;
        }

        __int128 position = 2020;

        ans = (res_a * position + res_b) % N;

    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
