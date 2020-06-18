
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

class Chem;
using Link = std::pair<Chem*, unsigned long>;

class Chem
{
    private:
        std::vector<Link> children;
        unsigned long needed = 0;
        unsigned long nb_parents = 0;
        unsigned long processed_links = 0;

    public:
        std::string name;
        unsigned long stochio;
        unsigned long nb_prod = 0;

        Chem(std::pair<std::string, unsigned long> init) : name(init.first), stochio(init.second) {}
        
        void reset() {
            needed = 0;
            processed_links = 0;
        }

        void add_dep(Link l) {
            children.push_back(l);
        }

        void add_parent() {
            nb_parents++;
        }

        unsigned long count(unsigned long get) {
            if (name == "ORE")
                return get;

            needed += get;

            if (++processed_links < nb_parents)
                return 0;

            unsigned long to_get = needed / stochio;
            if (needed % stochio != 0)
                to_get++;

            unsigned long ans = 0;
            for (Link link : children) {
                ans += link.first->count(to_get * link.second);
            }
            return ans;
        }
};

std::pair<std::string, unsigned long> split_stochio(std::string product)
{
    unsigned long split = product.find(' ');
    unsigned long stochio = std::stoi(product.substr(0, split));
    std::string name = product.substr(split+1);
    return {name, stochio};
}

int main(void)
{
    bool first_half = false;
	unsigned long ans = 0;
    std::unordered_map<std::string, Chem*> chems;

	std::ifstream myfile ("Day14Input");
	if (myfile.is_open()) {
        std::string input;

		while (getline(myfile, input))
        {
            unsigned long split = input.find(" => ");
            std::string product = input.substr(split + 4);

            Chem* chem = new Chem(split_stochio(product));
            chems.insert({chem->name, chem});
        }

        Chem* ore = new Chem({"ORE", 1});
        chems.insert({"ORE", ore});

        myfile.clear();
        myfile.seekg(0, std::ios::beg);
        while (getline(myfile, input))
        {
            std::string prod = input.substr(input.rfind(' ')+1);
            int split = input.find(" => ");
            std::string reaction = input.substr(0, split);

            do {
                split = reaction.find(", ");
                std::string reac = reaction.substr(0, split);
                auto [child, stoch] = split_stochio(reac);

                Link link = {chems[child], stoch};
                chems[prod]->add_dep(link);
                chems[child]->add_parent();

                if (split > -1)
                    reaction.erase(0, split+2);

            } while (split != -1);
        }

        myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        ans = chems["FUEL"]->count(1);
    } else {
        unsigned long base = chems["FUEL"]->count(1);
        unsigned long obj = 1000000000000;
        for (auto elt : chems)
            elt.second->reset();
        unsigned long i = obj / base;

        while (ans < obj) {
            ans = chems["FUEL"]->count(++i);
            if (i % 1000000 == 0)
                std::cout << "i = " << i << " ==> ans = " << ans << std::endl;

            for (auto elt : chems)
                elt.second->reset();
        }
        ans = i-1;
    }

    for (std::pair<std::string, Chem*> chem : chems) {
        delete chem.second;
    }

	std::cout << "Answer : " << ans << std::endl;
	return 0;
}
