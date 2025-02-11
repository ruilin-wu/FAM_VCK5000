# Build the Design



```
make all
```


## AI Engine Design

The following AI Engine features are used in this design:

* single precision floating-point compute of the FAM alogorithms 
* 134 tile design with 128 parallel accelerators
* 1:128 broadcast stream
* PL Kernels designed to support data move in AI Engine

## Stage1 Graph

### **FAM Stage 1 Processing (fam_stage1() kernel)**
Review the `kernels/fam_stage1.cpp` file.
- The `fam_stage1` function processes complex floating-point (cfloat) input data.
- It applies a **256-point FFT** to chunks of 256 complex numbers.
- The function performs **windowing**, **256-pt FFT**, **Downconversion**, and **matrix transposition**.
- It processes two input buffers (`inputx0` and `inputx1`) and generates a single output buffer (`outputy`).
- The function operates in a **block-based loop**, iterating over four data blocks for each input buffer.

### **CONV Processing (conv_stage1() kernel)**
Review the `kernels/conv_stage1.cpp` file.
- The `conv_stage1` function takes two **2048-element complex input buffers**.
- It writes the input data to two output streams (`outputy0` and `outputy1`).
- The function uses **pipelined loop processing** to efficiently stream data.
- Data is written incrementally in chunks of four complex elements per iteration.

![alt text](../../images/design2/stage1.png)
- **Input (`FAMDataIn_x`)**: These correspond to the input files fed into the **FAM stage**.
- **FAM processing (`fam_stage1`)**: The first computational stage processes the data using FFT-based accumulation.
- **Memory buffer**: Intermediate results are stored in **buffer** and then passed to the **CONV stage**.
- **CONV processing (`conv_stage1`)**: The second computational stage streams the processed data to the output.
- **Final output**: The data is finally written to the output stream.



## Stage2 Graph

### **FAM Stage 2 Processing (fam_stage2() kernel)**
<div align="center">
    <img src="../../images/design2/stage2.png" alt="FAM Stage 2" />
</div>

Review the `kernels/fam_stage2.cpp` file.
- The `fam_stage2` function processes **complex floating point (cfloat) input streams** from two input sources (`inputx0`, `inputx1`).
- The function reads **16-element vectorized complex numbers** from the input stream.
- It uses the `stage2_cm` function to compute the result after conjugate multiplication.
- A **32-point FFT (`FFT_32pt`)** is performed on the data after conjugate multiplication.
- The transformed data is written to the output stream (`outputy`).


## Design Overview

<div align="center">
    <img src="../../images/design2/Design.png" alt="Design" />
</div>




### AI Engine Array View


<div align="center">
    <img src="../../images/design2/AIE_graph.png" alt="AIE_graph" />
</div>



## Next Steps

After compiling the 100 compute unit N-Body Simulator design, you are ready to create the PL datamover kernels in the next module, [Module 03 - PL Design](../Module_03_pl).

