**The following tests were held with the following inputs**

**(64, 64)**

**(100, 64)**

**(100, 64)**

**matmulFloat**

DMA: 100 sample(s), 10496 input words, 6400 output words in 8.78 ms (87.8 us/sample)
HW timer: 484009 cycles = 4840.1 us @ 100 MHz
Latency per sample = 48.4





**matmulFixednopipe**



DMA: 100 sample(s), 10496 input words, 6400 output words in 37.40 ms (374.0 us/sample)

HW timer: 2270822 cycles = 22708.2 us @ 100 MHz

Latency per sample = 227.1

Average L2 error: 3.826226e-06

DMA: 100 sample(s), 10496 input words, 6400 output words in 37.40 ms (374.0 us/sample)

HW timer: 2270822 cycles = 22708.2 us @ 100 MHz

Latency per sample = 227.1

Average L2 error: 3.826226e-06




**matmulunroll - factor (16)**



DMA: 100 sample(s), 10496 input words, 6400 output words in 10.70 ms (107.0 us/sample)

HW timer: 588638 cycles = 5886.4 us @ 100 MHz

Latency per sample = 58.9



Average L2 error: 3.8199614e-06



**matmul- array part 32**



DMA: 100 sample(s), 10496 input words, 6400 output words in 8.33 ms (83.3 us/sample)

HW timer: 455413 cycles = 4554.1 us @ 100 MHz

Latency per sample = 45.5



Average L2 error: 3.8256917e-06





**matmul-pipeline II = 4 - allocation limit - 20 (didn't care)**



DMA: 100 sample(s), 10496 input words, 6400 output words in 7.30 ms (73.0 us/sample)

HW timer: 330892 cycles = 3308.9 us @ 100 MHz

Latency per sample = 33.1



Average L2 error: 3.8259545e-06







**matmul pipeline II - 1**



DMA: 100 sample(s), 10496 input words, 6400 output words in 5.98 ms (59.8 us/sample)

HW timer: 292291 cycles = 2922.9 us @ 100 MHz

Latency per sample = 29.2



Average L2 error: 3.8227627e-06



