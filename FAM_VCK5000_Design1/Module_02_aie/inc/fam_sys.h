#ifndef __SSCA_SYS_H__
#define __SSCA_SYS_H__

#include <adf.h>

#include "fft_subsys.h"
#include "parameters.h"
#include "fam_stage1.h"
#include "fam_stage2.h"
#include "conv_stage1.h"
#include "conv_stage2.h"
using namespace adf;

template<int xoff, int yoff, int index>
class stage1_s1: public graph {
public:
  kernel core;
  port<input>  in0;
  port<input>  in1;
  port<output> out;

  stage1_s1(void) {
    // Create FFT kernels
    core = kernel::create_object<fam_stage1>(index);
    runtime<ratio>(core) = 0.99;
    source(core) = "fam_stage1.cpp";

    // Make connections
    connect<>(in0, core.in[0]);
    connect<>(in1, core.in[1]);
    connect<>(core.out[0], out);

#define TWD_LOC_0(xloc, yoff)  \
    location<kernel>(core) = tile(xloc+0, yoff+0); \
    location<stack>(core) = bank(xloc+1, yoff+0, 0); \
    location<buffer>(core.out[0]) = {bank(xloc+0, yoff-1, 0), bank(xloc+0, yoff-1, 2)};\
    location<buffer>(core.in[0]) = {bank(xloc+0, yoff+0, 0), bank(xloc+0, yoff+0, 1)}; \
    location<buffer>(core.in[1]) = {bank(xloc+0, yoff+0, 2), bank(xloc+0, yoff+0, 3)};
    TWD_LOC_0(xoff, yoff);
#undef TWD_LOC_0

    };
};

template<int xoff, int yoff, int index>
class stage1_s2: public graph {
public:
  kernel core;
  port<input>  in0;
  port<input>  in1;
  port<output> out;

  stage1_s2(void) {
    // Create FFT kernels
    core = kernel::create_object<fam_stage1>(index);
    runtime<ratio>(core) = 0.99;
    source(core) = "fam_stage1.cpp";

    // Make connections
    connect<>(in0, core.in[0]);
    connect<>(in1, core.in[1]);
    connect<>(core.out[0], out);
// 根据buffer_loc定义不同的宏
#define TWD_LOC_0(xloc, yoff)  \
    location<kernel>(core) = tile(xloc+0, yoff+0); \
    location<stack>(core) = bank(xloc+0, yoff+0, 0); \
    location<buffer>(core.out[0]) = {bank(xloc-1, yoff+0, 0), bank(xloc-1, yoff+0, 2)};\
    location<buffer>(core.in[0]) =  {bank(xloc+0, yoff+1, 0), bank(xloc+0, yoff+1, 1)}; \
    location<buffer>(core.in[1]) =  {bank(xloc+0, yoff+1, 2), bank(xloc+0, yoff+1, 3)};
    TWD_LOC_0(xoff, yoff);
#undef TWD_LOC_0

    };
};


template<int xoff, int yoff,int window_size>
class convergent_stage1: public graph {
public:
  kernel core;
  port<input>  in[2];
  port<output> out[2];

  convergent_stage1(void) {
    // Create FFT kernels
    core = kernel::create_object<conv_stage1>(window_size);
    runtime<ratio>(core) = 0.99;
    source(core) = "conv_stage1.cpp";

    // Make connections
    connect<>(in[0], core.in[0]);
    connect<>(in[1], core.in[1]);

    connect<>(core.out[0], out[0]);
    connect<>(core.out[1], out[1]);
// 设定位置和分配的坐标
#define TWD_LOC(xloc, yoff)  location<kernel>   (core)          =  tile(xloc+0, yoff+0); \
                             location<stack>    (core)          =  bank(xloc+0, yoff+1, 2);\
                             location<buffer>(core.in[0])      = {bank(xloc-1, yoff+0, 0), bank(xloc-1, yoff+0, 2)};\
                             location<buffer>(core.in[1])      = {bank(xloc+0, yoff+0, 0), bank(xloc+0, yoff+0, 2)};  
                            //location<buffer>(core.out[0])      = {address(xloc+0, yoff, 0x0000), address(xloc+0, yoff, 0x2000)}; \
                            // 
    TWD_LOC(xoff , yoff);

#undef 	TWD_LOC

  };
};

template<int xoff, int yoff>
class stage1_graph_x2: public graph {
public:
  stage1_s1<xoff+0,yoff+1,0>  k8;
  stage1_s2<xoff+2,yoff+0,1>  k9;
  convergent_stage1<xoff+1,yoff+0,1024>  c_1;

  stage1_s1<xoff+3,yoff+1,0>  k10;
  stage1_s2<xoff+5,yoff+0,1>  k11;
  convergent_stage1<xoff+4,yoff+0,1024>  c_2;

  port<input>  in[8];
  port<output> out0[2];
  port<output> out1[2];

  stage1_graph_x2(void) {
    // Create FFT kernels
    connect<>(in[0], k8.in0);
    connect<>(in[1], k8.in1);
    connect<>(in[2], k9.in0);
    connect<>(in[3], k9.in1);
    connect<>(k8.out, c_1.in[0]);
    connect<>(k9.out, c_1.in[1]);
    connect<>(c_1.out[0], out0[0]);
    connect<>(c_1.out[1], out0[1]);

    connect<>(in[4], k10.in0);
    connect<>(in[5], k10.in1);
    connect<>(in[6], k11.in0);
    connect<>(in[7], k11.in1);
    connect<>(k10.out, c_2.in[0]);
    connect<>(k11.out, c_2.in[1]);
    connect<>(c_2.out[0], out1[0]);
    connect<>(c_2.out[1], out1[1]);
    

  };
};

template<int xoff, int yoff,int index>
class stage2: public graph {   
public:
  kernel core;
  port<input>  in0;
  port<input>  in1;
  port<output> out;

  stage2(void) {
    // Create FFT kernels
    core = kernel::create_object<fam_stage2>(index);
    runtime<ratio>(core) = 0.99;
    source(core) = "fam_stage2.cpp";

    // Make connections
    connect<>(in0, async(core.in[0]));      // 指定32点的window大小
    connect<>(in1, async(core.in[1]));      // 指定32点的window大小
    //connect<>(in0, core.in[0]);
    //connect<>(in1, core.in[1]);
    connect<>(core.out[0], out);    // 指定32点的window大小
    // 设定一种链接方式


// 设定位置和分配的坐标
#define TWD_LOC(xloc, yoff)  location<kernel>   (core)          =  tile(xloc+0, yoff+0); \
                             location<stack>    (core)          =  bank(xloc+0, yoff+0, 0);    
    TWD_LOC(xoff , yoff);

#undef 	TWD_LOC

  };
};



template<int xoff, int yoff,int index>
class convergent_stage2: public graph {   
public:
  kernel core;
  port<input>  in0;
  port<input>  in1;
  port<output> out;

