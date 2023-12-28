
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>


template<int M, int N>
using Matrix = std::array<std::array<__int128, N>, M>;

__int128 abs(__int128 n) {
    return (n > 0 ? n : -n);
}

__int128 gcd(__int128 a, __int128 b) {
    while (b != 0) {
        __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

template<int M, int N>
Matrix<M, N> gauss_jordan_wo_pivot(Matrix<M, N> mat) {
    int r = 0;
    for (int j = 0; j < N - 1; ++j) {
        while (true) {
            int k = -1;
            __int128 min_elt = std::numeric_limits<__int128>::max();
            int count_non_zero = 0;
            for (int i = r; i < M; ++i) {
                if (mat[i][j] != 0)
                    count_non_zero++;

                if (0 < abs(mat[i][j]) && abs(mat[i][j]) < min_elt) {
                    min_elt = abs(mat[i][j]);
                    k = i;
                }
            }

            __int128 row_gcd = 0;
            for (int jj = 0; jj < N; ++jj)
                row_gcd = gcd(row_gcd, mat[k][jj]);
            for (int jj = 0; jj < N; ++jj)
                mat[k][jj] /= row_gcd;

            __int128 pivot = mat[k][j];
            if (k != r)
                mat[k].swap(mat[r]);

            for (int i = 0; i < M; ++i) {
                if (i == r)
                    continue;

                __int128 sub_value = (mat[i][j] / pivot);
                for (int jj = 0; jj < N; ++jj) {
                    mat[i][jj] -= sub_value * mat[r][jj];
                }
            }

            if (count_non_zero == 1)
                break;
        }
        r++;
    }

    return mat;
}

template<int N>
Matrix<N, 1> solve_system(const Matrix<N, N>& A, const Matrix<N, 1>& b) {
    Matrix<N, N+1> mat;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            mat[i][j] = A[i][j];
            mat[i][N] = b[i][0];
        }
    }

    Matrix<N, N+1> reduced_mat = gauss_jordan_wo_pivot<N, N+1>(mat);
    Matrix<N, 1> res;
    for (int i = N - 1; i >= 0; --i) {
        __int128 value = reduced_mat[i][N];
        for (int j = N - 1; j > i; --j)
            value -= reduced_mat[i][j] * res[j][0];
        res[i][0] = value / reduced_mat[i][i];
    }
    return res;
}

struct Hailstone {
    long long px, py, pz;
    long long vx, vy, vz;

    explicit Hailstone(const std::string& input) {
        size_t beg_ind = 0;
        size_t end_ind = input.find(',');
        px = std::stoll(input.substr(beg_ind, end_ind - beg_ind));

        beg_ind = end_ind + 2;
        end_ind = input.find(',', beg_ind);
        py = std::stoll(input.substr(beg_ind, end_ind - beg_ind));

        beg_ind = end_ind + 2;
        end_ind = input.find(' ', beg_ind);
        pz = std::stoll(input.substr(beg_ind, end_ind - beg_ind));

        beg_ind = end_ind + 3;
        end_ind = input.find(',', beg_ind);
        vx = std::stoll(input.substr(beg_ind, end_ind - beg_ind));

        beg_ind = end_ind + 2;
        end_ind = input.find(',', beg_ind);
        vy = std::stoll(input.substr(beg_ind, end_ind - beg_ind));

        vz = std::stoll(input.substr(end_ind + 2));
    }

    std::array<__int128, 4> get_segment(long long limit_min, long long limit_max) const {
        long long dx_min = ((vx > 0 ? limit_min : limit_max) - px) / vx;
        long long dy_min = ((vy > 0 ? limit_min : limit_max) - py) / vy;
        long long coeff_min = std::max(0LL, std::max(dx_min, dy_min));

        long long dx_max = ((vx > 0 ? limit_max : limit_min) - px) / vx;
        long long dy_max = ((vy > 0 ? limit_max : limit_min) - py) / vy;
        long long coeff_max = std::min(dx_max, dy_max);

        std::array<__int128, 4> points;
        points[0] = px + coeff_min * vx;
        points[1] = py + coeff_min * vy;
        points[2] = px + coeff_max * vx;
        points[3] = py + coeff_max * vy;

        return points;
    }

