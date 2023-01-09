

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


struct Node {
    int value;
    int scaled_value;
    Node* prev = nullptr;
    Node* next = nullptr;
};

void delete_list(std::vector<Node*> node_list) {
    Node* node = node_list[0];
    node->prev->next = nullptr;

    while (node) {
        Node* next_node = node->next;
        delete node;
        node = next_node;
    }
    node_list.clear();
}

void move(int size, Node* node) {
    if (node->scaled_value == 0)
        return;

    int value = node->scaled_value;
    node->prev->next = node->next;
    node->next->prev = node->prev;

    Node* insert_point = node->prev;
    if (value <= size / 2) {
        for (int i=0; i<value; i++)
            insert_point = insert_point->next;
    } else {
        for (int i=0; i<(size - value - 1); i++)
            insert_point = insert_point->prev;
    }

    node->next = insert_point->next;
    node->prev = insert_point;
    insert_point->next = node;
    node->next->prev = node;
}

int main(int argc, char* argv[])
{
    bool first_half = true;
	long ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Node*> node_list;
	if (myfile.is_open()) {
        std::string input;

        getline(myfile, input);
        Node* root = new Node();
        root->value = std::stoi(input);
        node_list.push_back(root);

        Node* curr_node = root;
        while (getline(myfile, input)) {
            Node* new_node = new Node();
            new_node->value = std::stoi(input);
            new_node->prev = curr_node;
            curr_node->next = new_node;

            node_list.push_back(new_node);
            curr_node = new_node;
        }
        root->prev = curr_node;
        curr_node->next = root;

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int size = node_list.size();
    int decryption_key = (first_half ? 1 : 811589153);
    for (Node* node : node_list) {
        int positive_value = (node->value + 2 * (size - 1)) % (size - 1);
        node->scaled_value = (positive_value * (long)decryption_key) % (size - 1);
    }

    int nb_rounds = (first_half ? 1 : 10);
    for (int round=0; round<nb_rounds; round++) {
        for (Node* node : node_list)
            move(node_list.size(), node);
    }

    Node* answer_node = node_list[0];
    while (answer_node->value != 0)
        answer_node = answer_node->next;

    int nb_steps = 1000 % node_list.size();
    for (int i=0; i<3; i++) {
        for (int j=0; j<nb_steps; j++)
            answer_node = answer_node->next;
        ans += answer_node->value;
    }
    ans *= decryption_key;

    delete_list(node_list);

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
