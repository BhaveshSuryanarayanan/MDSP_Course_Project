#include "matmul.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>

#ifdef USE_FIXED
#define TOLERANCE 1e-4
#else
#define TOLERANCE 1e-6
#endif
using namespace std;

union float_bits { float f; unsigned u; };
std::vector<float> read_vector(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<float> vec;


    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return vec;
    }

    float value;
    while (file >> value) {
        vec.push_back(value);
    }

    file.close();
    return vec;
}

std::vector<std::vector<float>> read_matrix(const std::string &filename, int n) {
    std::ifstream file(filename);
    std::vector<std::vector<float>> mat(n, std::vector<float>(n));

    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return mat;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!(file >> mat[i][j])) {
                std::cerr << "Error reading matrix at (" << i << "," << j << ")\n";
                return mat;
            }
        }
    }

    file.close();
    return mat;
}

static axis_t to_pkt(float val, bool last = false) {
    axis_t pkt;
#ifdef USE_FIXED
    data_t fixed_val = (data_t)val;
    pkt.data = (ap_int<32>)(ap_uint<32>)fixed_val.range();
#else
    float_bits conv;
    conv.f = val;
    pkt.data = (ap_int<32>)conv.u;
#endif
    pkt.keep = -1;
    pkt.last = last;
    return pkt;
}

static float from_pkt(axis_t pkt) {
#ifdef USE_FIXED
    data_t fixed_val;
    fixed_val.range() = (ap_uint<32>)pkt.data;
    return (float)fixed_val;
#else
    float_bits conv;
    conv.u = (unsigned)(ap_int<32>)pkt.data;
    return conv.f;
#endif
}

int main() {
    const int N = MAT_SIZE;

    std::string input_file = "test/input/" + std::to_string(N) + ".txt";
    std::string matrix_file = "test/matrices/" + std::to_string(N) + ".txt";
    std::string output_file = "test/output/" + std::to_string(N) + ".txt";

    std::vector<float> input = read_vector(input_file);
    std::vector<std::vector<float>> matrix = read_matrix(matrix_file, N);
    std::vector<float> Y_expected = read_vector(output_file);

    hls::stream<axis_t> in_stream("in");
    hls::stream<axis_t> out_stream("out");

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            in_stream.write(to_pkt(matrix[i][j]));
    
    int num_inputs = 1;
    for (int num = 0; num < num_inputs; num++)
        for (int i = 0; i < N; i++)
            in_stream.write(to_pkt(input[i], i == N - 1));
    

    matmul_stream_2(in_stream, out_stream, num_inputs);

    int num_errors = 0;
    for (int num = 0; num < num_inputs; num++) {
        float err = 0;
        for (int i = 0; i < N; i++) {
            axis_t pkt = out_stream.read();
            float y = from_pkt(pkt);
            err += (y - Y_expected[i]) * (y - Y_expected[i]);
            std::cout << y << " " << Y_expected[i] << std::endl;
        }
        err = std::sqrt(err);
        cout << "L2 error: " << err << endl;
        if (err > TOLERANCE) {
            printf("MISMATCH at sample %d: (err=%.6e)\n",
                   num, err);
            num_errors++;
        }
    }

    if (num_errors == 0) {
        printf("PASS: %d samples matched (tolerance=%.0e)\n", num_inputs, TOLERANCE);
    } else {
        printf("FAIL: %d / %d mismatches\n", num_errors, num_inputs);
    }

    return 0;

}
