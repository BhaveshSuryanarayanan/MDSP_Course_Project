#include "matmul.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#define TOLERANCE 1e-4

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

int main(){

    int N = 10;

    std::string input_file = "test/input/" + std::to_string(N) + ".txt";
    std::string matrix_file = "test/matrices/" + std::to_string(N) + ".txt";
    std::string output_file = "test/output/" + std::to_string(N) + ".txt";

    std::vector<float> input = read_vector(input_file);
    std::vector<vector<float>> matrix = read_matrix(matrix_file, N);
    std::vector<float> Y_expected = read_vector(output_file);

    hls::stream<axis_pkt_t> in_stream("in");
    hls::stream<axis_pkt_t> out_stream("out");

    float_bits conv;

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            axis_pkt_t pkt;
            conv.f = matrix[i][j];
            pkt.data = (data_t)conv.u;
            pkt.keep = -1;
            // pkt.strb = -1;
            // pkt.user = 0;
            // pkt.id   = 0;
            // pkt.dest = 0;
            pkt.last = 0;
            in_stream.write(pkt);
        }
    }
    
    int num_inputs = 1;
    for(int num = 0; num < num_inputs; num++){
        for (int i = 0; i < N; i++){
            axis_pkt_t pkt;
            conv.f = input[i];
            pkt.data = (data_t)conv.u;
            pkt.keep = -1;
            // pkt.strb = -1;
            // pkt.user = 0;
            // pkt.id   = 0;
            // pkt.dest = 0;
            pkt.last = (i==N-1);
            in_stream.write(pkt);
        }
    }
    

    matmul_stream(in_stream, out_stream, num_inputs);

    int num_errors = 0;
    for(int num = 0; num < num_inputs; num++){
        float err = 0;
        for(int i=0; i<N; i++){
            axis_t pkt = out_stream.read();
            conv.u = (unsigned)(data_t) pt.data;
            float y = conv.f;
            err += (y-Y_expected[i])*(y-Y_expected[i]);
        }
        err = std::sqrt(err);
        if(err>TOLERANCE){
            printf("MISMATCH at sample %d: (err=%.6e)\n",
                   num, err);
            num_errors++;
        }
    }

    if (num_errors == 0)
        printf("PASS: %d samples matched (tolerance=%.0e)\n", num_inputs, TOLERANCE);
    else
        printf("FAIL: %d / %d mismatches\n", errors, num_inputs);

    return num_errors ? 1 : 0;

}