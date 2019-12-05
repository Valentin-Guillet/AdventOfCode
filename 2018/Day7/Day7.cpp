
#include "iostream"
#include "fstream"
#include <string>
#include <vector>
#include <unordered_map>

struct node
{
	std::string name;
	std::string root = "";
	int weight = 0;
	int true_weight = -1;
	std::vector<std::string> children;
	int get_weight(std::unordered_map<std::string, node>* nodes) {
		if (true_weight != -1) {
			return true_weight;
		}
		int sum_weight = weight;
		for (std::string child : children) {
			sum_weight += (*nodes)[child].get_weight(nodes);
		}
		true_weight = sum_weight;
		return sum_weight;
	}
};

int main(void)
{
	std::string input;
	int ans = 0;

	std::ifstream myfile ("Day7Input");
	if (myfile.is_open())
	{
		std::unordered_map<std::string, node> nodes;
		std::vector<std::string> children_list;
		std::string name, children, child_name;
		node child_node;
		int weight;
		while (getline(myfile, input)) {
			int pos = input.find(" ");
			name = input.substr(0, pos);
			weight = std::stoi(input.substr(pos+2, input.find(")") - pos - 2));
			pos = input.find(">");
			children_list.clear();
			if (pos != std::string::npos) {
				children = input.substr(pos+2, std::string::npos);
				while ((pos = children.find(", ")) != std::string::npos) {
					child_name = children.substr(0, pos);
					if (nodes.find(child_name) == nodes.end()) {
						node child_node;
						child_node.root = name;
						nodes[child_name] = child_node;
					} else {
						nodes[child_name].root = name;
					}
					// std::cout << nodes[child_name].root << std::endl;
					children_list.push_back(child_name);
					children.erase(0, pos+2);
				}
				child_name = children.substr(0, pos);
				if (nodes.find(child_name) == nodes.end()) {
					node child_node;
					child_node.root = name;
					nodes[child_name] = child_node;
				} else {
					nodes[child_name].root = name;
				}
				children_list.push_back(child_name);
				// std::cout << nodes[child_name].root << std::endl;
			}
			if (nodes.find(name) == nodes.end()) {
				node new_node;
				new_node.children = children_list;
				new_node.weight = weight;
				new_node.name = name;
				nodes[name] = new_node;
			} else {
				nodes[name].children = children_list;
				nodes[name].weight = weight;
				nodes[name].name = name;
			}
		}
		std::string name_root;
		for (auto it : nodes) {
			if (it.second.root == "") {
				std::cout << it.first << ", weight : " << it.second.get_weight(&nodes) << std::endl;
				name_root = it.first;
			}
		}
		std::vector<node> errors;
		errors.push_back(nodes[name_root]);
		node error;

		while (errors.size() > 0) {
			error = errors.back();
			errors.pop_back();

			std::cout << error.name << " : " << error.get_weight(&nodes) << std::endl;

			std::unordered_map<int, int> weights;
			for (auto child : error.children) {
				int w = nodes[child].get_weight(&nodes);
				if (weights.find(w) == weights.end()) {
					weights[w] = 0;
				}
				weights[w] += 1;
			}
			int min = 10000;
			for (std::pair<int, int> w : weights) {
				if (w.second < min) {
					min = w.second;
				}
				std::cout << "    " << w.first << ", " << w.second << std::endl;
			}
			if (weights.size() > 1) {
				for (auto child : error.children) {
					if (weights[nodes[child].get_weight(&nodes)] == min) {
						errors.push_back(nodes[child]);
					}
				}
			}
		}
		
		myfile.close();
	}

	else std::cout << "Unable to open file"; 


	return 0;
}