//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __FAM_FUNCS_H__ 
#define __FAM_FUNCS_H__
#include <adf.h>
#include "parameters.h"
#include "fft_twiddle_lut_dit_cfloat.h"


template <int stage, int TP_POINT_SIZE, int TP_START_RANK, int TP_END_RANK>
void INLINE_DECL opt_cfloat_stage_32pt(
    cfloat* xbuff, cfloat* obuff, cfloat** tmp_bufs, cfloat** tw_table, unsigned int& pingPong);

template <int stage, int TP_POINT_SIZE, int TP_START_RANK, int TP_END_RANK>
void INLINE_DECL opt_cfloat_stage_256pt(
    cfloat* xbuff, cfloat* obuff, cfloat** tmp_bufs, cfloat** tw_table, unsigned int& pingPong);




inline __attribute__((always_inline)) void FFT_256pt (cfloat * restrict xbuff, cfloat * restrict tmp1_buf, cfloat * restrict obuff)
{ 
  set_rnd(rnd_pos_inf); // Match the twiddle round mode of Matlab.
  set_sat();            // do saturate. 

  //alignas(aie::vector_decl_align) cfloat tmp1_buf[256];  
  alignas(aie::vector_decl_align) cfloat* tmp_bufs[2] = {(cfloat*)tmp1_buf, (cfloat*)xbuff};
  
  static constexpr int  TP_POINT_SIZE         = 256;
  static constexpr int  TP_START_RANK         = 0;
  static constexpr int  TP_END_RANK           = 8;
  unsigned int          pingPong              = 1;
  bool                  inv                   = false ;
  static constexpr cfloat* __restrict tw1 = (cfloat*)fft_lut_tw1_cfloat;
  static constexpr cfloat* __restrict tw2 = (cfloat*)fft_lut_tw2_cfloat;
  static constexpr cfloat* __restrict tw4 = (cfloat*)fft_lut_tw4_cfloat;
  static constexpr cfloat* __restrict tw8 = (cfloat*)fft_lut_tw8_cfloat;
  static constexpr cfloat* __restrict tw16 = (cfloat*)fft_lut_tw16_cfloat;
  static constexpr cfloat* __restrict tw32 = (cfloat*)fft_lut_tw32_cfloat;
  static constexpr cfloat* __restrict tw64 = (cfloat*)fft_lut_tw64_cfloat;
  static constexpr cfloat* __restrict tw128 = (cfloat*)fft_lut_tw128_cfloat;
  static cfloat* tw_table[24] = {tw1,tw2,tw4,tw8,tw16,tw32,tw64,tw128,NULL,NULL,NULL,NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

  opt_cfloat_stage_256pt<0, TP_POINT_SIZE, TP_START_RANK, TP_END_RANK>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_256pt<1, TP_POINT_SIZE, TP_START_RANK, TP_END_RANK>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_256pt<2, TP_POINT_SIZE, TP_START_RANK, TP_END_RANK>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_256pt<3, TP_POINT_SIZE, TP_START_RANK, TP_END_RANK>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_256pt<4, TP_POINT_SIZE, TP_START_RANK, TP_END_RANK>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_256pt<5, TP_POINT_SIZE, TP_START_RANK, TP_END_RANK>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_256pt<6, TP_POINT_SIZE, TP_START_RANK, TP_END_RANK>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_256pt<7, TP_POINT_SIZE, TP_START_RANK, TP_END_RANK>(xbuff, obuff, tmp_bufs, tw_table, pingPong);


//FFT shift
    if (TP_POINT_SIZE == 256) {
        // The final output is already in obuff
        cfloat* outptr   = obuff;
        // Use tmp_bufs[pingPong] as a temporary buffer
        cfloat* temp_buf = tmp_bufs[pingPong];

        // Copy the first 128 points to the temporary buffer
        for(int i = 0; i < (TP_POINT_SIZE/2)/16; i++)
            chess_prepare_for_pipelining
            chess_loop_range((TP_POINT_SIZE/2)/16, (TP_POINT_SIZE/2)/16)
        {
            v32float* src = (v32float*)(outptr + i*16);      // Source pointer pointing to the first half of obuff
            v32float* dst = (v32float*)(temp_buf + i*16);    // Destination pointer pointing to the temporary buffer
            *dst = *src;                                     // Copy data from obuff to the temporary buffer
        }

        // Move the last 128 points to the front
        for(int i = 0; i < (TP_POINT_SIZE/2)/16; i++)
            chess_prepare_for_pipelining
            chess_loop_range((TP_POINT_SIZE/2)/16, (TP_POINT_SIZE/2)/16)
        {
            v32float* src = (v32float*)(outptr + TP_POINT_SIZE/2 + i*16); // Source pointer pointing to the second half of obuff
            v32float* dst = (v32float*)(outptr + i*16);                   // Destination pointer pointing to the first half of obuff
            *dst = *src;                                                 // Move the second half data to the first half of obuff
        }

        // Move the first 128 points from the temporary buffer to the second half of obuff
        for(int i = 0; i < (TP_POINT_SIZE/2)/16; i++)
            chess_prepare_for_pipelining
            chess_loop_range((TP_POINT_SIZE/2)/16, (TP_POINT_SIZE/2)/16)
        {
            v32float* src = (v32float*)(temp_buf + i*16);                 // Source pointer pointing to the temporary buffer
            v32float* dst = (v32float*)(outptr + TP_POINT_SIZE/2 + i*16); // Destination pointer pointing to the second half of obuff
            *dst = *src;                                                 // Move data from the temporary buffer back to the second half of obuff
        }
    }
}



// https://github.com/Xilinx/Vitis_Libraries/blob/1db9f57db27d23d9124a6f069fa5546d0652cb70/dsp/L1/src/aie/fft_ifft_dit_1ch.cpp#L300
void FFT_32pt (cfloat * restrict xbuff, cfloat * restrict tmp2_buf, cfloat * restrict obuff)
{ 
  set_rnd(rnd_pos_inf); // Match the twiddle round mode of Matlab.
  set_sat();            // do saturate. 

  alignas(aie::vector_decl_align) cfloat* tmp_bufs[2] = {(cfloat*)tmp2_buf, (cfloat*)xbuff};
  
  
  //static constexpr int  TP_POINT_SIZE         = 32;
  //static constexpr int  TP_START_RANK         = 0;
  //static constexpr int  TP_END_RANK           = 5;
  unsigned int          pingPong              = 1;
  //bool                  inv                   = false ;
  static constexpr cfloat* __restrict tw1 = (cfloat*)fft_lut_tw1_cfloat;       
  static constexpr cfloat* __restrict tw2 = (cfloat*)fft_lut_tw2_cfloat;      
  static constexpr cfloat* __restrict tw4 = (cfloat*)fft_lut_tw4_cfloat;     
  static constexpr cfloat* __restrict tw8 = (cfloat*)fft_lut_tw8_cfloat;      
  static constexpr cfloat* __restrict tw16 = (cfloat*)fft_lut_tw16_cfloat;
  static cfloat* tw_table[24] = {tw1,tw2,tw4,tw8,tw16,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
  
  opt_cfloat_stage_32pt<0, 32, 0, 5>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_32pt<1, 32, 0, 5>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_32pt<2, 32, 0, 5>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_32pt<3, 32, 0, 5>(xbuff, obuff, tmp_bufs, tw_table, pingPong);
  opt_cfloat_stage_32pt<4, 32, 0, 5>(xbuff, obuff, tmp_bufs, tw_table, pingPong);

}

//https://github.com/Xilinx/Vitis_Libraries/blob/1db9f57db27d23d9124a6f069fa5546d0652cb70/dsp/L1/include/aie/fft_ifft_dit_1ch_utils.hpp#L357
//https://github.com/Xilinx/Vitis_Libraries/blob/1db9f57db27d23d9124a6f069fa5546d0652cb70/dsp/L1/include/aie/fft_ifft_dit_1ch_utils.hpp#L135
template <int stage, int TP_POINT_SIZE, int TP_START_RANK, int TP_END_RANK>
inline __attribute__((always_inline)) void opt_cfloat_stage_32pt(
    cfloat* xbuff, cfloat* obuff, cfloat** tmp_bufs, cfloat** tw_table, unsigned int& pingPong) {   
    //TP_END_RANK :256pt 8; 32pt 5
    cfloat* outptr = (stage == TP_END_RANK - 1) ? obuff : tmp_bufs[1 - pingPong];
    cfloat* inptr = (stage == TP_START_RANK) ? xbuff : tmp_bufs[pingPong];

    // Integrating the logic of stage_radix2_dit
    constexpr unsigned int kStageRadix = 2;
    constexpr unsigned int shift_tw = 0; // Assuming cfloat input, shift_tw is 0
    constexpr unsigned int TP_R = (TP_POINT_SIZE >> (1 + stage)); // Compute the FFT size for this stage

    using FFT = ::aie::fft_dit<TP_R, kStageRadix, cfloat, cfloat>; // Define FFT type
    FFT fft;
    auto it_stage = fft.begin_stage(inptr, tw_table[stage]);
    auto it_out0 = ::aie::begin_restrict_vector<FFT::out_vector_size>(outptr);
    auto it_out1 = ::aie::begin_restrict_vector<FFT::out_vector_size>(outptr + TP_POINT_SIZE / 2);
    
    for (int j = 0; j < TP_POINT_SIZE / (kStageRadix * FFT::out_vector_size); ++j)//19cycles
        //chess_prepare_for_pipelining chess_loop_range(1,) 
        chess_prepare_for_pipelining chess_flatten_loop 
        //flatten_loop is faster than chess_loop_range at 32pt, but 256pt requires chess_loop_range because chess_flatten_loop generates more commands
        {
            const auto out = fft.dit(*it_stage++, shift_tw, 0, false);
            *it_out0++ = out[0];
            *it_out1++ = out[1];
        }
    pingPong = 1 - pingPong;
}

template <int stage, int TP_POINT_SIZE, int TP_START_RANK, int TP_END_RANK>
inline __attribute__((always_inline)) void opt_cfloat_stage_256pt(
    cfloat* xbuff, cfloat* obuff, cfloat** tmp_bufs, cfloat** tw_table, unsigned int& pingPong) {   
    //TP_END_RANK :256pt 8; 32pt 5
    cfloat* outptr = (stage == TP_END_RANK - 1) ? obuff : tmp_bufs[1 - pingPong];
    cfloat* inptr = (stage == TP_START_RANK) ? xbuff : tmp_bufs[pingPong];

    // Integrating the logic of stage_radix2_dit
    constexpr unsigned int kStageRadix = 2;
    constexpr unsigned int shift_tw = 0; // Assuming cfloat input, shift_tw is 0
    constexpr unsigned int TP_R = (TP_POINT_SIZE >> (1 + stage)); // Compute the FFT size for this stage

    using FFT = ::aie::fft_dit<TP_R, kStageRadix, cfloat, cfloat>; // Define FFT type
    FFT fft;
    auto it_stage = fft.begin_stage(inptr, tw_table[stage]);
    auto it_out0 = ::aie::begin_restrict_vector<FFT::out_vector_size>(outptr);
    auto it_out1 = ::aie::begin_restrict_vector<FFT::out_vector_size>(outptr + TP_POINT_SIZE / 2);
    
    for (int j = 0; j < TP_POINT_SIZE / (kStageRadix * FFT::out_vector_size); ++j)//19cycles
        chess_prepare_for_pipelining chess_loop_range(1,) 
        //chess_prepare_for_pipelining chess_flatten_loop 
        //flatten_loop is faster than chess_loop_range at 32pt, but 256pt requires chess_loop_range because chess_flatten_loop generates more commands
        {
            const auto out = fft.dit(*it_stage++, shift_tw, 0, false);
            *it_out0++ = out[0];
            *it_out1++ = out[1];
        }


    pingPong = 1 - pingPong;
}


//
// 19 cycles
inline __attribute__((always_inline))  void stage2_cm (cfloat * restrict px0, cfloat * restrict px1,  cfloat * restrict py0)
{   
       
    v4cfloat * restrict po1 = (v4cfloat * restrict) py0;   
    v4cfloat * restrict pi0 = (v4cfloat * restrict) px0;   
    v4cfloat * restrict pi1 = (v4cfloat * restrict) px1;
    
    for (int j = 0; j < 32/4/2 ; ++j)  // 19cycles
        //chess_prepare_for_pipelining chess_loop_range(8,8) //加了延迟时间
        chess_prepare_for_pipelining chess_flatten_loop 
    {
        v4cfloat x1 = *pi0++;
        v4cfloat x2 = *pi1++;
        *po1++ = fpmul_nc(x1, x2);
        v4cfloat x3 = *pi0++;
        v4cfloat x4 = *pi1++;      
        *po1++ = fpmul_nc(x3, x4);
        
    }
}


inline __attribute__((always_inline)) void transpose_256(cfloat* restrict tmp2, cfloat* restrict ybuff, int index) {
    
    for (int i = 0; i < 256; i++) 
        chess_prepare_for_pipelining
        chess_loop_range(256,256)
    {
        ybuff[index + i*8] = tmp2[i];
    }
}

inline __attribute__((always_inline)) void window_fam (cfloat * restrict px0,  cfloat * restrict py0)
{   
    //static constexpr float* __restrict tw1 = (float*)window_factor;
    v8float * restrict ptw1 = (v8float * restrict) window_factor1;
    //v8float * restrict ptw2 = (v8float * restrict) (window_factor1 + 32/2);

    v8float * restrict pi1 = (v8float * restrict) px0;
    //v8float * restrict pi2 = (v8float * restrict) px0 + 32/2;

    v8float * restrict po1 = (v8float * restrict) py0;
    //v8float * restrict po2 = (v8float * restrict) py0 + 32/2;

    
    for (int j = 0; j < 32; ++j)  
        chess_prepare_for_pipelining chess_flatten_loop
    {
        
        v8float x1 = *pi1++;
        v8float x2 = *pi1++;      
        v8float coef1 = *ptw1++;
        v8float coef2 = *ptw1++;       
        
        *po1++ = fpmul(x1, coef1);
        *po1++ = fpmul(x2, coef2);

        
    }
}



inline __attribute__((always_inline)) void stage1_dc (cfloat * restrict px0, unsigned int index,  cfloat * restrict py0)
{   
       
    v4cfloat * restrict ptw;   
    switch (index) {
    case 0:
        ptw = (v4cfloat * restrict) dc_coef1;
        break;
    case 1: 
        ptw = (v4cfloat * restrict) dc_coef2;
        break;
    case 2: 
        ptw = (v4cfloat * restrict) dc_coef3;
        break;
    case 3: 
        ptw = (v4cfloat * restrict) dc_coef4;
        break;
    default:
        return;  
    }
    
    v4cfloat * restrict pi = (v4cfloat * restrict) px0;
    v4cfloat * restrict po = (v4cfloat * restrict) py0;   
    v4cfloat coef = *ptw;  
   

    
    for (int j = 0; j < 32 ; ++j)  
        chess_prepare_for_pipelining chess_flatten_loop
    {
        
        v4cfloat x1 = *pi++;
        v4cfloat x2 = *pi++;      
             
        *po++ = fpmul(x1, coef);
        *po++ = fpmul(x2, coef);
        
    }
}



inline __attribute__((always_inline)) void stage1 (cfloat * restrict px0, unsigned int n, cfloat * restrict py)
{
  v8cfloat * restrict po = (v8cfloat * restrict) py;
  v8cfloat * restrict pi = (v8cfloat * restrict) px0;

  for (int j = 0; j < (n/8); ++j)
    chess_prepare_for_pipelining
      chess_loop_range(1,)
    {
      po[j] = pi[j];
    }
}

inline __attribute__((always_inline)) void stage2 (cfloat * restrict px0,  cfloat * restrict py)
{
  v16cfloat * restrict po = (v16cfloat * restrict) py;
  v16cfloat * restrict pi = (v16cfloat * restrict) px0;

  for (int j = 0; j < (32/16/2); ++j)
    chess_prepare_for_pipelining
      chess_loop_range(1,)
    {
      
      *po++ = *pi++;
      *po++ = *pi++;
    }
}


#endif // __FAM_FUNCS_H__