  convergent_stage2(void) {
    // Create FFT kernels
    core = kernel::create_object<conv_stage2>(index);
    runtime<ratio>(core) = 0.99;
    source(core) = "conv_stage2.cpp";

    // Make connections
    connect<>(in0, core.in[0]);      // 指定32点的window大小
    connect<>(in1, core.in[1]);      // 指定32点的window大小
    connect<>(core.out[0], out);    // 指定32点的window大小
    // 设定一种链接方式


// 设定位置和分配的坐标
#define TWD_LOC(xloc, yoff)  location<kernel>   (core)          =  tile(xloc+0, yoff+0); \
                             location<stack>    (core)          =  bank(xloc+0, yoff+0, 0);    
    TWD_LOC(xoff , yoff);

#undef 	TWD_LOC

  };
};


template<int xoff, int yoff,int index1,int index2>
class stage2_graph_x3_vertical: public graph {
public:
  stage2<xoff+0,yoff+1,index1>  k1;
  stage2<xoff+0,yoff-1,index2>  k3;
  convergent_stage2<xoff+0,yoff+0,1024>   k2;


  port<input>  in[2];
  port<output> out[1];

  stage2_graph_x3_vertical(void) {
    // Create FFT kernels
    //connect<>(in[0], k1.in0);
    //connect<>(in[0], k3.in0);
    //connect<>(in[1], k1.in1);
    //connect<>(in[1], k3.in1);

    connect<>(in[0], k1.in0);
    connect<>(in[0], k3.in0);
    connect<>(in[1], k1.in1);
    connect<>(in[1], k3.in1);

    connect<>(k1.out, k2.in0);
    connect<>(k3.out, k2.in1);
    connect<>(k2.out, out[0]);
  };
};

template<int xoff, int yoff,int index1,int index2>
class stage2_graph_x3_horizon: public graph {
public:
  stage2<xoff-1,yoff+0,index1>  k1;
  stage2<xoff+1,yoff+0,index2>  k3;
  convergent_stage2<xoff+0,yoff+0,1024>   k2;


  port<input>  in[2];
  port<output> out[1];

  stage2_graph_x3_horizon(void) {
    // Create FFT kernels
    //connect<>(in[0], k1.in0);
    //connect<>(in[0], k3.in0);
    //connect<>(in[1], k1.in1);
    //connect<>(in[1], k3.in1);
    connect<>(in[0], k1.in0);
    connect<>(in[0], k3.in0);
    connect<>(in[1], k1.in1);
    connect<>(in[1], k3.in1);
    connect<>(k1.out, k2.in0);
    connect<>(k3.out, k2.in1);
    connect<>(k2.out, out[0]);
  };
};



