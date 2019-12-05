
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>

long long ppcm(long x, long y)
{
    long long a = x, b = y;
    while (b != 0) {
        long long c = a % b;
        a = b;
        b = c;
    }
    long long z = (long long) x * (long long) y;
    return z / a;
}

struct Submoon
{
    int pos, vel = 0;
    void apply_velocity() { pos += vel; }

    static void apply_gravity(Submoon &a, Submoon &b) {
        if (a.pos < b.pos) {
            a.vel++;
            b.vel--;
        } else if (a.pos > b.pos) {
            a.vel--;
            b.vel++;
        }
    }
};

class Moon
{
    public:
        Submoon sub[3];
        void set_pos(int x, int y, int z) {
            sub[0].pos = x;
            sub[1].pos = y;
            sub[2].pos = z;
        }

        void print() {
            std::cout << "<x=" << sub[0].pos << ", y=" << sub[1].pos << ", z=" << sub[2].pos;
            std::cout << " / vx=" << sub[0].vel << ", vy=" << sub[1].vel << ", vz=" << sub[2].vel << ">" << std::endl;
        }

        void apply_velocity(int i) {
            sub[i].apply_velocity();
        }

        int get_energy() {
            int potential_energy = abs(sub[0].pos) + abs(sub[1].pos) + abs(sub[2].pos);
            int kinetic_energy = abs(sub[0].vel) + abs(sub[1].vel) + abs(sub[2].vel);
            return potential_energy * kinetic_energy;
        }

        static void apply_gravity(Moon &a, Moon &b, int i) {
            Submoon::apply_gravity(a.sub[i], b.sub[i]);
        }

        static void step(Moon moons[4], int i) {
            for (int j=0; j<4; ++j) {
                for (int k=j+1; k<4; ++k) {
                    Moon::apply_gravity(moons[j], moons[k], i);
                }
            }
            for (int j=0; j<4; ++j)
                moons[j].apply_velocity(i);
        }

        static std::string get_state(Moon moons[4], int i) {
            std::string state;
            for (int j=0; j<4; ++j) {
                state += std::to_string(moons[j].sub[i].pos) + ',';
                state += std::to_string(moons[j].sub[i].vel);
                if (j<3)
                    state += '/';
            }
            return state;
        }
};

int get_period(Moon moons[4], int i)
{
    std::unordered_set<std::string> seen;
    std::string state = Moon::get_state(moons, i);
    int count = 0;
    while (seen.find(state) == seen.end()) {
        seen.insert(state);
        Moon::step(moons, i);
        state = Moon::get_state(moons, i);
        count++;
    }
    return count;
}


int main(void)
{
    bool firstHalf = false;
	std::string input;
	long long ans = 0;
	Moon moons[4];

	std::ifstream myfile ("Day12Input");
	if (myfile.is_open())
	{
        std::regex rgx("<x=(-?[0-9]+), y=(-?[0-9]+), z=(-?[0-9]+)>");
        std::smatch match;
        int i = 0;
		while (getline(myfile, input)) {
            if (!std::regex_match(input, match, rgx)) {
                std::cout << "Unable to parse input" << std::endl;
                return -1;
            }
            int x = std::stoi(match[1].str());
            int y = std::stoi(match[2].str());
            int z = std::stoi(match[3].str());
            moons[i].set_pos(x, y, z);
            i++;
        }
		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (firstHalf)
    {
        for (int count=0; count<1000; ++count) {
            for (int i=0; i<4; ++i) {
                for (int j=i+1; j<4; ++j) {
                    for (int k=0; k<3; ++k)
                        Moon::apply_gravity(moons[i], moons[j], k);
                }
            }
            for (int i=0; i<4; ++i) {
                for (int k=0; k<3; ++k)
                    moons[i].apply_velocity(k);
            }
        }
        for (Moon moon : moons)
            ans += (long long) moon.get_energy();

    } else {
        long periodx = (long) get_period(moons, 0);
        long periody = (long) get_period(moons, 1);
        long periodz = (long) get_period(moons, 2);

        ans = ppcm(periodx, ppcm(periody, periodz));
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
