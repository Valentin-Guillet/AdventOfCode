
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


class CircularList
{
    private:
        int value;
        CircularList* prev;
        CircularList* next;

        void remove() {
            if (next == this)
                return;     // Can't remove single node

            this->next->prev = this->prev;
            this->prev->next = this->next;
            this->prev = this;
            this->next = this;
            delete this;
        }

    public:
        CircularList(int value) : value(value), prev(this), next(this) { }

        ~CircularList() {
            while (next != this)
                next->remove();
        }

        CircularList* insert(int val) {
            CircularList *new_node = new CircularList(val);
            new_node->prev = this;
            new_node->next = this->next;

            this->next->prev = new_node;
            this->next = new_node;

            return new_node;
        }

        CircularList* remove_prev() {
            prev->remove();
            return this;
        }

        int get_value() {
            return value;
        }

        CircularList* operator+(int value) {
            CircularList *node = this;
            for (int i=0; i<value; ++i)
                node = node->next;
            return node;
        }

        CircularList* operator-(int value) {
            CircularList *node = this;
            for (int i=0; i<value; ++i)
                node = node->prev;
            return node;
        }

        void disp() {
            std::cout << value;

            CircularList *node = this->next;
            while (node != this) {
                std::cout << ", " << node->value;
                node = node->next;
            }
            std::cout << std::endl;
        }
};

int main(void)
{
    bool first_half = false;
    std::string input;
    long ans = 0;

    std::ifstream myfile ("input");
    if (myfile.is_open())
    {
        getline(myfile, input);
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int id1 = input.find("worth ") + 6;
    int id2 = input.find(" points");

    int nb_players = std::stoi(input.substr(0, input.find(' ')));
    int last_marble = std::stoi(input.substr(id1, id2-id1));

    if (!first_half)
        last_marble *= 100;

    std::vector<long> scores;
    for (int i=0; i<nb_players; ++i)
        scores.push_back(0);

    CircularList *marbles = new CircularList(0);
    marbles = marbles->insert(1);

    int id_player = 2;

    for (int marble=2; marble<=last_marble; ++marble) {

        if (marble % 23 == 0) {
            scores[id_player-1] += marble;
            marbles = *marbles - 7;

            scores[id_player-1] += marbles->get_value();
            marbles = *marbles + 1;
            marbles = marbles->remove_prev();
        } else {
            marbles = *marbles + 1;
            marbles = marbles->insert(marble);
        }

        id_player = (id_player % nb_players) + 1;
    }
    delete marbles;

    ans = *std::max_element(scores.begin(), scores.end());

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
