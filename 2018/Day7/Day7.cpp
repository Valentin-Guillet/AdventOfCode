
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::string ALPH = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

struct Step {
    char id;
    int length, begin = -1;
    std::string requirements;

    Step(char id) : id(id) {
        length = 60 + (id - 'A' + 1);
    }

    void add_req(char c) {
        requirements += c;
    }

    bool valid(std::string done) {
        if (done.find(id) != std::string::npos)
            return false;

        for (char c : requirements) {
            if (done.find(c) == std::string::npos)
                return false;
        }

        if (begin > -1)
            return false;

        return true;
    }

    bool update(int time) {
        return (begin > -1 && time == (begin + length));
    }
};

int main(void)
{
    bool first_half = false;
    std::string ans;
    std::vector<Step> steps;

    for (char c : ALPH)
        steps.push_back(Step(c));

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            char req = input[5];
            char dep = input[36];

            steps[dep - 'A'].add_req(req);
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    std::string order;

    if (first_half) {
        while (order.size() < 26) {
            for (Step& step : steps) {
                if (step.valid(order)) {
                    order += step.id;
                    break;
                }
            }
        }
        ans = order;

    } else {
        int time = 0;
        int worker = 0;

        while (order.size() < 26) {
            for (Step& step : steps) {
                if (step.valid(order)) {
                    step.begin = time;
                    worker++;
                }

                if (worker == 5)
                    break;
            }

            do {
                time++;
                for (Step& step : steps) {
                    if (step.update(time)) {
                        worker--;
                        order += step.id;
                    }
                }
            } while (worker == 5);
        }

        ans = std::to_string(time);
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
