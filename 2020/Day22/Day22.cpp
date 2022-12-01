
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <unordered_set>


using Deck = std::deque<int>;
using GameState = std::string;

GameState compute_state(Deck const& player_one, Deck const& player_two) {
    GameState state;
    for (int card : player_one)
        state += std::to_string(card) + ",";
    state += "/";
    for (int card : player_two)
        state += std::to_string(card) + ",";
    return state;
}

std::pair<bool, Deck> play_game(Deck player_one, Deck player_two, bool recursive) {
    std::unordered_set<GameState> seen;
    while (!player_one.empty() && !player_two.empty()) {
        bool player_one_wins;

        if (!recursive) {
            player_one_wins = (player_one.front() > player_two.front());

        } else {
            GameState state = compute_state(player_one, player_two);
            if (seen.find(state) != seen.end())
                return {true, player_one};
            seen.insert(state);

            if ((int) player_one.size() > player_one.front() && (int) player_two.size() > player_two.front()) {
                Deck copy_player_one;
                for (int i=1; i<player_one.front()+1; ++i)
                    copy_player_one.push_back(player_one[i]);

                Deck copy_player_two;
                for (int i=1; i<player_two.front()+1; ++i)
                    copy_player_two.push_back(player_two[i]);

                player_one_wins = play_game(copy_player_one, copy_player_two, recursive).first;

            } else {
                player_one_wins = (player_one.front() > player_two.front());
            }
        }

        if (player_one_wins) {
            player_one.push_back(player_one.front());
            player_one.push_back(player_two.front());
        } else {
            player_two.push_back(player_two.front());
            player_two.push_back(player_one.front());
        }

        player_one.pop_front();
        player_two.pop_front();
    }

    if (player_one.empty())
        return {false, player_two};
    else
        return {true, player_one};
}

int main()
{
    bool first_half = false;
	int ans = 0;
    Deck player_one, player_two;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

        getline(myfile, input);
        getline(myfile, input);
        while (input != "") {
            player_one.push_back(std::stoi(input));
            getline(myfile, input);
        }

        getline(myfile, input);
        while (getline(myfile, input))
            player_two.push_back(std::stoi(input));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }
    auto [winner, winning_deck] = play_game(player_one, player_two, !first_half);

    int cumul = 0;
    while (!winning_deck.empty()) {
        cumul += winning_deck.front();
        ans += cumul;
        winning_deck.pop_front();
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
