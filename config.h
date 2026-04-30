#ifndef CONFIG_H
#define CONFIG_H

#define MAT_SIZE 10

//#define USE_FLOAT
#define USE_FIXED

#ifdef USE_FIXED
    #include <ap_fixed.h>
	#define FRACTIONAL_BITS 14
    typedef ap_fixed<16, 16-FRACTIONAL_BITS> data_t;
    #define DATA_WIDTH 16
#else
    typedef float data_t;
    #define USE_FLOAT
    #define DATA_WIDTH 32   
#endif

#endif
