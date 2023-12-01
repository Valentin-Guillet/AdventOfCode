
#include "iostream"
#include <math.h>

int around(int tab[1000][1000], int x, int y)
{
    int ans = 0;
    ans += tab[x+1][y+1];
    ans += tab[x+1][y];
    ans += tab[x+1][y-1];
    ans += tab[x][y+1];
    ans += tab[x][y-1];
    ans += tab[x-1][y+1];
    ans += tab[x-1][y];
    ans += tab[x-1][y-1];
    return ans;
}

int main(void)
{
    int ans = 0;
    bool first_half = false;

    int input = 368078;

    if (first_half)
    {
        int corner = ceil(sqrt(input));
        if (corner % 2 == 0) {corner++;}

        int layer = (corner-1) / 2;
        corner = corner*corner;

        int corner_prec = (2*layer - 1)*(2*layer - 1);
        int pos_on_side = (input - corner_prec) % (2*layer);

        ans = abs(pos_on_side - layer) + layer;
    } else {
        int tab[1000][1000] = {0};
        int x = 500, y = 500;
        tab[x][y] = 1;
        std::cout << x-500 << ", " << y-500 << ", " << tab[x][y] << std::endl;
        y++;
        tab[x][y] = 1;
        std::cout << x-500 << ", " << y-500 << ", " << tab[x][y] << std::endl;
        int turn = 0;
        bool second = false;
        int offset = 0;
        int turnx = 0, turny = 1;
        while (tab[x][y] < input) {
            if (turn == offset) {
                turn = 0;
                if (second) {
                    offset++;
                    second = false;
                } else {
                    second = true;
                }
                int tmp = turnx;
                turnx = -turny;
                turny = tmp;
            } else {
                turn++;
            }
            x += turnx;
            y += turny;
            tab[x][y] = around(tab, x, y);
            std::cout << x-500 << ", " << y-500 << ", " << tab[x][y] << std::endl;
        }
        ans = tab[x][y];
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}