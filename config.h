#ifndef CONFIG_H
#define CONFIG_H

#define MAT_SIZE 10

#define USE_FLOAT

#ifdef USE_FIXED
    #include <ap_fixed.h>
    typedef ap_fixed<16, 4> data_t;
    #define DATA_WIDTH 16
#else
    typedef float data_t;
    #define USE_FLOAT
    #define DATA_WIDTH 32   
#endif

#endif
