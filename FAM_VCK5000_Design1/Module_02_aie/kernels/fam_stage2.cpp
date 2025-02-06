//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "fam_stage2.h"
#include "fft_stages.h"
#include "fam_funcs.h"
#include "parameters.h"
// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

fam_stage2::fam_stage2(int xoff) : m_xoff(xoff)
{
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}



void fam_stage2::run(input_stream_cfloat* __restrict inputx0,
                      input_stream_cfloat* __restrict inputx1,
                     output_stream_cfloat * __restrict outputy)
{   
    int index1 = m_xoff ;
    int index;
    alignas(aie::vector_decl_align) v16cfloat xbuf0[2]; 
    alignas(aie::vector_decl_align) v16cfloat conj[2];  
    alignas(aie::vector_decl_align) v16cfloat ybuff[2];  
    alignas(aie::vector_decl_align) v16cfloat tmp2[2];
    alignas(aie::vector_decl_align) v16cfloat tmp1[2];

    const int n = 256; 
    if (index1 > 128)
    {
       index = index1 - 128;
    }
    else
    {
        index = index1;
    }
    
    
    if (index > 1) {
    for (int i = 0; i < index - 1; i++)
        chess_prepare_for_pipelining chess_loop_range(1,) 
    {   
        readincr_v<16>(inputx0);  
        readincr_v<16>(inputx1);                            
    }
    }
    
    if (index > 0) {
        xbuf0[0] = readincr_v<16>(inputx0);
        xbuf0[1] = readincr_v<16>(inputx1);
    }

    
    if (index < 128) {
        for (int i = 0; i < 128 - index; i++)
            chess_prepare_for_pipelining chess_loop_range(1,) 
        {   
            readincr_v<16>(inputx0);  
            readincr_v<16>(inputx1);
        }
    }
    
    
    for (int i = 0; i < n; i++)  
    chess_prepare_for_pipelining chess_loop_range(1,)  
    {   
        conj[0] = readincr_v<16>(inputx0);
        conj[1] = readincr_v<16>(inputx1);        
        stage2_cm((cfloat * restrict)xbuf0, (cfloat * restrict)conj, (cfloat * restrict)tmp1);
        FFT_32pt((cfloat * restrict)tmp1, (cfloat * restrict)tmp2, (cfloat * restrict)ybuff);       
        //190643000 ps
     
        
        writeincr(outputy, ext_v(ybuff[1], 4));
        writeincr(outputy, ext_v(ybuff[1], 5));
        writeincr(outputy, ext_v(ybuff[1], 6));
        writeincr(outputy, ext_v(ybuff[1], 7));

       
        writeincr(outputy, ext_v(ybuff[0], 0));
        writeincr(outputy, ext_v(ybuff[0], 1));
        writeincr(outputy, ext_v(ybuff[0], 2));
        writeincr(outputy, ext_v(ybuff[0], 3));             
    }
    


  
}


