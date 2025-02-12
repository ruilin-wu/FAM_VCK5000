

# The second design of FFT Accumulation Method using VCK5000 Versal Development Card

***Version: Vitis 2022.2***

## Introduction

This project is the second implementation of the FAM algorithm in the AI ​​Engine. It is a system-level design that uses AI Engine, PL, and PS resources to demonstrate the following capabilities:

* Simulation of FAM using matlab software on an x86 machine.
* Scalable AI Engine design that can utilize up to 144 AI Engine tiles
* AI Engine single-precision cfloat computation
* Code-free PL HLS data mover kernel from AMD Vitis™ utility library
* Host application for verifying AI Engine design data
* Matlab model of the FAM algorithm
* Effective throughput computation (GFLOPS) and throughput

## Before You Begin

This tutorial can be run on the [VCK5000 Board](https://www.xilinx.com/products/boards-and-kits/vck5000.html).

By default, this project targets the `xilinx_vck5000_gen4x8_qdma_2_202220_1` platform for VCK5000. You can try to compile the [Xilinx tutorial project](https://github.com/Xilinx/xup_aie_training/tree/main/sources/vadd_lab) before compiling this project.


## The FFT Accumulation Method

### Definition

The FFT Accumulation Method (FAM) produces a large number of point estimates of the cross-spectral correlation function. The point estimates are given by:

$$
S_{xy_T}^{\alpha_i + q\Delta\alpha}(rL, f_j)_{\Delta t} = 
$$

$$
\sum_{r} X_T(rL, f_k) Y_T^*(rL, f_l) g_c(n - r) e^{-i 2 \pi \frac{r q}{P}} \quad (1)
$$

where the complex demodulates are defined as:

$$
X_T(n, f) = \sum_{r = -N'/2}^{N'/2} a(r) x(n - r) e^{-i 2 \pi f (n - r) T_s} \quad (2)
$$

However, as noted in [R4], Equation (2) should be corrected to sum over \( N' \) samples instead of \( N' + 1 \):

$$
X_T(n, f) = \sum_{r = -N'/2}^{N'/2 - 1} a(r) x(n - r) e^{-i 2 \pi f (n - r) T_s} \quad (3)
$$

### Explanation

- <code>g<sub>c</sub>(n)</code>: A data-tapering window, often a unit-height rectangle (no multiplications needed).
- **<code>a(r)</code>**: A tapering window, commonly a Hamming window (can be generated using MATLAB’s `hamming.m`).

The **sampling rate** is defined as:

$$
f_s = \frac{1}{T_s}
$$

In Equations (2) and (3), the following relationships hold:

- <code>T = N' T<sub>s</sub></code>
- The channelizer's short-time Fourier transforms use a tapering window <code>a(r)</code> of width <code>T<sub>s</sub> N'</code>.
- The output long-time Fourier transform applies <code>g<sub>c</sub>(r)</code>, spanning <code>N T<sub>s</sub></code>, the data block's length.

### FAM Channelization

The FAM channelizes input data using short Fourier transforms of length <code>N'</code>, hopped in time by <code>L</code> samples. This results in a sequence of transforms with length:

$$
P = \frac{N}{L} \quad (4)
$$

*Assumption*: Both <code>N</code> and <code>L</code> are dyadic integers, with <code>L ≪ N</code>.

### Cycle-Frequency Resolution

Defined in normalized frequency units as:

$$
\Delta \alpha = \frac{1}{N} \quad (5)
$$

### Spectral Components

The point estimate is associated with:

- **Cycle Frequency**:

$$
\alpha_i = f_k - f_l \quad (6)
$$

- **Spectral Frequency**:

$$
f_j = \frac{f_k + f_l}{2} \quad (7)
$$


Source: [CSP Estimators: The FFT Accumulation Method](https://cyclostationary.blog/2018/06/01/csp-estimators-the-fft-accumulation-method/)

### System Design Overview
The FAM algorithm implementation is implemented on a VCK5000 board. It consists of a PL HLS data mover core (`dma_hls`) from the AMD Vitis utility library, and a 144-tile AI engine design. In addition, the design includes a host application that supports the entire design, verifies the data output by the AI ​​engine, and runs the design for multiple time steps.

#### Data Flow

* Multiple `memin` memory channels read 64-bit data through `readDDR_to_AIE1()` and write it to the input stream of **AIE Stage1**.
* Data enters AIE and performs the first stage FFT Accumulation Method (FAM) calculation, and outputs to `stage1_FAMOut` after processing.
* `readAIE1_to_memtrans()` reads data from AIE Stage1 and stores it in `memtrans` as a transit storage on the PL side.
* `plTranspose_to_aie2()` reorganizes the data in `memtrans` and writes it to **AIE Stage2** for further calculation.
* AIE Stage2 performs the second round of FFT calculation and outputs the result to `stage2_FAMOut`.
* `aie2_to_ddr()` reads the data calculated by AIE Stage2, packages it into 512-bit and writes it back to `memout`.
* `phase1()` processes **DDR -> AIE1 -> PL**, `phase2()` processes **PL -> AIE2 -> DDR**, and finally forms a complete AIE calculation pipeline.

## Where We're Headed ...  
Complete modules 01-05 in the following order:

### Module 01 - Matlab Simulations on x86
The module shows a matlab implementation of the FAM and execution time to run the FAM on an x86 machine.

[Read more ...](Module_01_matlab_sim)

### Module 02 - AI Engine Design
This module presents the final 144 tile AI Engine design:

* The `ssr_stage1_graph` system with 16 `fam_stage1()` cores is used to calculate and send the results of the first stage
* The `ssr_stage2_graph` system with 128 `fam_stage2()` cores is used to receive data from the first stage and output data from the second stage
* Calling the AI ​​Engine Compiler

[Read more...](Module_02_aie)

### Module 03 - PL Kernels
This modules presents the PL HLS kernels:

* Create datamover PL HLS kernels from AMD Vitis Utility Library


[Read more...](Module_03_pl)

### Module 04 - Hardware Link
This module shows how to link the AI Engine design and PL kernels together.

[Read more...](Module_04_hw_link)

### Module 05 - Host Software
This module presents the host software that enables the entire design:
* Create a functional host application that compares AI Engine output data to golden data

* Create a host application that runs the system design for multiple timesteps and create animation data for post-processing

[Read more...](Module_05_host)


Modules_01-05 builds walks through building the final 144 Compute Unit design. 




## Build Flows
This tutorial has two build flows you can choose from depending on your comfort level with AMD design processes.  

### For more advanced users
For those who are already familiar with the creating AI Engine designs and AMD Vitis projects, you may just want to build the entire design with a single command. You can do this by running the following command from the top-level folder:

```
make all
```

### For more novice users

For those who are just starting out, you may want to build each module one at time, view the output on the terminal, and learn as you work your way through the tutorial. In this case, cd into each Module folder and run the `make all` command to build just that component of the design. The specific commands `make all` runs under the hood is specificed in each Module's README.md.


```
cd Module_0i
make all
```


## References

* [CSP Estimators: The FFT Accumulation Method](https://cyclostationary.blog/2018/06/01/csp-estimators-the-fft-accumulation-method/)


## Next Steps


Let’s get started with running the Matlab model of FAM on an x86 machine in [Module 01 - Matlab Simulations on x86](Module_01_matlab_sim).


