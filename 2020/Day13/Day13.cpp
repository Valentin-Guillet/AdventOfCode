
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


long modular_inverse(long n, long p) {
    long old_r = n, r = p;
    long old_s = 1, s = 0;

    while (r != 0) {
        int q = old_r / r;
        int tmp_old_r = old_r;
        old_r = r;
        r = tmp_old_r - q * r;

        int tmp_old_s = old_s;
        old_s = s;
        s = tmp_old_s - q * s;
    }
    return (old_s + p) % p;
}

int main()
{
    bool first_half = false;
	long ans = 0;
    long min_depart;
    std::vector<std::pair<long, long>> buses;

    std::ifstream myfile ("Day13Input");
	if (myfile.is_open()) {
        std::string input;

		getline(myfile, input);
        min_depart = std::stol(input);
        size_t index = 0;
        while (getline(myfile, input, ',')) {
            if (input != "x")
                buses.push_back({index, std::stoi(input)});
            index++;
        }

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        int departure_time = 1000000000;
        int final_id;
        for (size_t i=0; i<buses.size(); ++i) {
            int id = buses[i].second;
            int timestamp = ((min_depart - 1) / id + 1) * id;
            if (timestamp < departure_time) {
                departure_time = timestamp;
                final_id = id;
            }
        }

        ans = (departure_time - min_depart) * final_id;
    } else {
        long coeff = 0;
        long prime_product = 1;

        for (size_t i=0; i<buses.size(); ++i) {
            auto [c_i, p_i] = buses[i];
            long invert_prime_product = 1;
            for (size_t j=0; j<i; ++j) {
                invert_prime_product *= modular_inverse(buses[j].second, p_i) % p_i;
                invert_prime_product %= p_i;
            }
            long tmp_coeff = (-1 * invert_prime_product * (coeff + c_i)) % p_i;
            tmp_coeff = (tmp_coeff + p_i) % p_i;
            coeff = prime_product * tmp_coeff + coeff;
            prime_product *= p_i;
        }

        ans = coeff;
    }

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
