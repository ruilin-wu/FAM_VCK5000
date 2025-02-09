//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class ssr_fft_fp_a {
public:
  // Constructor:
  ssr_fft_fp_a(void);

  // Run:
  void run( input_buffer_1d<int,1024,0>& __restrict inputx0,
            input_buffer_1d<int,1024,0>& __restrict inputx1,
            output_buffer_1d<int,2048>& __restrict outputy );

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ssr_fft_fp_a::run );
  }
};