template<int xoff>
class stage2_graph_x3_x128: public graph {
public:
  //行7左
  stage2_graph_x3_horizon<1,7,1,2>     k_1_2;
  stage2_graph_x3_horizon<4,7,3,4>     k_3_4;
  stage2_graph_x3_horizon<7,7,5,6>     k_5_6;
  stage2_graph_x3_horizon<10,7,7,8>    k_7_8;
  stage2_graph_x3_horizon<13,7,9,10>   k_9_10;
  stage2_graph_x3_horizon<16,7,11,12>  k_11_12;
  stage2_graph_x3_horizon<19,7,13,14>  k_13_14;
  //行7右
  stage2_graph_x3_horizon<30,7,129,130>     k_129_130;
  stage2_graph_x3_horizon<33,7,131,132>     k_131_132;
  stage2_graph_x3_horizon<36,7,133,134>     k_133_134;
  stage2_graph_x3_horizon<39,7,135,136>    k_135_136;
  stage2_graph_x3_horizon<42,7,137,138>   k_137_138;
  stage2_graph_x3_horizon<45,7,139,140>  k_139_140;
  stage2_graph_x3_horizon<48,7,141,142>  k_141_142;
  //行6左
  stage2_graph_x3_horizon<1,6,15,16>  k_15_16;
  stage2_graph_x3_horizon<4,6,17,18>  k_17_18;
  stage2_graph_x3_horizon<7,6,19,20>  k_19_20;
  stage2_graph_x3_horizon<10,6,21,22>  k_21_22;
  stage2_graph_x3_horizon<13,6,23,24>  k_23_24;
  stage2_graph_x3_horizon<16,6,25,26>  k_25_26;
  stage2_graph_x3_horizon<19,6,27,28>  k_27_28;
  //行6右
  stage2_graph_x3_horizon<30,6,143,144>  k_143_144;
  stage2_graph_x3_horizon<33,6,145,146>  k_145_146;
  stage2_graph_x3_horizon<36,6,147,148>  k_147_148;
  stage2_graph_x3_horizon<39,6,149,150>  k_149_150;
  stage2_graph_x3_horizon<42,6,151,152>  k_151_152;
  stage2_graph_x3_horizon<45,6,153,154>  k_153_154;
  stage2_graph_x3_horizon<48,6,155,156>  k_155_156;
  //行3，4，5左
  stage2_graph_x3_vertical<0,4,29,30>   k_29_30;
  stage2_graph_x3_vertical<1,4,31,32>   k_31_32;
  stage2_graph_x3_vertical<2,4,33,34>   k_33_34;
  stage2_graph_x3_vertical<3,4,35,36>   k_35_36;
  stage2_graph_x3_vertical<4,4,37,38>   k_37_38;
  stage2_graph_x3_vertical<5,4,39,40>   k_39_40;
  stage2_graph_x3_vertical<6,4,41,42>   k_41_42;
  stage2_graph_x3_vertical<7,4,43,44>   k_43_44;
  stage2_graph_x3_vertical<8,4,45,46>   k_45_46;
  stage2_graph_x3_vertical<9,4,47,48>   k_47_48;
  stage2_graph_x3_vertical<10,4,49,50>   k_49_50;
  stage2_graph_x3_vertical<11,4,51,52>   k_51_52;
  stage2_graph_x3_vertical<12,4,53,54>   k_53_54;
  stage2_graph_x3_vertical<13,4,55,56>   k_55_56;
  stage2_graph_x3_vertical<14,4,57,58>   k_57_58;
  stage2_graph_x3_vertical<15,4,59,60>   k_59_60;
  stage2_graph_x3_vertical<16,4,61,62>   k_61_62;
  stage2_graph_x3_vertical<17,4,63,64>   k_63_64;
  stage2_graph_x3_vertical<18,4,65,66>   k_65_66;
  stage2_graph_x3_vertical<19,4,67,68>   k_67_68;
  stage2_graph_x3_vertical<20,4,69,70>  k_69_70;
  stage2_graph_x3_vertical<21,4,71,72>  k_71_72;
  stage2_graph_x3_vertical<22,4,73,74>  k_73_74;
  stage2_graph_x3_vertical<23,4,75,76>  k_75_76;
  stage2_graph_x3_vertical<24,4,77,78>  k_77_78;
  //行3，4，5右
  stage2_graph_x3_vertical<25,4,157,158>   k_157_158;
  stage2_graph_x3_vertical<26,4,159,160>   k_159_160;
  stage2_graph_x3_vertical<27,4,161,162>   k_161_162;
  stage2_graph_x3_vertical<28,4,163,164>   k_163_164;
  stage2_graph_x3_vertical<29,4,165,166>   k_165_166;
  stage2_graph_x3_vertical<30,4,167,168>   k_167_168;
  stage2_graph_x3_vertical<31,4,169,170>   k_169_170;
  stage2_graph_x3_vertical<32,4,171,172>   k_171_172;
  stage2_graph_x3_vertical<33,4,173,174>   k_173_174;
  stage2_graph_x3_vertical<34,4,175,176>   k_175_176;
  stage2_graph_x3_vertical<35,4,177,178>   k_177_178;
  stage2_graph_x3_vertical<36,4,179,180>   k_179_180;
  stage2_graph_x3_vertical<37,4,181,182>   k_181_182;
  stage2_graph_x3_vertical<38,4,183,184>   k_183_184;
  stage2_graph_x3_vertical<39,4,185,186>   k_185_186;
  stage2_graph_x3_vertical<40,4,187,188>   k_187_188;
  stage2_graph_x3_vertical<41,4,189,190>   k_189_190;
  stage2_graph_x3_vertical<42,4,191,192>   k_191_192;
  stage2_graph_x3_vertical<43,4,193,194>   k_193_194;
  stage2_graph_x3_vertical<44,4,195,196>   k_195_196;
  stage2_graph_x3_vertical<45,4,197,198>  k_197_198;
  stage2_graph_x3_vertical<46,4,199,200>  k_199_200;
  stage2_graph_x3_vertical<47,4,201,202>  k_201_202;
  stage2_graph_x3_vertical<48,4,203,204>  k_203_204;
  stage2_graph_x3_vertical<49,4,205,206>  k_205_206;
  //行0，1，2左
  stage2_graph_x3_vertical<0,1,79,80>   k_79_80;
  stage2_graph_x3_vertical<1,1,81,82>   k_81_82;
  stage2_graph_x3_vertical<2,1,83,84>   k_83_84;
  stage2_graph_x3_vertical<3,1,85,86>   k_85_86;
  stage2_graph_x3_vertical<4,1,87,88>   k_87_88;
  stage2_graph_x3_vertical<5,1,89,90>   k_89_90;
  stage2_graph_x3_vertical<6,1,91,92>   k_91_92;
  stage2_graph_x3_vertical<7,1,93,94>   k_93_94;
  stage2_graph_x3_vertical<8,1,95,96>   k_95_96;
  stage2_graph_x3_vertical<9,1,97,98>   k_97_98;
  stage2_graph_x3_vertical<10,1,99,100>   k_99_100;
  stage2_graph_x3_vertical<11,1,101,102>   k_101_102;
  stage2_graph_x3_vertical<12,1,103,104>   k_103_104;
  stage2_graph_x3_vertical<13,1,105,106>   k_105_106;
  stage2_graph_x3_vertical<14,1,107,108>   k_107_108;
  stage2_graph_x3_vertical<15,1,109,110>   k_109_110;
  stage2_graph_x3_vertical<16,1,111,112>   k_111_112;
  stage2_graph_x3_vertical<17,1,113,114>   k_113_114;
  stage2_graph_x3_vertical<18,1,115,116>   k_115_116;
  stage2_graph_x3_vertical<19,1,117,118>   k_117_118;
  stage2_graph_x3_vertical<20,1,119,120>  k_119_120;
  stage2_graph_x3_vertical<21,1,121,122>  k_121_122;
  stage2_graph_x3_vertical<22,1,123,124>  k_123_124;
  stage2_graph_x3_vertical<23,1,125,126>  k_125_126;
  stage2_graph_x3_vertical<24,1,127,128>  k_127_128;
  //行0，1，2右
  stage2_graph_x3_vertical<25,1,207,208>   k_207_208;
  stage2_graph_x3_vertical<26,1,209,210>   k_209_210;
  stage2_graph_x3_vertical<27,1,211,212>   k_211_212;
  stage2_graph_x3_vertical<28,1,213,214>   k_213_214;
  stage2_graph_x3_vertical<29,1,215,216>   k_215_216;
  stage2_graph_x3_vertical<30,1,217,218>   k_217_218;
  stage2_graph_x3_vertical<31,1,219,220>   k_219_220;
  stage2_graph_x3_vertical<32,1,221,222>   k_221_222;
  stage2_graph_x3_vertical<33,1,223,224>   k_223_224;
  stage2_graph_x3_vertical<34,1,225,226>   k_225_226;
  stage2_graph_x3_vertical<35,1,227,228>   k_227_228;
  stage2_graph_x3_vertical<36,1,229,230>   k_229_230;
  stage2_graph_x3_vertical<37,1,231,232>   k_231_232;
  stage2_graph_x3_vertical<38,1,233,234>   k_233_234;
  stage2_graph_x3_vertical<39,1,235,236>   k_235_236;
  stage2_graph_x3_vertical<40,1,237,238>   k_237_238;
  stage2_graph_x3_vertical<41,1,239,240>   k_239_240;
  stage2_graph_x3_vertical<42,1,241,242>   k_241_242;
  stage2_graph_x3_vertical<43,1,243,244>   k_243_244;
  stage2_graph_x3_vertical<44,1,245,246>   k_245_246;
  stage2_graph_x3_vertical<45,1,247,248>   k_247_248;
  stage2_graph_x3_vertical<46,1,249,250>   k_249_250;
  stage2_graph_x3_vertical<47,1,251,252>   k_251_252;
  stage2_graph_x3_vertical<48,1,253,254>   k_253_254;
  stage2_graph_x3_vertical<49,1,255,256>   k_255_256;
  



  port<input>  in0[2];
  port<input>  in1[2];
  port<output> out[128];

