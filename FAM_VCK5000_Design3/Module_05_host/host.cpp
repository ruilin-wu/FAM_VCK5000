/***************************************************************************/
/*  Copyright (C) 2023 Advanced Micro Devices, Inc
 *  SPDX-License-Identifier: MIT
 *
 *  Example: For 8 input channels, read 1024 cfloat from local files (FAMDataIn_0.txt ~ FAMDataIn_7.txt)
 *  Copy to 8 BOs under the same group_id(0);
 *  Output a BO of 128×8192 cfloat.
 */
/***************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include "xrt.h"
#include "experimental/xrt_kernel.h"

//-----------------------------------------------
// Input/Output parameters
//-----------------------------------------------
static const int N_IN_CH = 16; // 16 input channels
static const int NSAMPLES = 512; // 512 cfloat per channel
static const int buffer_size = 8192; // intermediate buffer 8192 cfloat
static const int NPSIZE = 0; // set to non-zero if additional zeros are needed

static const int N_OUT_CH = 128; // If additional 0's are needed, set to non-zero.  
static const int NSAMP_OUT = 8192; // Each output channel 8192 cfloat

static const int ITER = 10; // Number of times the demo has been executed.

int main(int argc, char** argv)
{
    //-------------------------------------
    // 1) Open the device & load xclbin
    //-------------------------------------
    std::cout << "Open the device...\n";
    auto device = xrt::device(0);

    std::string xclbin = "../build.hw/fam.xclbin";
    std::cout << "Load the xclbin: " << xclbin << std::endl;
    auto uuid = device.load_xclbin(xclbin);

    //-------------------------------------
    // 2) Get references to datamovers compute units
    //-------------------------------------
    std::cout << "Get references to datamovers compute units\n";
    auto dma0 = xrt::kernel(device, uuid, "dma_hls:{dma_hls_0}");

    //-------------------------------------
    // 3) Define the size of input/output BOs
    //-------------------------------------
    // Input: Each channel (NSAMPLES+NPSIZE) cfloat, each cfloat=2×float => (NSAMPLES+NPSIZE)*2*sizeof(float)
    size_t in_size_bytes = (NSAMPLES + NPSIZE) * 2 * sizeof(float);

    // Input: Each channel (NSAMPLES+NPSIZE) cfloat, each cfloat=2×float => (NSAMPLES+NPSIZE)*2*sizeof(float)
    size_t trans_size_bytes = (buffer_size) * 2 * sizeof(float);

    // Output: 128 channels × 8192 cfloat × (2×float)
    size_t out_size_bytes = (size_t)N_OUT_CH * NSAMP_OUT * 2 * sizeof(float);

    std::cout << "[Info] Input BO size per channel = " << in_size_bytes
              << " bytes\n";
    std::cout << "[Info] Trans BO size = " << trans_size_bytes
              << " bytes\n";
    std::cout << "[Info] Output BO size = " << out_size_bytes
              << " bytes (for 128×8192 cfloat)\n";

    //-------------------------------------
    // 4) Allocate host buffer for each input channel sample_vector_in[ch]
    //    size = (NSAMPLES + NPSIZE)×2
    //-------------------------------------
    //    Read 1024 lines from FAMDataIn_ch.txt (if NPSIZE>0, pad with zeros)
    //-------------------------------------
    // Here we first open a two-dimensional container: sample_vector_in[ch], each shape [NSAMPLES+NPSIZE][2]
    std::vector<std::vector<float>> sample_vector_in(N_IN_CH,
        std::vector<float>((NSAMPLES + NPSIZE)*2, 0.0f)
    );

    // Read line by line from file
    for(int ch=0; ch<N_IN_CH; ch++){
        std::string fname = "./data/FAMDataIn_" + std::to_string(ch) + ".txt";
        std::ifstream fin(fname);
        if(!fin.is_open()){
            std::cerr<<"Error: Could not open "<< fname <<"\n";
            return 1;
        }

        float newvalr, newvali;
        for(size_t ii=0; ii< (size_t)(NSAMPLES + NPSIZE); ii++){
            if(ii < (size_t)NSAMPLES){
                if(!(fin >> newvalr >> newvali)){
                    // If the number of lines in the file is insufficient, perform default processing
                    newvalr = 0.0f;
                    newvali = 0.0f;
                }
                sample_vector_in[ch][2*ii  ] = newvalr;
                sample_vector_in[ch][2*ii+1] = newvali;
            }
            else {
                // If the number of lines in the file is insufficient, perform default processing
                sample_vector_in[ch][2*ii  ] = 0.0f;
                sample_vector_in[ch][2*ii+1] = 0.0f;
            }
        }
        fin.close();
    }

    //-------------------------------------
    // 5) Allocate BOs on FPGA global memory
    //-------------------------------------
    //    Because all are mapped to group_id(0) => MC_NOC0
    //-------------------------------------
    auto in_bohdl0 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl1 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl2 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl3 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl4 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl5 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl6 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl7 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl8 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl9 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl10 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl11 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl12 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl13 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl14 = xrt::bo(device, in_size_bytes, dma0.group_id(0));
    auto in_bohdl15 = xrt::bo(device, in_size_bytes, dma0.group_id(0));

    auto trans_bohdl  = xrt::bo(device, trans_size_bytes, dma0.group_id(0));

    auto out_bohdl  = xrt::bo(device, out_size_bytes, dma0.group_id(0));

    // Map to CPU pointers
    auto in_bomapped0 = in_bohdl0.map<float*>();
    auto in_bomapped1 = in_bohdl1.map<float*>();
    auto in_bomapped2 = in_bohdl2.map<float*>();
    auto in_bomapped3 = in_bohdl3.map<float*>();
    auto in_bomapped4 = in_bohdl4.map<float*>();
    auto in_bomapped5 = in_bohdl5.map<float*>();
    auto in_bomapped6 = in_bohdl6.map<float*>();
    auto in_bomapped7 = in_bohdl7.map<float*>();
    auto in_bomapped8 = in_bohdl8.map<float*>();
    auto in_bomapped9 = in_bohdl9.map<float*>();
    auto in_bomapped10 = in_bohdl10.map<float*>();
    auto in_bomapped11 = in_bohdl11.map<float*>();
    auto in_bomapped12 = in_bohdl12.map<float*>();
    auto in_bomapped13 = in_bohdl13.map<float*>();
    auto in_bomapped14 = in_bohdl14.map<float*>();
    auto in_bomapped15 = in_bohdl15.map<float*>();

    auto out_bomapped = out_bohdl.map<float*>();
    //auto trans_bomapped = trans_bohdl.map<float*>();
    //-------------------------------------
    // 6) Copy sample_vector_in[ch] into the corresponding in_bomappedX
    //    then sync to device
    //-------------------------------------
    // Here we manually write 8 segments; you can also make a loop
    {
        // ch0
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped0[k] = sample_vector_in[0][k];
        }
        in_bohdl0.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch1
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped1[k] = sample_vector_in[1][k];
        }
        in_bohdl1.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch2
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped2[k] = sample_vector_in[2][k];
        }
        in_bohdl2.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch3
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped3[k] = sample_vector_in[3][k];
        }
        in_bohdl3.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch4
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped4[k] = sample_vector_in[4][k];
        }
        in_bohdl4.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch5
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped5[k] = sample_vector_in[5][k];
        }
        in_bohdl5.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch6
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped6[k] = sample_vector_in[6][k];
        }
        in_bohdl6.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch7
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped7[k] = sample_vector_in[7][k];
        }
        in_bohdl7.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch8
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped8[k] = sample_vector_in[8][k];
        }
        in_bohdl8.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch9
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped9[k] = sample_vector_in[9][k];
        }
        in_bohdl9.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch10
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped10[k] = sample_vector_in[10][k];
        }
        in_bohdl10.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch11
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped11[k] = sample_vector_in[11][k];
        }
        in_bohdl11.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch12
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped12[k] = sample_vector_in[12][k];
        }
        in_bohdl12.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch13
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped13[k] = sample_vector_in[13][k];
        }
        in_bohdl13.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch14
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped14[k] = sample_vector_in[14][k];
        }
        in_bohdl14.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        // ch15
        for(size_t k=0; k < (size_t)(NSAMPLES + NPSIZE)*2; k++){
            in_bomapped15[k] = sample_vector_in[15][k];
        }
        in_bohdl15.sync(XCL_BO_SYNC_BO_TO_DEVICE, in_size_bytes, 0);

        
    }

    //-------------------------------------
    // 7) Launch Kernel
    //-------------------------------------
    std::cout << "Launching Kernel...\n";
    xrt::run dma_run0;
    auto t1 = std::chrono::high_resolution_clock::now();

    for(int i=0; i< ITER; i++){
        auto dma_run0 = dma0(
            in_bohdl0, in_bohdl1, in_bohdl2, in_bohdl3,
            in_bohdl4, in_bohdl5, in_bohdl6, in_bohdl7,
            in_bohdl8, in_bohdl9, in_bohdl10, in_bohdl11,
            in_bohdl12, in_bohdl13, in_bohdl14, in_bohdl15,

            trans_bohdl,out_bohdl,

            
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 16 DDR to AIE
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 16 AIE stage1 to PL TRANSPONSE
            
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 128 AIE stage2 to PL TRANSPONSE
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 128 AIE stage2 to PL TRANSPONSE
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            
            1 // iter
        );
        dma_run0.wait();
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    double dt_s = std::chrono::duration<double>(t2 - t1).count();
    std::cout << "Kernel done.\n";
    std::cout << "[Performance] average time= " << (dt_s/ITER) <<" s\n";
    double T = dt_s / ITER;  // Execution time of a single iteration (seconds)

    // Calculate the size of input and output data (MB)
    double input_data_MB = (double)(N_IN_CH * NSAMPLES * 2 * sizeof(float)) / (1024.0 * 1024.0);
    double output_data_MB = (double)(N_OUT_CH * NSAMP_OUT * 2 * sizeof(float)) / (1024.0 * 1024.0);
    double data_size_MB = input_data_MB + output_data_MB;  

    // Calculate throughput (GB/s)
    double throughput_GBps = data_size_MB / (T * 1000.0);

    // Calculate GFLOPS
    size_t total_FLOP = N_OUT_CH * NSAMP_OUT * 5;
    double gflops = total_FLOP / (T * 1e9);

    std::cout << "[Performance] Input Data: " << input_data_MB << " MB\n";
    std::cout << "[Performance] Output Data: " << output_data_MB << " MB\n";
    std::cout << "[Performance] Total Data: " << data_size_MB << " MB\n";
    std::cout << "[Performance] Throughput: " << throughput_GBps << " GB/s\n";
    std::cout << "[Performance] GFLOPS: " << gflops << " GFLOPS\n";

    //-------------------------------------
    // 8) Synchronize output FPGA->Host
    //-------------------------------------

    //trans_bohdl.sync(XCL_BO_SYNC_BO_FROM_DEVICE, trans_size_bytes, 0);
    out_bohdl.sync(XCL_BO_SYNC_BO_FROM_DEVICE, out_size_bytes, 0);

    //-------------------------------------
    // 9) Write the results to 128 FAMOut_x.txt files
    //    Each file contains 8192 lines of cfloat (real imag)
    //-------------------------------------

    int reorder[16] = {
    0, 8, 1, 9, 2, 10, 3, 11,  
    4, 12, 5, 13, 6, 14, 7, 15   
    };

    for (size_t c = 0; c < N_OUT_CH; c++) {
        std::string fname = "./output1/FAMOut_" + std::to_string(c) + ".txt";
        FILE* fout = fopen(fname.c_str(), "w");
        if (!fout) {
            std::cerr << "Error: cannot open " << fname << "\n";
            return 1;
        }
       
        for (size_t block = 0; block < NSAMP_OUT / 16; block+=2) {
            float temp[32];  
            for (size_t i = 0; i < 16; i++) {
                size_t idx = (c * NSAMP_OUT + block * 16 + i) * 2;
                temp[i * 2]     = out_bomapped[idx];     
                temp[i * 2 + 1] = out_bomapped[idx + 1]; 
            }            
            for (size_t i = 0; i < 16; i++) {
                size_t new_idx = reorder[i];  
                fprintf(fout, "%f %f\n", temp[new_idx * 2], temp[new_idx * 2 + 1]);
            }
        }      
        for (size_t block = 1; block < NSAMP_OUT / 16; block+=2) {
            float temp[32];             
            for (size_t i = 0; i < 16; i++) {
                size_t idx = (c * NSAMP_OUT + block * 16 + i) * 2;
                temp[i * 2]     = out_bomapped[idx];    
                temp[i * 2 + 1] = out_bomapped[idx + 1]; 
            }           
            for (size_t i = 0; i < 16; i++) {
                size_t new_idx = reorder[i];  
                fprintf(fout, "%f %f\n", temp[new_idx * 2], temp[new_idx * 2 + 1]);
            }
        }

        fclose(fout);
    }
  
    std::cout << "TEST PASS - Data saved in 128 files.\n";
    return 0;
}
