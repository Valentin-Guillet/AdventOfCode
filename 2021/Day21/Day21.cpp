
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>


int const IND[7] = {1, 3, 6, 7, 6, 3, 1};

struct Entry {
    size_t id_player;
    int score1, score2;
    int player1, player2;
};

struct EntryHasher {
    size_t operator()(Entry const& entry) const {
        size_t hash = entry.id_player;
        hash ^= entry.score1 + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= entry.score2 + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= entry.player1 + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= entry.player2 + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }
};

bool operator==(Entry const& e1, Entry const& e2) {
    return (e1.id_player == e2.id_player &&
            e1.score1 == e2.score1 && e1.score2 == e2.score2 &&
            e1.player1 == e2.player1 && e1.player2 == e2.player2);
}

using NbWins = std::pair<long, long>;

NbWins operator*(int value, NbWins const& nb_wins) {
    return NbWins{value * nb_wins.first, value * nb_wins.second};
}
void operator+=(NbWins& nw1, NbWins const& nw2) {
    nw1.first += nw2.first;
    nw1.second += nw2.second;
}

std::unordered_map<Entry, NbWins, EntryHasher> _cache_map;

NbWins compute_wins(size_t id_player, int scores[2], int players[2]) {
    Entry entry{id_player, scores[0], scores[1], players[0], players[1]};
    if (_cache_map.contains(entry))
        return _cache_map[entry];

    if (scores[0] >= 21) {
        _cache_map[entry] = {1, 0};
        return {1, 0};
    }
    if (scores[1] >= 21) {
        _cache_map[entry] = {0, 1};
        return {0, 1};
    }

    NbWins ans;
    int new_scores[2] = {scores[0], scores[1]};
    int new_players[2] = {players[0], players[1]};
    for (int i=0; i<7; i++) {
        new_players[id_player] = (players[id_player] + i+3 - 1) % 10 + 1;
        new_scores[id_player] = scores[id_player] + new_players[id_player];
        ans += IND[i] * compute_wins(1 - id_player, new_scores, new_players);
    }

    _cache_map[entry] = ans;
    return ans;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	long ans = 0;
    int players[2];

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

	if (myfile.is_open()) {
        std::string input;

		getline(myfile, input);
        players[0] = std::stoi(input.substr(28));
		getline(myfile, input);
        players[1] = std::stoi(input.substr(28));

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int scores[2] = {0};
    size_t id_player = 0;

    if (first_half) {
        int die = 1;
        int nb_throws = 0;
        while (scores[0] < 1000 && scores[1] < 1000) {
            int value;
            if (die < 99)
                value = 3*die + 3;
            else if (die == 99)
                value = 200;
            else
                value = 103;
            die += 3;
            die = (die - 1) % 100 + 1;
            players[id_player] = (players[id_player] + value);
            players[id_player] = (players[id_player] - 1) % 10 + 1;
            scores[id_player] += players[id_player];

            id_player = 1 - id_player;
            nb_throws += 3;
        }

        ans = scores[id_player] * nb_throws;

    } else {
        NbWins nb_wins = compute_wins(0, scores, players);

        ans = nb_wins.first;
        if (nb_wins.first < nb_wins.second)
            ans = nb_wins.second;
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
