#ifndef MATMUL_H
#define MATMUL_H

#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "config.h"

typedef ap_axis<32, 0, 0, 0> axis_t;

void matmul_stream_2(hls::stream<axis_t> &in_stream,
                     hls::stream<axis_t> &out_stream,
                     int N);

#endif