  stage2_graph_x3_x128(void) {
    // Create FFT kernels
  connect<>(in0[0], k_1_2.in[0]);
  connect<>(in0[0], k_3_4.in[0]);
  connect<>(in0[0], k_5_6.in[0]);
  connect<>(in0[0], k_7_8.in[0]);
  connect<>(in0[0], k_9_10.in[0]);
  connect<>(in0[0], k_11_12.in[0]);
  connect<>(in0[0], k_13_14.in[0]);
  connect<>(in0[0], k_15_16.in[0]);
  connect<>(in0[0], k_17_18.in[0]);
  connect<>(in0[0], k_19_20.in[0]);
  connect<>(in0[0], k_21_22.in[0]);
  connect<>(in0[0], k_23_24.in[0]);
  connect<>(in0[0], k_25_26.in[0]);
  connect<>(in0[0], k_27_28.in[0]);
  connect<>(in0[0], k_29_30.in[0]);
  connect<>(in0[0], k_31_32.in[0]);
  connect<>(in0[0], k_33_34.in[0]);
  connect<>(in0[0], k_35_36.in[0]);
  connect<>(in0[0], k_37_38.in[0]);
  connect<>(in0[0], k_39_40.in[0]);
  connect<>(in0[0], k_41_42.in[0]);
  connect<>(in0[0], k_43_44.in[0]);
  connect<>(in0[0], k_45_46.in[0]);
  connect<>(in0[0], k_47_48.in[0]);
  connect<>(in0[0], k_49_50.in[0]);
  connect<>(in0[0], k_51_52.in[0]);
  connect<>(in0[0], k_53_54.in[0]);
  connect<>(in0[0], k_55_56.in[0]);
  connect<>(in0[0], k_57_58.in[0]);
  connect<>(in0[0], k_59_60.in[0]);
  connect<>(in0[0], k_61_62.in[0]);
  connect<>(in0[0], k_63_64.in[0]);
  connect<>(in0[0], k_65_66.in[0]);
  connect<>(in0[0], k_67_68.in[0]);
  connect<>(in0[0], k_69_70.in[0]);
  connect<>(in0[0], k_71_72.in[0]);
  connect<>(in0[0], k_73_74.in[0]);
  connect<>(in0[0], k_75_76.in[0]);
  connect<>(in0[0], k_77_78.in[0]);
  connect<>(in0[0], k_79_80.in[0]);
  connect<>(in0[0], k_81_82.in[0]);
  connect<>(in0[0], k_83_84.in[0]);
  connect<>(in0[0], k_85_86.in[0]);
  connect<>(in0[0], k_87_88.in[0]);
  connect<>(in0[0], k_89_90.in[0]);
  connect<>(in0[0], k_91_92.in[0]);
  connect<>(in0[0], k_93_94.in[0]);
  connect<>(in0[0], k_95_96.in[0]);
  connect<>(in0[0], k_97_98.in[0]);
  connect<>(in0[0], k_99_100.in[0]);
  connect<>(in0[0], k_101_102.in[0]);
  connect<>(in0[0], k_103_104.in[0]);
  connect<>(in0[0], k_105_106.in[0]);
  connect<>(in0[0], k_107_108.in[0]);
  connect<>(in0[0], k_109_110.in[0]);
  connect<>(in0[0], k_111_112.in[0]);
  connect<>(in0[0], k_113_114.in[0]);
  connect<>(in0[0], k_115_116.in[0]);
  connect<>(in0[0], k_117_118.in[0]);
  connect<>(in0[0], k_119_120.in[0]);
  connect<>(in0[0], k_121_122.in[0]);
  connect<>(in0[0], k_123_124.in[0]);
  connect<>(in0[0], k_125_126.in[0]);
  connect<>(in0[0], k_127_128.in[0]);
  connect<>(in0[1], k_129_130.in[0]);
  connect<>(in0[1], k_131_132.in[0]);
  connect<>(in0[1], k_133_134.in[0]);
  connect<>(in0[1], k_135_136.in[0]);
  connect<>(in0[1], k_137_138.in[0]);
  connect<>(in0[1], k_139_140.in[0]);
  connect<>(in0[1], k_141_142.in[0]);
  connect<>(in0[1], k_143_144.in[0]);
  connect<>(in0[1], k_145_146.in[0]);
  connect<>(in0[1], k_147_148.in[0]);
  connect<>(in0[1], k_149_150.in[0]);
  connect<>(in0[1], k_151_152.in[0]);
  connect<>(in0[1], k_153_154.in[0]);
  connect<>(in0[1], k_155_156.in[0]);
  connect<>(in0[1], k_157_158.in[0]);
  connect<>(in0[1], k_159_160.in[0]);
  connect<>(in0[1], k_161_162.in[0]);
  connect<>(in0[1], k_163_164.in[0]);
  connect<>(in0[1], k_165_166.in[0]);
  connect<>(in0[1], k_167_168.in[0]);
  connect<>(in0[1], k_169_170.in[0]);
  connect<>(in0[1], k_171_172.in[0]);
  connect<>(in0[1], k_173_174.in[0]);
  connect<>(in0[1], k_175_176.in[0]);
  connect<>(in0[1], k_177_178.in[0]);
  connect<>(in0[1], k_179_180.in[0]);
  connect<>(in0[1], k_181_182.in[0]);
  connect<>(in0[1], k_183_184.in[0]);
  connect<>(in0[1], k_185_186.in[0]);
  connect<>(in0[1], k_187_188.in[0]);
  connect<>(in0[1], k_189_190.in[0]);
  connect<>(in0[1], k_191_192.in[0]);
  connect<>(in0[1], k_193_194.in[0]);
  connect<>(in0[1], k_195_196.in[0]);
  connect<>(in0[1], k_197_198.in[0]);
  connect<>(in0[1], k_199_200.in[0]);
  connect<>(in0[1], k_201_202.in[0]);
  connect<>(in0[1], k_203_204.in[0]);
  connect<>(in0[1], k_205_206.in[0]);
  connect<>(in0[1], k_207_208.in[0]);
  connect<>(in0[1], k_209_210.in[0]);
  connect<>(in0[1], k_211_212.in[0]);
  connect<>(in0[1], k_213_214.in[0]);
  connect<>(in0[1], k_215_216.in[0]);
  connect<>(in0[1], k_217_218.in[0]);
  connect<>(in0[1], k_219_220.in[0]);
  connect<>(in0[1], k_221_222.in[0]);
  connect<>(in0[1], k_223_224.in[0]);
  connect<>(in0[1], k_225_226.in[0]);
  connect<>(in0[1], k_227_228.in[0]);
  connect<>(in0[1], k_229_230.in[0]);
  connect<>(in0[1], k_231_232.in[0]);
  connect<>(in0[1], k_233_234.in[0]);
  connect<>(in0[1], k_235_236.in[0]);
  connect<>(in0[1], k_237_238.in[0]);
  connect<>(in0[1], k_239_240.in[0]);
  connect<>(in0[1], k_241_242.in[0]);
  connect<>(in0[1], k_243_244.in[0]);
  connect<>(in0[1], k_245_246.in[0]);
  connect<>(in0[1], k_247_248.in[0]);
  connect<>(in0[1], k_249_250.in[0]);
  connect<>(in0[1], k_251_252.in[0]);
  connect<>(in0[1], k_253_254.in[0]);
  connect<>(in0[1], k_255_256.in[0]);



  connect<>(in1[0], k_1_2.in[1]);
  connect<>(in1[0], k_3_4.in[1]);
  connect<>(in1[0], k_5_6.in[1]);
  connect<>(in1[0], k_7_8.in[1]);
  connect<>(in1[0], k_9_10.in[1]);
  connect<>(in1[0], k_11_12.in[1]);
  connect<>(in1[0], k_13_14.in[1]);
  connect<>(in1[0], k_15_16.in[1]);
  connect<>(in1[0], k_17_18.in[1]);
  connect<>(in1[0], k_19_20.in[1]);
  connect<>(in1[0], k_21_22.in[1]);
  connect<>(in1[0], k_23_24.in[1]);
  connect<>(in1[0], k_25_26.in[1]);
  connect<>(in1[0], k_27_28.in[1]);
  connect<>(in1[0], k_29_30.in[1]);
  connect<>(in1[0], k_31_32.in[1]);
  connect<>(in1[0], k_33_34.in[1]);
  connect<>(in1[0], k_35_36.in[1]);
  connect<>(in1[0], k_37_38.in[1]);
  connect<>(in1[0], k_39_40.in[1]);
  connect<>(in1[0], k_41_42.in[1]);
  connect<>(in1[0], k_43_44.in[1]);
  connect<>(in1[0], k_45_46.in[1]);
  connect<>(in1[0], k_47_48.in[1]);
  connect<>(in1[0], k_49_50.in[1]);
  connect<>(in1[0], k_51_52.in[1]);
  connect<>(in1[0], k_53_54.in[1]);
  connect<>(in1[0], k_55_56.in[1]);
  connect<>(in1[0], k_57_58.in[1]);
  connect<>(in1[0], k_59_60.in[1]);
  connect<>(in1[0], k_61_62.in[1]);
  connect<>(in1[0], k_63_64.in[1]);
  connect<>(in1[0], k_65_66.in[1]);
  connect<>(in1[0], k_67_68.in[1]);
  connect<>(in1[0], k_69_70.in[1]);
  connect<>(in1[0], k_71_72.in[1]);
  connect<>(in1[0], k_73_74.in[1]);
  connect<>(in1[0], k_75_76.in[1]);
  connect<>(in1[0], k_77_78.in[1]);
  connect<>(in1[0], k_79_80.in[1]);
  connect<>(in1[0], k_81_82.in[1]);
  connect<>(in1[0], k_83_84.in[1]);
  connect<>(in1[0], k_85_86.in[1]);
  connect<>(in1[0], k_87_88.in[1]);
  connect<>(in1[0], k_89_90.in[1]);
  connect<>(in1[0], k_91_92.in[1]);
  connect<>(in1[0], k_93_94.in[1]);
  connect<>(in1[0], k_95_96.in[1]);
  connect<>(in1[0], k_97_98.in[1]);
  connect<>(in1[0], k_99_100.in[1]);
  connect<>(in1[0], k_101_102.in[1]);
  connect<>(in1[0], k_103_104.in[1]);
  connect<>(in1[0], k_105_106.in[1]);
  connect<>(in1[0], k_107_108.in[1]);
  connect<>(in1[0], k_109_110.in[1]);
  connect<>(in1[0], k_111_112.in[1]);
  connect<>(in1[0], k_113_114.in[1]);
  connect<>(in1[0], k_115_116.in[1]);
  connect<>(in1[0], k_117_118.in[1]);
  connect<>(in1[0], k_119_120.in[1]);
  connect<>(in1[0], k_121_122.in[1]);
  connect<>(in1[0], k_123_124.in[1]);
  connect<>(in1[0], k_125_126.in[1]);
  connect<>(in1[0], k_127_128.in[1]);
  connect<>(in1[1], k_129_130.in[1]);
  connect<>(in1[1], k_131_132.in[1]);
  connect<>(in1[1], k_133_134.in[1]);
  connect<>(in1[1], k_135_136.in[1]);
  connect<>(in1[1], k_137_138.in[1]);
  connect<>(in1[1], k_139_140.in[1]);
  connect<>(in1[1], k_141_142.in[1]);
  connect<>(in1[1], k_143_144.in[1]);
  connect<>(in1[1], k_145_146.in[1]);
  connect<>(in1[1], k_147_148.in[1]);
  connect<>(in1[1], k_149_150.in[1]);
  connect<>(in1[1], k_151_152.in[1]);
  connect<>(in1[1], k_153_154.in[1]);
  connect<>(in1[1], k_155_156.in[1]);
  connect<>(in1[1], k_157_158.in[1]);
  connect<>(in1[1], k_159_160.in[1]);
  connect<>(in1[1], k_161_162.in[1]);
  connect<>(in1[1], k_163_164.in[1]);
  connect<>(in1[1], k_165_166.in[1]);
  connect<>(in1[1], k_167_168.in[1]);
  connect<>(in1[1], k_169_170.in[1]);
  connect<>(in1[1], k_171_172.in[1]);
  connect<>(in1[1], k_173_174.in[1]);
  connect<>(in1[1], k_175_176.in[1]);
  connect<>(in1[1], k_177_178.in[1]);
  connect<>(in1[1], k_179_180.in[1]);
  connect<>(in1[1], k_181_182.in[1]);
  connect<>(in1[1], k_183_184.in[1]);
  connect<>(in1[1], k_185_186.in[1]);
  connect<>(in1[1], k_187_188.in[1]);
  connect<>(in1[1], k_189_190.in[1]);
  connect<>(in1[1], k_191_192.in[1]);
  connect<>(in1[1], k_193_194.in[1]);
  connect<>(in1[1], k_195_196.in[1]);
  connect<>(in1[1], k_197_198.in[1]);
  connect<>(in1[1], k_199_200.in[1]);
  connect<>(in1[1], k_201_202.in[1]);
  connect<>(in1[1], k_203_204.in[1]);
  connect<>(in1[1], k_205_206.in[1]);
  connect<>(in1[1], k_207_208.in[1]);
  connect<>(in1[1], k_209_210.in[1]);
  connect<>(in1[1], k_211_212.in[1]);
  connect<>(in1[1], k_213_214.in[1]);
  connect<>(in1[1], k_215_216.in[1]);
  connect<>(in1[1], k_217_218.in[1]);
  connect<>(in1[1], k_219_220.in[1]);
  connect<>(in1[1], k_221_222.in[1]);
  connect<>(in1[1], k_223_224.in[1]);
  connect<>(in1[1], k_225_226.in[1]);
  connect<>(in1[1], k_227_228.in[1]);
  connect<>(in1[1], k_229_230.in[1]);
  connect<>(in1[1], k_231_232.in[1]);
  connect<>(in1[1], k_233_234.in[1]);
  connect<>(in1[1], k_235_236.in[1]);
  connect<>(in1[1], k_237_238.in[1]);
  connect<>(in1[1], k_239_240.in[1]);
  connect<>(in1[1], k_241_242.in[1]);
  connect<>(in1[1], k_243_244.in[1]);
  connect<>(in1[1], k_245_246.in[1]);
  connect<>(in1[1], k_247_248.in[1]);
  connect<>(in1[1], k_249_250.in[1]);
  connect<>(in1[1], k_251_252.in[1]);
  connect<>(in1[1], k_253_254.in[1]);
  connect<>(in1[1], k_255_256.in[1]);


  connect<>(k_1_2.out[0], out[0]);
  connect<>(k_3_4.out[0], out[1]);
  connect<>(k_5_6.out[0], out[2]);
  connect<>(k_7_8.out[0], out[3]);
  connect<>(k_9_10.out[0], out[4]);
  connect<>(k_11_12.out[0], out[5]);
  connect<>(k_13_14.out[0], out[6]);
  connect<>(k_15_16.out[0], out[7]);
  connect<>(k_17_18.out[0], out[8]);
  connect<>(k_19_20.out[0], out[9]);
  connect<>(k_21_22.out[0], out[10]);
  connect<>(k_23_24.out[0], out[11]);
  connect<>(k_25_26.out[0], out[12]);
  connect<>(k_27_28.out[0], out[13]);
  connect<>(k_29_30.out[0], out[14]);
  connect<>(k_31_32.out[0], out[15]);
  connect<>(k_33_34.out[0], out[16]);
  connect<>(k_35_36.out[0], out[17]);
  connect<>(k_37_38.out[0], out[18]);
  connect<>(k_39_40.out[0], out[19]);
  connect<>(k_41_42.out[0], out[20]);
  connect<>(k_43_44.out[0], out[21]);
  connect<>(k_45_46.out[0], out[22]);
  connect<>(k_47_48.out[0], out[23]);
  connect<>(k_49_50.out[0], out[24]);
  connect<>(k_51_52.out[0], out[25]);
  connect<>(k_53_54.out[0], out[26]);
  connect<>(k_55_56.out[0], out[27]);
  connect<>(k_57_58.out[0], out[28]);
  connect<>(k_59_60.out[0], out[29]);
  connect<>(k_61_62.out[0], out[30]);
  connect<>(k_63_64.out[0], out[31]);
  connect<>(k_65_66.out[0], out[32]);
  connect<>(k_67_68.out[0], out[33]);
  connect<>(k_69_70.out[0], out[34]);
  connect<>(k_71_72.out[0], out[35]);
  connect<>(k_73_74.out[0], out[36]);
  connect<>(k_75_76.out[0], out[37]);
  connect<>(k_77_78.out[0], out[38]);
  connect<>(k_79_80.out[0], out[39]);
  connect<>(k_81_82.out[0], out[40]);
  connect<>(k_83_84.out[0], out[41]);
  connect<>(k_85_86.out[0], out[42]);
  connect<>(k_87_88.out[0], out[43]);
  connect<>(k_89_90.out[0], out[44]);
  connect<>(k_91_92.out[0], out[45]);
  connect<>(k_93_94.out[0], out[46]);
  connect<>(k_95_96.out[0], out[47]);
  connect<>(k_97_98.out[0], out[48]);
  connect<>(k_99_100.out[0], out[49]);
  connect<>(k_101_102.out[0], out[50]);
  connect<>(k_103_104.out[0], out[51]);
  connect<>(k_105_106.out[0], out[52]);
  connect<>(k_107_108.out[0], out[53]);
  connect<>(k_109_110.out[0], out[54]);
  connect<>(k_111_112.out[0], out[55]);
  connect<>(k_113_114.out[0], out[56]);
  connect<>(k_115_116.out[0], out[57]);
  connect<>(k_117_118.out[0], out[58]);
  connect<>(k_119_120.out[0], out[59]);
  connect<>(k_121_122.out[0], out[60]);
  connect<>(k_123_124.out[0], out[61]);
  connect<>(k_125_126.out[0], out[62]);
  connect<>(k_127_128.out[0], out[63]);
  connect<>(k_129_130.out[0], out[64]);
  connect<>(k_131_132.out[0], out[65]);
  connect<>(k_133_134.out[0], out[66]);
  connect<>(k_135_136.out[0], out[67]);
  connect<>(k_137_138.out[0], out[68]);
  connect<>(k_139_140.out[0], out[69]);
  connect<>(k_141_142.out[0], out[70]);
  connect<>(k_143_144.out[0], out[71]);
  connect<>(k_145_146.out[0], out[72]);
  connect<>(k_147_148.out[0], out[73]);
  connect<>(k_149_150.out[0], out[74]);
  connect<>(k_151_152.out[0], out[75]);
  connect<>(k_153_154.out[0], out[76]);
  connect<>(k_155_156.out[0], out[77]);
  connect<>(k_157_158.out[0], out[78]);
  connect<>(k_159_160.out[0], out[79]);
  connect<>(k_161_162.out[0], out[80]);
  connect<>(k_163_164.out[0], out[81]);
  connect<>(k_165_166.out[0], out[82]);
  connect<>(k_167_168.out[0], out[83]);
  connect<>(k_169_170.out[0], out[84]);
  connect<>(k_171_172.out[0], out[85]);
  connect<>(k_173_174.out[0], out[86]);
  connect<>(k_175_176.out[0], out[87]);
  connect<>(k_177_178.out[0], out[88]);
  connect<>(k_179_180.out[0], out[89]);
  connect<>(k_181_182.out[0], out[90]);
  connect<>(k_183_184.out[0], out[91]);
  connect<>(k_185_186.out[0], out[92]);
  connect<>(k_187_188.out[0], out[93]);
  connect<>(k_189_190.out[0], out[94]);
  connect<>(k_191_192.out[0], out[95]);
  connect<>(k_193_194.out[0], out[96]);
  connect<>(k_195_196.out[0], out[97]);
  connect<>(k_197_198.out[0], out[98]);
  connect<>(k_199_200.out[0], out[99]);
  connect<>(k_201_202.out[0], out[100]);
  connect<>(k_203_204.out[0], out[101]);
  connect<>(k_205_206.out[0], out[102]);
  connect<>(k_207_208.out[0], out[103]);
  connect<>(k_209_210.out[0], out[104]);
  connect<>(k_211_212.out[0], out[105]);
  connect<>(k_213_214.out[0], out[106]);
  connect<>(k_215_216.out[0], out[107]);
  connect<>(k_217_218.out[0], out[108]);
  connect<>(k_219_220.out[0], out[109]);
  connect<>(k_221_222.out[0], out[110]);
  connect<>(k_223_224.out[0], out[111]);
  connect<>(k_225_226.out[0], out[112]);
  connect<>(k_227_228.out[0], out[113]);
  connect<>(k_229_230.out[0], out[114]);
  connect<>(k_231_232.out[0], out[115]);
  connect<>(k_233_234.out[0], out[116]);
  connect<>(k_235_236.out[0], out[117]);
  connect<>(k_237_238.out[0], out[118]);
  connect<>(k_239_240.out[0], out[119]);
  connect<>(k_241_242.out[0], out[120]);
  connect<>(k_243_244.out[0], out[121]);
  connect<>(k_245_246.out[0], out[122]);
  connect<>(k_247_248.out[0], out[123]);
  connect<>(k_249_250.out[0], out[124]);
  connect<>(k_251_252.out[0], out[125]);
  connect<>(k_253_254.out[0], out[126]);
  connect<>(k_255_256.out[0], out[127]);

 


  };
};








