
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>


using Reg = std::array<int, 4>;
using Instr = std::array<int, 4>;

Reg read_reg(std::string input) {
    input = input.substr(8);
    Reg output;
    output[0] = input[1] - '0';
    output[1] = input[4] - '0';
    output[2] = input[7] - '0';
    output[3] = input[10] - '0';

    return output;
}

Instr read_instr(std::string input) {
    std::stringstream stream(input);
    int op, a, b, c;
    stream >> op >> a >> b >> c;
    Instr output = {op, a, b, c};
    return output;
}

struct Sample {
    Reg before;
    Instr instr;
    Reg after;

    Sample(Reg before, Instr instr, Reg after) :
        before(before), instr(instr), after(after) { }
};

Reg apply_op(Reg before, Instr instr, int op) {
    Reg after(before);

    switch (op) {
        case 0: // addr
            after[instr[3]] = after[instr[1]] + after[instr[2]];
            break;

        case 1: // addi
            after[instr[3]] = after[instr[1]] + instr[2];
            break;

        case 2: // mulr
            after[instr[3]] = after[instr[1]] * after[instr[2]];
            break;

        case 3: // muli
            after[instr[3]] = after[instr[1]] * instr[2];
            break;

        case 4: // banr
            after[instr[3]] = after[instr[1]] & after[instr[2]];
            break;

        case 5: // bani
            after[instr[3]] = after[instr[1]] & instr[2];
            break;

        case 6: // borr
            after[instr[3]] = after[instr[1]] | after[instr[2]];
            break;

        case 7: // bori
            after[instr[3]] = after[instr[1]] | instr[2];
            break;

        case 8: // setr
            after[instr[3]] = after[instr[1]];
            break;

        case 9: // seti
            after[instr[3]] = instr[1];
            break;

        case 10: // gtir
            after[instr[3]] = (int) (instr[1] > after[instr[2]]);
            break;

        case 11: // gtri
            after[instr[3]] = (int) (after[instr[1]] > instr[2]);
            break;

        case 12: // gtrr
            after[instr[3]] = (int) (after[instr[1]] > after[instr[2]]);
            break;

        case 13: // eqir
            after[instr[3]] = (int) (instr[1] == after[instr[2]]);
            break;

        case 14: // eqri
            after[instr[3]] = (int) (after[instr[1]] == instr[2]);
            break;

        case 15: // eqrr
            after[instr[3]] = (int) (after[instr[1]] == after[instr[2]]);
            break;

        default:
            break;
    }

    return after;
}

void validate(std::vector<int>& opcodes, Sample sample) {
    for (auto it=opcodes.begin(); it!=opcodes.end(); ) {
        if (apply_op(sample.before, sample.instr, *it) != sample.after) {
            it = opcodes.erase(it);
        } else {
            ++it;
        }
    }
}

int main()
{
    bool first_half = false;
    int ans = 0;
    std::vector<Sample> samples;
    std::vector<Instr> program;
    std::array<std::vector<int>, 16> opcode_list;
    for (int i=0; i<16; ++i) {
        for (int j=0; j<16; ++j)
            opcode_list[i].push_back(j);
    }

    std::ifstream myfile ("input");
    if (myfile.is_open()) {
        std::string input;

        while (getline(myfile, input)) {
            if (input.substr(0, 6) == "Before") {
                Reg before = read_reg(input);
                getline(myfile, input);
                Instr instr = read_instr(input);
                getline(myfile, input);
                Reg after = read_reg(input);
                samples.push_back(Sample(before, instr, after));
                getline(myfile, input);
            } else {
                program.push_back(read_instr(input));
            }
        }

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        for (Sample sample : samples) {
            int nb_opcodes = 0;
            for (int op=0; op<16; ++op) {
                if (apply_op(sample.before, sample.instr, op) == sample.after)
                    ++nb_opcodes;
            }
            if (nb_opcodes > 2)
                ++ans;
        }
    } else {
        bool finished[16] = { false };
        do {
            for (Sample sample : samples) {
                int i = sample.instr[0];
                if (opcode_list[i].size() > 1)
                    validate(opcode_list[i], sample);
            }

            bool assign = true;
            while (assign) {
                assign = false;
                for (int i=0; i<16; ++i) {
                    if (!finished[i] && opcode_list[i].size() == 1) {
                        finished[i] = true;
                        for (int j=0; j<16; ++j) {
                            auto it = std::find(opcode_list[j].begin(),
                                                opcode_list[j].end(),
                                                opcode_list[i][0]);
                            if (i != j && it != opcode_list[j].end())
                                opcode_list[j].erase(it);
                        }
                        assign = true;
                    }
                }
            }

        } while (std::find(finished, finished+16, false) != finished+16);

        Reg reg = {0, 0, 0, 0};
        for (Instr instr : program)
            reg = apply_op(reg, instr, opcode_list[instr[0]][0]);

        ans = reg[0];
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
