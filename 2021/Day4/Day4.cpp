
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>


struct Board {
    int tab[5][5];
    bool marked[5][5] = {false};
    bool finished = false;

    Board(int values[5][5]) {
        for (int x=0; x<5; x++) {
            for (int y=0; y<5; y++)
                tab[x][y] = values[x][y];
        }
    }

    void mark(int value) {
        for (int x=0; x<5; x++) {
            for (int y=0; y<5; y++) {
                if (tab[x][y] == value)
                    marked[x][y] = true;
            }
        }
    }

    bool won() {
        // Lines
        for (int x=0; x<5; x++) {
            bool win = true;
            for (int y=0; y<5; y++)
                win &= marked[x][y];
            if (win)
                return true;
        }

        // Columns
        for (int y=0; y<5; y++) {
            bool win = true;
            for (int x=0; x<5; x++)
                win &= marked[x][y];
            if (win)
                return true;
        }

        return false;
    }

    int sum_unmarked() {
        int count = 0;
        for (int x=0; x<5; x++) {
            for (int y=0; y<5; y++) {
                if (!marked[x][y])
                    count += tab[x][y];
            }
        }
        return count;
    }
};


void debug(Board board) {
    for (int x=0; x<5; x++) {
        for (int y=0; y<5; y++)
            std::cout << board.tab[x][y] << (board.marked[x][y] ? '*' : ' ') << " ";
        std::cout << std::endl;
    }
}


int main(int argc, char* argv[])
{
    bool first_half = true;
    int ans = 0;
    std::queue<int> drawn;
    std::vector<Board> boards;

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
        std::istringstream iss(input);
        std::string s;
        while (getline(iss, s, ','))
            drawn.push(std::stoi(s));

        while (getline(myfile, input)) {
            int tab[5][5];
            for (int i=0; i<5; i++) {
                getline(myfile, input);
                for (int j=0; j<5; j++) {
                    int value = std::stoi(input.substr(j*3, 2));
                    tab[i][j] = value;
                }
            }
            boards.push_back(Board(tab));
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }


    bool finished = false;
    while (!finished && !drawn.empty()) {
        int value = drawn.front();
        drawn.pop();

        finished = true;
        for (Board &board : boards) {
            if (board.finished)
                continue;

            finished = false;
            board.mark(value);
            if (board.won()) {
                ans = value * board.sum_unmarked();
                board.finished = true;
                if (first_half)
                    finished = true;
            }
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
