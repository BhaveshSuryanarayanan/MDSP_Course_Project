#ifndef CONFIG_H
#define CONFIG_H

#define MAT_SIZE 64

// Comment this line to build the floating-point implementation.
#define USE_FIXED

#ifdef USE_FIXED
    #include <ap_fixed.h>
    #define FRACTIONAL_BITS 26
    #define DATA_WIDTH 32
    typedef ap_fixed<DATA_WIDTH, DATA_WIDTH - FRACTIONAL_BITS> data_t;
#else
    typedef float data_t;
    #define DATA_WIDTH 32
#endif

#endif
