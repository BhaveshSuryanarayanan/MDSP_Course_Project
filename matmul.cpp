#include "matmul.h"

union float_bits {
    float    f;
    unsigned u;
};

static inline data_t unpack(ap_int<AP_SIZE> raw) {
#ifdef USE_FIXED
    data_t val;
    val.range() = raw.range(DATA_WIDTH - 1, 0);
    return val;
#else
    float_bits fb;
    fb.u = (unsigned)raw;
    return fb.f;
#endif
}

static inline ap_int<32> pack(data_t val) {
#ifdef USE_FIXED
    ap_int<32> raw = 0;
    raw.range(DATA_WIDTH - 1, 0) = val.range();
    return raw;
#else
    float_bits fb;
    fb.f = val;
    return (ap_int<32>)fb.u;
#endif
}


// in_stream: input data (x)
// out_stream: output data (y)
// N : A register that determines the number of inputs (set by CPU)
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

    // Streaming the transformation matrix(A)
    for (int i = 0; i < MAT_SIZE; i++) {
        for (int j = 0; j < MAT_SIZE; j++) {
            axis_t in_pkt = in_stream.read();
            A[i][j] = unpack(in_pkt.data);
        }   
    }
    
    // Stream in X vectors one-by-one
    input_iter:
    for (int num_iter = 0; num_iter < N; num_iter++) {
        
        // Read elements of X vector
    	data_in:
        for (int j = 0; j < MAT_SIZE; j++) {
            axis_t in_pkt = in_stream.read();
            X[j] = unpack(in_pkt.data);
        }

        // Matrix multiplcation operation
        matrix_mul:
        for (int i = 0; i < MAT_SIZE; i++) {
            Y[i] = 0;
            ith_element:
            for (int j = 0; j < MAT_SIZE; j++) // calculate ith element
                Y[i] += A[i][j] * X[j];
        }
        
        // Stream out Y-vector one-by-one
        data_out:
        for (int y_ind = 0; y_ind < MAT_SIZE; y_ind++) {
            axis_t pkt;
            pkt.data = pack(Y[y_ind]);
            pkt.last = (y_ind == MAT_SIZE - 1) && (num_iter == N - 1);
            pkt.keep = -1;

            out_stream.write(pkt);
        }
    }
}
