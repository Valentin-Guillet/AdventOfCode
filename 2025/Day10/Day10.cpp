
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using State = int;
using Matrix = std::vector<std::vector<int>>;

int compute_gcd(int a, int b) {
  if (a < 0)
    a = -a;
  if (b < 0)
    b = -b;
  while (b != 0) {
    int tmp = b;
    b = a % b;
    a = tmp;
  }
  return a;
}

std::vector<int> parse_ints(const std::string &input) {
  std::istringstream input_stream(input);
  std::vector<int> results;
  std::string int_str;
  while (getline(input_stream, int_str, ','))
    results.emplace_back(std::stoi(int_str));
  return results;
}

size_t find_pivot_row(const Matrix &matrix, size_t col, size_t row_start) {
  for (size_t row = row_start; row < matrix.size(); ++row) {
    if (matrix[row][col] != 0)
      return row;
  }
  return -1;
}

void remove_common_factors(Matrix &matrix, size_t row, size_t col) {
  int pivot = matrix[row][col];
  if (pivot < 0) {
    pivot *= -1;
    for (size_t j = col; j < matrix[0].size(); ++j)
      matrix[row][j] *= -1;
  }

  if (row == 0)
    return;

  size_t div_ind = 0;
  while (div_ind < matrix[0].size() && matrix[row - 1][div_ind] == 0)
    div_ind++;
  if (div_ind == matrix[0].size())
    return;
  int divider = matrix[row - 1][div_ind];

  for (size_t i = row + 1; i < matrix.size(); ++i) {
    for (size_t j = col; j < matrix[0].size(); ++j) {
      matrix[i][j] /= divider;
    }
  }
}

void substract_pivot_down(Matrix &matrix, size_t pivot_row, size_t pivot_col) {
  int pivot = matrix[pivot_row][pivot_col];
  for (size_t row = pivot_row + 1; row < matrix.size(); ++row) {
    int row_pivot = matrix[row][pivot_col];
    for (size_t col = pivot_col; col < matrix[0].size(); ++col) {
      matrix[row][col] =
          pivot * matrix[row][col] - row_pivot * matrix[pivot_row][col];
    }
  }
}

void substract_pivot_up(Matrix &matrix, size_t pivot_row, size_t pivot_col) {
  int pivot = matrix[pivot_row][pivot_col];
  for (size_t row = 0; row < pivot_row; ++row) {
    int row_pivot = matrix[row][pivot_col];
    if (row_pivot == 0)
      continue;
    int pivot_gcd = compute_gcd(pivot, row_pivot);
    row_pivot /= pivot_gcd;
    for (size_t col = 0; col < matrix[0].size(); ++col) {
      matrix[row][col] = pivot / pivot_gcd * matrix[row][col] -
                         row_pivot * matrix[pivot_row][col];
    }
  }
}

std::vector<size_t> gauss_elimination(Matrix &matrix) {
  const size_t M = matrix.size();
  const size_t N = matrix[0].size();

  std::vector<size_t> free_indices;

  // Transform matrix in row echelon form
  size_t curr_row = 0;
  for (size_t col = 0; col < N - 1; ++col) {
    size_t row_ind = find_pivot_row(matrix, col, curr_row);

    if (row_ind == (size_t)-1) {
      free_indices.emplace_back(col);
      continue;
    }

    std::swap(matrix[curr_row], matrix[row_ind]);
    substract_pivot_down(matrix, curr_row, col);
    remove_common_factors(matrix, curr_row, col);
    curr_row++;
  }

  // Transform into reduced row echelon form
  for (size_t rev_col = 0; rev_col < N - 1; ++rev_col) {
    size_t col = N - 2 - rev_col;
    if (std::find(free_indices.begin(), free_indices.end(), col) !=
        free_indices.end())
      continue;
    size_t pivot_row = M - 1;
    while (matrix[pivot_row][col] == 0)
      pivot_row--;

    substract_pivot_up(matrix, pivot_row, col);
  }

  return free_indices;
}

class Machine {
public:
  Machine(const std::string &input) {
    size_t index = 1;
    while (input[index] != ']') {
      if (input[index] == '#')
        target_state |= (1 << (index - 1));
      index++;
    }

    index += 2;
    while (input[index] == '(') {
      size_t end_index = input.find(')', index + 1);
      State button_state = 0;
      std::vector<int> button;
      for (int wiring :
           parse_ints(input.substr(index + 1, end_index - index - 1))) {
        button_state |= (1 << wiring);
        button.emplace_back(wiring);
      }
      button_states.emplace_back(button_state);
      buttons.emplace_back(button);
      index = end_index + 2;
    }

    joltages = parse_ints(input.substr(index + 1, input.size() - index - 2));
  }

  int get_fewest_presses_light() const {
    for (size_t nb_press = 1; nb_press < button_states.size() + 1; ++nb_press) {
      if (is_sequence_valid(0, 0, nb_press))
        return nb_press;
    }
    return 0;
  }

