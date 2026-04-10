#ifndef MATMUL_H
#define MATMUL_H

#include <hls_strem.h>
#include <ap_axi_sdata.h>
#include "config.h"

void matmul_stream(hls::stream<axis_t> &,
                   hls::stream<axis_t> &,
                   int);

#endif