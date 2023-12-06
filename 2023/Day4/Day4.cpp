
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>


std::vector<int> read_numbers(const std::string& input) {
    std::vector<int> numbers;
    std::stringstream line_stream(input);

    std::copy(std::istream_iterator<int>(line_stream), std::istream_iterator<int>(),
              std::back_inserter(numbers));

    return numbers;
}

struct Card {
    int id;
    int nb_wins;
    int nb_copies = 1;
    std::vector<int> winning;
    std::vector<int> scratched;

    explicit Card(const std::string& input) {
        size_t ind_id = input.find(':');
        id = std::stoi(input.substr(5, ind_id - 5));

        size_t ind_bar = input.find('|');
        winning = read_numbers(input.substr(ind_id + 2, ind_bar - ind_id - 3));
        scratched = read_numbers(input.substr(ind_bar + 2));

        nb_wins = 0;
        for (int number : scratched) {
            if (std::find(winning.begin(), winning.end(), number) != winning.end())
                nb_wins++;
        }
    }

    int get_score() const {
        if (nb_wins == 0)
            return 0;

        return 1 << (nb_wins - 1);
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

    std::vector<Card> cards;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input); )
            cards.emplace_back(input);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    for (const Card& card : cards) {
        if (first_half) {
            ans += card.get_score();
        } else {
            ans += card.nb_copies;
            for (int i = 0; i < card.nb_wins; ++i)
                cards[card.id + i].nb_copies += card.nb_copies;
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
