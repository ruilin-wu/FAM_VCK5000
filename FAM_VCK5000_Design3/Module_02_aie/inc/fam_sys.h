#ifndef __SSCA_SYS_H__
#define __SSCA_SYS_H__

#include <adf.h>

#include "fft_subsys.h"
#include "parameters.h"
#include "fam_stage1.h"
#include "fam_stage2.h"
using namespace adf;

template<int xoff, int yoff>
class stage1: public graph {
public:
  kernel core;
  port<input>  in;
  port<output> out;

  stage1(void) {
    // Create FFT kernels
    core = kernel::create_object<fam_stage1>(xoff);
    runtime<ratio>(core) = 0.8;
    source(core) = "fam_stage1.cpp";

    // Make connections
    connect<>(in, core.in[0]);
    connect<>(core.out[0], out);
// 设定位置和分配的坐标
#define TWD_LOC(xloc, yoff)  location<kernel>   (core)          =  tile(xloc+0, yoff+0); \
                             location<stack>    (core)          =  bank(xloc+0, yoff+0, 2); \
                            location<buffer>(core.out[0])      = {address(xloc+0, yoff, 0x0000), address(xloc+0, yoff, 0x2000)}; \
                            location<buffer>(core.in[0])      = {address(xloc+0, yoff, 0x1000), address(xloc+0, yoff, 0x3000)};   
    TWD_LOC(xoff , yoff);

#undef 	TWD_LOC

  };
};


template<int xoff, int yoff>
class stage2: public graph {   
public:
  kernel core;
  port<input>  in;
  port<output> out;

  stage2(void) {
    // Create FFT kernels
    core = kernel::create_object<fam_stage2>();
    runtime<ratio>(core) = 0.8;
    source(core) = "fam_stage2.cpp";

    // Make connections
    connect<>(in, core.in[0]);      // 指定32点的window大小
    connect<>(core.out[0], out);    // 指定32点的window大小
    // 设定一种链接方式


// 设定位置和分配的坐标
#define TWD_LOC(xloc, yoff)  location<kernel>   (core)          =  tile(xloc+0, yoff+0); \
                             location<stack>    (core)          =  bank(xloc+0, yoff+0, 0);    
    TWD_LOC(xoff , yoff);

#undef 	TWD_LOC

  };
};




template<int xoff, int yoff>
class ssr_stage2_graph_x32: public graph {
public:

  stage2<xoff+0,yoff>  k0;
  stage2<xoff+1,yoff>  k1;
  stage2<xoff+2,yoff>  k2;
  stage2<xoff+3,yoff>  k3;
  stage2<xoff+4,yoff>  k4;
  stage2<xoff+5,yoff>  k5;
  stage2<xoff+6,yoff>  k6;
  stage2<xoff+7,yoff>  k7;
  stage2<xoff+8,yoff>  k8;
  stage2<xoff+9,yoff>  k9;
  stage2<xoff+10,yoff>  k10;
  stage2<xoff+11,yoff>  k11;
  stage2<xoff+12,yoff>  k12;
  stage2<xoff+13,yoff>  k13;
  stage2<xoff+14,yoff>  k14;
  stage2<xoff+15,yoff>  k15;
  stage2<xoff+16,yoff>  k16;
  stage2<xoff+17,yoff>  k17;
  stage2<xoff+18,yoff>  k18;
  stage2<xoff+19,yoff>  k19;
  stage2<xoff+20,yoff>  k20;
  stage2<xoff+21,yoff>  k21;
  stage2<xoff+22,yoff>  k22;
  stage2<xoff+23,yoff>  k23;
  stage2<xoff+24,yoff>  k24;
  stage2<xoff+25,yoff>  k25;
  stage2<xoff+26,yoff>  k26;
  stage2<xoff+27,yoff>  k27;
  stage2<xoff+28,yoff>  k28;
  stage2<xoff+29,yoff>  k29;
  stage2<xoff+30,yoff>  k30;
  stage2<xoff+31,yoff>  k31;
  

  port<input>  in[32];
  port<output> out[32];


