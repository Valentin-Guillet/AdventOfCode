
#include <iostream>
#include <unordered_set>


long next(long reg2) {

    long reg5 = (reg2 | 65536);
    reg2 = 2238642;

    reg2 = (reg2 + (reg5 & 255)) & 16777215;
    reg2 = (reg2 * 65899) & 16777215;

    while (reg5 >= 256) {
        reg5 /= 256;

        reg2 = (reg2 + (reg5 & 255)) & 16777215;
        reg2 = (reg2 * 65899) & 16777215;
    }

    return reg2;
}

int main()
{
    bool first_half = false;
    long ans = 0;
    // An analysis of the program shows that it follows a procedure to generate numbers
    // in register 2, and once in a while, check if that number is equal to register 0 (the input)
    //
    // For the first half, the answer is just the value of the first check executed
    // For the second half, the answer is the value of the last check before looping

    if (first_half) {
        ans = next(0);

    } else {
        std::unordered_set<long> seen;
        long reg2 = 0;

        while (seen.find(reg2) == seen.end()) {
            ans = reg2;
            seen.insert(reg2);

            reg2 = next(reg2);
        }
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
