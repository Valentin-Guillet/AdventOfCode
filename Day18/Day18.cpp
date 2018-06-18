
#include "iostream"
#include "fstream"
#include <string>
#include <regex>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cctype>


class Thread
{
public:
	Thread(int i) {
		pos = 0;
		stopped = false;
		registers['p'] = i;
		snd = 0;
		id = i;
	}

	int get(std::string s) {
		if (std::isalpha(s[0]))
			return registers[s[0]];
		else
			return std::stoi(s);
	}

	void act(std::array<std::string, 3> s) {
		// std::cout << "Id " << id << ", pos " << pos << std::endl;
		// std::cout << s[0] << " " << s[1] << " " << s[2] << std::endl;
		// disp(registers);
		// std::cout << std::endl;

		if (stopped && queue.size() == 0)
			return;
		
		stopped = false;

		if (s[0] == "set") {
			registers[s[1][0]] = get(s[2]);

		} else if (s[0] == "add") {
			registers[s[1][0]] += get(s[2]);

		} else if (s[0] == "mul") {
			registers[s[1][0]] *= get(s[2]);

		} else if (s[0] == "mod") {
			registers[s[1][0]] %= get(s[2]);

		} else if (s[0] == "snd") {
			other->add_queue(get(s[1]));
			++snd;

		} else if (s[0] == "rcv") {
			if (queue.size() > 0) {
				registers[s[1][0]] = queue.front();
				queue.pop();
			} else {
				--pos;
				stopped = true;
			}
		}

		if (s[0] == "jgz") {
			if (std::isalpha(s[1][0])) {
				if (registers[s[1][0]] > 0)
					pos += get(s[2]);
				else
					++pos;
			} else {
				if (std::stoi(s[1]) > 0)
					pos += get(s[2]);
				else
					++pos;
			}
		} else {
			++pos;
		}
	}

	void disp(std::unordered_map<char, long long int> registers) {
		for (auto it : registers)
			std::cout << it.first << " : " << it.second << ";  ";
		std::cout << std::endl;
		std::queue<int> q = queue;
		for (int i = 0; i < queue.size(); ++i) {
			std::cout << q.front() << ", ";
			q.pop();
		}
		std::cout << std::endl;
	}

	void add_queue(int elt) {
		queue.push(elt);
	}

	bool is_stopped() {
		return stopped;
	}

	int get_pos() {
		return pos;
	}

	int get_ans() {
		return snd;
	}

	void set_other(Thread *oth) {
		other = oth;
	}

private:
	int pos;
	std::unordered_map<char, long long int> registers;
	bool stopped;
	std::queue<int> queue;
	Thread *other;
	int snd;
	int id;
};


int main(void)
{
	std::string input;
	int ans = 0;
	std::vector<std::array<std::string, 3>> instr;
	Thread t1(0);
	Thread t2(1);

	t1.set_other(&t2);
	t2.set_other(&t1);

	std::regex rgx("([a-z]{3}) ([0-9a-z]) ?(.*)");
	std::smatch sm;

	std::ifstream myfile ("Day18Input");
	if (myfile.is_open())
	{
		while(getline(myfile, input)) {
			std::regex_match(input, sm, rgx);
			std::cout << sm[1] << ", " << sm[2] << ", " << sm[3] << std::endl;
			instr.push_back({sm[1], sm[2], sm[3]});
		}
		myfile.close();
	}
	else std::cout << "Unable to open file";

	while (!t1.is_stopped() || !t2.is_stopped()) {
		t1.act(instr[t1.get_pos()]);
		t2.act(instr[t2.get_pos()]);
	}

	std::cout << "Answer : " << t2.get_ans() << std::endl;

	return 0;
}