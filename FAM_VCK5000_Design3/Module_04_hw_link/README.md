## **Block Diagram**
The following diagram illustrates the **high-throughput dataflow** between DDR, PL, and AI Engine:

<div align="center">
    <img src="../../images/design3/dma_hls1.png" alt="dma" />
</div>

## **Connectivity Configuration**
The following **`config.cfg`** file ensures proper **stream connections** between **DMA HLS, AI Engine, and DDR**:

```ini
[connectivity]
nk=dma_hls:1:dma_hls_0

# DDR to AI Engine (Stage 1)
stream_connect=dma_hls_0.stage1_FAMDataIn_0:ai_engine_0.stage1_FAMDataIn_0
stream_connect=dma_hls_0.stage1_FAMDataIn_1:ai_engine_0.stage1_FAMDataIn_1
...
stream_connect=dma_hls_0.stage1_FAMDataIn_15:ai_engine_0.stage1_FAMDataIn_15

# AI Engine (Stage 1) to PL Transpose
stream_connect=ai_engine_0.stage1_FAMOut0:dma_hls_0.stage1_FAMOut0
stream_connect=ai_engine_0.stage1_FAMOut1:dma_hls_0.stage1_FAMOut1
...
stream_connect=ai_engine_0.stage1_FAMOut15:dma_hls_0.stage1_FAMOut15

# PL Transpose to AI Engine (Stage 2)
stream_connect=dma_hls_0.stage2_FAMDataIn_0:ai_engine_0.stage2_FAMDataIn_0
stream_connect=dma_hls_0.stage2_FAMDataIn_1:ai_engine_0.stage2_FAMDataIn_1
...
stream_connect=dma_hls_0.stage2_FAMDataIn_127:ai_engine_0.stage2_FAMDataIn_127

# AI Engine (Stage 2) to DDR
stream_connect=ai_engine_0.stage2_FAMOut0:dma_hls_0.stage2_FAMOut0
stream_connect=ai_engine_0.stage2_FAMOut1:dma_hls_0.stage2_FAMOut1
...
stream_connect=ai_engine_0.stage2_FAMOut127:dma_hls_0.stage2_FAMOut127

# Memory Mapping to NoC for Optimized DDR Bandwidth
sp=dma_hls_0.memin0:MC_NOC0
sp=dma_hls_0.memtrans:MC_NOC0
sp=dma_hls_0.memout:MC_NOC0
```

### **Summary**
- **Input:** `8 × 64-bit` streams at **500MHz** from DDR.
- **Processing:** **FFT Accumulation Method (FAM)** expands the data into `128 × 64-bit` streams at **500MHz**.
- **Output:** Processed data is packed into a **single `1 × 512-bit` AXI stream** at **500MHz**, then written to DDR.
- **Streams are connected via AXI-Stream interfaces** between **DMA HLS, AI Engine, and DDR**.
- **Memory controllers (`MC_NOC0`) are used** to efficiently access DDR.

This **optimized connectivity** ensures **high-throughput data movement** between **memory, AI Engine, and processing logic**. 🚀


## Next Steps

After linking the PL and AIE kernels, we are ready to create a host application in the next module, [Module 05 - Host App](../Module_05_host).