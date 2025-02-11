# Building the Design



```
make all
```


# **README - Host Code for DMA HLS Kernel Execution**

## **Overview**
This project implements a **host application** to interact with the **DMA HLS kernel** on an **AMD Versal AI Engine platform**. The host code:
- Loads input data from **local files** (`FAMDataIn_0.txt` to `FAMDataIn_7.txt`).
- Transfers **8 input channels × 1024 cfloat** to the kernel.
- Receives **128 output channels × 8192 cfloat** from the kernel.
- Writes the processed data into **128 output files** (`FAMOut_0.txt` to `FAMOut_127.txt`).
- Measures **performance metrics** such as throughput and GFLOPS.

---

## **Execution Steps**
1. **Open the device** and load the compiled **xclbin**.
2. **Allocate Buffer Objects (BOs)** for input and output data.
3. **Read input data** from local files.
4. **Transfer input data to the FPGA**.
5. **Launch the kernel and wait for execution completion**.
6. **Retrieve processed data from the FPGA**.
7. **Save output data** into **128 text files**.
8. **Measure performance metrics**.

---

## **Performance Results**
After execution, the kernel performance is reported as follows:

```sh
Open the device...
Load the xclbin: ../build.hw/fam.xclbin
Get references to datamovers compute units
[Info] Input BO size per channel = 8192 bytes
[Info] Output BO size = 8388608 bytes (for 128×8192 cfloat)
Launching Kernel...
Kernel done.
[Performance] Average execution time = 0.00133521 s
[Performance] Input Data: 0.0625 MB
[Performance] Output Data: 8 MB
[Performance] Total Data: 8.0625 MB
[Performance] Throughput: 6.03837 GB/s
[Performance] GFLOPS: 3.92663 GFLOPS
```

---

## **Host Code (`host.cpp`)**
### **Key Functionalities**
- Reads **input data** from files and loads into FPGA memory.
- Configures **DMA HLS kernel** and executes it.
- Collects **output data** and saves to disk.
- Measures **execution time, throughput, and GFLOPS**.

---

## **How to Compile and Run**
### **1. Compile**
```sh
g++ -g -std=c++11 -Wall -Werror -O2 host.cpp -o host_exec -lxrt_core -lxrt_coreutil
```

### **2. Run the Application**
```sh
./host_exec
```

---

## **Generated Output Files**
- **`FAMOut_0.txt` → `FAMOut_127.txt`**:  
  Each file contains **8192 lines** of **complex float (cfloat)** values (`real imag` format).

---

## **Conclusion**
This host application provides an efficient interface for data movement between DDR and the **DMA HLS kernel**. The achieved **throughput of 6.04 GB/s** and **3.93 GFLOPS** demonstrate the efficiency of this approach for **FFT Accumulation Method (FAM) processing**.

🚀 **Ready to Deploy on AMD Versal!** 🚀