template<int xoff, int yoff, int index,int buffer_loc>
class stage1: public graph {
public:
  kernel core;
  port<input>  in0;
  port<input>  in1;
  port<output> out;

  stage1(void) {
    // Create FFT kernels
    core = kernel::create_object<fam_stage1>(index);
    runtime<ratio>(core) = 0.99;
    source(core) = "fam_stage1.cpp";

    // Make connections
    connect<>(in0, core.in[0]);
    connect<>(in1, core.in[1]);
    connect<>(core.out[0], out);
// 根据buffer_loc定义不同的宏
#define TWD_LOC_0(xloc, yoff)  \
    location<kernel>(core) = tile(xloc+0, yoff+0); \
    location<stack>(core) = address(xloc+1, yoff, 0x0000); \
    location<buffer>(core.out[0]) = {bank(xloc, yoff+1, 0), bank(xloc, yoff+1, 2)};\
    location<buffer>(core.in[0]) = {bank(xloc+0, yoff, 0), bank(xloc+0, yoff, 2)}; \
    location<buffer>(core.in[1]) = {bank(xloc+0, yoff, 1), bank(xloc+0, yoff, 3)};
    TWD_LOC_0(xoff, yoff);
#undef TWD_LOC_0

    };
};





template<int xoff, int yoff, int window_size>
class convergent: public graph {
public:
  kernel core;
  port<input>  in[2];
  port<output> out;

