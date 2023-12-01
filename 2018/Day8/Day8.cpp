
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>


class Node {
    private:
        int nb_child;
        int nb_meta;

        std::vector<Node*> children;
        std::vector<int> metadata;

    public:

        ~Node() {
            for (Node* child : children)
                delete child;
        }

        std::queue<int> set(std::queue<int> input) {
            nb_child = input.front();
            input.pop();
            nb_meta = input.front();
            input.pop();

            for (int i=0; i<nb_child; ++i) {
                Node* child = new Node();
                input = child->set(input);
                children.push_back(child);
            }

            for (int i=0; i<nb_meta; ++i) {
                metadata.push_back(input.front());
                input.pop();
            }

            return input;
        }

        int sum_metadata() {
            int ans = 0;
            for (int i=0; i<nb_meta; ++i)
                ans += metadata[i];

            for (Node* child : children)
                ans += child->sum_metadata();

            return ans;
        }

        int get_value() {
            if (children.empty())
                return sum_metadata();

            int ans = 0;
            for (int id : metadata) {
                if (1 <= id && id <= nb_child)
                    ans += children[id-1]->get_value();
            }

            return ans;
        }
};

int main(void)
{
    bool first_half = false;
    int ans = 0;
    std::queue<int> tab;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input, ' '))
            tab.push(std::stoi(input));

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    Node* root = new Node();
    if (!root->set(tab).empty())
        std::cout << "Error, something went wrong !";

    if (first_half) {
        ans = root->sum_metadata();
    } else {
        ans = root-> get_value();
    }

    delete root;

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
