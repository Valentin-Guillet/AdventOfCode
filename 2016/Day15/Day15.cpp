/* This problem is almost exactly an application of the Chinese Remainder Theorem */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


struct Disc
{
    int n;
    int a_i;

    Disc(int level, int nb_pos, int init_pos) {
        n = nb_pos;
        a_i = -1 * (init_pos + level);
        while (a_i < 0)
            a_i += n;
    }

};

pair<int, int> euclide(int a, int b) {
    int r = a, r2 = b, u = 1, v = 0, u2 = 0, v2 = 1;

    while (r2 != 0) {
        int q = r / r2;
        int rs = r, us = u, vs = v;
        r = r2; u = u2; v = v2;
        r2 = rs - q*r2; u2 = us - q*u2; v2 = vs - q*v2;
    }

    return {u, v};
}

int main()
{
    bool first_half = false;
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

    if (!first_half)
        tab.push_back("Disc #7 has 11 positions; at time=0, it is at position 0.");

    vector<Disc> discs;
    int n_tot = 1;
    for (size_t i=0; i<tab.size(); ++i) {
        string line = tab[i];

        int id1 = line.find("has ") + 4;
        int id2 = line.find(" positions;");
        int nb_pos = stoi(line.substr(id1, id2-id1));

        id1 = line.find("position ") + 9;
        id2 = line.find(".");
        int init_pos = stoi(line.substr(id1, id2-id1));

        Disc new_disc(i+1, nb_pos, init_pos);
        discs.push_back(new_disc);

        n_tot *= nb_pos;
    }

    for (Disc disc : discs) {
        int n_i = n_tot / disc.n;
        auto [u_i, v_i] = euclide(disc.n, n_i);
        int e_i = v_i * n_i;

        ans += disc.a_i * e_i;
    }

    while (ans < 0) {
        ans += n_tot;
    }

	cout << "Answer : " << ans << endl;
	return 0;
}
