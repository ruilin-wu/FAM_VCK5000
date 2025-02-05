

# The second design of FFT Accumulation Method using VCK5000 Versal Development Card

***Version: Vitis 2022.2***

## Introduction

This project is the second implementation of the FAM algorithm in the AI ​​Engine. It is a system-level design that uses AI Engine, PL, and PS resources to demonstrate the following capabilities:

* Simulation of FAM using matlab software on an x86 machine.
* Scalable AI Engine design that can utilize up to 134 AI Engine tiles
* AI Engine single-precision cfloat computation
* Code-free PL HLS data mover kernel from AMD Vitis™ utility library
* Host application for verifying AI Engine design data
* Matlab model of the FAM algorithm
* Effective throughput computation (GFLOPS) and throughput

## Before You Begin

This tutorial can be run on the [VCK5000 Board](https://www.xilinx.com/products/boards-and-kits/vck5000.html).

By default, this project targets the `xilinx_vck5000_gen4x8_qdma_2_202220_1` platform for VCK5000. You can try to compile the [Xilinx tutorial project](https://github.com/Xilinx/xup_aie_training/tree/main/sources/vadd_lab) before compiling this project.


## The FFT Accumulation Method
The N-Body problem is the problem of predicting the motions of a group of N objects which each have a gravitational force on each other. For any particle `i` in the system, the summation of the gravitational forces from all the other particles results in the acceleration of particle `i`. From this acceleration, we can calculate a particle's velocity and position (`x y z vx vy vz`) will be in the next timestep. Newtonian physics describes the behavior of very large bodies/particles within our universe. With certain assumptions, the laws can be applied to bodies/particles ranging from astronomical size to a golf ball (and even smaller).

# FFT Accumulation Method (FAM)

## Definition

The FFT Accumulation Method (FAM) produces a large number of point estimates of the cross-spectral correlation function. As described in [R4], the point estimates are given by:

$$
S_{xy_T}^{\alpha_i + q\Delta\alpha} (rL, f_j)_{\Delta t} = \sum_{r} X_T(rL, f_k) Y_T^* (rL, f_l) g_c(n - r) e^{-i2\pi r q / P} \quad (1)
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

- **\( g_c(n) \)**: A data-tapering window, often a unit-height rectangle (no multiplications needed).
- **\( a(r) \)**: A tapering window, commonly a Hamming window (can be generated using MATLAB’s `hamming.m`).

The **sampling rate** is defined as:

$$
f_s = \frac{1}{T_s}
$$

In Equations (2) and (3), the following relationships hold:

- \( T = N' T_s \)
- The channelizer's short-time Fourier transforms use a tapering window \( a(r) \) of width \( T_s N' \).
- The output long-time Fourier transform applies \( g_c(r) \), spanning \( NT_s \), the data block's length.

### FAM Channelization

The FAM channelizes input data using short Fourier transforms of length \( N' \), hopped in time by \( L \) samples. This results in a sequence of transforms with length:

$$
P = \frac{N}{L} \quad (4)
$$

*Assumption*: Both \( N \) and \( L \) are dyadic integers, with \( L \ll N \).

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

---

### 📚 References
- [R4] Reference for detailed derivation and theoretical background.


Source: [GRAPE-6: Massively-Parallel Special-Purpose Computer for Astrophysical Particle Simulations](https://academic.oup.com/pasj/article/55/6/1163/2056223)

### System Design Overview
The N-Body Simulator is implemented on an `XCVC1902 AMD Versal Adaptive SoC` device on the VCK190 board. It consists of PL HLS datamover kernels from the AMD Vitis Utility Library (`mm2s_mp` and `s2mm_mp`), custom HLS kernels that enable packet switching (`packet_sender` and `packet_receiver`), and a 400 tile AI Engine design. Additionaly, the design consists of host applications that enable the entire design, verify the data coming out of the AI Engine, and run the design for multiple timesteps.

![alt text](images/System_diagram.PNG)

#### Dataflow
* The host applications store input data (`i` and `j`) in global memory (DDR) and turn on the PL HLS kernels (running at 300 MHz) and the AI Engine graph (running at 1GHz).
* Data moves from DDR to the dual-channel HLS datamover kernel `mm2s_mp`. The `i` data goes into one channel and the `j` data goes into the other channel. Here, data movement switches from AXI-MM to AXI-Stream. The read/write bandwith of DDR is set to the default 0.04 Gbps.
* The AI Engine graph performs packet switching on the `input_i` data, so the `i` data needs to be packaged appropriately before being sent to the AI Engine. So from the `mm2s_mp` kernel, it is streamed to the HLS `packet_sender` kernel. The `packet_sender` kernel sends a packet header and appropriately asserts `TLAST` before sending packets of `i` data to the 100 `input_i` ports in the AI Engine.
* The AI Engine graph expects the `j` data to be streamed directly into the AI Engine kernels, so no additional packaging is needed. The `j` data is directly streamed from the `mm2s_mp` kernel into the AI Engine.  
* The AI Engine distributes the gravity equation computations onto 100 accelerators (each using 4 AI Engine tiles). The AI Engine graph outputs new `i` data through the 100 `output_i` ports. The `output_i` data is also packet switched and needs to be appropriately managed by the `packet_receiver`.
* The `packet_receiever` kernel receives a packet and evaluates the header as 0, 1, 2, or 3 and appropriately sends the `output_i` data to the `k0`, `k1`, `k2`, or `k3` streams.
* The `s2mm_mp` quad-channel HLS datamover kernel receives the `output_i` data and writes it to global memory (DDR). Here, data movement switches from AXI-Stream to AXI-MM.
* Then, depending on the host application, the new output data is read and compared against the golden expected data or saved as the next iteration of `i` data and the AI Engine N-Body Simulator runs for another timestep.

*Note:* The entire design is a compute-bound problem, meaning we are limited to how fast the AI Engine tiles compute the floating-point gravity equations. This is not a memory-bound design.

## Where We're Headed ...  
Complete modules 01-07 in the following order:

### Module 01 - Python Simulations on x86
The module shows a python implementation of the N-Body Simulator and execution times to run the N-Body Simulator on an x86 machine.

[Read more ...](Module_01_python_sims)

### Module 02 - AI Engine Design
This module presents the final 400 tile AI Engine design:

  * A single AI Engine kernel (`nbody()`)
  * An N-Body Subsystem with 4 `nbody()` kernels which are packet switched (`nbody_subsystem` graph)
  * An N-Body System with 100 `nbody_subsystem` graphs (i.e., 400 `nbody()` kernels) which use all 400 AI Engine tile resources
  * Invoke the AI Engine compiler

[Read more...](Module_02_aie)

### Module 03 - PL Kernels
This modules presents the PL HLS kernels:

* Create datamover PL HLS kernels from AMD Vitis Utility Library
* Create and simulate packet switching PL HLS kernels

[Read more...](Module_03_pl_kernels)

### Module 04 - Full System Design
This module shows how to link the AI Engine design and PL kernels together into a single XCLBIN and view the actual hardware implementation Vivado™ solution.

[Read more...](Module_04_xclbin)

### Module 05 - Host Software
This module presents the host software that enables the entire design:
* Create a functional host application that compares AI Engine output data to golden data
* Create a C++ N-Body Simulator to profile and compare performance between the A72 processor and AI Engine
* Create a host application that runs the system design for multiple timesteps and create animation data for post-processing

[Read more...](Module_05_host_sw)

### Module 06 - SD Card and Hardware Run
This module conducts the hardware run:
* Create the `sd_card.img`
* Execute the host applications and runs the system design on hardware
* Save animation data from hardware run

[Read more...](Module_06_sd_card_and_hw_run)

### Module 07 - Results  
This module review the results of the hardware run:
* Create an animation for 12,800 particle for 300 timesteps
* Compare latency results between Python x86, C++ Arm A72, and AI Engine N-Body Simulator designs
* Estimate the number of GFLOPS of the design
* Explore ways to increase design bandwidth

[Read more...](Module_07_results)

### (Optional) x1_design and x10_design  
This tutorial contains 3 AI Engine designs:
* x100_design (100 Compute Units using all 400 AI Engine tiles)
* x10_design (10 Compute Units using 40 AI Engine tiles)
* x1_design (1 Compute Unit using 4 AI Engine tiles)

Modules_01-07 builds walks through building the final 100 Compute Unit design. The intermediate designs (x1_design and x10_design) are also provided if you want to build an N-Body Simulator with shorter build times or run hardware emulation within a reasonable amount of time.  

## Build Flows
This tutorial has two build flows you can choose from depending on your comfort level with AMD design processes.  

### For more advanced users
For those who are already familiar with the creating AI Engine designs and AMD Vitis projects, you may just want to build the entire design with a single command. You can do this by running the following command from the top-level folder:

*Estimated Time: 6 hours*

```
make all
```

### For more novice users

For those who are just starting out, you may want to build each module one at time, view the output on the terminal, and learn as you work your way through the tutorial. In this case, cd into each Module folder and run the `make all` command to build just that component of the design. The specific commands `make all` runs under the hood is specificed in each Module's README.md.

*Estimated Time: depends on the Module you're building*

```
cd Module_0*
make all
```

### A Word about Makefiles

This design is built with the help of Makefiles. Each module can be run from the top-level Makefile or from the Makefile inside each module. You can quickly see which make commands are available by running the `make help` command. You can also use the `make clean` command to remove the generated files.  

### Building for VCK190 ES1 Board

By default, the Makefiles build the design for the VCK190 Production board (i.e. using the xilinx_vck190_base_<ver> embedded platform). In order to build the design for the VCK190 ES1 board, ensure you've downloaded the xilinx_vck190_es1_base_<ver> embedded platform from the lounge, so its available for this design build. Then specify the environment variable `export PLATFORM=xilinx_vck190_es1_base_<ver>` with your `sample_env_setup.sh` script.  

## References

* [GRAPE-6: Massively-Parallel Special-Purpose Computer for Astrophysical Particle Simulations](https://academic.oup.com/pasj/article/55/6/1163/2056223)

* [N-body problem wiki page](https://en.wikipedia.org/wiki/N-body_problem)

* [Ultra96 FPGA-Accelerated Parallel N-Particle Gravity Sim](https://www.hackster.io/rajeev-patwari-ultra96-2019/ultra96-fpga-accelerated-parallel-n-particle-gravity-sim-87f45e)

## Next Steps

Let's get started with running the python model of the N-Body simulator on an x86 machine in [Module 01 - Python Simulations on x86](Module_01_python_sims).


### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [support.xilinx.com](http://support.xilinx.com/).



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>