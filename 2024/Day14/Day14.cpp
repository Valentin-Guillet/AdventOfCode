
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Robot {
  int pos_x, pos_y;
  int vel_x, vel_y;

  Robot(const std::string &input, const int width, const int height) {
    size_t beg_ind = input.find('=');
    size_t end_ind = input.find(',');
    pos_x = std::stoi(input.substr(beg_ind + 1, end_ind - beg_ind));

    beg_ind = end_ind;
    end_ind = input.find(' ');
    pos_y = std::stoi(input.substr(beg_ind + 1, end_ind - beg_ind));

    beg_ind = input.find('=', end_ind);
    end_ind = input.find(',', beg_ind);
    vel_x = std::stoi(input.substr(beg_ind + 1, end_ind - beg_ind));

    beg_ind = end_ind;
    vel_y = std::stoi(input.substr(beg_ind + 1));

    vel_x = (vel_x + height) % height;
    vel_y = (vel_y + width) % width;
  }

  void update(const int width, const int height, int coef = 1) {
    pos_x = (pos_x + coef * vel_x) % height;
    pos_y = (pos_y + coef * vel_y) % width;
  }
};

int compute_safety_factor(const int width, const int height, const std::vector<Robot> &robots) {
  int quadrants_count[4] = {};

  for (const Robot &robot : robots) {
    if (robot.pos_x == height / 2 || robot.pos_y == width / 2)
      continue;

    int index = (robot.pos_x > height / 2 ? 1 : 0);
    if (robot.pos_y > width / 2)
      index += 2;
    quadrants_count[index]++;
  }

  return quadrants_count[0] * quadrants_count[1] * quadrants_count[2] * quadrants_count[3];
}

int main(int argc, char *argv[]) {
  bool first_half = true;
  int ans = 0;

  std::ifstream myfile("input");
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);
    if (arg == "-t" || arg == "--test")
      myfile = std::ifstream("test_input");

    else if (arg == "-s" || arg == "--second")
      first_half = false;
  }

  const int width = 103;
  const int height = 101;

  std::vector<Robot> robots;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      robots.push_back(Robot(input, width, height));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    for (Robot &robot : robots)
      robot.update(width, height, 100);
    ans = compute_safety_factor(width, height, robots);

  } else {
    // Robot coordinates are necessarly periodic with period at most = width or height
    int min_var_x = 100000000;
    int min_var_y = 100000000;
    int period_x;
    int period_y;

    for (int step = 0; step < width; ++step) {
      float sum_of_squares_x = 0;
      float sum_of_squares_y = 0;
      float average_x = 0;
      float average_y = 0;
      for (Robot &robot : robots) {
        robot.update(width, height);
        sum_of_squares_x += robot.pos_x * robot.pos_x;
        sum_of_squares_y += robot.pos_y * robot.pos_y;
        average_x += robot.pos_x;
        average_y += robot.pos_y;
      }

      float var_x = sum_of_squares_x - average_x * average_x / robots.size();
      float var_y = sum_of_squares_y - average_y * average_y / robots.size();

      if (var_x < min_var_x) {
        min_var_x = var_x;
        period_x = step;
      }

      if (var_y < min_var_y) {
        min_var_y = var_y;
        period_y = step;
      }
    }

    // The step on which the robots form the Christmas tree must be such that:
    // { step = period_x modulo width
    // { step = period_y modulo height
    // We use the Chinese remainder theorem: we find a and b such that:
    // a * 101 + b * 103 = 1  =>  a = 51, b = -50
    // So step = -50 * 103 * period_x + 51 * 101 * period_y module (width * height)
    ans = (51 * 101 * period_y - 50 * 103 * period_x) % (101 * 103);

    // Start at second 1
    ans++;
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