  convergent(void) {
    // Create FFT kernels
    core = kernel::create_object<conv_stage1>(window_size);
    runtime<ratio>(core) = 0.99;
    source(core) = "conv_stage1.cpp";

    // Make connections
    connect<stream>(in[0], core.in[0]);
    connect<stream>(in[1], core.in[1]);
    connect<stream>(core.out[0], out);
// 设定位置和分配的坐标
#define TWD_LOC(xloc, yoff)  location<kernel>   (core)          =  tile(xloc+0, yoff+0); \
                             location<stack>    (core)          =  bank(xloc+0, yoff+0, 0);
                             //location<buffer>(core.in[0])      = {bank(xloc+0, yoff+0, 0), bank(xloc+0, yoff+0, 1)};   
                            //location<buffer>(core.out[0])      = {address(xloc+0, yoff, 0x0000), address(xloc+0, yoff, 0x2000)}; \
                            //location<buffer>(core.in[0])      = {address(xloc+0, yoff, 0x1000), address(xloc+0, yoff, 0x3000)};   
    TWD_LOC(xoff , yoff);

#undef 	TWD_LOC

  };
};


template<int xoff, int yoff>
class stage1_conv_x8: public graph {
public:
  stage1<xoff+0,yoff,0,0>  k8;
  stage1<xoff+0,yoff,1,1>  k9;
  stage1<xoff+1,yoff,2,0>  k10;
  stage1<xoff+1,yoff,3,1>  k11;
  stage1<xoff+9,yoff,0,0>  k12;
  stage1<xoff+9,yoff,1,1>  k13;
  stage1<xoff+10,yoff,2,0>  k14;
  stage1<xoff+10,yoff,3,1>  k15;

