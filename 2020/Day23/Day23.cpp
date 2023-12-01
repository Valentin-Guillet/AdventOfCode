
#include <iostream>
#include <vector>


struct Node {
    int value;
    Node *next;
};

int main()
{
    bool first_half = false;
    long ans = 0;
    size_t N = (first_half ? 9 : 1000000);
    size_t NB_STEPS = (first_half ? 100 : 10000000);
    std::vector<int> input = {4, 7, 6, 1, 3, 8, 2, 5, 9};

    std::vector<Node*> nodes;
    for (size_t i=0; i<N; ++i)
        nodes.push_back(nullptr);

    Node *last_node;
    for (size_t i=0; i<N; ++i) {
        int index = (i < input.size() ? input[i] : i+1);
        Node *new_node = new Node{index, nullptr};
        if (i > 0)
            last_node->next = new_node;
        nodes[index-1] = new_node;
        last_node = new_node;
    }
    nodes[last_node->value-1]->next = nodes[input[0]-1];

    Node *curr_node = nodes[input[0]-1];
    for (size_t step=0; step<NB_STEPS; ++step) {
        Node *moved_node = curr_node->next;
        curr_node->next = moved_node->next->next->next;

        int dest_label = curr_node->value;
        do {
            dest_label = (dest_label + N-2) % N + 1;
        } while (dest_label == moved_node->value || dest_label == moved_node->next->value || dest_label == moved_node->next->next->value);

        Node *dest_node = nodes[dest_label-1];
        Node *next_node = dest_node->next;

        dest_node->next = moved_node;
        moved_node->next->next->next = next_node;

        curr_node = curr_node->next;
    }

    if (first_half) {
        for (Node *node=nodes[0]->next; node!=nodes[0]; node=node->next)
            ans = 10*ans + node->value;

    } else {
        ans = (long) nodes[0]->next->value * nodes[0]->next->next->value;
    }

    for (Node *node : nodes)
        delete node;

    std::cout << "Answer: " << ans << std::endl;
    return 0;
}
