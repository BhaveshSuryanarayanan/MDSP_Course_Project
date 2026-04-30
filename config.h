#ifndef CONFIG_H
#define CONFIG_H

#include <ap_fixed.h>

#define MAT_SIZE        10
#define FRACTIONAL_BITS 20

typedef ap_fixed<32, 32 - FRACTIONAL_BITS> data_t;

#endif
