
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int get_combo(int operand, int &reg_A, int &reg_B, int &reg_C) {
  if (operand < 4) return operand;
  if (operand == 4) return reg_A;
  if (operand == 5) return reg_B;
  if (operand == 6) return reg_C;

  std::cerr << "ERROR! Invalid combo value" << std::endl;
  return -1;
}

int divide_A(int operand, int &reg_A, int &reg_B, int &reg_C) {
  int combo = get_combo(operand, reg_A, reg_B, reg_C);
  int power = 1;
  for (int i = 0; i < combo; ++i) power <<= 1;
  return reg_A / power;
}

void run(std::vector<int> &program, int &reg_A, int &reg_B, int &reg_C) {
  int instruction_pointer = 0;
  std::vector<int> output;

  while (instruction_pointer < (int)program.size()) {
    int opcode = program[instruction_pointer++];
    int operand = program[instruction_pointer++];

    if (opcode == 0) { // adv
      reg_A = divide_A(operand, reg_A, reg_B, reg_C);

    } else if (opcode == 1) { // bxl
      reg_B = (reg_B ^ operand) % 8;

    } else if (opcode == 2) { // bst
      reg_B = get_combo(operand, reg_A, reg_B, reg_C) % 8;

    } else if (opcode == 3) { // jnz
      if (reg_A != 0)
        instruction_pointer = operand;

    } else if (opcode == 4) { // bxc
      reg_B ^= reg_C;

    } else if (opcode == 5) { // out
      output.push_back(get_combo(operand, reg_A, reg_B, reg_C) % 8);

    } else if (opcode == 6) { // bdv
      reg_B = divide_A(operand, reg_A, reg_B, reg_C);

    } else if (opcode == 7) { // cdv
      reg_C = divide_A(operand, reg_A, reg_B, reg_C);
    }
  }

  if (output.empty())
    return;

  std::cout << output[0];
  for (size_t i = 1; i < output.size(); ++i)
    std::cout << ',' << output[i];
  std::cout << std::endl;
}

long compute_quine(std::vector<int> &program, int index = -1, long reg_A = 0) {
  if (index == -1)
    index = program.size() - 1;

  // Cf. notes.txt
  int b = program[index];
  std::vector<int> possibilities;
  for (int x = 0; x < 8; ++x) {
    if (index == (int)program.size() - 1 && x == 0)
      continue;

    int n = x ^ 3;

    // If n == 0 (i.e. x == 3), the program will necessarily return 0, so
    // only works is b == 0
    if (n == 0) {
      if (b == 0)
        possibilities.push_back(x);
      continue;
    }

    int a0, a1, a2;
    a2 = ((b >> 2) & 1) ^ ((reg_A >> (n - 1)) & 1);

    if      (n >= 2) a1 = ((b >> 1) & 1) ^ ((reg_A >> (n - 2)) & 1);
    else if (n == 1) a1 = ((b >> 1) & 1) ^ a2;

    if      (n >= 3) a0 = ((b >> 0) & 1) ^ ((reg_A >> (n - 3)) & 1);
    else if (n == 2) a0 = ((b >> 0) & 1) ^ a2;
    else if (n == 1) a0 = ((b >> 0) & 1) ^ a1;

    if (a2 == ((x >> 2) & 1) && a1 == ((x >> 1) & 1) && a0 == (x & 1))
      possibilities.push_back(x);
  }

  if (possibilities.empty())
    return -1;

  // Multiple values work for the last instruction: only return the lowest number
  if (index == 0)
    return (reg_A << 3) + possibilities[0];

  for (int value : possibilities) {
    long reg_A_rec = compute_quine(program, index - 1, (reg_A << 3) + value);
    if (reg_A_rec != -1)
      return reg_A_rec;
  }
  return -1;
}

int main(int argc, char *argv[]) {
  bool first_half = true;
  long ans = 0;

  std::ifstream myfile("input");
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);
    if (arg == "-t" || arg == "--test")
      myfile = std::ifstream("test_input");

    else if (arg == "-s" || arg == "--second")
      first_half = false;
  }

  int reg_A, reg_B, reg_C;
  std::vector<int> program;
  if (myfile.is_open()) {
    std::string input;

    getline(myfile, input);
    reg_A = std::stoi(input.substr(12));

    getline(myfile, input);
    reg_B = std::stoi(input.substr(12));

    getline(myfile, input);
    reg_C = std::stoi(input.substr(12));

    getline(myfile, input);
    getline(myfile, input);
    size_t index = 9;
    while (index < input.size()) {
      program.push_back(input[index] - '0');
      index += 2;
    }

    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    run(program, reg_A, reg_B, reg_C);
  } else {
    ans = compute_quine(program);
    std::cout << "Answer : " << ans << std::endl;
  }

  return 0;
}
