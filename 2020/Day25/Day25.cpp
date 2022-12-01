
#include <iostream>
#include <fstream>


int main()
{
	long ans = 1;
    long card_public_key, door_public_key;

    std::ifstream myfile ("input");
	if (myfile.is_open()) {
        std::string input;

		getline(myfile, input);
        card_public_key = std::stol(input);

		getline(myfile, input);
        door_public_key = std::stol(input);

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int card_loop_size = 0;
    long value = 1;
    while (value != card_public_key) {
        value = (value * 7) % 20201227;
        card_loop_size++;
    }

    for (int i=0; i<card_loop_size; ++i)
        ans = (ans * door_public_key) % 20201227;

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
