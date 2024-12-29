
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Gate {
  std::string arg1, op, arg2;

  inline bool has(const std::string &wire) const {
    return (arg1 == wire || arg2 == wire);
  }
};

using Gates = std::unordered_map<std::string, Gate>;

inline std::string wire_name(char letter, int bit) {
  return std::string(1, letter) + (bit < 10 ? "0" : "") + std::to_string(bit);
}

bool get_value(const Gates &gates, const std::string &wire, long x, long y) {
  if (wire[0] == 'x') {
    int index = std::stoi(wire.substr(1));
    return (x >> index) & 1;
  } else if (wire[0] == 'y') {
    int index = std::stoi(wire.substr(1));
    return (y >> index) & 1;
  }

  const Gate &gate = gates.at(wire);
  if (gate.op == "AND")
    return (get_value(gates, gate.arg1, x, y) && get_value(gates, gate.arg2, x, y));
  if (gate.op == "OR")
    return (get_value(gates, gate.arg1, x, y) || get_value(gates, gate.arg2, x, y));

  return (get_value(gates, gate.arg1, x, y) != get_value(gates, gate.arg2, x, y));
}

long add_values(const Gates &gates, long x, long y, int nb_bits) {
  long sum = 0;
  int z_power = 0;
  for (int bit = 0; bit < nb_bits; ++bit) {
    long value = get_value(gates, wire_name('z', bit), x, y);
    sum |= (value << z_power);
    z_power++;
  }
  return sum;
}

std::string get_gate(const Gates &gates, std::string op,
                     const std::string &wire1, const std::string &wire2) {
  for (const auto &[wire, gate] : gates) {
    if (gate.op == op && (gate.has(wire1) || gate.has(wire2)))
      return wire;
  }
  return "";
}

void fix_gate_args(Gates &gates, const std::string &gate_wire,
                   const std::string &wire1, const std::string &wire2,
                   std::vector<std::string> &swapped_wires) {
  if (gate_wire.empty())
    return;

  const Gate &gate = gates.at(gate_wire);
  if (gate.has(wire1) && gate.has(wire2))
    return;

  std::string invalid_wire = (gate.has(wire1) ? wire2 : wire1);
  std::string valid_wire = (gate.has(wire1) ? wire1 : wire2);
  std::string other_wire = (gate.arg1 == valid_wire ? gate.arg2 : gate.arg1);

  const Gate &tmp = gates[invalid_wire];
  gates[invalid_wire] = gates[other_wire];
  gates[other_wire] = tmp;
  swapped_wires.push_back(invalid_wire);
  swapped_wires.push_back(other_wire);
}

void fix_swapped_pair(Gates &gates, int bit, Gate &prev_zgate,
                      std::vector<std::string> &swapped_wires) {
  // Assume only one swap per bit computation
  std::string prev_x_wire = wire_name('x', bit - 1);
  std::string x_wire = wire_name('x', bit);

  std::string prev_and_wire, x_and_wire, x_xor_wire;
  for (const auto &[wire, gate] : gates) {
    if (gate.op == "AND") {
      if (gate.has(prev_zgate.arg1))
        prev_and_wire = wire;
      else if (gate.has(prev_x_wire))
        x_and_wire = wire;
    } else if (gate.op == "XOR" && gate.has(x_wire)) {
      x_xor_wire = wire;
    }
  }

  // Check that both AND are both sides of a OR gate
  std::string or_wire = get_gate(gates, "OR", prev_and_wire, x_and_wire);
  fix_gate_args(gates, or_wire, prev_and_wire, x_and_wire, swapped_wires);

  // Check that OR and XOR are both sides of a XOR gate
  std::string xor_wire = get_gate(gates, "XOR", or_wire, x_xor_wire);
  fix_gate_args(gates, xor_wire, or_wire, x_xor_wire, swapped_wires);

  // Check that XOR gate outputs the next z bit
  if (xor_wire[0] != 'z') {
    swapped_wires.push_back(xor_wire);
    swapped_wires.push_back(wire_name('z', bit));
  }
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

  Gates gates;
  int nb_bits = 0;
  long x_init(0), y_init(0);
  if (myfile.is_open()) {
    std::string input;
    int x_power(0), y_power(0);
    while (getline(myfile, input)) {
      if (input.empty())
        break;

      if (input[0] == 'x') {
        if (input[5] == '1')
          x_init |= (1L << x_power);
        x_power++;
      } else {
        if (input[5] == '1')
          y_init |= (1L << y_power);
        y_power++;
      }
    }

    while (getline(myfile, input)) {
      Gate gate;
      size_t index = input.find(' ', 5);
      gate.arg1 = input.substr(0, 3);
      gate.op = input.substr(4, index - 4);
      gate.arg2 = input.substr(index + 1, 3);

      index = input.find('>');
      if (input[index + 2] == 'z')
        nb_bits++;
      gates[input.substr(index + 2)] = gate;
    }

    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  if (first_half) {
    ans = add_values(gates, x_init, y_init, nb_bits);
  } else {
    std::vector<std::string> swapped_wires;
    for (int i = 0; i < nb_bits - 1; ++i) {
      long sum = add_values(gates, 0, (1L << i), nb_bits);
      if (sum != (1L << i)) {
        Gate &prev_zgate = gates[wire_name('z', i - 1)];
        fix_swapped_pair(gates, i, prev_zgate, swapped_wires);
      }
    }

    std::sort(swapped_wires.begin(), swapped_wires.end());
    std::cout << swapped_wires[0];
    for (size_t i = 1; i < swapped_wires.size(); ++i)
      std::cout << ',' << swapped_wires[i];
    std::cout << '\n';
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
