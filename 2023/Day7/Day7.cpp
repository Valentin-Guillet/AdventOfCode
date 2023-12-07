
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


using Card = char;
std::string CARD_ORDER;

bool compare_cards(Card c1, Card c2) {
    return CARD_ORDER.find(c1) < CARD_ORDER.find(c2);
}

struct Hand {
    std::string cards;
    int bid;
    int type;

    Hand(const std::string& input, bool first_half) {
        cards = input.substr(0, 5);
        bid = std::stoi(input.substr(6));
        compute_type(first_half);
    }

    void compute_type(bool first_half) {
        std::unordered_map<char, int> count_map;
        for (Card c : cards)
                count_map[c]++;

        // Transform jokers only if there are < 5
        if (!first_half && count_map['J'] < 5) {
            int count_jokers = count_map['J'];
            count_map.erase('J');
            auto max_count = std::max_element(count_map.begin(), count_map.end(),
                    [](const auto& p1, const auto& p2) { return p1.second < p2.second; });
            max_count->second += count_jokers;
        }

        std::string counts;
        for (auto [key, value] : count_map)
            counts += std::to_string(value);

        if (counts.contains('5'))
            type = 7;           // five of a kind
        else if (counts.contains('4'))
            type = 6;           // four of a kind
        else if (counts.contains('3') && counts.contains('2'))
            type = 5;           // full house
        else if (counts.contains('3'))
            type = 4;           // three of a kind
        else if (std::ranges::count(counts, '2') == 2)
            type = 3;           // two pairs
        else if (std::ranges::count(counts, '2') == 1)
            type = 2;           // one pair
        else
            type = 1;           // high card
    }

    bool operator<(const Hand& other) {
        if (type != other.type)
            return (type < other.type);

        for (size_t i = 0; i < 5; i++) {
            if (cards[i] != other.cards[i])
                return (compare_cards(cards[i], other.cards[i]));
        }
        return true;
    }
};

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

    std::vector<Hand> hands;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input); )
            hands.emplace_back(input, first_half);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    CARD_ORDER = (first_half ? "23456789TJQKA" : "J234567898TQKA");

    std::sort(hands.begin(), hands.end());
    for (size_t i = 0; i < hands.size(); ++i)
        ans += (i + 1) * hands[i].bid;

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
