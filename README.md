# Project:  Matrix-Vector Multiply

## Problem statement overview

**Problem**: Compute y = A * x, where A is a matrix (up to 64x64) whose coefficients are loaded once via
AXI-Lite, and x is streamed in via AXI-Stream. The result y is streamed out.

**Baseline C**: Double nested loop – straightforward dot product per output element.

**Optimization exploration**: - Pipeline the inner (dot product) loop to II=1 - Partition rows of A to allow
parallel MAC operations - Explore tiling: process multiple output elements concurrently - Trade off:
number of DSP48s used vs throughput (vectors/second)

**Pynq evaluation**: - Python generates random A and x, computes y_ref = A @ x with numpy - Stream x
through hardware, compare y_hw with y_ref - Measure: latency per vector, throughput, compare with
numpy on ARM - Vary matrix size (16x16, 32x32, 64x64) and report scaling

**Why this is a good project**: Minimal algorithmic complexity, clean data access pattern, directly
exercises array partitioning and loop pipelining. The main challenge is managing BRAM usage for the
matrix storage and resolving port conflicts for parallel access


