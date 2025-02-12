# Building the Design



```
make all
```

## **Overview**
This `host.cpp` implements a **host application** to interact with the **DMA HLS kernel** on an **AI Engine platform**. The host code:
- Loads input data from **local files** (`FAMDataIn_0.txt` to `FAMDataIn_7.txt`).
- Transfers **16 input channels × 512 cfloat** to the kernel.
- Receives **128 output channels × 8192 cfloat** from the kernel.
- Writes the processed data into **128 output files** (`FAMOut_0.txt` to `FAMOut_127.txt`).
- Measures **performance metrics** such as throughput and GFLOPS.



## **Execution Steps**
1. **Open the device** and load the compiled **xclbin**.
2. **Allocate Buffer Objects (BOs)** for input and output data.
3. **Read input data** from local files.
4. **Transfer input data to the FPGA**.
5. **Launch the kernel and wait for execution completion**.
6. **Retrieve processed data from the FPGA**.
7. **Save output data** into **128 text files**.
8. **Measure performance metrics**.



## **Performance Results**


To execute the **FAM kernel**, navigate to the **Module_05_host** directory and run the following command:

```sh
cd Module_05_host
make run
```

This command compiles and runs the **host application (`fam.exe`)**, which loads the **FAM kernel (`fam.xclbin`)** and executes it. The kernel processes **16 input channels × 512 cfloat** and produces **128 output channels × 8192 cfloat**. 

Upon execution, the following performance results were reported:

```sh
FAM_VCK5000_design3/Module_05_host$ make run
./fam.exe ../build.hw/fam.xclbin
Open the device...
Load the xclbin: ../build.hw/fam.xclbin
Get references to datamovers compute units
[Info] Input BO size per channel = 4096 bytes
[Info] Trans BO size = 65536 bytes
[Info] Output BO size = 8388608 bytes (for 128×8192 cfloat)
Launching Kernel...
Kernel done.
[Performance] average time= 0.00150527 s
[Performance] Input Data: 0.0625 MB
[Performance] Output Data: 8 MB
[Performance] Total Data: 8.0625 MB
[Performance] Throughput: 5.35617 GB/s
[Performance] GFLOPS: 3.48301 GFLOPS
TEST PASS - Data saved in 128 files.
```


## **Generated Output Files**
- **`FAMOut_0.txt` → `FAMOut_127.txt`**:  
  Each file contains **8192 lines** of **complex float (cfloat)** values (`real imag` format).


## **Conclusion**
This host application provides an efficient interface for data movement between DDR and the **DMA HLS kernel**. The achieved **throughput of 5.35617 GB/s** and **3.48301 GFLOPS** demonstrate the efficiency of this approach for **FFT Accumulation Method (FAM) processing**. And **Average execution time** is **0.00150527s**.

