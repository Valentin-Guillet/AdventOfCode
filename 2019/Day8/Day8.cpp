
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const int HEIGHT = 6;
const int WIDTH = 25;

void disp_image(int image[][HEIGHT][WIDTH], int nb_layers)
{
    for (int l=0; l<nb_layers; ++l) {
        std::cout << "Layer " << l << std::endl;
        for (int h=0; h<HEIGHT; ++h) {
            std::cout << "\t";
            for (int w=0; w<WIDTH; ++w) {
                std::cout << image[l][h][w];
            }
            std::cout << std::endl;
        }
    }
}

int count(int layer[HEIGHT][WIDTH], int elt)
{
    int res = 0;
    for (int h=0; h<HEIGHT; ++h) {
        for (int w=0; w<WIDTH; ++w) {
            if (layer[h][w] == elt)
                res++;
        }
    }
    return res;
}

int get_pixel(int image[][HEIGHT][WIDTH], int nb_layers, int h, int w)
{
    for (int i=0; i<nb_layers; ++i) {
        if (image[i][h][w] != 2)
            return image[i][h][w];
    }
    return 0;
}


int main(void)
{
    bool firstHalf = false;
	std::string input;
	int ans = 0;
	std::vector<int> tab;

	std::ifstream myfile ("Day8Input");
	if (myfile.is_open())
	{
		getline(myfile, input);
		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    int nb_layers = input.size() / WIDTH / HEIGHT;
    int image[nb_layers][HEIGHT][WIDTH];
    int w, h, l;

    for (unsigned int i=0; i<input.size(); ++i) {
        l = i * nb_layers / input.size();
        h = (i % (HEIGHT*WIDTH)) / WIDTH;
        w = (i % WIDTH);
        image[l][h][w] = input[i] - '0';
    }

    int index, min = input.size();
    for (int i=0; i<nb_layers; ++i) {
        if (count(image[i], 0) < min) {
            min = count(image[i], 0);
            index = i;
        }
    }

    // disp_image(image, nb_layers);

    if (firstHalf) {
        ans = count(image[index], 1) * count(image[index], 2);

        std::cout << "Answer : " << ans << std::endl;
    } else {
        for (int h=0; h<HEIGHT; ++h) {
            for (int w=0; w<WIDTH; ++w) {
                if (get_pixel(image, nb_layers, h, w)) {
                    std::cout << '#';
                } else {
                    std::cout << ' ';
                }
            }
            std::cout << std::endl;
        }
    }

	return 0;
}
