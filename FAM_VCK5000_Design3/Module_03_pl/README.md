

# Building the Design



```
make all
```


## HLS PL Kernel
As shown in figure below, the processing pipeline consists of three main components:

- **DDR is used as the data source** to store input data `in_bohdl0~15`, and the data is transmitted to PL (FPGA programmable logic) through **16 × 64-bit streams @ 500MHz**.
- **PL transmits data to AIE through AXI-Stream**, and inputs the data to `stage1_FAMDataIn0~15` for the first stage calculation.
- **After AIE (stage 1) calculation is completed**, the result data `stage1_FAMOut0~15` is transmitted back to PL.
- **PL stores the first stage calculation result in `trans_bohdl`** of DDR as an intermediate buffer.
- **PL reads `trans_bohdl` and reformats the data**, converting it into **128 × 64-bit streams @ 500MHz**, and then transmits it to AIE (stage 2) `stage2_FAMDataIn0~127`.
- **After AIE (stage 2) calculation**, output `stage2_FAMOut_0~127` is sent back to PL.
- **PL writes the second stage calculation result to DDR (`out_bohdl`)** for further storage or transmission.

<div align="center">
    <img src="../../images/design3/dma_hls.png" alt="dma" />
</div>


## **Code explanation**
### **HLS Kernel: `dma_hls.cpp`**
The `dma_hls` function is the top-level HLS kernel that performs:
1. **Reading Input Data from DDR (`memin0` to `memin7`)**
- Uses `ap_uint<512>` to read **8 cfloat values** at once.
- Writes to **8 separate AXI streams (`FAMDataIn_0` to `FAMDataIn_7`)**.

2. **AIE Computation (via AXI Streams)**
- Streams **input data to AI Engine** for FFT Accumulation Processing.
- Receives **128 output streams** from AI Engine.

3. **Writing Output Data to DDR (`memout`)**
- Reads **128 output streams (`FAMOut_0` to `FAMOut_127`)**.
- Packs data into `ap_uint<512>` to perform **high-bandwidth writes**.
- Uses loop optimizations for efficient memory access.

### **AXI Interface Configuration**
```cpp
#pragma HLS INTERFACE m_axi offset=slave bundle=gmem0 port=memin0 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS INTERFACE axis port=FAMDataIn_0
#pragma HLS INTERFACE axis port=FAMOut_0
#pragma HLS PIPELINE II=1
```
- Uses **AXI Master (`m_axi`)** for memory accesses.
- Uses **AXI Stream (`axis`)** for fast data movement.
- Applies **loop pipelining** to maximize throughput.

---

### **Performance Metrics**
| Metric               | Value                 |
|----------------------|----------------------|
| **Input Data Rate**  | `8 × 64-bit @ 500MHz` |
| **Output Data Rate** | `128 × 64-bit @ 500MHz` |
| **Memory Write Rate** | `1 × 512-bit @ 500MHz` |

---




## Next Steps

After compiling the PL datamover kernels, we are ready to link the entire hardware design together in the next module, [Module 04 - Hardware Link](../Module_04_hw_link).

