# Matrix‑Vector Multiply (MDSP Project)

## Problem statement overview
Compute y = A * x, where:
- A is a matrix (up to 64×64) loaded once via AXI‑Lite.
- x is streamed via AXI‑Stream.
- y is streamed out.

Baseline solution: nested loops computing dot products. Optimization targets:
- Pipeline inner loop to II=1
- Partition rows of A for parallel MACs
- Explore tiling / parallel output elements
Trade‑offs: DSP48 usage vs throughput.

## PYNQ evaluation
- Python generates random A and x, computes y_ref = A @ x with NumPy
- PS streams x through hardware via DMA → FPGA kernel → DMA back
- Compare y_hw with y_ref, measure latency and throughput
- Vary matrix size (16, 32, 64) and report scaling

## Interface convention
- Data I/O: AXI‑Stream, 32‑bit packets via DMA
- Parameters / coefficients: AXI‑Lite registers (loaded once)
- Timing: AXI Timer IP for cycle‑accurate measurement
- PYNQ: Python script allocates DMA buffers, streams data, verifies results, measures time

## Workflow
1. Baseline C implementation (floating point, no pragmas)  
2. Convert to fixed‑point (ap_fixed) and verify against float reference  
3. Add HLS pragmas; explore ≥3 design points (Pareto front)  
4. Wrap with AXI‑Stream interface, synthesize, generate bitstream  
5. Deploy on PYNQ; measure with DMA + AXI Timer  
6. Compare against pure‑software (ARM) execution

Notes:
- Can use FFT demo or lab5 (FIR) streaming harness as a starting point.
- Any deviation from AXI‑Stream must be discussed and justified.

## Deliverables
- HLS source code with ≥3 design‑point variants  
- Synthesis reports for each variant (LUT/FF/DSP/BRAM/latency/II)  
- Vivado block design TCL script  
- PYNQ Python script with correctness check and timing measurement  
- Short report (2–3 pages): design choices, Pareto tradeoffs, measured results

## Contact / Project rules
- Maximum group size: 3 students. Each member must document contributions.  
- Multiple groups may choose the same project; document collaboration where applicable.