/***************************************************************************/
/*  Copyright (C) 2023 Advanced Micro Devices, Inc
 *  SPDX-License-Identifier: MIT
 *
 *  示 例：使用 ap_uint<512> 做 AXI Master 接口，每次搬运 8 个 cfloat(64bit) 
 *  从 memin 读取 8 个输入通道 × 1024 cfloat 
 *  写到 128 个输出通道 × 8192 cfloat
 */
/***************************************************************************/

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

// 每个 cfloat = 64bit
typedef ap_axiu<64, 0, 0, 0> axis_data_64;

// 输入通道数量 & 输出通道数量
#define N_IN_CH   8
#define N_OUT_CH  128

// 每个输入通道含 1024 个 cfloat
#define NSAMP_IN  1024
// 每个输出通道含 8192 个 cfloat
#define NSAMP_OUT 8192


    

//------------------------------
// 顶层HLS内核
//------------------------------
extern "C"
void dma_hls(
    // 使用 ap_uint<512> 作为端口类型
        // 8个分开指针
    ap_uint<64> *memin0,
    ap_uint<64> *memin1,
    ap_uint<64> *memin2,
    ap_uint<64> *memin3,
    ap_uint<64> *memin4,
    ap_uint<64> *memin5,
    ap_uint<64> *memin6,
    ap_uint<64> *memin7,
    ap_uint<512> *memout,

    // 8 个输入流
    hls::stream<axis_data_64> &FAMDataIn_0,
    hls::stream<axis_data_64> &FAMDataIn_1,
    hls::stream<axis_data_64> &FAMDataIn_2,
    hls::stream<axis_data_64> &FAMDataIn_3,
    hls::stream<axis_data_64> &FAMDataIn_4,
    hls::stream<axis_data_64> &FAMDataIn_5,
    hls::stream<axis_data_64> &FAMDataIn_6,
    hls::stream<axis_data_64> &FAMDataIn_7,

    // 128 个输出流
    hls::stream<axis_data_64> &FAMOut_0,
    hls::stream<axis_data_64> &FAMOut_1,
    hls::stream<axis_data_64> &FAMOut_2,
    hls::stream<axis_data_64> &FAMOut_3,
    hls::stream<axis_data_64> &FAMOut_4,
    hls::stream<axis_data_64> &FAMOut_5,
    hls::stream<axis_data_64> &FAMOut_6,
    hls::stream<axis_data_64> &FAMOut_7,
    hls::stream<axis_data_64> &FAMOut_8,
    hls::stream<axis_data_64> &FAMOut_9,
    hls::stream<axis_data_64> &FAMOut_10,
    hls::stream<axis_data_64> &FAMOut_11,
    hls::stream<axis_data_64> &FAMOut_12,
    hls::stream<axis_data_64> &FAMOut_13,
    hls::stream<axis_data_64> &FAMOut_14,
    hls::stream<axis_data_64> &FAMOut_15,
    hls::stream<axis_data_64> &FAMOut_16,
    hls::stream<axis_data_64> &FAMOut_17,
    hls::stream<axis_data_64> &FAMOut_18,
    hls::stream<axis_data_64> &FAMOut_19,
    hls::stream<axis_data_64> &FAMOut_20,
    hls::stream<axis_data_64> &FAMOut_21,
    hls::stream<axis_data_64> &FAMOut_22,
    hls::stream<axis_data_64> &FAMOut_23,
    hls::stream<axis_data_64> &FAMOut_24,
    hls::stream<axis_data_64> &FAMOut_25,
    hls::stream<axis_data_64> &FAMOut_26,
    hls::stream<axis_data_64> &FAMOut_27,
    hls::stream<axis_data_64> &FAMOut_28,
    hls::stream<axis_data_64> &FAMOut_29,
    hls::stream<axis_data_64> &FAMOut_30,
    hls::stream<axis_data_64> &FAMOut_31,
    hls::stream<axis_data_64> &FAMOut_32,
    hls::stream<axis_data_64> &FAMOut_33,
    hls::stream<axis_data_64> &FAMOut_34,
    hls::stream<axis_data_64> &FAMOut_35,
    hls::stream<axis_data_64> &FAMOut_36,
    hls::stream<axis_data_64> &FAMOut_37,
    hls::stream<axis_data_64> &FAMOut_38,
    hls::stream<axis_data_64> &FAMOut_39,
    hls::stream<axis_data_64> &FAMOut_40,
    hls::stream<axis_data_64> &FAMOut_41,
    hls::stream<axis_data_64> &FAMOut_42,
    hls::stream<axis_data_64> &FAMOut_43,
    hls::stream<axis_data_64> &FAMOut_44,
    hls::stream<axis_data_64> &FAMOut_45,
    hls::stream<axis_data_64> &FAMOut_46,
    hls::stream<axis_data_64> &FAMOut_47,
    hls::stream<axis_data_64> &FAMOut_48,
    hls::stream<axis_data_64> &FAMOut_49,
    hls::stream<axis_data_64> &FAMOut_50,
    hls::stream<axis_data_64> &FAMOut_51,
    hls::stream<axis_data_64> &FAMOut_52,
    hls::stream<axis_data_64> &FAMOut_53,
    hls::stream<axis_data_64> &FAMOut_54,
    hls::stream<axis_data_64> &FAMOut_55,
    hls::stream<axis_data_64> &FAMOut_56,
    hls::stream<axis_data_64> &FAMOut_57,
    hls::stream<axis_data_64> &FAMOut_58,
    hls::stream<axis_data_64> &FAMOut_59,
    hls::stream<axis_data_64> &FAMOut_60,
    hls::stream<axis_data_64> &FAMOut_61,
    hls::stream<axis_data_64> &FAMOut_62,
    hls::stream<axis_data_64> &FAMOut_63,
    hls::stream<axis_data_64> &FAMOut_64,
    hls::stream<axis_data_64> &FAMOut_65,
    hls::stream<axis_data_64> &FAMOut_66,
    hls::stream<axis_data_64> &FAMOut_67,
    hls::stream<axis_data_64> &FAMOut_68,
    hls::stream<axis_data_64> &FAMOut_69,
    hls::stream<axis_data_64> &FAMOut_70,
    hls::stream<axis_data_64> &FAMOut_71,
    hls::stream<axis_data_64> &FAMOut_72,
    hls::stream<axis_data_64> &FAMOut_73,
    hls::stream<axis_data_64> &FAMOut_74,
    hls::stream<axis_data_64> &FAMOut_75,
    hls::stream<axis_data_64> &FAMOut_76,
    hls::stream<axis_data_64> &FAMOut_77,
    hls::stream<axis_data_64> &FAMOut_78,
    hls::stream<axis_data_64> &FAMOut_79,
    hls::stream<axis_data_64> &FAMOut_80,
    hls::stream<axis_data_64> &FAMOut_81,
    hls::stream<axis_data_64> &FAMOut_82,
    hls::stream<axis_data_64> &FAMOut_83,
    hls::stream<axis_data_64> &FAMOut_84,
    hls::stream<axis_data_64> &FAMOut_85,
    hls::stream<axis_data_64> &FAMOut_86,
    hls::stream<axis_data_64> &FAMOut_87,
    hls::stream<axis_data_64> &FAMOut_88,
    hls::stream<axis_data_64> &FAMOut_89,
    hls::stream<axis_data_64> &FAMOut_90,
    hls::stream<axis_data_64> &FAMOut_91,
    hls::stream<axis_data_64> &FAMOut_92,
    hls::stream<axis_data_64> &FAMOut_93,
    hls::stream<axis_data_64> &FAMOut_94,
    hls::stream<axis_data_64> &FAMOut_95,
    hls::stream<axis_data_64> &FAMOut_96,
    hls::stream<axis_data_64> &FAMOut_97,
    hls::stream<axis_data_64> &FAMOut_98,
    hls::stream<axis_data_64> &FAMOut_99,
    hls::stream<axis_data_64> &FAMOut_100,
    hls::stream<axis_data_64> &FAMOut_101,
    hls::stream<axis_data_64> &FAMOut_102,
    hls::stream<axis_data_64> &FAMOut_103,
    hls::stream<axis_data_64> &FAMOut_104,
    hls::stream<axis_data_64> &FAMOut_105,
    hls::stream<axis_data_64> &FAMOut_106,
    hls::stream<axis_data_64> &FAMOut_107,
    hls::stream<axis_data_64> &FAMOut_108,
    hls::stream<axis_data_64> &FAMOut_109,
    hls::stream<axis_data_64> &FAMOut_110,
    hls::stream<axis_data_64> &FAMOut_111,
    hls::stream<axis_data_64> &FAMOut_112,
    hls::stream<axis_data_64> &FAMOut_113,
    hls::stream<axis_data_64> &FAMOut_114,
    hls::stream<axis_data_64> &FAMOut_115,
    hls::stream<axis_data_64> &FAMOut_116,
    hls::stream<axis_data_64> &FAMOut_117,
    hls::stream<axis_data_64> &FAMOut_118,
    hls::stream<axis_data_64> &FAMOut_119,
    hls::stream<axis_data_64> &FAMOut_120,
    hls::stream<axis_data_64> &FAMOut_121,
    hls::stream<axis_data_64> &FAMOut_122,
    hls::stream<axis_data_64> &FAMOut_123,
    hls::stream<axis_data_64> &FAMOut_124,
    hls::stream<axis_data_64> &FAMOut_125,
    hls::stream<axis_data_64> &FAMOut_126,
    hls::stream<axis_data_64> &FAMOut_127,

    unsigned int iter  // 迭代次数
)
{
    /************************************************************************/
    /*  HLS接口配置： 512 bits 宽度                                         */
    /************************************************************************/


#pragma HLS INTERFACE s_axilite bundle=control port=memin0
#pragma HLS INTERFACE s_axilite bundle=control port=memin1
#pragma HLS INTERFACE s_axilite bundle=control port=memin2
#pragma HLS INTERFACE s_axilite bundle=control port=memin3
#pragma HLS INTERFACE s_axilite bundle=control port=memin4
#pragma HLS INTERFACE s_axilite bundle=control port=memin5
#pragma HLS INTERFACE s_axilite bundle=control port=memin6
#pragma HLS INTERFACE s_axilite bundle=control port=memin7
#pragma HLS INTERFACE s_axilite bundle=control port=memout 
#pragma HLS INTERFACE s_axilite bundle=control port=iter   
#pragma HLS INTERFACE s_axilite bundle=control port=return 
#pragma HLS interface m_axi offset=slave bundle=gmem0 port=memin0 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem1 port=memin1 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem2 port=memin2 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem3 port=memin3 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem4 port=memin4 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem5 port=memin5 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem6 port=memin6 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem7 port=memin7 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem0 port=memout max_write_burst_length=16 num_write_outstanding=64



    // AXI4-Stream接口：8个输入
#pragma HLS INTERFACE axis port=FAMDataIn_0
#pragma HLS INTERFACE axis port=FAMDataIn_1
#pragma HLS INTERFACE axis port=FAMDataIn_2
#pragma HLS INTERFACE axis port=FAMDataIn_3
#pragma HLS INTERFACE axis port=FAMDataIn_4
#pragma HLS INTERFACE axis port=FAMDataIn_5
#pragma HLS INTERFACE axis port=FAMDataIn_6
#pragma HLS INTERFACE axis port=FAMDataIn_7

    // AXI4-Stream接口：128个输出
// AXI4-Stream接口：128个输出
#pragma HLS INTERFACE axis port=FAMOut_0
#pragma HLS INTERFACE axis port=FAMOut_1
#pragma HLS INTERFACE axis port=FAMOut_2
#pragma HLS INTERFACE axis port=FAMOut_3
#pragma HLS INTERFACE axis port=FAMOut_4
#pragma HLS INTERFACE axis port=FAMOut_5
#pragma HLS INTERFACE axis port=FAMOut_6
#pragma HLS INTERFACE axis port=FAMOut_7
#pragma HLS INTERFACE axis port=FAMOut_8
#pragma HLS INTERFACE axis port=FAMOut_9
#pragma HLS INTERFACE axis port=FAMOut_10
#pragma HLS INTERFACE axis port=FAMOut_11
#pragma HLS INTERFACE axis port=FAMOut_12
#pragma HLS INTERFACE axis port=FAMOut_13
#pragma HLS INTERFACE axis port=FAMOut_14
#pragma HLS INTERFACE axis port=FAMOut_15
#pragma HLS INTERFACE axis port=FAMOut_16
#pragma HLS INTERFACE axis port=FAMOut_17
#pragma HLS INTERFACE axis port=FAMOut_18
#pragma HLS INTERFACE axis port=FAMOut_19
#pragma HLS INTERFACE axis port=FAMOut_20
#pragma HLS INTERFACE axis port=FAMOut_21
#pragma HLS INTERFACE axis port=FAMOut_22
#pragma HLS INTERFACE axis port=FAMOut_23
#pragma HLS INTERFACE axis port=FAMOut_24
#pragma HLS INTERFACE axis port=FAMOut_25
#pragma HLS INTERFACE axis port=FAMOut_26
#pragma HLS INTERFACE axis port=FAMOut_27
#pragma HLS INTERFACE axis port=FAMOut_28
#pragma HLS INTERFACE axis port=FAMOut_29
#pragma HLS INTERFACE axis port=FAMOut_30
#pragma HLS INTERFACE axis port=FAMOut_31
#pragma HLS INTERFACE axis port=FAMOut_32
#pragma HLS INTERFACE axis port=FAMOut_33
#pragma HLS INTERFACE axis port=FAMOut_34
#pragma HLS INTERFACE axis port=FAMOut_35
#pragma HLS INTERFACE axis port=FAMOut_36
#pragma HLS INTERFACE axis port=FAMOut_37
#pragma HLS INTERFACE axis port=FAMOut_38
#pragma HLS INTERFACE axis port=FAMOut_39
#pragma HLS INTERFACE axis port=FAMOut_40
#pragma HLS INTERFACE axis port=FAMOut_41
#pragma HLS INTERFACE axis port=FAMOut_42
#pragma HLS INTERFACE axis port=FAMOut_43
#pragma HLS INTERFACE axis port=FAMOut_44
#pragma HLS INTERFACE axis port=FAMOut_45
#pragma HLS INTERFACE axis port=FAMOut_46
#pragma HLS INTERFACE axis port=FAMOut_47
#pragma HLS INTERFACE axis port=FAMOut_48
#pragma HLS INTERFACE axis port=FAMOut_49
#pragma HLS INTERFACE axis port=FAMOut_50
#pragma HLS INTERFACE axis port=FAMOut_51
#pragma HLS INTERFACE axis port=FAMOut_52
#pragma HLS INTERFACE axis port=FAMOut_53
#pragma HLS INTERFACE axis port=FAMOut_54
#pragma HLS INTERFACE axis port=FAMOut_55
#pragma HLS INTERFACE axis port=FAMOut_56
#pragma HLS INTERFACE axis port=FAMOut_57
#pragma HLS INTERFACE axis port=FAMOut_58
#pragma HLS INTERFACE axis port=FAMOut_59
#pragma HLS INTERFACE axis port=FAMOut_60
#pragma HLS INTERFACE axis port=FAMOut_61
#pragma HLS INTERFACE axis port=FAMOut_62
#pragma HLS INTERFACE axis port=FAMOut_63
#pragma HLS INTERFACE axis port=FAMOut_64
#pragma HLS INTERFACE axis port=FAMOut_65
#pragma HLS INTERFACE axis port=FAMOut_66
#pragma HLS INTERFACE axis port=FAMOut_67
#pragma HLS INTERFACE axis port=FAMOut_68
#pragma HLS INTERFACE axis port=FAMOut_69
#pragma HLS INTERFACE axis port=FAMOut_70
#pragma HLS INTERFACE axis port=FAMOut_71
#pragma HLS INTERFACE axis port=FAMOut_72
#pragma HLS INTERFACE axis port=FAMOut_73
#pragma HLS INTERFACE axis port=FAMOut_74
#pragma HLS INTERFACE axis port=FAMOut_75
#pragma HLS INTERFACE axis port=FAMOut_76
#pragma HLS INTERFACE axis port=FAMOut_77
#pragma HLS INTERFACE axis port=FAMOut_78
#pragma HLS INTERFACE axis port=FAMOut_79
#pragma HLS INTERFACE axis port=FAMOut_80
#pragma HLS INTERFACE axis port=FAMOut_81
#pragma HLS INTERFACE axis port=FAMOut_82
#pragma HLS INTERFACE axis port=FAMOut_83
#pragma HLS INTERFACE axis port=FAMOut_84
#pragma HLS INTERFACE axis port=FAMOut_85
#pragma HLS INTERFACE axis port=FAMOut_86
#pragma HLS INTERFACE axis port=FAMOut_87
#pragma HLS INTERFACE axis port=FAMOut_88
#pragma HLS INTERFACE axis port=FAMOut_89
#pragma HLS INTERFACE axis port=FAMOut_90
#pragma HLS INTERFACE axis port=FAMOut_91
#pragma HLS INTERFACE axis port=FAMOut_92
#pragma HLS INTERFACE axis port=FAMOut_93
#pragma HLS INTERFACE axis port=FAMOut_94
#pragma HLS INTERFACE axis port=FAMOut_95
#pragma HLS INTERFACE axis port=FAMOut_96
#pragma HLS INTERFACE axis port=FAMOut_97
#pragma HLS INTERFACE axis port=FAMOut_98
#pragma HLS INTERFACE axis port=FAMOut_99
#pragma HLS INTERFACE axis port=FAMOut_100
#pragma HLS INTERFACE axis port=FAMOut_101
#pragma HLS INTERFACE axis port=FAMOut_102
#pragma HLS INTERFACE axis port=FAMOut_103
#pragma HLS INTERFACE axis port=FAMOut_104
#pragma HLS INTERFACE axis port=FAMOut_105
#pragma HLS INTERFACE axis port=FAMOut_106
#pragma HLS INTERFACE axis port=FAMOut_107
#pragma HLS INTERFACE axis port=FAMOut_108
#pragma HLS INTERFACE axis port=FAMOut_109
#pragma HLS INTERFACE axis port=FAMOut_110
#pragma HLS INTERFACE axis port=FAMOut_111
#pragma HLS INTERFACE axis port=FAMOut_112
#pragma HLS INTERFACE axis port=FAMOut_113
#pragma HLS INTERFACE axis port=FAMOut_114
#pragma HLS INTERFACE axis port=FAMOut_115
#pragma HLS INTERFACE axis port=FAMOut_116
#pragma HLS INTERFACE axis port=FAMOut_117
#pragma HLS INTERFACE axis port=FAMOut_118
#pragma HLS INTERFACE axis port=FAMOut_119
#pragma HLS INTERFACE axis port=FAMOut_120
#pragma HLS INTERFACE axis port=FAMOut_121
#pragma HLS INTERFACE axis port=FAMOut_122
#pragma HLS INTERFACE axis port=FAMOut_123
#pragma HLS INTERFACE axis port=FAMOut_124
#pragma HLS INTERFACE axis port=FAMOut_125
#pragma HLS INTERFACE axis port=FAMOut_126
#pragma HLS INTERFACE axis port=FAMOut_127


//#pragma HLS DATAFLOW
     // **必须先执行完整的 `read_input`**
    // 8通道 × 1024 cfloat
    //-------------------------
    // 搬运输入：memin -> 8个输入流
    //-------------------------
    //
    // 1) 每次读 ap_uint<512> (8个 cfloat)，拆分成 8×64 bits
    // 2) 依次送到 8 个输入流的 write()
    // 3) 总输入 cfloat = 8 * 1024 = 8192 => 8192/8 = 1024 个512bit
    //-------------------------
    axis_data_64 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    for(int i=0; i<NSAMP_IN; i++){
    #pragma HLS PIPELINE II=1    
        tmp0.data = memin0[i];  FAMDataIn_0.write(tmp0);
        tmp1.data = memin1[i];  FAMDataIn_1.write(tmp1);
        tmp2.data = memin2[i];  FAMDataIn_2.write(tmp2);
        tmp3.data = memin3[i];  FAMDataIn_3.write(tmp3);
        tmp4.data = memin4[i];  FAMDataIn_4.write(tmp4);
        tmp5.data = memin5[i];  FAMDataIn_5.write(tmp5);
        tmp6.data = memin6[i];  FAMDataIn_6.write(tmp6);
        tmp7.data = memin7[i];  FAMDataIn_7.write(tmp7);
    }
    
    // **等 AI Engine 计算完成后**，才执行 `write_output`
    // 遍历所有 128 个 channel
    // 搬运输出：128个输出流 -> memout
    //-------------------------
    //
    // 1) 每次要写 128 * 8192 = 1,048,576 个 cfloat
    // 2) 每写一次 ap_uint<512> (8×64bit)
    // 3) 所以共需 1,048,576 / 8 = 131,072 次写
    // 4) 对于 每个channel(共128) -> each有8192 cfloat => each channel 8192/8=1024个512bit
    //-------------------------

    // 计算 memout 的基准偏移 //0.00121071 s
    //unsigned int out_offset = it * 131072; // 128*8192/8=131072

    // 遍历所有 128 个 channel
    hls::stream<axis_data_64>* FAMOutArray[128] = {
        &FAMOut_0, &FAMOut_1, &FAMOut_2, &FAMOut_3, &FAMOut_4, &FAMOut_5, &FAMOut_6, &FAMOut_7,
        &FAMOut_8, &FAMOut_9, &FAMOut_10, &FAMOut_11, &FAMOut_12, &FAMOut_13, &FAMOut_14, &FAMOut_15,
        &FAMOut_16, &FAMOut_17, &FAMOut_18, &FAMOut_19, &FAMOut_20, &FAMOut_21, &FAMOut_22, &FAMOut_23,
        &FAMOut_24, &FAMOut_25, &FAMOut_26, &FAMOut_27, &FAMOut_28, &FAMOut_29, &FAMOut_30, &FAMOut_31,
        &FAMOut_32, &FAMOut_33, &FAMOut_34, &FAMOut_35, &FAMOut_36, &FAMOut_37, &FAMOut_38, &FAMOut_39,
        &FAMOut_40, &FAMOut_41, &FAMOut_42, &FAMOut_43, &FAMOut_44, &FAMOut_45, &FAMOut_46, &FAMOut_47,
        &FAMOut_48, &FAMOut_49, &FAMOut_50, &FAMOut_51, &FAMOut_52, &FAMOut_53, &FAMOut_54, &FAMOut_55,
        &FAMOut_56, &FAMOut_57, &FAMOut_58, &FAMOut_59, &FAMOut_60, &FAMOut_61, &FAMOut_62, &FAMOut_63,
        &FAMOut_64, &FAMOut_65, &FAMOut_66, &FAMOut_67, &FAMOut_68, &FAMOut_69, &FAMOut_70, &FAMOut_71,
        &FAMOut_72, &FAMOut_73, &FAMOut_74, &FAMOut_75, &FAMOut_76, &FAMOut_77, &FAMOut_78, &FAMOut_79,
        &FAMOut_80, &FAMOut_81, &FAMOut_82, &FAMOut_83, &FAMOut_84, &FAMOut_85, &FAMOut_86, &FAMOut_87,
        &FAMOut_88, &FAMOut_89, &FAMOut_90, &FAMOut_91, &FAMOut_92, &FAMOut_93, &FAMOut_94, &FAMOut_95,
        &FAMOut_96, &FAMOut_97, &FAMOut_98, &FAMOut_99, &FAMOut_100, &FAMOut_101, &FAMOut_102, &FAMOut_103,
        &FAMOut_104, &FAMOut_105, &FAMOut_106, &FAMOut_107, &FAMOut_108, &FAMOut_109, &FAMOut_110, &FAMOut_111,
        &FAMOut_112, &FAMOut_113, &FAMOut_114, &FAMOut_115, &FAMOut_116, &FAMOut_117, &FAMOut_118, &FAMOut_119,
        &FAMOut_120, &FAMOut_121, &FAMOut_122, &FAMOut_123, &FAMOut_124, &FAMOut_125, &FAMOut_126, &FAMOut_127
    };
    #pragma HLS array_partition variable=FAMOutArray complete
    //#pragma HLS PIPELINE II=1
    //#pragma HLS UNROLL factor=8
    //#pragma HLS UNROLL


    /**/
    for(int i = 0; i < 256; i++) {
    #pragma HLS PIPELINE II=1
        //每次i遍历128个通道，读取16个cfloat
        for(int ch = 0; ch < 128; ch++) {
            #pragma HLS UNROLL 

            /*
            ap_uint<512> tmp1;
            // 读取前 8 个 cfloat（64bit），存入 tmp1
            for(int k = 0; k < 8; k++) {
            #pragma HLS UNROLL
                axis_data_64 x = FAMOutArray[ch]->read();
                tmp1(64*k + 63, 64*k) = x.data;
            }
            memout[ ch*(512*2) + i*2     ] = tmp1;
            // 读取后 8 个 cfloat（64bit），存入 tmp1
            for(int k = 0; k < 8; k++) {
            #pragma HLS UNROLL
                axis_data_64 x = FAMOutArray[ch]->read();
                tmp1(64*k + 63, 64*k) = x.data;
            }
            memout[ ch*(512*2) + i*2 + 1 ] = tmp1;   
            */                
            //速度更快，但是顺序改变
            //正常顺序1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
            //此代码顺序1,3,5,7,9,11,13,15,2,4,6,8,10,12,14,16
            ap_uint<512> tmp1, tmp2;           
            //读取16个cfloat
            for(int k = 0; k < 8; k++) {
            #pragma HLS UNROLL      
                axis_data_64 x1 = FAMOutArray[ch]->read();
                axis_data_64 x2 = FAMOutArray[ch]->read();
                tmp1(64*k+63, 64*k) = x1.data;
                tmp2(64*k+63, 64*k) = x2.data;
            }
            memout[ ch*(512*2) + i*2     ] = tmp1;
            memout[ ch*(512*2) + i*2 + 1 ] = tmp2;  


            for(int k = 0; k < 8; k++) {
            #pragma HLS UNROLL      
                axis_data_64 x1 = FAMOutArray[ch]->read();
                axis_data_64 x2 = FAMOutArray[ch]->read();
                tmp1(64*k+63, 64*k) = x1.data;
                tmp2(64*k+63, 64*k) = x2.data;
            }
            memout[ (ch*2+1)*512 + i*2     ] = tmp1;
            memout[ (ch*2+1)*512 + i*2 + 1 ] = tmp2;  


        }
    }

}


