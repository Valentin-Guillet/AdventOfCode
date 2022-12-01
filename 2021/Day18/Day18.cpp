
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class Node {
    public:
        Node(int value, int depth) : value(value), depth(depth) { }
        Node(std::string input, int node_depth=0) {
            size_t index = 0;
            while (input[index] == '[' || input[index] == ']') {
                if (input[index] == '[')
                    node_depth++;
                else if (input[index] == ']')
                    node_depth--;
                index++;
            }

            value = (input[index] == ',' ? -1 : (input[index] - '0'));
            depth = node_depth;
            bool end = true;
            for (char c : input.substr(index+1)) {
                if (c != ']') {
                    end = false;
                    break;
                }
            }
            if (!end) {
                next = new Node(input.substr(index+1), node_depth);
                next->prev = this;
            }
        }

        ~Node() {
            delete next;
        }

        int compute_magnitude(int target_depth=0) {
            if (depth == target_depth && value != -1)
                return value;

            int left_mag = compute_magnitude(target_depth+1);
            Node* comma = find_next_comma(target_depth+1);
            int right_mag = comma->next->compute_magnitude(target_depth+1);

            return 3*left_mag + 2*right_mag;
        }

        void add(Node* right) {
            increase_depth();
            right->increase_depth();
            Node* new_comma = new Node(-1, 1);
            new_comma->next = right;
            right->prev = new_comma;
            set_end(new_comma);

            bool reduced = false;
            while (!reduced) {
                while (explode()) { }
                reduced = !split();
            }
        }

        void debug(int curr_depth=0) {
            while (depth > curr_depth) {
                std::cout << "[";
                curr_depth++;
            }
            while (depth < curr_depth) {
                std::cout << "]";
                curr_depth--;
            }
            if (value == -1)
                std::cout << ",";
            else
                std::cout << value;
            if (next) {
                next->debug(curr_depth);
            } else {
                for (int i=0; i<curr_depth; i++)
                    std::cout << ']';
                std::cout << std::endl;
            }
        }

    /* private: */
        int value;
        int depth;

        Node* prev = nullptr;
        Node* next = nullptr;

        Node* find_next_comma(int target_depth) {
            if (depth == target_depth && value == -1)
                return this;
            return next->find_next_comma(target_depth);
        }

        void increase_depth() {
            depth++;
            if (next)
                next->increase_depth();
        }

        void set_end(Node* right) {
            if (next) {
                next->set_end(right);
            } else {
                next = right;
                right->prev = this;
            }
        }

        bool explode() {
            if (depth < 5) {
                if (!next)
                    return false;
                else
                    return next->explode();
            }

            if (prev)
                prev->prev->value += value;
            Node* comma = next;
            Node* right = comma->next;
            if (right->next) {
                right->next->prev = this;

                // Update next value
                right->next->next->value += right->value;
            }
            value = 0;
            depth--;

            next = right->next;
            right->next = nullptr;
            delete comma;

            return true;
        }

        bool split() {
            if (value < 10) {
                if (!next)
                    return false;
                else
                    return next->split();
            }

            int left_value = value / 2;
            int right_value = value / 2 + (value % 2 == 1);

            value = left_value;
            depth++;

            Node* new_comma = new Node(-1, depth);
            Node* new_right = new Node(right_value, depth);

            new_comma->prev = this;
            new_comma->next = new_right;
            new_right->prev = new_comma;
            new_right->next = next;
            if (next)
                next->prev = new_right;
            next = new_comma;

            return true;
        }
};

int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;
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
        Node* root = new Node(lines[0]);

        for (size_t i=1; i<lines.size(); i++) {
            Node* new_node = new Node(lines[i]);
            root->add(new_node);
        }

        ans = root->compute_magnitude();
        delete root;

    } else {
        for (size_t i=0; i<lines.size(); i++) {
            for (size_t j=0; j<lines.size(); j++) {
                if (i == j)
                    continue;

                Node* node = new Node(lines[i]);
                node->add(new Node(lines[j]));
                int mag = node->compute_magnitude();
                if (mag > ans)
                    ans = mag;

                delete node;
            }
        }
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