  convergent<xoff+2,yoff,256>  c4;
  convergent<xoff+3,yoff,256>  c5;
  convergent<xoff+7,yoff,256>  c6;
  convergent<xoff+8,yoff,256>  c7;

  convergent<xoff+4,yoff,512>  c_2;
  convergent<xoff+6,yoff,512>  c_3;

  convergent<xoff+5,yoff,1024>  c_1;

  port<input>  in[8];
  port<output> out[1];

  stage1_conv_x8(void) {
    // Create FFT kernels
    connect<>(in[0], k8.in0);
    connect<>(in[1], k9.in0);
    connect<>(in[2], k10.in0);
    connect<>(in[3], k11.in0);
    connect<>(in[4], k12.in0);
    connect<>(in[5], k13.in0);
    connect<>(in[6], k14.in0);
    connect<>(in[7], k15.in0);

    connect<>(k8.out, c4.in[0]);
    connect<>(k9.out, c4.in[1]);
    connect<>(k10.out, c5.in[0]);
    connect<>(k11.out, c5.in[1]);
    connect<>(k12.out, c6.in[0]);
    connect<>(k13.out, c6.in[1]);
    connect<>(k14.out, c7.in[0]);
    connect<>(k15.out, c7.in[1]);

    connect<>(c4.out, c_2.in[0]);
    connect<>(c5.out, c_2.in[1]);
    connect<>(c6.out, c_3.in[0]);
    connect<>(c7.out, c_3.in[1]);

    // 第四层连接：c_2和c_3到c_1
    connect<>(c_2.out, c_1.in[0]);
    connect<>(c_3.out, c_1.in[1]);

    connect<>(c_1.out, out[0]);


  };
};
/*
template<int xoff, int yoff>
class stage1_conv_x8: public graph {
public:
  stage1<xoff+0,yoff,0,0>  k8;
  stage1<xoff+0,yoff,1,1>  k9;
  stage1<xoff+1,yoff,2,0>  k10;
  stage1<xoff+1,yoff,3,1>  k11;
  stage1<xoff+9,yoff,0,0>  k12;
  stage1<xoff+9,yoff,1,1>  k13;
  stage1<xoff+10,yoff,2,0>  k14;
  stage1<xoff+10,yoff,3,1>  k15;

  convergent<xoff+2,yoff,256>  c4;
  convergent<xoff+3,yoff,256>  c5;
  convergent<xoff+7,yoff,256>  c6;
  convergent<xoff+8,yoff,256>  c7;
  

  port<input>  in[8];
  port<output> out[4];

  stage1_conv_x8(void) {
    // Create FFT kernels
    connect<>(in[0], k8.in0);
    connect<>(in[1], k9.in0);
    connect<>(in[2], k10.in0);
    connect<>(in[3], k11.in0);
    connect<>(in[4], k12.in0);
    connect<>(in[5], k13.in0);
    connect<>(in[6], k14.in0);
    connect<>(in[7], k15.in0);

    connect<>(k8.out, c4.in[0]);
    connect<>(k9.out, c4.in[1]);
    connect<>(k10.out, c5.in[0]);
    connect<>(k11.out, c5.in[1]);
    connect<>(k12.out, c6.in[0]);
    connect<>(k13.out, c6.in[1]);
    connect<>(k14.out, c7.in[0]);
    connect<>(k15.out, c7.in[1]);

    connect<>(c4.out, out[0]);
    connect<>(c5.out, out[1]);
    connect<>(c6.out, out[2]);
    connect<>(c7.out, out[3]);

    


  };
};
*/
template<int xoff, int yoff>
class ssr_stage2_graph_x32: public graph {
public:

