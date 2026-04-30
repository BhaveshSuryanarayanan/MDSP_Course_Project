#include "matmul.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>

#define TOLERANCE 1e-4

static std::vector<float> read_vector(const std::string &path) {
    std::ifstream f(path);
    if (!f) { std::cerr << "Cannot open: " << path << "\n"; std::exit(1); }
    std::vector<float> v;
    float x;
    while (f >> x) v.push_back(x);
    return v;
}

static std::vector<std::vector<float>> read_matrix(const std::string &path, int n) {
    std::ifstream f(path);
    if (!f) { std::cerr << "Cannot open: " << path << "\n"; std::exit(1); }
    std::vector<std::vector<float>> m(n, std::vector<float>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            f >> m[i][j];
    return m;
}

// float -> fixed-point bits -> stream packet
static axis_t to_pkt(float val, int last = 0) {
    data_t v = (data_t) val;
    axis_t pkt;
    pkt.data = (ap_int<32>)(ap_uint<32>) v.range();
    pkt.keep = -1;
    pkt.last = last;
    return pkt;
}

// stream packet -> fixed-point bits -> float
static float from_pkt(axis_t pkt) {
    data_t v;
    v.range() = (ap_uint<32>) pkt.data;
    return (float) v;
}

int main() {
    const int N = MAT_SIZE;

    // ---- adjust paths to your test data ----
    const std::string in_f  = "C:\\Users\\suthinthararaj\\Documents\\2021matmul\\MDSP_Course_Project\\test\\input\\10.txt";
    const std::string mat_f = "C:\\Users\\suthinthararaj\\Documents\\2021matmul\\MDSP_Course_Project\\test\\matrices\\10.txt";
    const std::string out_f = "C:\\Users\\suthinthararaj\\Documents\\2021matmul\\MDSP_Course_Project\\test\\output\\10.txt";
    // ----------------------------------------

    auto input  = read_vector(in_f);
    auto matrix = read_matrix(mat_f, N);
    auto expect = read_vector(out_f);

    hls::stream<axis_t> in_stream("in");
    hls::stream<axis_t> out_stream("out");

    // Send matrix A row-major
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            in_stream.write(to_pkt(matrix[i][j]));

    // Send input vector X
    for (int i = 0; i < N; i++)
        in_stream.write(to_pkt(input[i], i == N - 1));

    matmul_stream_1(in_stream, out_stream, 1);

    // Check output
    float err = 0;
    for (int i = 0; i < N; i++) {
        float y    = from_pkt(out_stream.read());
        float diff = y - expect[i];
        err += diff * diff;
        std::cout << "y[" << i << "] = " << y << "  expected = " << expect[i] << "\n";
    }
    err = std::sqrt(err);
    std::cout << "L2 error: " << err << "\n";
    std::cout << (err < TOLERANCE ? "PASS" : "FAIL") << "\n";

    return 0;
}