    bool path_cross(const Hailstone& other, long long limit_min, long long limit_max) const {
        auto [p1_x, p1_y, p2_x, p2_y] = get_segment(limit_min, limit_max);
        auto [p3_x, p3_y, p4_x, p4_y] = other.get_segment(limit_min, limit_max);

        __int128 d = (p2_x - p1_x) * (p4_y - p3_y) - (p4_x - p3_x) * (p2_y - p1_y);
        if (d == 0)
            return (p3_x - p1_x) * vy == (p3_y - p1_y) * vx;

        __int128 t = (p3_x - p1_x) * (p4_y - p3_y) - (p4_x - p3_x) * (p3_y - p1_y);
        __int128 u = (p3_x - p1_x) * (p2_y - p1_y) - (p2_x - p1_x) * (p3_y - p1_y);
        if (d > 0)
            return (0 <= t && t <= d) && (0 <= u && u <= d);
        return (0 >= t && t >= d) && (0 >= u && u >= d);
    }
};

template<int N>
void build_system(const std::vector<Hailstone>& hailstones, Matrix<N, N>& A, Matrix<N, 1>& b) {
    const Hailstone& h0 = hailstones[0];
    for (int i = 0; i < 2; ++i) {
        const Hailstone& hi = hailstones[i + 1];
        // x - y
        A[3*i+0][0] =   hi.vy - h0.vy;
        A[3*i+0][1] = - hi.vx + h0.vx;
        A[3*i+0][2] = 0;
        A[3*i+0][3] = - hi.py + h0.py;
        A[3*i+0][4] =   hi.px - h0.px;
        A[3*i+0][5] = 0;
        b[3*i+0][0] = hi.px * hi.vy - h0.px * h0.vy - hi.py * hi.vx +  h0.py * h0.vx;

        // x - z
        A[3*i+1][0] =   hi.vz - h0.vz;
        A[3*i+1][1] = 0;
        A[3*i+1][2] = - hi.vx + h0.vx;
        A[3*i+1][3] = - hi.pz + h0.pz;
        A[3*i+1][4] = 0;
        A[3*i+1][5] =   hi.px - h0.px;
        b[3*i+1][0] = hi.px * hi.vz - h0.px * h0.vz - hi.pz * hi.vx +  h0.pz * h0.vx;

        // y - z
        A[3*i+2][0] = 0;
        A[3*i+2][1] =   hi.vz - h0.vz;
        A[3*i+2][2] = - hi.vy + h0.vy;
        A[3*i+2][3] = 0;
        A[3*i+2][4] = - hi.pz + h0.pz;
        A[3*i+2][5] =   hi.py - h0.py;
        b[3*i+2][0] = hi.py * hi.vz - h0.py * h0.vz - hi.pz * hi.vy +  h0.pz * h0.vy;
    }
}

int main(int argc, char* argv[])
{
    bool first_half = true;
    long long ans = 0;

    std::ifstream myfile("input");
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Hailstone> hailstones;
    if (myfile.is_open()) {
        for (std::string input; getline(myfile, input); )
            hailstones.emplace_back(input);

        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    if (first_half) {
        long long limit_min = 200000000000000;
        long long limit_max = 400000000000000;
        for (size_t i = 0; i < hailstones.size(); ++i) {
            for (size_t j = i + 1; j < hailstones.size(); ++j)
                ans += hailstones[i].path_cross(hailstones[j], limit_min, limit_max);
        }
    } else {
        Matrix<6, 6> A;
        Matrix<6, 1> b, res;
        build_system<6>(hailstones, A, b);
        res = solve_system<6>(A, b);
        ans = res[0][0] + res[1][0] + res[2][0];
    }

    std::cout << "Answer : " << ans << std::endl;
    return 0;
}