  stage2<xoff+0,yoff,0>  k0;
  stage2<xoff+1,yoff,1>  k1;
  stage2<xoff+2,yoff,2>  k2;
  stage2<xoff+3,yoff,3>  k3;
  stage2<xoff+4,yoff,4>  k4;
  stage2<xoff+5,yoff,5>  k5;
  stage2<xoff+6,yoff,6>  k6;
  stage2<xoff+7,yoff,7>  k7;
  stage2<xoff+8,yoff,8>  k8;
  stage2<xoff+9,yoff,9>  k9;
  stage2<xoff+10,yoff,10>  k10;
  stage2<xoff+11,yoff,11>  k11;
  stage2<xoff+12,yoff,7>  k12;
  stage2<xoff+13,yoff,13>  k13;
  stage2<xoff+14,yoff,14>  k14;
  stage2<xoff+15,yoff,15>  k15;
  stage2<xoff+16,yoff,16>  k16;
  stage2<xoff+17,yoff,17>  k17;
  stage2<xoff+18,yoff,18>  k18;
  stage2<xoff+19,yoff,19>  k19;
  stage2<xoff+20,yoff,20>  k20;
  stage2<xoff+21,yoff,21>  k21;
  stage2<xoff+22,yoff,22>  k22;
  stage2<xoff+23,yoff,23>  k23;
  stage2<xoff+24,yoff,24>  k24;
  stage2<xoff+25,yoff,25>  k25;
  stage2<xoff+26,yoff,26>  k26;
  stage2<xoff+27,yoff,27>  k27;
  stage2<xoff+28,yoff,28>  k28;
  stage2<xoff+29,yoff,1>  k29;
  stage2<xoff+30,yoff,2>  k30;
  stage2<xoff+31,yoff,3>  k31;
  

  port<input>  in0[32];
  port<input>  in1[32];
  port<output> out[32];


  ssr_stage2_graph_x32() {

    
    connect<>(in0[0], k0.in0);
    connect<>(in0[1], k1.in0);
    connect<>(in0[2], k2.in0);
    connect<>(in0[3], k3.in0);
    connect<>(in0[4], k4.in0);
    connect<>(in0[5], k5.in0);
    connect<>(in0[6], k6.in0);
    connect<>(in0[7], k7.in0);
    connect<>(in0[8], k8.in0);
    connect<>(in0[9], k9.in0);
    connect<>(in0[10], k10.in0);
    connect<>(in0[11], k11.in0);
    connect<>(in0[12], k12.in0);
    connect<>(in0[13], k13.in0);
    connect<>(in0[14], k14.in0);
    connect<>(in0[15], k15.in0);
    connect<>(in0[16], k16.in0);
    connect<>(in0[17], k17.in0);
    connect<>(in0[18], k18.in0);
    connect<>(in0[19], k19.in0);
    connect<>(in0[20], k20.in0);
    connect<>(in0[21], k21.in0);
    connect<>(in0[22], k22.in0);
    connect<>(in0[23], k23.in0);
    connect<>(in0[24], k24.in0);
    connect<>(in0[25], k25.in0);
    connect<>(in0[26], k26.in0);
    connect<>(in0[27], k27.in0);
    connect<>(in0[28], k28.in0);
    connect<>(in0[29], k29.in0);
    connect<>(in0[30], k30.in0);
    connect<>(in0[31], k31.in0);

    connect<>(in1[0], k0.in1);
    connect<>(in1[1], k1.in1);
    connect<>(in1[2], k2.in1);
    connect<>(in1[3], k3.in1);
    connect<>(in1[4], k4.in1);
    connect<>(in1[5], k5.in1);
    connect<>(in1[6], k6.in1);
    connect<>(in1[7], k7.in1);
    connect<>(in1[8], k8.in1);
    connect<>(in1[9], k9.in1);
    connect<>(in1[10], k10.in1);
    connect<>(in1[11], k11.in1);
    connect<>(in1[12], k12.in1);
    connect<>(in1[13], k13.in1);
    connect<>(in1[14], k14.in1);
    connect<>(in1[15], k15.in1);
    connect<>(in1[16], k16.in1);
    connect<>(in1[17], k17.in1);
    connect<>(in1[18], k18.in1);
    connect<>(in1[19], k19.in1);
    connect<>(in1[20], k20.in1);
    connect<>(in1[21], k21.in1);
    connect<>(in1[22], k22.in1);
    connect<>(in1[23], k23.in1);
    connect<>(in1[24], k24.in1);
    connect<>(in1[25], k25.in1);
    connect<>(in1[26], k26.in1);
    connect<>(in1[27], k27.in1);
    connect<>(in1[28], k28.in1);
    connect<>(in1[29], k29.in1);
    connect<>(in1[30], k30.in1);
    connect<>(in1[31], k31.in1);


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

  stage1<xoff+0,yoff,0,0>  k0;
  stage1<xoff+1,yoff,1,1>  k1;
  stage1<xoff+2,yoff,2,0>  k2;
  stage1<xoff+3,yoff,3,1>  k3;
  stage1<xoff+4,yoff,4,0>  k4;
  stage1<xoff+5,yoff,5,1>  k5;
  stage1<xoff+6,yoff,6,0>  k6;
  stage1<xoff+7,yoff,7,1>  k7;
  
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
  //ssr_stage2_graph_x32<xoff,4> stage2_graph_x32e;
  //ssr_stage2_graph_x32<xoff,5> stage2_graph_x32f;
  //ssr_stage2_graph_x32<xoff,6> stage2_graph_x32g;
  //ssr_stage2_graph_x32<xoff,7> stage2_graph_x32h;
  

  port<input>        in0[32*4];
  port<input>        in1[32*4];
  port<output>     out[ 32*4]; 
  ssr_stage2_graph() {
    for(int i=0; i<32; i++){
      connect<>(in0[i], stage2_graph_x32a.in0[i]);
      connect<>(in1[i], stage2_graph_x32a.in1[i]);
      connect<>(stage2_graph_x32a.out[i], out[i]);

      connect<>(in0[i+32], stage2_graph_x32b.in0[i]);
      connect<>(in1[i+32], stage2_graph_x32b.in1[i]);
      connect<>(stage2_graph_x32b.out[i], out[i+32]);

      connect<>(in0[i+64], stage2_graph_x32c.in0[i]);
      connect<>(in1[i+64], stage2_graph_x32c.in1[i]);
      connect<>(stage2_graph_x32c.out[i], out[i+64]);

      connect<>(in0[i+96], stage2_graph_x32d.in0[i]);
      connect<>(in1[i+96], stage2_graph_x32d.in1[i]);
      connect<>(stage2_graph_x32d.out[i], out[i+96]);
      //connect<>(in[i+128], stage2_graph_x32e.in[i]);
      //connect<>(stage2_graph_x32e.out[i], out[i+128]);
      //connect<>(in[i+160], stage2_graph_x32f.in[i]);
      //connect<>(stage2_graph_x32f.out[i], out[i+160]);
      //connect<>(in[i+192], stage2_graph_x32g.in[i]);
      //connect<>(stage2_graph_x32g.out[i], out[i+192]);
      //connect<>(in[i+224], stage2_graph_x32h.in[i]);
      //connect<>(stage2_graph_x32h.out[i], out[i+224]);
      
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

