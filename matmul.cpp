#include "matmul.h"

void matmul_stream_2(hls::stream<axis_t> &in_stream,
                     hls::stream<axis_t> &out_stream,
                     int N) {

#pragma HLS INTERFACE axis      port=in_stream
#pragma HLS INTERFACE axis      port=out_stream
#pragma HLS INTERFACE s_axilite port=N
#pragma HLS INTERFACE s_axilite port=return

    static data_t A[MAT_SIZE][MAT_SIZE];
    static data_t X[MAT_SIZE];
    static data_t Y[MAT_SIZE];

    // Receive matrix A row-major
    for (int i = 0; i < MAT_SIZE; i++) {
        for (int j = 0; j < MAT_SIZE; j++) {
            axis_t pkt = in_stream.read();
            ap_int<32> raw = (ap_int<32>) pkt.data;
            A[i][j].range() = raw;
        }
    }

    // Process N input vectors
    for (int n = 0; n < N; n++) {

        // Receive X
        for (int j = 0; j < MAT_SIZE; j++) {
            axis_t pkt = in_stream.read();
            ap_int<32> raw = (ap_int<32>) pkt.data;
            X[j].range() = raw;
        }

        // Y = A * X
        for (int i = 0; i < MAT_SIZE; i++) {
            Y[i] = 0;
            for (int j = 0; j < MAT_SIZE; j++)
                Y[i] += A[i][j] * X[j];
        }

        // Send Y
        for (int i = 0; i < MAT_SIZE; i++) {
            axis_t pkt;
            ap_int<32> raw = Y[i].range();
            pkt.data = (ap_int<32>) raw;
            pkt.keep = -1;
            pkt.last = (i == MAT_SIZE - 1) && (n == N - 1);
            out_stream.write(pkt);
        }
    }
}
