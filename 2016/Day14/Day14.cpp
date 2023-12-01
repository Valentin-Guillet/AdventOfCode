
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "md5.cpp"

using namespace std;


class MyHasher
{
    private:
        string salt;
        bool first_half;
        unordered_map<string, string> hash_map;

    public:
        MyHasher(string salt, bool first_half) : salt(salt), first_half(first_half) { }

        string operator()(int value) {
            string to_hash = salt + to_string(value);

            if (hash_map.find(to_hash) == hash_map.end()) {

                if (first_half) {
                    hash_map[to_hash] = md5(to_hash);
                } else {
                    string final_hash = to_hash;
                    for (int i=0; i<2017; ++i)
                        final_hash = md5(final_hash);
                    hash_map[to_hash] = final_hash;
                }
            }

            return hash_map[to_hash];
        }

};

pair<bool, char> is_possible_key(string hash) {
    for (unsigned int i=0; i<hash.size()-2; ++i) {
        if (hash[i] == hash[i+1] && hash[i] == hash[i+2])
            return {true, hash[i]};
    }
    return {false, '0'};
}


bool has_quintuple(string hash, char value) {
    for (unsigned int i=0; i<hash.size()-4; ++i) {
        if (hash[i] == value && hash[i+1] == value &&
                                hash[i+2] == value &&
                                hash[i+3] == value &&
                                hash[i+4] == value)
            return true;
    }
    return false;
}

int main()
{
    bool first_half = false;
    string input = "ngcjuoqr";
    int ans = 0;

    MyHasher hasher(input, first_half);

    int index = 0;
    int nb_key = 0;
    while (nb_key < 64) {
        string hash = hasher(index);

        auto [possible, value] = is_possible_key(hash);
        if (possible) {

            for (int i=index+1; i<index+1000; ++i) {
                if (has_quintuple(hasher(i), value)) {
                    nb_key++;
                    break;
                }
            }

        }
        index++;
    }
    ans = index-1;

    cout << "Answer : " << ans << endl;
    return 0;
}
