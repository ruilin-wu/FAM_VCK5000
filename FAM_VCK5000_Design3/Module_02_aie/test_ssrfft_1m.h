//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "fft_subsys.h"
#include "fam_sys.h"
#include "parameters.h"
#define PLFreq 520 //1000MHZ

using namespace adf ;


class dut: public graph {

public:

    static constexpr unsigned int stage1_kernel_num = 8*2;
    static constexpr unsigned int stage2_kernel_num = 32*4; 
  

    input_plio      stage1_in[stage1_kernel_num];
	output_plio    stage1_out[stage1_kernel_num];
	ssr_stage1_graph<40> stage1_graph;
    
    
    input_plio      stage2_in[stage2_kernel_num];
	output_plio    stage2_out[stage2_kernel_num];
	ssr_stage2_graph<1> stage2_graph;
    


    dut(){
        for (int i = 0; i < stage1_kernel_num; ++i) {
            std::string fname1_in = "data_stage1_x2/FAMDataIn_" + std::to_string(i) + ".txt";
            //std::string fname1_in = "data_stage1/FAMDataIn_0.txt";
            std::string fname1_out = "data_stage1_x2/FAMOut" + std::to_string(i) + ".txt";

            
            stage1_in[i] = input_plio::create("stage1_FAMDataIn_" + std::to_string(i), plio_64_bits, fname1_in, PLFreq);
            stage1_out[i] = output_plio::create("stage1_FAMOut" + std::to_string(i), plio_64_bits, fname1_out, PLFreq);

            
            connect<>(stage1_in[i].out[0], stage1_graph.in[i]);
            connect<>(stage1_graph.out[i], stage1_out[i].in[0]);

        }
        
        for (int i = 0; i < stage2_kernel_num; ++i) {
            std::string fname2_in = "data_stage2_full/FAMDataIn_" + std::to_string(i) + ".txt";
            //std::string fname2_in = "data_stage2_full/FAMDataIn_0.txt";
            std::string fname2_out = "data_stage2_full/FAMOut" + std::to_string(i) + ".txt";

            
            stage2_in[i] = input_plio::create("stage2_FAMDataIn_" + std::to_string(i), plio_64_bits, fname2_in, PLFreq);
            stage2_out[i] = output_plio::create("stage2_FAMOut" + std::to_string(i), plio_64_bits, fname2_out, PLFreq);

            
            connect<stream>(stage2_in[i].out[0], stage2_graph.in[i]);
            connect<stream>(stage2_graph.out[i], stage2_out[i].in[0]);
        }
        


	};

}; // end of class


