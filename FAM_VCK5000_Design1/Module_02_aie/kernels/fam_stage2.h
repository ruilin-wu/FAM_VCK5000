//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

class fam_stage2 {
private:
    int m_xoff;  
public:
  // Constructor:
  fam_stage2(int xoff);

  // Run:
  void run(input_stream<cfloat>* __restrict inputx0,
           input_stream<cfloat>* __restrict inputx1,
                     output_stream<cfloat> * __restrict outputy);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( fam_stage2::run );
  }
};