  int get_fewest_presses_joltages() const {
    Matrix system = build_system();
    std::vector<size_t> free_indices = gauss_elimination(system);

    // System is not underdetermined -> only one possible answer
    if (free_indices.empty()) {
      int ans = 0;
      for (size_t row = 0; row < system[0].size() - 1; ++row)
        ans += system[row][system[0].size() - 1] / system[row][row];
      return ans;
    }

    std::vector<int> max_values;
    for (size_t row = 0; row < system.size(); ++row)
      max_values.emplace_back(system[row][system[0].size() - 1]);

    return branch_on_free_indices(system, free_indices, 0, max_values, 100000);
  }

private:
  State target_state;
  std::vector<State> button_states;
  std::vector<std::vector<int>> buttons;
  std::vector<int> joltages;

  bool is_sequence_valid(State state, int next_press, int nb_press) const {
    if (nb_press == 0)
      return state == target_state;

    for (size_t i = next_press; i < button_states.size(); ++i) {
      if (is_sequence_valid(state ^ button_states[i], i + 1, nb_press - 1))
        return true;
    }
    return false;
  }

  Matrix build_system() const {
    const size_t M = joltages.size();
    const size_t N = buttons.size();
    Matrix system = std::vector(M, std::vector<int>(N + 1, 0));

    for (size_t i = 0; i < N; ++i) {
      for (size_t ind : buttons[i])
        system[ind][i] = 1;
    }
    for (size_t i = 0; i < M; ++i)
      system[i][N] = joltages[i];

    return system;
  }

  int branch_on_free_indices(const Matrix &system,
                             const std::vector<size_t> &free_indices,
                             size_t index, std::vector<int> &targets,
                             int min_presses) const {
    const size_t M = system.size();

    if (index == free_indices.size())
      return get_total_presses(system, free_indices, targets);

    size_t free_index = free_indices[index];
    auto [min_free_value, max_free_value] =
        get_free_value_limits(system, targets, free_indices, index);

    if (max_free_value < min_free_value)
      return min_presses;

    for (size_t row = 0; row < M; ++row)
      targets[row] -= min_free_value * system[row][free_index];

    int free_value;
    for (free_value = min_free_value; free_value <= max_free_value;
         ++free_value) {
      int nb_presses = free_value;
      if (nb_presses > min_presses)
        break;

      // Branch
      int branch_nb_press = branch_on_free_indices(
          system, free_indices, index + 1, targets, min_presses);

      for (size_t row = 0; row < M; ++row)
        targets[row] -= system[row][free_index];

      if (branch_nb_press == -1)
        continue;

      nb_presses += branch_nb_press;
      if (nb_presses < min_presses)
        min_presses = nb_presses;
    }
    for (size_t row = 0; row < M; ++row)
      targets[row] += free_value * system[row][free_index];

    return min_presses;
  }

  std::pair<int, int>
  get_free_value_limits(const Matrix &system, const std::vector<int> &targets,
                        const std::vector<size_t> &free_indices,
                        size_t index) const {
    size_t free_index = free_indices[index];

    int max_free_value = *std::max_element(targets.begin(), targets.end());
    int min_free_value = 0;
    for (size_t row = 0; row < system.size(); ++row) {
      int factor = system[row][free_index];
      if (factor == 0)
        continue;

      bool has_next_pos = false;
      bool has_next_neg = false;

      for (size_t i = index + 1; i < free_indices.size(); ++i) {
        int next_free_index = system[row][free_indices[i]];
        if (next_free_index > 0)
          has_next_pos = true;
        if (next_free_index < 0)
          has_next_neg = true;
      }

      if (factor > 0) {
        // We can't say anything if there's a free index
        // with negative factor after
        if (has_next_neg)
          continue;

        int target = targets[row] / factor;
        // Can't reach a negative value: break and exit
        if (target < 0)
          return {1, 0};

        if (target < max_free_value)
          max_free_value = target;
      } else if (targets[row] < 0) {
        // We can't say anything if there's a free index
        // with positive factor after
        if (has_next_pos)
          continue;

        int target = -targets[row] / factor;
        // Can't reach a positive value: break and exit
        if (target > 0)
          return {1, 0};

        if (target > min_free_value)
          min_free_value = target;
      }
    }

    return {min_free_value, max_free_value};
  }

  int get_total_presses(const Matrix &system,
                        const std::vector<size_t> &free_indices,
                        const std::vector<int> &targets) const {
    int total_nb_presses = 0;
    size_t free_index_id = 0;
    size_t col = 0;
    for (size_t row = 0; row < targets.size(); ++row) {
      if (system[row][col] == 0)
        break;

      // Invalid solution: number of push is negative
      if (targets[row] < 0)
        return -1;

      // Invalid solution: number of push is not an integer
      if (targets[row] % system[row][col] != 0)
        return -1;

      total_nb_presses += targets[row] / system[row][col];
      col++;
      while (free_index_id < free_indices.size() &&
             col == free_indices[free_index_id]) {
        col++;
        free_index_id++;
      }
    }
    return total_nb_presses;
  }
};

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

  std::vector<Machine> machines;
  if (myfile.is_open()) {
    std::string input;
    while (getline(myfile, input))
      machines.emplace_back(std::move(input));
    myfile.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  for (const Machine &machine : machines) {
    if (first_half)
      ans += machine.get_fewest_presses_light();
    else
      ans += machine.get_fewest_presses_joltages();
  }

  std::cout << "Answer : " << ans << std::endl;
  return 0;
}
