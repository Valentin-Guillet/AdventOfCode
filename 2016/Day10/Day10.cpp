
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>

using namespace std;


class Bot
{
    private:
        int id;
        bool output;
        Bot *low, *high;
        vector<int> chips;

        void update() {
            if (chips[0] > chips[1])
                chips = {chips[1], chips[0]};

            if (chips[0] == 17 && chips[1] == 61)
                cout << "I am bot " << id << endl;

            low->add(chips[0]);
            high->add(chips[1]);
        }

    public:
        Bot(int id, bool output=false) : id(id), output(output) { }

        void set_targets(Bot *low_id, Bot *high_id) {
            low = low_id;
            high = high_id;
        }

        void add(int value) {
            chips.push_back(value);
            if (!output && chips.size() > 1)
                update();
        }

        int value() {
            return chips[0];
        }

};


int main()
{
	string input;
	int ans = 0;
	vector<string> tab;

	ifstream myfile ("input");
	if (myfile.is_open())
	{
		while (getline(myfile, input))
            tab.push_back(input);

		myfile.close();
	} else {
        cout << "Unable to open file" << endl;
        return 1;
    }

    unordered_map<int, Bot*> bots;
    unordered_map<int, Bot*> outputs;

    regex VALUE("value (\\d+) goes to bot (\\d+)");
    regex BOT("bot (\\d+) gives low to (bot|output) (\\d+) and high to (bot|output) (\\d+)");
    smatch match;

    for (string line : tab) {
        if (regex_search(line, match, BOT)) {
            int id = stoi(match[1]);
            bots[id] = new Bot(stoi(match[1]));
        }
    }

    for (string line : tab) {
        if (regex_search(line, match, BOT)) {
            Bot *low, *high;
            int bot_id = stoi(match[1]);
            int low_id = stoi(match[3]);
            int high_id = stoi(match[5]);

            if (match[2] == "bot") {
                low = bots[low_id];
            } else {
                if (outputs.find(low_id) == outputs.end())
                    outputs[low_id] = new Bot(low_id, true);
                low = outputs[low_id];
            }

            if (match[4] == "bot") {
                high = bots[high_id];
            } else {
                if (outputs.find(high_id) == outputs.end())
                    outputs[high_id] = new Bot(high_id, true);
                high = outputs[high_id];
            }

            bots[bot_id]->set_targets(low, high);
        }
    }

    for (string line : tab) {
        if (regex_search(line, match, VALUE)) {
            int bot_id = stoi(match[2]);
            int value = stoi(match[1]);

            bots[bot_id]->add(value);
        }
    }

    ans = outputs[0]->value() * outputs[1]->value() * outputs[2]->value();

    for (pair<int, Bot*> bot : bots)
        delete bot.second;

    for (pair<int, Bot*> output : outputs)
        delete output.second;

    cout << "Answer : " << ans << endl;
	return 0;
}
