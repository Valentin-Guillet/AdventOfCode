
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>


char corrupted_char(std::string line) {
    std::stack<char> chunks;
    for (char c : line) {
        if (c == '(') chunks.push(')');
        else if (c == '[') chunks.push(']');
        else if (c == '{') chunks.push('}');
        else if (c == '<') chunks.push('>');
        else if (chunks.top() == c) chunks.pop();
        else return c;
    }
    return '0';
}

std::string complete_line(std::string line) {
    std::stack<char> chunks;
    for (char c : line) {
        if (c == '(') chunks.push(')');
        else if (c == '[') chunks.push(']');
        else if (c == '{') chunks.push('}');
        else if (c == '<') chunks.push('>');
        else if (chunks.top() == c) chunks.pop();
        else return "";
    }

    if (chunks.empty())
        return "";

    std::string ans;
    while (!chunks.empty()) {
        ans.push_back(chunks.top());
        chunks.pop();
    }
    return ans;
}

long  compute_score(std::string line) {
    long score = 0;
    for (char c : line) {
        long add;
        if (c == ')') add = 1;
        else if (c == ']') add = 2;
        else if (c == '}') add = 3;
        else if (c == '>') add = 4;
        score = 5*score + add;
    }
    return score;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
    long ans = 0;
    std::vector<std::string> lines;

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

        while (getline(myfile, input))
            lines.push_back(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (std::string line : lines) {
            switch (corrupted_char(line)) {
                case '0': ans += 0; break;
                case ')': ans += 3; break;
                case ']': ans += 57; break;
                case '}': ans += 1197; break;
                case '>': ans += 25137; break;
            }
        }
    } else {
        std::vector<long> scores;
        for (std::string line : lines) {
            std::string end_line = complete_line(line);
            long score = compute_score(end_line);
            if (score > 0)
                scores.push_back(score);
        }

        std::sort(scores.begin(), scores.end());
        size_t middle_index = (scores.size()-1) / 2;
        ans = scores[middle_index];
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