/*

    for(int i = 0; i < 512; i++) { // 8192/16 = 512
    #pragma HLS PIPELINE II=1
        for(int ch = 0; ch < 128; ch++) {
            #pragma HLS PIPELINE II=1
            // 从输出流读16个 cfloat => 拼成2×512bit
            ap_uint<512> tmp1, tmp2;
            for(int k=0; k<8; k++){
                //#pragma HLS unroll
                #pragma HLS PIPELINE II=1
                axis_data_64 x1, x2;
                switch(ch) {
                case 0: x1 = FAMOut_0.read();  x2 = FAMOut_0.read();  break;
                case 1: x1 = FAMOut_1.read();  x2 = FAMOut_1.read();  break;
                case 2: x1 = FAMOut_2.read();  x2 = FAMOut_2.read();  break;
                case 3: x1 = FAMOut_3.read();  x2 = FAMOut_3.read();  break;
                case 4: x1 = FAMOut_4.read();  x2 = FAMOut_4.read();  break;
                case 5: x1 = FAMOut_5.read();  x2 = FAMOut_5.read();  break;
                case 6: x1 = FAMOut_6.read();  x2 = FAMOut_6.read();  break;
                case 7: x1 = FAMOut_7.read();  x2 = FAMOut_7.read();  break;
                case 8: x1 = FAMOut_8.read();  x2 = FAMOut_8.read();  break;
                case 9: x1 = FAMOut_9.read();  x2 = FAMOut_9.read();  break;
                case 10: x1 = FAMOut_10.read();  x2 = FAMOut_10.read();  break;
                case 11: x1 = FAMOut_11.read();  x2 = FAMOut_11.read();  break;
                case 12: x1 = FAMOut_12.read();  x2 = FAMOut_12.read();  break;
                case 13: x1 = FAMOut_13.read();  x2 = FAMOut_13.read();  break;
                case 14: x1 = FAMOut_14.read();  x2 = FAMOut_14.read();  break;
                case 15: x1 = FAMOut_15.read();  x2 = FAMOut_15.read();  break;
                case 16: x1 = FAMOut_16.read();  x2 = FAMOut_16.read();  break;
                case 17: x1 = FAMOut_17.read();  x2 = FAMOut_17.read();  break;
                case 18: x1 = FAMOut_18.read();  x2 = FAMOut_18.read();  break;
                case 19: x1 = FAMOut_19.read();  x2 = FAMOut_19.read();  break;
                case 20: x1 = FAMOut_20.read();  x2 = FAMOut_20.read();  break;
                case 21: x1 = FAMOut_21.read();  x2 = FAMOut_21.read();  break;
                case 22: x1 = FAMOut_22.read();  x2 = FAMOut_22.read();  break;
                case 23: x1 = FAMOut_23.read();  x2 = FAMOut_23.read();  break;
                case 24: x1 = FAMOut_24.read();  x2 = FAMOut_24.read();  break;
                case 25: x1 = FAMOut_25.read();  x2 = FAMOut_25.read();  break;
                case 26: x1 = FAMOut_26.read();  x2 = FAMOut_26.read();  break;
                case 27: x1 = FAMOut_27.read();  x2 = FAMOut_27.read();  break;
                case 28: x1 = FAMOut_28.read();  x2 = FAMOut_28.read();  break;
                case 29: x1 = FAMOut_29.read();  x2 = FAMOut_29.read();  break;
                case 30: x1 = FAMOut_30.read();  x2 = FAMOut_30.read();  break;
                case 31: x1 = FAMOut_31.read();  x2 = FAMOut_31.read();  break;
                case 32: x1 = FAMOut_32.read();  x2 = FAMOut_32.read();  break;
                case 33: x1 = FAMOut_33.read();  x2 = FAMOut_33.read();  break;
                case 34: x1 = FAMOut_34.read();  x2 = FAMOut_34.read();  break;
                case 35: x1 = FAMOut_35.read();  x2 = FAMOut_35.read();  break;
                case 36: x1 = FAMOut_36.read();  x2 = FAMOut_36.read();  break;
                case 37: x1 = FAMOut_37.read();  x2 = FAMOut_37.read();  break;
                case 38: x1 = FAMOut_38.read();  x2 = FAMOut_38.read();  break;
                case 39: x1 = FAMOut_39.read();  x2 = FAMOut_39.read();  break;
                case 40: x1 = FAMOut_40.read();  x2 = FAMOut_40.read();  break;
                case 41: x1 = FAMOut_41.read();  x2 = FAMOut_41.read();  break;
                case 42: x1 = FAMOut_42.read();  x2 = FAMOut_42.read();  break;
                case 43: x1 = FAMOut_43.read();  x2 = FAMOut_43.read();  break;
                case 44: x1 = FAMOut_44.read();  x2 = FAMOut_44.read();  break;
                case 45: x1 = FAMOut_45.read();  x2 = FAMOut_45.read();  break;
                case 46: x1 = FAMOut_46.read();  x2 = FAMOut_46.read();  break;
                case 47: x1 = FAMOut_47.read();  x2 = FAMOut_47.read();  break;
                case 48: x1 = FAMOut_48.read();  x2 = FAMOut_48.read();  break;
                case 49: x1 = FAMOut_49.read();  x2 = FAMOut_49.read();  break;
                case 50: x1 = FAMOut_50.read();  x2 = FAMOut_50.read();  break;
                case 51: x1 = FAMOut_51.read();  x2 = FAMOut_51.read();  break;
                case 52: x1 = FAMOut_52.read();  x2 = FAMOut_52.read();  break;
                case 53: x1 = FAMOut_53.read();  x2 = FAMOut_53.read();  break;
                case 54: x1 = FAMOut_54.read();  x2 = FAMOut_54.read();  break;
                case 55: x1 = FAMOut_55.read();  x2 = FAMOut_55.read();  break;
                case 56: x1 = FAMOut_56.read();  x2 = FAMOut_56.read();  break;
                case 57: x1 = FAMOut_57.read();  x2 = FAMOut_57.read();  break;
                case 58: x1 = FAMOut_58.read();  x2 = FAMOut_58.read();  break;
                case 59: x1 = FAMOut_59.read();  x2 = FAMOut_59.read();  break;
                case 60: x1 = FAMOut_60.read();  x2 = FAMOut_60.read();  break;
                case 61: x1 = FAMOut_61.read();  x2 = FAMOut_61.read();  break;
                case 62: x1 = FAMOut_62.read();  x2 = FAMOut_62.read();  break;
                case 63: x1 = FAMOut_63.read();  x2 = FAMOut_63.read();  break;
                case 64: x1 = FAMOut_64.read();  x2 = FAMOut_64.read();  break;
                case 65: x1 = FAMOut_65.read();  x2 = FAMOut_65.read();  break;
                case 66: x1 = FAMOut_66.read();  x2 = FAMOut_66.read();  break;
                case 67: x1 = FAMOut_67.read();  x2 = FAMOut_67.read();  break;
                case 68: x1 = FAMOut_68.read();  x2 = FAMOut_68.read();  break;
                case 69: x1 = FAMOut_69.read();  x2 = FAMOut_69.read();  break;
                case 70: x1 = FAMOut_70.read();  x2 = FAMOut_70.read();  break;
                case 71: x1 = FAMOut_71.read();  x2 = FAMOut_71.read();  break;
                case 72: x1 = FAMOut_72.read();  x2 = FAMOut_72.read();  break;
                case 73: x1 = FAMOut_73.read();  x2 = FAMOut_73.read();  break;
                case 74: x1 = FAMOut_74.read();  x2 = FAMOut_74.read();  break;
                case 75: x1 = FAMOut_75.read();  x2 = FAMOut_75.read();  break;
                case 76: x1 = FAMOut_76.read();  x2 = FAMOut_76.read();  break;
                case 77: x1 = FAMOut_77.read();  x2 = FAMOut_77.read();  break;
                case 78: x1 = FAMOut_78.read();  x2 = FAMOut_78.read();  break;
                case 79: x1 = FAMOut_79.read();  x2 = FAMOut_79.read();  break;
                case 80: x1 = FAMOut_80.read();  x2 = FAMOut_80.read();  break;
                case 81: x1 = FAMOut_81.read();  x2 = FAMOut_81.read();  break;
                case 82: x1 = FAMOut_82.read();  x2 = FAMOut_82.read();  break;
                case 83: x1 = FAMOut_83.read();  x2 = FAMOut_83.read();  break;
                case 84: x1 = FAMOut_84.read();  x2 = FAMOut_84.read();  break;
                case 85: x1 = FAMOut_85.read();  x2 = FAMOut_85.read();  break;
                case 86: x1 = FAMOut_86.read();  x2 = FAMOut_86.read();  break;
                case 87: x1 = FAMOut_87.read();  x2 = FAMOut_87.read();  break;
                case 88: x1 = FAMOut_88.read();  x2 = FAMOut_88.read();  break;
                case 89: x1 = FAMOut_89.read();  x2 = FAMOut_89.read();  break;
                case 90: x1 = FAMOut_90.read();  x2 = FAMOut_90.read();  break;
                case 91: x1 = FAMOut_91.read();  x2 = FAMOut_91.read();  break;
                case 92: x1 = FAMOut_92.read();  x2 = FAMOut_92.read();  break;
                case 93: x1 = FAMOut_93.read();  x2 = FAMOut_93.read();  break;
                case 94: x1 = FAMOut_94.read();  x2 = FAMOut_94.read();  break;
                case 95: x1 = FAMOut_95.read();  x2 = FAMOut_95.read();  break;
                case 96: x1 = FAMOut_96.read();  x2 = FAMOut_96.read();  break;
                case 97: x1 = FAMOut_97.read();  x2 = FAMOut_97.read();  break;
                case 98: x1 = FAMOut_98.read();  x2 = FAMOut_98.read();  break;
                case 99: x1 = FAMOut_99.read();  x2 = FAMOut_99.read();  break;
                case 100: x1 = FAMOut_100.read();  x2 = FAMOut_100.read();  break;
                case 101: x1 = FAMOut_101.read();  x2 = FAMOut_101.read();  break;
                case 102: x1 = FAMOut_102.read();  x2 = FAMOut_102.read();  break;
                case 103: x1 = FAMOut_103.read();  x2 = FAMOut_103.read();  break;
                case 104: x1 = FAMOut_104.read();  x2 = FAMOut_104.read();  break;
                case 105: x1 = FAMOut_105.read();  x2 = FAMOut_105.read();  break;
                case 106: x1 = FAMOut_106.read();  x2 = FAMOut_106.read();  break;
                case 107: x1 = FAMOut_107.read();  x2 = FAMOut_107.read();  break;
                case 108: x1 = FAMOut_108.read();  x2 = FAMOut_108.read();  break;
                case 109: x1 = FAMOut_109.read();  x2 = FAMOut_109.read();  break;
                case 110: x1 = FAMOut_110.read();  x2 = FAMOut_110.read();  break;
                case 111: x1 = FAMOut_111.read();  x2 = FAMOut_111.read();  break;
                case 112: x1 = FAMOut_112.read();  x2 = FAMOut_112.read();  break;
                case 113: x1 = FAMOut_113.read();  x2 = FAMOut_113.read();  break;
                case 114: x1 = FAMOut_114.read();  x2 = FAMOut_114.read();  break;
                case 115: x1 = FAMOut_115.read();  x2 = FAMOut_115.read();  break;
                case 116: x1 = FAMOut_116.read();  x2 = FAMOut_116.read();  break;
                case 117: x1 = FAMOut_117.read();  x2 = FAMOut_117.read();  break;
                case 118: x1 = FAMOut_118.read();  x2 = FAMOut_118.read();  break;
                case 119: x1 = FAMOut_119.read();  x2 = FAMOut_119.read();  break;
                case 120: x1 = FAMOut_120.read();  x2 = FAMOut_120.read();  break;
                case 121: x1 = FAMOut_121.read();  x2 = FAMOut_121.read();  break;
                case 122: x1 = FAMOut_122.read();  x2 = FAMOut_122.read();  break;
                case 123: x1 = FAMOut_123.read();  x2 = FAMOut_123.read();  break;
                case 124: x1 = FAMOut_124.read();  x2 = FAMOut_124.read();  break;
                case 125: x1 = FAMOut_125.read();  x2 = FAMOut_125.read();  break;
                case 126: x1 = FAMOut_126.read();  x2 = FAMOut_126.read();  break;
                case 127: x1 = FAMOut_127.read(); x2 = FAMOut_127.read(); break;
                }
                tmp1(64*k+63, 64*k) = x1.data;
                tmp2(64*k+63, 64*k) = x2.data;
            }

            // 写到 memout
            memout[ i*(128*2) + ch*2 + 0 ] = tmp1;
            memout[ i*(128*2) + ch*2 + 1 ] = tmp2;
        }
    }

*/