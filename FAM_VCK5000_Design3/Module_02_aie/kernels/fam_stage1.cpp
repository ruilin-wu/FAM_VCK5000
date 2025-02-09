//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "fam_stage1.h"
#include "fft_stages.h"
#include "fam_funcs.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

fam_stage1::fam_stage1(int xoff) : m_xoff(xoff)  
{
    aie::set_rounding(aie::rounding_mode::symmetric_inf);
    aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void fam_stage1::run( input_buffer_1d<cfloat,512,0>& __restrict inputx0,
                        output_buffer_1d<cfloat,512>& __restrict outputy )
{ 
  //int index = m_xoff & 3;
  int index =(m_xoff & 1) ? 2 : 0;
  alignas(aie::vector_decl_align) cfloat  tmp1[256];  
  
  cfloat * restrict xbuf0 =  (cfloat * restrict) inputx0.data();
  cfloat * restrict ybuff =  (cfloat * restrict) outputy.data();
  for (int i = 0; i < 1; i++)
    chess_prepare_for_pipelining chess_loop_range(1,)
  {
  // Processing First 256 points
  window_fam(xbuf0, (cfloat * restrict)tmp1);
  FFT_256pt((cfloat * restrict)tmp1, ybuff, xbuf0);
  stage1_dc(xbuf0, index, ybuff);
  
  // Processing Second 256 points
  window_fam(xbuf0 + 256, (cfloat * restrict)tmp1);
  FFT_256pt((cfloat * restrict)tmp1, ybuff + 256, xbuf0 + 256);
  stage1_dc(xbuf0 + 256, (index + 1), ybuff + 256);  

  reorder512(ybuff);  
  }
}