  ssr_stage2_graph_x32() {

    
    connect<>(in[0], k0.in);
    connect<>(in[1], k1.in);
    connect<>(in[2], k2.in);
    connect<>(in[3], k3.in);
    connect<>(in[4], k4.in);
    connect<>(in[5], k5.in);
    connect<>(in[6], k6.in);
    connect<>(in[7], k7.in);
    connect<>(in[8], k8.in);
    connect<>(in[9], k9.in);
    connect<>(in[10], k10.in);
    connect<>(in[11], k11.in);
    connect<>(in[12], k12.in);
    connect<>(in[13], k13.in);
    connect<>(in[14], k14.in);
    connect<>(in[15], k15.in);
    connect<>(in[16], k16.in);
    connect<>(in[17], k17.in);
    connect<>(in[18], k18.in);
    connect<>(in[19], k19.in);
    connect<>(in[20], k20.in);
    connect<>(in[21], k21.in);
    connect<>(in[22], k22.in);
    connect<>(in[23], k23.in);
    connect<>(in[24], k24.in);
    connect<>(in[25], k25.in);
    connect<>(in[26], k26.in);
    connect<>(in[27], k27.in);
    connect<>(in[28], k28.in);
    connect<>(in[29], k29.in);
    connect<>(in[30], k30.in);
    connect<>(in[31], k31.in);


    connect<>(k0.out, out[0]);
    connect<>(k1.out, out[1]);
    connect<>(k2.out, out[2]);
    connect<>(k3.out, out[3]);
    connect<>(k4.out, out[4]);
    connect<>(k5.out, out[5]);
    connect<>(k6.out, out[6]);
    connect<>(k7.out, out[7]);
    connect<>(k8.out, out[8]);
    connect<>(k9.out, out[9]);
    connect<>(k10.out, out[10]);
    connect<>(k11.out, out[11]);
    connect<>(k12.out, out[12]);
    connect<>(k13.out, out[13]);
    connect<>(k14.out, out[14]);
    connect<>(k15.out, out[15]);
    connect<>(k16.out, out[16]);
    connect<>(k17.out, out[17]);
    connect<>(k18.out, out[18]);
    connect<>(k19.out, out[19]);
    connect<>(k20.out, out[20]);
    connect<>(k21.out, out[21]);
    connect<>(k22.out, out[22]);
    connect<>(k23.out, out[23]);
    connect<>(k24.out, out[24]);
    connect<>(k25.out, out[25]);
    connect<>(k26.out, out[26]);
    connect<>(k27.out, out[27]);
    connect<>(k28.out, out[28]);
    connect<>(k29.out, out[29]);
    connect<>(k30.out, out[30]);
    connect<>(k31.out, out[31]); 
  };
};

template<int xoff, int yoff>
class ssr_stage1_graph_x8: public graph {
public:

  stage1<xoff+0,yoff>  k0;
  stage1<xoff+1,yoff>  k1;
  stage1<xoff+2,yoff>  k2;
  stage1<xoff+3,yoff>  k3;
  stage1<xoff+4,yoff>  k4;
  stage1<xoff+5,yoff>  k5;
  stage1<xoff+6,yoff>  k6;
  stage1<xoff+7,yoff>  k7;
  
  port<input>  in[8];
  port<output> out[8];


  ssr_stage1_graph_x8() {   
    connect<>(in[0], k0.in);
    connect<>(in[1], k1.in);
    connect<>(in[2], k2.in);
    connect<>(in[3], k3.in);
    connect<>(in[4], k4.in);
    connect<>(in[5], k5.in);
    connect<>(in[6], k6.in);
    connect<>(in[7], k7.in);
    connect<>(k0.out, out[0]);
    connect<>(k1.out, out[1]);
    connect<>(k2.out, out[2]);
    connect<>(k3.out, out[3]);
    connect<>(k4.out, out[4]);
    connect<>(k5.out, out[5]);
    connect<>(k6.out, out[6]);
    connect<>(k7.out, out[7]);
    
  };
};

template<int xoff>
class ssr_stage2_graph: public graph {
public:

  ssr_stage2_graph_x32<xoff,0> stage2_graph_x32a;
  ssr_stage2_graph_x32<xoff,1> stage2_graph_x32b;
  ssr_stage2_graph_x32<xoff,2> stage2_graph_x32c;
  ssr_stage2_graph_x32<xoff,3> stage2_graph_x32d;
  

  port<input>        in[32*4];
  port<output>     out[ 32*4]; 
  ssr_stage2_graph() {
    for(int i=0; i<32; i++){
      connect<>(in[i], stage2_graph_x32a.in[i]);
      connect<>(stage2_graph_x32a.out[i], out[i]);
      connect<>(in[i+32], stage2_graph_x32b.in[i]);
      connect<>(stage2_graph_x32b.out[i], out[i+32]);
      connect<>(in[i+64], stage2_graph_x32c.in[i]);
      connect<>(stage2_graph_x32c.out[i], out[i+64]);
      connect<>(in[i+96], stage2_graph_x32d.in[i]);
      connect<>(stage2_graph_x32d.out[i], out[i+96]);
      
    }
  };
};

template<int xoff>
class ssr_stage1_graph: public graph {
public:

  ssr_stage1_graph_x8<xoff,0> stage1_graph_x8a;
  ssr_stage1_graph_x8<xoff,1> stage1_graph_x8b;
  //ssr_stage1_graph_x8<xoff,2> stage1_graph_x8c;
  //ssr_stage1_graph_x8<xoff,6> stage1_graph_x8d;
  
  port<input>        in[8*2];
  port<output>     out[ 8*2]; 
  ssr_stage1_graph() {
    for(int i=0; i<8; i++){
      connect<>(in[i], stage1_graph_x8a.in[i]);
      connect<>(stage1_graph_x8a.out[i], out[i]);
      connect<>(in[i+8], stage1_graph_x8b.in[i]);
      connect<>(stage1_graph_x8b.out[i], out[i+8]);
      //connect<stream>(in[i+16], stage1_graph_x8c.in[i]);
      //connect<stream>(stage1_graph_x8c.out[i], out[i+16]);
      //connect<stream>(in[i+24], stage1_graph_x8d.in[i]);
      //connect<stream>(stage1_graph_x8d.out[i], out[i+24]);
     
    }
  };
};


#endif //__SSCA_SYS_H__

