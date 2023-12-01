
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

struct link {
    std::string parent;
    std::string child;
};

struct node {
    std::string name;
    int depth;
};


int main(void)
{
    bool first_half = false;
    int ans = 0;
    std::vector<link> tab;

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            link my_link;
            int pos = input.find(')');
            my_link.parent = input.substr(0, pos);
            my_link.child = input.substr(pos+1);
            tab.push_back(my_link);
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::queue<node> waiting;
    node current, child;

    if (first_half)
    {
        node root = {"COM", 0};
        waiting.push(root);

        while (!waiting.empty()) {
            current = waiting.front();
            waiting.pop();
            ans += current.depth;
            for (link l : tab) {
                if (l.parent == current.name) {
                    child.name = l.child;
                    child.depth = current.depth + 1;
                    waiting.push(child);
                }
            }
        }

    } else {
        node root = {"YOU", 0};

        waiting.push(root);
        current = root;
        std::unordered_set<std::string> seen;

        while (current.name != "SAN") {
            current = waiting.front();
            waiting.pop();
            seen.insert(current.name);
            for (link l : tab) {
                if ((seen.find(l.child) == seen.end()) && l.parent == current.name) {
                    child.name = l.child;
                    child.depth = current.depth + 1;
                    waiting.push(child);
                } else if ((seen.find(l.parent) == seen.end()) && l.child == current.name) {
                    child.name = l.parent;
                    child.depth = current.depth + 1;
                    waiting.push(child);
                }
            }
        }
        ans = current.depth - 2;
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
