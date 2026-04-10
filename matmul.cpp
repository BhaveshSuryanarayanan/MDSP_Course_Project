#include <matmul.h>
typedef ap_axis<32,0,0,0> axis_t;

void matmul_stream(hls::stream<axis_t> &in,
                   hls::stream<axis_t> &out,
                   int N) {

#pragma HLS INTERFACE axis port=in
#pragma HLS INTERFACE axis port=out

#pragma HLS INTERFACE s_axilite port=N
#pragma HLS INTERFACE s_axilite port=return

    float A[MAT_SIZE][MAT_SIZE];
    float B[MAT_SIZE][MAT_SIZE];
    float C[MAT_SIZE][MAT_SIZE];

#pragma HLS ARRAY_PARTITION variable=A complete dim=2
#pragma HLS ARRAY_PARTITION variable=B complete dim=1

    axis_t temp;

    // Read A
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            temp = in.read();
            A[i][j] = *((float*)&temp.data);
        }
    }

    // Read B
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            temp = in.read();
            B[i][j] = *((float*)&temp.data);
        }
    }

    // Compute
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            float sum = 0;
            for(int k=0;k<N;k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    // Write C
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            axis_t out_val;
            out_val.data = *((int*)&C[i][j]);
            out_val.last = (i==N-1 && j==N-1);
            out.write(out_val);
        }
    }
}