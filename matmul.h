#ifndef MATMUL_H
#define MATMUL_H

#include <hls_strem.h>
#include <ap_axi_sdata.h>
#include "config.h"

#define AP_SIZE 32

// AXI stream data type
// data | keep | strb | user | TLAST | id, dest 
// TODO: configure these
typedef ap_axis<AP_SIZE,0,0,0> axis_t;

void matmul_stream(hls::stream<axis_t> &,
                   hls::stream<axis_t> &,
                   int);

#endif