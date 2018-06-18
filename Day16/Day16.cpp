
#include "iostream"
#include "fstream"
#include <string>
#include <algorithm>
#include <iterator>

void disp(char tab[16]) {
	for (int i = 0; i < 16; ++i)
		std::cout << tab[i];
	std::cout << std::endl;
}

void swap(char *tab, int n) {
	char tmp[16];
	for (int i = 0; i < 16; ++i)
		tmp[(i+n)%16] = tab[i];
	for (int i = 0; i < 16; ++i)
		tab[i] = tmp[i];
}

void exchange(char *tab, int a, int b) {
	char tmp = tab[a];
	tab[a] = tab[b];
	tab[b] = tmp;
}

void partner(char *tab, char a, char b) {
	int pos_a = std::find(tab, tab+16, a) - tab;
	int pos_b = std::find(tab, tab+16, b) - tab;
	char tmp = tab[pos_a];
	tab[pos_a] = tab[pos_b];
	tab[pos_b] = tmp;
}

char* dance(char *tab, std::vector<std::string> dance_instr) {
	for (std::string instr : dance_instr) {
		int pos;
		if (instr[0] == 's')
			swap(tab, std::stoi(instr.substr(1)));
		else if (instr[0] == 'x') {
			pos = instr.find("/");
			int a = std::stoi(instr.substr(1, pos));
			int b = std::stoi(instr.substr(pos+1));
			exchange(tab, a, b);
		} else {
			pos = instr.find("/");
			char a = instr[1], b = instr[3];
			partner(tab, a, b);
		}
	}
	return tab;
}

int main(void)
{
	std::string input;
	char letters[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
	char tab[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};

	std::ifstream myfile ("Day16Input");
	if (myfile.is_open())
	{
		getline(myfile, input);
		myfile.close();
	}
	else std::cout << "Unable to open file";

	std::vector<std::string> dance_instr;
	input += ',';
	int pos = 0;
	while ((pos = input.find(",")) != std::string::npos) {
		dance_instr.push_back(input.substr(0, pos));
		input.erase(0, pos+1);
	}

	for (int i = 0; i < 1000000000%24; ++i)
	{
		dance(tab, dance_instr);
		disp(tab);
	}

	return 0;
}