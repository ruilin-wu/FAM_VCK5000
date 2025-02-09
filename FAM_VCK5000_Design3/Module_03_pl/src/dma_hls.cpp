/***************************************************************************/
/*  Copyright (C) 2023 Advanced Micro Devices, Inc
 *  SPDX-License-Identifier: MIT
 *
 *  Example: Using ap_uint<512> for AXI Master interface, transferring 8 cfloat (64bit) at a time
 *  Reading 8 input channels × 1024 cfloat from memin
 *  Writing to 128 output channels × 8192 cfloat
 */
/***************************************************************************/

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

//-------------------------------------
// Defines the data type of the 64bit width AXI-Stream.
//-------------------------------------
typedef ap_axiu<64, 0, 0, 0> axis_data_64;

//-------------------------------------
// 1) Read data from DDR -> Write to input stream of AIE stage1
//-------------------------------------
void readDDR_to_AIE1(
    ap_uint<64> *memin0,
    ap_uint<64> *memin1,
    ap_uint<64> *memin2,
    ap_uint<64> *memin3,
    ap_uint<64> *memin4,
    ap_uint<64> *memin5,
    ap_uint<64> *memin6,
    ap_uint<64> *memin7,
    ap_uint<64> *memin8,
    ap_uint<64> *memin9,
    ap_uint<64> *memin10,
    ap_uint<64> *memin11,
    ap_uint<64> *memin12,
    ap_uint<64> *memin13,
    ap_uint<64> *memin14,
    ap_uint<64> *memin15,
    
    hls::stream<axis_data_64> &stage1_FAMDataIn_0,
    hls::stream<axis_data_64> &stage1_FAMDataIn_1,
    hls::stream<axis_data_64> &stage1_FAMDataIn_2,
    hls::stream<axis_data_64> &stage1_FAMDataIn_3,
    hls::stream<axis_data_64> &stage1_FAMDataIn_4,
    hls::stream<axis_data_64> &stage1_FAMDataIn_5,
    hls::stream<axis_data_64> &stage1_FAMDataIn_6,
    hls::stream<axis_data_64> &stage1_FAMDataIn_7,
    hls::stream<axis_data_64> &stage1_FAMDataIn_8,
    hls::stream<axis_data_64> &stage1_FAMDataIn_9,
    hls::stream<axis_data_64> &stage1_FAMDataIn_10,
    hls::stream<axis_data_64> &stage1_FAMDataIn_11,
    hls::stream<axis_data_64> &stage1_FAMDataIn_12,
    hls::stream<axis_data_64> &stage1_FAMDataIn_13,
    hls::stream<axis_data_64> &stage1_FAMDataIn_14,
    hls::stream<axis_data_64> &stage1_FAMDataIn_15
    
    
) {
#pragma HLS INLINE off
    axis_data_64 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14, tmp15;
    for(int i=0; i<512; i++){
    #pragma HLS PIPELINE II=1    
        tmp0.data = memin0[i];  stage1_FAMDataIn_0.write(tmp0);
        tmp1.data = memin1[i];  stage1_FAMDataIn_1.write(tmp1);
        tmp2.data = memin2[i];  stage1_FAMDataIn_2.write(tmp2);
        tmp3.data = memin3[i];  stage1_FAMDataIn_3.write(tmp3);
        tmp4.data = memin4[i];  stage1_FAMDataIn_4.write(tmp4);
        tmp5.data = memin5[i];  stage1_FAMDataIn_5.write(tmp5);
        tmp6.data = memin6[i];  stage1_FAMDataIn_6.write(tmp6);
        tmp7.data = memin7[i];  stage1_FAMDataIn_7.write(tmp7);
        tmp8.data = memin8[i];  stage1_FAMDataIn_8.write(tmp8);
        tmp9.data = memin9[i];  stage1_FAMDataIn_9.write(tmp9);
        tmp10.data = memin10[i];  stage1_FAMDataIn_10.write(tmp10);
        tmp11.data = memin11[i];  stage1_FAMDataIn_11.write(tmp11);
        tmp12.data = memin12[i];  stage1_FAMDataIn_12.write(tmp12);
        tmp13.data = memin13[i];  stage1_FAMDataIn_13.write(tmp13);
        tmp14.data = memin14[i];  stage1_FAMDataIn_14.write(tmp14);
        tmp15.data = memin15[i];  stage1_FAMDataIn_15.write(tmp15);
    }
}

//-------------------------------------
// 2) Output stream from AIE stage1 -> read and store to intermediate cache memtrans
//-------------------------------------
void readAIE1_to_memtrans(
    hls::stream<axis_data_64> &stage1_FAMOut0,
    hls::stream<axis_data_64> &stage1_FAMOut1,
    hls::stream<axis_data_64> &stage1_FAMOut2,
    hls::stream<axis_data_64> &stage1_FAMOut3,
    hls::stream<axis_data_64> &stage1_FAMOut4,
    hls::stream<axis_data_64> &stage1_FAMOut5,
    hls::stream<axis_data_64> &stage1_FAMOut6,
    hls::stream<axis_data_64> &stage1_FAMOut7,
    hls::stream<axis_data_64> &stage1_FAMOut8,
    hls::stream<axis_data_64> &stage1_FAMOut9,
    hls::stream<axis_data_64> &stage1_FAMOut10,
    hls::stream<axis_data_64> &stage1_FAMOut11,
    hls::stream<axis_data_64> &stage1_FAMOut12,
    hls::stream<axis_data_64> &stage1_FAMOut13,
    hls::stream<axis_data_64> &stage1_FAMOut14,
    hls::stream<axis_data_64> &stage1_FAMOut15,
    
    ap_uint<64>* memtrans  // intermediate cache
    
) {
#pragma HLS INLINE off

    axis_data_64 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    axis_data_64 tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14, tmp15;
    axis_data_64 tmp16, tmp17, tmp18, tmp19, tmp20, tmp21, tmp22, tmp23;
    axis_data_64 tmp24, tmp25, tmp26, tmp27, tmp28, tmp29, tmp30, tmp31;
    
    for (int i = 0; i < 256; i++) {
        #pragma HLS UNROLL factor=4
        
        
        tmp0  = stage1_FAMOut0.read();
        memtrans[i * 32 + 0]  = tmp0.data;
        tmp1  = stage1_FAMOut0.read();
        memtrans[i * 32 + 1]  = tmp1.data;

        tmp2  = stage1_FAMOut1.read();
        memtrans[i * 32 + 2]  = tmp2.data;
        tmp3  = stage1_FAMOut1.read();
        memtrans[i * 32 + 3]  = tmp3.data;

        tmp4  = stage1_FAMOut2.read();
        memtrans[i * 32 + 4]  = tmp4.data;
        tmp5  = stage1_FAMOut2.read();
        memtrans[i * 32 + 5]  = tmp5.data;

        tmp6  = stage1_FAMOut3.read();
        memtrans[i * 32 + 6]  = tmp6.data;
        tmp7  = stage1_FAMOut3.read();
        memtrans[i * 32 + 7]  = tmp7.data;

        tmp8  = stage1_FAMOut4.read();
        memtrans[i * 32 + 8]  = tmp8.data;
        tmp9  = stage1_FAMOut4.read();
        memtrans[i * 32 + 9]  = tmp9.data;

        tmp10 = stage1_FAMOut5.read();
        memtrans[i * 32 + 10] = tmp10.data;
        tmp11 = stage1_FAMOut5.read();
        memtrans[i * 32 + 11] = tmp11.data;

        tmp12 = stage1_FAMOut6.read();
        memtrans[i * 32 + 12] = tmp12.data;
        tmp13 = stage1_FAMOut6.read();
        memtrans[i * 32 + 13] = tmp13.data;

        tmp14 = stage1_FAMOut7.read();
        memtrans[i * 32 + 14] = tmp14.data;
        tmp15 = stage1_FAMOut7.read();
        memtrans[i * 32 + 15] = tmp15.data;

        tmp16 = stage1_FAMOut8.read();
        memtrans[i * 32 + 16] = tmp16.data;
        tmp17 = stage1_FAMOut8.read();
        memtrans[i * 32 + 17] = tmp17.data;

        tmp18 = stage1_FAMOut9.read();
        memtrans[i * 32 + 18] = tmp18.data;
        tmp19 = stage1_FAMOut9.read();
        memtrans[i * 32 + 19] = tmp19.data;

        tmp20 = stage1_FAMOut10.read();
        memtrans[i * 32 + 20] = tmp20.data;
        tmp21 = stage1_FAMOut10.read();
        memtrans[i * 32 + 21] = tmp21.data;

        tmp22 = stage1_FAMOut11.read();
        memtrans[i * 32 + 22] = tmp22.data;
        tmp23 = stage1_FAMOut11.read();
        memtrans[i * 32 + 23] = tmp23.data;

        tmp24 = stage1_FAMOut12.read();
        memtrans[i * 32 + 24] = tmp24.data;
        tmp25 = stage1_FAMOut12.read();
        memtrans[i * 32 + 25] = tmp25.data;

        tmp26 = stage1_FAMOut13.read();
        memtrans[i * 32 + 26] = tmp26.data;
        tmp27 = stage1_FAMOut13.read();
        memtrans[i * 32 + 27] = tmp27.data;

        tmp28 = stage1_FAMOut14.read();
        memtrans[i * 32 + 28] = tmp28.data;
        tmp29 = stage1_FAMOut14.read();
        memtrans[i * 32 + 29] = tmp29.data;

        tmp30 = stage1_FAMOut15.read();
        memtrans[i * 32 + 30] = tmp30.data;
        tmp31 = stage1_FAMOut15.read();
        memtrans[i * 32 + 31] = tmp31.data;
    }
}


//-------------------------------------
// 3) Read from memtrans -> write to input stream of AIE stage2
//-------------------------------------
void plTranspose_to_aie2(
    ap_uint<64>* memtrans,  
    hls::stream<axis_data_64> &stage2_FAMDataIn_0,hls::stream<axis_data_64> &stage2_FAMDataIn_1,hls::stream<axis_data_64> &stage2_FAMDataIn_2,hls::stream<axis_data_64> &stage2_FAMDataIn_3,
    hls::stream<axis_data_64> &stage2_FAMDataIn_4,hls::stream<axis_data_64> &stage2_FAMDataIn_5,hls::stream<axis_data_64> &stage2_FAMDataIn_6,hls::stream<axis_data_64> &stage2_FAMDataIn_7,
    hls::stream<axis_data_64> &stage2_FAMDataIn_8,hls::stream<axis_data_64> &stage2_FAMDataIn_9,hls::stream<axis_data_64> &stage2_FAMDataIn_10,hls::stream<axis_data_64> &stage2_FAMDataIn_11,
    hls::stream<axis_data_64> &stage2_FAMDataIn_12,hls::stream<axis_data_64> &stage2_FAMDataIn_13,hls::stream<axis_data_64> &stage2_FAMDataIn_14,hls::stream<axis_data_64> &stage2_FAMDataIn_15,
    hls::stream<axis_data_64> &stage2_FAMDataIn_16,hls::stream<axis_data_64> &stage2_FAMDataIn_17,hls::stream<axis_data_64> &stage2_FAMDataIn_18,hls::stream<axis_data_64> &stage2_FAMDataIn_19,
    hls::stream<axis_data_64> &stage2_FAMDataIn_20,hls::stream<axis_data_64> &stage2_FAMDataIn_21,hls::stream<axis_data_64> &stage2_FAMDataIn_22,hls::stream<axis_data_64> &stage2_FAMDataIn_23,
    hls::stream<axis_data_64> &stage2_FAMDataIn_24,hls::stream<axis_data_64> &stage2_FAMDataIn_25,hls::stream<axis_data_64> &stage2_FAMDataIn_26,hls::stream<axis_data_64> &stage2_FAMDataIn_27,
    hls::stream<axis_data_64> &stage2_FAMDataIn_28,hls::stream<axis_data_64> &stage2_FAMDataIn_29,hls::stream<axis_data_64> &stage2_FAMDataIn_30,hls::stream<axis_data_64> &stage2_FAMDataIn_31,
    hls::stream<axis_data_64> &stage2_FAMDataIn_32,hls::stream<axis_data_64> &stage2_FAMDataIn_33,hls::stream<axis_data_64> &stage2_FAMDataIn_34,hls::stream<axis_data_64> &stage2_FAMDataIn_35,
    hls::stream<axis_data_64> &stage2_FAMDataIn_36,hls::stream<axis_data_64> &stage2_FAMDataIn_37,hls::stream<axis_data_64> &stage2_FAMDataIn_38,hls::stream<axis_data_64> &stage2_FAMDataIn_39,
    hls::stream<axis_data_64> &stage2_FAMDataIn_40,hls::stream<axis_data_64> &stage2_FAMDataIn_41,hls::stream<axis_data_64> &stage2_FAMDataIn_42,hls::stream<axis_data_64> &stage2_FAMDataIn_43,
    hls::stream<axis_data_64> &stage2_FAMDataIn_44,hls::stream<axis_data_64> &stage2_FAMDataIn_45,hls::stream<axis_data_64> &stage2_FAMDataIn_46,hls::stream<axis_data_64> &stage2_FAMDataIn_47,
    hls::stream<axis_data_64> &stage2_FAMDataIn_48,hls::stream<axis_data_64> &stage2_FAMDataIn_49,hls::stream<axis_data_64> &stage2_FAMDataIn_50,hls::stream<axis_data_64> &stage2_FAMDataIn_51,
    hls::stream<axis_data_64> &stage2_FAMDataIn_52,hls::stream<axis_data_64> &stage2_FAMDataIn_53,hls::stream<axis_data_64> &stage2_FAMDataIn_54,hls::stream<axis_data_64> &stage2_FAMDataIn_55,
    hls::stream<axis_data_64> &stage2_FAMDataIn_56,hls::stream<axis_data_64> &stage2_FAMDataIn_57,hls::stream<axis_data_64> &stage2_FAMDataIn_58,hls::stream<axis_data_64> &stage2_FAMDataIn_59,
    hls::stream<axis_data_64> &stage2_FAMDataIn_60,hls::stream<axis_data_64> &stage2_FAMDataIn_61,hls::stream<axis_data_64> &stage2_FAMDataIn_62,hls::stream<axis_data_64> &stage2_FAMDataIn_63,
    hls::stream<axis_data_64> &stage2_FAMDataIn_64,hls::stream<axis_data_64> &stage2_FAMDataIn_65,hls::stream<axis_data_64> &stage2_FAMDataIn_66,hls::stream<axis_data_64> &stage2_FAMDataIn_67,
    hls::stream<axis_data_64> &stage2_FAMDataIn_68,hls::stream<axis_data_64> &stage2_FAMDataIn_69,hls::stream<axis_data_64> &stage2_FAMDataIn_70,hls::stream<axis_data_64> &stage2_FAMDataIn_71,
    hls::stream<axis_data_64> &stage2_FAMDataIn_72,hls::stream<axis_data_64> &stage2_FAMDataIn_73,hls::stream<axis_data_64> &stage2_FAMDataIn_74,hls::stream<axis_data_64> &stage2_FAMDataIn_75,
    hls::stream<axis_data_64> &stage2_FAMDataIn_76,hls::stream<axis_data_64> &stage2_FAMDataIn_77,hls::stream<axis_data_64> &stage2_FAMDataIn_78,hls::stream<axis_data_64> &stage2_FAMDataIn_79,
    hls::stream<axis_data_64> &stage2_FAMDataIn_80,hls::stream<axis_data_64> &stage2_FAMDataIn_81,hls::stream<axis_data_64> &stage2_FAMDataIn_82,hls::stream<axis_data_64> &stage2_FAMDataIn_83,
    hls::stream<axis_data_64> &stage2_FAMDataIn_84,hls::stream<axis_data_64> &stage2_FAMDataIn_85,hls::stream<axis_data_64> &stage2_FAMDataIn_86,hls::stream<axis_data_64> &stage2_FAMDataIn_87,
    hls::stream<axis_data_64> &stage2_FAMDataIn_88,hls::stream<axis_data_64> &stage2_FAMDataIn_89,hls::stream<axis_data_64> &stage2_FAMDataIn_90,hls::stream<axis_data_64> &stage2_FAMDataIn_91,
    hls::stream<axis_data_64> &stage2_FAMDataIn_92,hls::stream<axis_data_64> &stage2_FAMDataIn_93,hls::stream<axis_data_64> &stage2_FAMDataIn_94,hls::stream<axis_data_64> &stage2_FAMDataIn_95,
    hls::stream<axis_data_64> &stage2_FAMDataIn_96,hls::stream<axis_data_64> &stage2_FAMDataIn_97,hls::stream<axis_data_64> &stage2_FAMDataIn_98,hls::stream<axis_data_64> &stage2_FAMDataIn_99,
    hls::stream<axis_data_64> &stage2_FAMDataIn_100,hls::stream<axis_data_64> &stage2_FAMDataIn_101,hls::stream<axis_data_64> &stage2_FAMDataIn_102,hls::stream<axis_data_64> &stage2_FAMDataIn_103,
    hls::stream<axis_data_64> &stage2_FAMDataIn_104,hls::stream<axis_data_64> &stage2_FAMDataIn_105,hls::stream<axis_data_64> &stage2_FAMDataIn_106,hls::stream<axis_data_64> &stage2_FAMDataIn_107,
    hls::stream<axis_data_64> &stage2_FAMDataIn_108,hls::stream<axis_data_64> &stage2_FAMDataIn_109,hls::stream<axis_data_64> &stage2_FAMDataIn_110,hls::stream<axis_data_64> &stage2_FAMDataIn_111,
    hls::stream<axis_data_64> &stage2_FAMDataIn_112,hls::stream<axis_data_64> &stage2_FAMDataIn_113,hls::stream<axis_data_64> &stage2_FAMDataIn_114,hls::stream<axis_data_64> &stage2_FAMDataIn_115,
    hls::stream<axis_data_64> &stage2_FAMDataIn_116,hls::stream<axis_data_64> &stage2_FAMDataIn_117,hls::stream<axis_data_64> &stage2_FAMDataIn_118,hls::stream<axis_data_64> &stage2_FAMDataIn_119,
    hls::stream<axis_data_64> &stage2_FAMDataIn_120,hls::stream<axis_data_64> &stage2_FAMDataIn_121,hls::stream<axis_data_64> &stage2_FAMDataIn_122,hls::stream<axis_data_64> &stage2_FAMDataIn_123,
    hls::stream<axis_data_64> &stage2_FAMDataIn_124,hls::stream<axis_data_64> &stage2_FAMDataIn_125,hls::stream<axis_data_64> &stage2_FAMDataIn_126,hls::stream<axis_data_64> &stage2_FAMDataIn_127
    
    
) {
#pragma HLS INLINE off

    hls::stream<axis_data_64>* stage2_FAMDataIn[128] = {
        &stage2_FAMDataIn_0, &stage2_FAMDataIn_1, &stage2_FAMDataIn_2, &stage2_FAMDataIn_3, &stage2_FAMDataIn_4, &stage2_FAMDataIn_5, &stage2_FAMDataIn_6, &stage2_FAMDataIn_7,
        &stage2_FAMDataIn_8, &stage2_FAMDataIn_9, &stage2_FAMDataIn_10, &stage2_FAMDataIn_11, &stage2_FAMDataIn_12, &stage2_FAMDataIn_13, &stage2_FAMDataIn_14, &stage2_FAMDataIn_15,
        &stage2_FAMDataIn_16, &stage2_FAMDataIn_17, &stage2_FAMDataIn_18, &stage2_FAMDataIn_19, &stage2_FAMDataIn_20, &stage2_FAMDataIn_21, &stage2_FAMDataIn_22, &stage2_FAMDataIn_23,
        &stage2_FAMDataIn_24, &stage2_FAMDataIn_25, &stage2_FAMDataIn_26, &stage2_FAMDataIn_27, &stage2_FAMDataIn_28, &stage2_FAMDataIn_29, &stage2_FAMDataIn_30, &stage2_FAMDataIn_31,
        &stage2_FAMDataIn_32, &stage2_FAMDataIn_33, &stage2_FAMDataIn_34, &stage2_FAMDataIn_35, &stage2_FAMDataIn_36, &stage2_FAMDataIn_37, &stage2_FAMDataIn_38, &stage2_FAMDataIn_39,
        &stage2_FAMDataIn_40, &stage2_FAMDataIn_41, &stage2_FAMDataIn_42, &stage2_FAMDataIn_43, &stage2_FAMDataIn_44, &stage2_FAMDataIn_45, &stage2_FAMDataIn_46, &stage2_FAMDataIn_47,
        &stage2_FAMDataIn_48, &stage2_FAMDataIn_49, &stage2_FAMDataIn_50, &stage2_FAMDataIn_51, &stage2_FAMDataIn_52, &stage2_FAMDataIn_53, &stage2_FAMDataIn_54, &stage2_FAMDataIn_55,
        &stage2_FAMDataIn_56, &stage2_FAMDataIn_57, &stage2_FAMDataIn_58, &stage2_FAMDataIn_59, &stage2_FAMDataIn_60, &stage2_FAMDataIn_61, &stage2_FAMDataIn_62, &stage2_FAMDataIn_63,
        &stage2_FAMDataIn_64, &stage2_FAMDataIn_65, &stage2_FAMDataIn_66, &stage2_FAMDataIn_67, &stage2_FAMDataIn_68, &stage2_FAMDataIn_69, &stage2_FAMDataIn_70, &stage2_FAMDataIn_71,
        &stage2_FAMDataIn_72, &stage2_FAMDataIn_73, &stage2_FAMDataIn_74, &stage2_FAMDataIn_75, &stage2_FAMDataIn_76, &stage2_FAMDataIn_77, &stage2_FAMDataIn_78, &stage2_FAMDataIn_79,
        &stage2_FAMDataIn_80, &stage2_FAMDataIn_81, &stage2_FAMDataIn_82, &stage2_FAMDataIn_83, &stage2_FAMDataIn_84, &stage2_FAMDataIn_85, &stage2_FAMDataIn_86, &stage2_FAMDataIn_87,
        &stage2_FAMDataIn_88, &stage2_FAMDataIn_89, &stage2_FAMDataIn_90, &stage2_FAMDataIn_91, &stage2_FAMDataIn_92, &stage2_FAMDataIn_93, &stage2_FAMDataIn_94, &stage2_FAMDataIn_95,
        &stage2_FAMDataIn_96, &stage2_FAMDataIn_97, &stage2_FAMDataIn_98, &stage2_FAMDataIn_99, &stage2_FAMDataIn_100, &stage2_FAMDataIn_101, &stage2_FAMDataIn_102, &stage2_FAMDataIn_103,
        &stage2_FAMDataIn_104, &stage2_FAMDataIn_105, &stage2_FAMDataIn_106, &stage2_FAMDataIn_107, &stage2_FAMDataIn_108, &stage2_FAMDataIn_109, &stage2_FAMDataIn_110, &stage2_FAMDataIn_111,
        &stage2_FAMDataIn_112, &stage2_FAMDataIn_113, &stage2_FAMDataIn_114, &stage2_FAMDataIn_115, &stage2_FAMDataIn_116, &stage2_FAMDataIn_117, &stage2_FAMDataIn_118, &stage2_FAMDataIn_119,
        &stage2_FAMDataIn_120, &stage2_FAMDataIn_121, &stage2_FAMDataIn_122, &stage2_FAMDataIn_123, &stage2_FAMDataIn_124, &stage2_FAMDataIn_125, &stage2_FAMDataIn_126, &stage2_FAMDataIn_127
    };
    #pragma HLS array_partition variable=stage2_FAMDataIn complete

    // Define 8 corresponding vectors
    axis_data_64 axis0, axis1, axis2, axis3, axis4, axis5, axis6, axis7;

    // Outer loop iterates over each data packet, inner loop iterates over all channels to achieve channel parallelization
    for (int pkt = 0; pkt < 8; pkt++) {  
        #pragma HLS PIPELINE II=1  // High-efficiency pipeline to maintain high throughput
        for (int ch = 0; ch < 128; ch++) {  
            #pragma HLS UNROLL factor=4  // Loop unrolling to enhance hardware parallelism
            // Compute the index for memtrans
            int idx = ch * 64 + pkt * 8;
            // Read a 512-bit data packet
            // ap_uint<512> packet = memtrans[idx];
            // Directly write 8 64-bit data to corresponding channels
            axis0.data = memtrans[idx + 0]; stage2_FAMDataIn[ch]->write(axis0);
            axis1.data = memtrans[idx + 1]; stage2_FAMDataIn[ch]->write(axis1);
            axis2.data = memtrans[idx + 2]; stage2_FAMDataIn[ch]->write(axis2);
            axis3.data = memtrans[idx + 3]; stage2_FAMDataIn[ch]->write(axis3);
            axis4.data = memtrans[idx + 4]; stage2_FAMDataIn[ch]->write(axis4);
            axis5.data = memtrans[idx + 5]; stage2_FAMDataIn[ch]->write(axis5);
            axis6.data = memtrans[idx + 6]; stage2_FAMDataIn[ch]->write(axis6);
            axis7.data = memtrans[idx + 7]; stage2_FAMDataIn[ch]->write(axis7);
        }
    }

    // Write all vectors
    // Outer loop iterates over each data packet, inner loop iterates over all channels to achieve channel parallelization
    for (int pkt = 0; pkt < 1024; pkt++) {  
        #pragma HLS PIPELINE II=1  // High-efficiency pipeline to maintain high throughput
        for (int ch = 0; ch < 128; ch++) {  
            #pragma HLS UNROLL factor=4  // Loop unrolling to enhance hardware parallelism
            // Compute the index for memtrans
            int idx = pkt * 8;
            // Read a 512-bit data packet
            // ap_uint<512> packet = memtrans[idx];
            // Directly write 8 64-bit data to corresponding channels
            axis0.data = memtrans[idx + 0]; stage2_FAMDataIn[ch]->write(axis0);
            axis1.data = memtrans[idx + 1]; stage2_FAMDataIn[ch]->write(axis1);
            axis2.data = memtrans[idx + 2]; stage2_FAMDataIn[ch]->write(axis2);
            axis3.data = memtrans[idx + 3]; stage2_FAMDataIn[ch]->write(axis3);
            axis4.data = memtrans[idx + 4]; stage2_FAMDataIn[ch]->write(axis4);
            axis5.data = memtrans[idx + 5]; stage2_FAMDataIn[ch]->write(axis5);
            axis6.data = memtrans[idx + 6]; stage2_FAMDataIn[ch]->write(axis6);
            axis7.data = memtrans[idx + 7]; stage2_FAMDataIn[ch]->write(axis7);
        }
    }  
}


//-------------------------------------
// 4) Output Stream from AIE stage2 -> splitting into 512bit and writing back to DDR memout
//-------------------------------------
void aie2_to_ddr(
    hls::stream<axis_data_64> &stage2_FAMOut0,hls::stream<axis_data_64> &stage2_FAMOut1,hls::stream<axis_data_64> &stage2_FAMOut2,hls::stream<axis_data_64> &stage2_FAMOut3,
    hls::stream<axis_data_64> &stage2_FAMOut4,hls::stream<axis_data_64> &stage2_FAMOut5,hls::stream<axis_data_64> &stage2_FAMOut6,hls::stream<axis_data_64> &stage2_FAMOut7,
    hls::stream<axis_data_64> &stage2_FAMOut8,hls::stream<axis_data_64> &stage2_FAMOut9,hls::stream<axis_data_64> &stage2_FAMOut10,hls::stream<axis_data_64> &stage2_FAMOut11,
    hls::stream<axis_data_64> &stage2_FAMOut12,hls::stream<axis_data_64> &stage2_FAMOut13,hls::stream<axis_data_64> &stage2_FAMOut14,hls::stream<axis_data_64> &stage2_FAMOut15,
    hls::stream<axis_data_64> &stage2_FAMOut16,hls::stream<axis_data_64> &stage2_FAMOut17,hls::stream<axis_data_64> &stage2_FAMOut18,hls::stream<axis_data_64> &stage2_FAMOut19,
    hls::stream<axis_data_64> &stage2_FAMOut20,hls::stream<axis_data_64> &stage2_FAMOut21,hls::stream<axis_data_64> &stage2_FAMOut22,hls::stream<axis_data_64> &stage2_FAMOut23,
    hls::stream<axis_data_64> &stage2_FAMOut24,hls::stream<axis_data_64> &stage2_FAMOut25,hls::stream<axis_data_64> &stage2_FAMOut26,hls::stream<axis_data_64> &stage2_FAMOut27,
    hls::stream<axis_data_64> &stage2_FAMOut28,hls::stream<axis_data_64> &stage2_FAMOut29,hls::stream<axis_data_64> &stage2_FAMOut30,hls::stream<axis_data_64> &stage2_FAMOut31,
    hls::stream<axis_data_64> &stage2_FAMOut32,hls::stream<axis_data_64> &stage2_FAMOut33,hls::stream<axis_data_64> &stage2_FAMOut34,hls::stream<axis_data_64> &stage2_FAMOut35,
    hls::stream<axis_data_64> &stage2_FAMOut36,hls::stream<axis_data_64> &stage2_FAMOut37,hls::stream<axis_data_64> &stage2_FAMOut38,hls::stream<axis_data_64> &stage2_FAMOut39,
    hls::stream<axis_data_64> &stage2_FAMOut40,hls::stream<axis_data_64> &stage2_FAMOut41,hls::stream<axis_data_64> &stage2_FAMOut42,hls::stream<axis_data_64> &stage2_FAMOut43,
    hls::stream<axis_data_64> &stage2_FAMOut44,hls::stream<axis_data_64> &stage2_FAMOut45,hls::stream<axis_data_64> &stage2_FAMOut46,hls::stream<axis_data_64> &stage2_FAMOut47,
    hls::stream<axis_data_64> &stage2_FAMOut48,hls::stream<axis_data_64> &stage2_FAMOut49,hls::stream<axis_data_64> &stage2_FAMOut50,hls::stream<axis_data_64> &stage2_FAMOut51,
    hls::stream<axis_data_64> &stage2_FAMOut52,hls::stream<axis_data_64> &stage2_FAMOut53,hls::stream<axis_data_64> &stage2_FAMOut54,hls::stream<axis_data_64> &stage2_FAMOut55,
    hls::stream<axis_data_64> &stage2_FAMOut56,hls::stream<axis_data_64> &stage2_FAMOut57,hls::stream<axis_data_64> &stage2_FAMOut58,hls::stream<axis_data_64> &stage2_FAMOut59,
    hls::stream<axis_data_64> &stage2_FAMOut60,hls::stream<axis_data_64> &stage2_FAMOut61,hls::stream<axis_data_64> &stage2_FAMOut62,hls::stream<axis_data_64> &stage2_FAMOut63,
    hls::stream<axis_data_64> &stage2_FAMOut64,hls::stream<axis_data_64> &stage2_FAMOut65,hls::stream<axis_data_64> &stage2_FAMOut66,hls::stream<axis_data_64> &stage2_FAMOut67,
    hls::stream<axis_data_64> &stage2_FAMOut68,hls::stream<axis_data_64> &stage2_FAMOut69,hls::stream<axis_data_64> &stage2_FAMOut70,hls::stream<axis_data_64> &stage2_FAMOut71,
    hls::stream<axis_data_64> &stage2_FAMOut72,hls::stream<axis_data_64> &stage2_FAMOut73,hls::stream<axis_data_64> &stage2_FAMOut74,hls::stream<axis_data_64> &stage2_FAMOut75,
    hls::stream<axis_data_64> &stage2_FAMOut76,hls::stream<axis_data_64> &stage2_FAMOut77,hls::stream<axis_data_64> &stage2_FAMOut78,hls::stream<axis_data_64> &stage2_FAMOut79,
    hls::stream<axis_data_64> &stage2_FAMOut80,hls::stream<axis_data_64> &stage2_FAMOut81,hls::stream<axis_data_64> &stage2_FAMOut82,hls::stream<axis_data_64> &stage2_FAMOut83,
    hls::stream<axis_data_64> &stage2_FAMOut84,hls::stream<axis_data_64> &stage2_FAMOut85,hls::stream<axis_data_64> &stage2_FAMOut86,hls::stream<axis_data_64> &stage2_FAMOut87,
    hls::stream<axis_data_64> &stage2_FAMOut88,hls::stream<axis_data_64> &stage2_FAMOut89,hls::stream<axis_data_64> &stage2_FAMOut90,hls::stream<axis_data_64> &stage2_FAMOut91,
    hls::stream<axis_data_64> &stage2_FAMOut92,hls::stream<axis_data_64> &stage2_FAMOut93,hls::stream<axis_data_64> &stage2_FAMOut94,hls::stream<axis_data_64> &stage2_FAMOut95,
    hls::stream<axis_data_64> &stage2_FAMOut96,hls::stream<axis_data_64> &stage2_FAMOut97,hls::stream<axis_data_64> &stage2_FAMOut98,hls::stream<axis_data_64> &stage2_FAMOut99,
    hls::stream<axis_data_64> &stage2_FAMOut100,hls::stream<axis_data_64> &stage2_FAMOut101,hls::stream<axis_data_64> &stage2_FAMOut102,hls::stream<axis_data_64> &stage2_FAMOut103,
    hls::stream<axis_data_64> &stage2_FAMOut104,hls::stream<axis_data_64> &stage2_FAMOut105,hls::stream<axis_data_64> &stage2_FAMOut106,hls::stream<axis_data_64> &stage2_FAMOut107,
    hls::stream<axis_data_64> &stage2_FAMOut108,hls::stream<axis_data_64> &stage2_FAMOut109,hls::stream<axis_data_64> &stage2_FAMOut110,hls::stream<axis_data_64> &stage2_FAMOut111,
    hls::stream<axis_data_64> &stage2_FAMOut112,hls::stream<axis_data_64> &stage2_FAMOut113,hls::stream<axis_data_64> &stage2_FAMOut114,hls::stream<axis_data_64> &stage2_FAMOut115,
    hls::stream<axis_data_64> &stage2_FAMOut116,hls::stream<axis_data_64> &stage2_FAMOut117,hls::stream<axis_data_64> &stage2_FAMOut118,hls::stream<axis_data_64> &stage2_FAMOut119,
    hls::stream<axis_data_64> &stage2_FAMOut120,hls::stream<axis_data_64> &stage2_FAMOut121,hls::stream<axis_data_64> &stage2_FAMOut122,hls::stream<axis_data_64> &stage2_FAMOut123,
    hls::stream<axis_data_64> &stage2_FAMOut124,hls::stream<axis_data_64> &stage2_FAMOut125,hls::stream<axis_data_64> &stage2_FAMOut126,hls::stream<axis_data_64> &stage2_FAMOut127,
    
    ap_uint<512>* memout
) {
#pragma HLS INLINE off
    hls::stream<axis_data_64>* stage2_FAMOut[128] = {
        &stage2_FAMOut0, &stage2_FAMOut1, &stage2_FAMOut2, &stage2_FAMOut3, &stage2_FAMOut4, &stage2_FAMOut5, &stage2_FAMOut6, &stage2_FAMOut7,
        &stage2_FAMOut8, &stage2_FAMOut9, &stage2_FAMOut10, &stage2_FAMOut11, &stage2_FAMOut12, &stage2_FAMOut13, &stage2_FAMOut14, &stage2_FAMOut15,
        &stage2_FAMOut16, &stage2_FAMOut17, &stage2_FAMOut18, &stage2_FAMOut19, &stage2_FAMOut20, &stage2_FAMOut21, &stage2_FAMOut22, &stage2_FAMOut23,
        &stage2_FAMOut24, &stage2_FAMOut25, &stage2_FAMOut26, &stage2_FAMOut27, &stage2_FAMOut28, &stage2_FAMOut29, &stage2_FAMOut30, &stage2_FAMOut31,
        &stage2_FAMOut32, &stage2_FAMOut33, &stage2_FAMOut34, &stage2_FAMOut35, &stage2_FAMOut36, &stage2_FAMOut37, &stage2_FAMOut38, &stage2_FAMOut39,
        &stage2_FAMOut40, &stage2_FAMOut41, &stage2_FAMOut42, &stage2_FAMOut43, &stage2_FAMOut44, &stage2_FAMOut45, &stage2_FAMOut46, &stage2_FAMOut47,
        &stage2_FAMOut48, &stage2_FAMOut49, &stage2_FAMOut50, &stage2_FAMOut51, &stage2_FAMOut52, &stage2_FAMOut53, &stage2_FAMOut54, &stage2_FAMOut55,
        &stage2_FAMOut56, &stage2_FAMOut57, &stage2_FAMOut58, &stage2_FAMOut59, &stage2_FAMOut60, &stage2_FAMOut61, &stage2_FAMOut62, &stage2_FAMOut63,
        &stage2_FAMOut64, &stage2_FAMOut65, &stage2_FAMOut66, &stage2_FAMOut67, &stage2_FAMOut68, &stage2_FAMOut69, &stage2_FAMOut70, &stage2_FAMOut71,
        &stage2_FAMOut72, &stage2_FAMOut73, &stage2_FAMOut74, &stage2_FAMOut75, &stage2_FAMOut76, &stage2_FAMOut77, &stage2_FAMOut78, &stage2_FAMOut79,
        &stage2_FAMOut80, &stage2_FAMOut81, &stage2_FAMOut82, &stage2_FAMOut83, &stage2_FAMOut84, &stage2_FAMOut85, &stage2_FAMOut86, &stage2_FAMOut87,
        &stage2_FAMOut88, &stage2_FAMOut89, &stage2_FAMOut90, &stage2_FAMOut91, &stage2_FAMOut92, &stage2_FAMOut93, &stage2_FAMOut94, &stage2_FAMOut95,
        &stage2_FAMOut96, &stage2_FAMOut97, &stage2_FAMOut98, &stage2_FAMOut99, &stage2_FAMOut100, &stage2_FAMOut101, &stage2_FAMOut102, &stage2_FAMOut103,
        &stage2_FAMOut104, &stage2_FAMOut105, &stage2_FAMOut106, &stage2_FAMOut107, &stage2_FAMOut108, &stage2_FAMOut109, &stage2_FAMOut110, &stage2_FAMOut111,
        &stage2_FAMOut112, &stage2_FAMOut113, &stage2_FAMOut114, &stage2_FAMOut115, &stage2_FAMOut116, &stage2_FAMOut117, &stage2_FAMOut118, &stage2_FAMOut119,
        &stage2_FAMOut120, &stage2_FAMOut121, &stage2_FAMOut122, &stage2_FAMOut123, &stage2_FAMOut124, &stage2_FAMOut125, &stage2_FAMOut126, &stage2_FAMOut127
    };
    #pragma HLS array_partition variable=stage2_FAMOut complete

    for(int i = 0; i < 512; i++) {
    #pragma HLS PIPELINE II=1        
        for(int ch = 0; ch < 128; ch++) {          
            #pragma HLS PIPELINE II=1        
            ap_uint<512> tmp1, tmp2;                     
            for(int k = 0; k < 8; k++) {                  
            #pragma HLS UNROLL factor=4
                axis_data_64 x1 = stage2_FAMOut[ch]->read();
                axis_data_64 x2 = stage2_FAMOut[ch]->read();
                tmp1(64*k+63, 64*k) = x1.data;
                tmp2(64*k+63, 64*k) = x2.data;
            }           
            int base_addr = (ch << 10) + (i << 1); 
            memout[base_addr]     = tmp1;
            memout[base_addr + 1] = tmp2;              
        }
    }

}


//-------------------------------------
// (Optional) Execute steps 1 and 2 sequentially, packing them into a phase1()
//-------------------------------------
void phase1(
    ap_uint<64>* memin0,
    ap_uint<64>* memin1,
    ap_uint<64>* memin2,
    ap_uint<64>* memin3,
    ap_uint<64>* memin4,
    ap_uint<64>* memin5,
    ap_uint<64>* memin6,
    ap_uint<64>* memin7,
    ap_uint<64>* memin8,
    ap_uint<64>* memin9,
    ap_uint<64>* memin10,
    ap_uint<64>* memin11,
    ap_uint<64>* memin12,
    ap_uint<64>* memin13,
    ap_uint<64>* memin14,
    ap_uint<64>* memin15,
    
    hls::stream<axis_data_64> &stage1_FAMDataIn_0,
    hls::stream<axis_data_64> &stage1_FAMDataIn_1,
    hls::stream<axis_data_64> &stage1_FAMDataIn_2,
    hls::stream<axis_data_64> &stage1_FAMDataIn_3,
    hls::stream<axis_data_64> &stage1_FAMDataIn_4,
    hls::stream<axis_data_64> &stage1_FAMDataIn_5,
    hls::stream<axis_data_64> &stage1_FAMDataIn_6,
    hls::stream<axis_data_64> &stage1_FAMDataIn_7,
    hls::stream<axis_data_64> &stage1_FAMDataIn_8,
    hls::stream<axis_data_64> &stage1_FAMDataIn_9,
    hls::stream<axis_data_64> &stage1_FAMDataIn_10,
    hls::stream<axis_data_64> &stage1_FAMDataIn_11,
    hls::stream<axis_data_64> &stage1_FAMDataIn_12,
    hls::stream<axis_data_64> &stage1_FAMDataIn_13,
    hls::stream<axis_data_64> &stage1_FAMDataIn_14,
    hls::stream<axis_data_64> &stage1_FAMDataIn_15,
    
    hls::stream<axis_data_64> &stage1_FAMOut0,
    hls::stream<axis_data_64> &stage1_FAMOut1,
    hls::stream<axis_data_64> &stage1_FAMOut2,
    hls::stream<axis_data_64> &stage1_FAMOut3,
    hls::stream<axis_data_64> &stage1_FAMOut4,
    hls::stream<axis_data_64> &stage1_FAMOut5,
    hls::stream<axis_data_64> &stage1_FAMOut6,
    hls::stream<axis_data_64> &stage1_FAMOut7,
    hls::stream<axis_data_64> &stage1_FAMOut8,
    hls::stream<axis_data_64> &stage1_FAMOut9,
    hls::stream<axis_data_64> &stage1_FAMOut10,
    hls::stream<axis_data_64> &stage1_FAMOut11,
    hls::stream<axis_data_64> &stage1_FAMOut12,
    hls::stream<axis_data_64> &stage1_FAMOut13,
    hls::stream<axis_data_64> &stage1_FAMOut14,
    hls::stream<axis_data_64> &stage1_FAMOut15,
    
    ap_uint<64>* memtrans
   
) {
    // Execute DDR -> AIE1 first.
    readDDR_to_AIE1(
        memin0, memin1, memin2, memin3, memin4, memin5, memin6, memin7, memin8, memin9, memin10, memin11, memin12, memin13, memin14, memin15,
        stage1_FAMDataIn_0, stage1_FAMDataIn_1, stage1_FAMDataIn_2, stage1_FAMDataIn_3, 
        stage1_FAMDataIn_4, stage1_FAMDataIn_5, stage1_FAMDataIn_6, stage1_FAMDataIn_7, 
        stage1_FAMDataIn_8, stage1_FAMDataIn_9, stage1_FAMDataIn_10, stage1_FAMDataIn_11, 
        stage1_FAMDataIn_12, stage1_FAMDataIn_13, stage1_FAMDataIn_14, stage1_FAMDataIn_15   
    );
    // Wait until the previous step is completely finished before executing AIE1 -> memtrans
    readAIE1_to_memtrans(
        stage1_FAMOut0, stage1_FAMOut1, stage1_FAMOut2, stage1_FAMOut3, 
        stage1_FAMOut4, stage1_FAMOut5, stage1_FAMOut6, stage1_FAMOut7, 
        stage1_FAMOut8, stage1_FAMOut9, stage1_FAMOut10, stage1_FAMOut11, 
        stage1_FAMOut12, stage1_FAMOut13, stage1_FAMOut14, stage1_FAMOut15,
        memtrans
    );
}


void phase2(
    ap_uint<64>* memtrans,
    hls::stream<axis_data_64> &stage2_FAMDataIn_0,hls::stream<axis_data_64> &stage2_FAMDataIn_1,hls::stream<axis_data_64> &stage2_FAMDataIn_2,hls::stream<axis_data_64> &stage2_FAMDataIn_3,
    hls::stream<axis_data_64> &stage2_FAMDataIn_4,hls::stream<axis_data_64> &stage2_FAMDataIn_5,hls::stream<axis_data_64> &stage2_FAMDataIn_6,hls::stream<axis_data_64> &stage2_FAMDataIn_7,
    hls::stream<axis_data_64> &stage2_FAMDataIn_8,hls::stream<axis_data_64> &stage2_FAMDataIn_9,hls::stream<axis_data_64> &stage2_FAMDataIn_10,hls::stream<axis_data_64> &stage2_FAMDataIn_11,
    hls::stream<axis_data_64> &stage2_FAMDataIn_12,hls::stream<axis_data_64> &stage2_FAMDataIn_13,hls::stream<axis_data_64> &stage2_FAMDataIn_14,hls::stream<axis_data_64> &stage2_FAMDataIn_15,
    hls::stream<axis_data_64> &stage2_FAMDataIn_16,hls::stream<axis_data_64> &stage2_FAMDataIn_17,hls::stream<axis_data_64> &stage2_FAMDataIn_18,hls::stream<axis_data_64> &stage2_FAMDataIn_19,
    hls::stream<axis_data_64> &stage2_FAMDataIn_20,hls::stream<axis_data_64> &stage2_FAMDataIn_21,hls::stream<axis_data_64> &stage2_FAMDataIn_22,hls::stream<axis_data_64> &stage2_FAMDataIn_23,
    hls::stream<axis_data_64> &stage2_FAMDataIn_24,hls::stream<axis_data_64> &stage2_FAMDataIn_25,hls::stream<axis_data_64> &stage2_FAMDataIn_26,hls::stream<axis_data_64> &stage2_FAMDataIn_27,
    hls::stream<axis_data_64> &stage2_FAMDataIn_28,hls::stream<axis_data_64> &stage2_FAMDataIn_29,hls::stream<axis_data_64> &stage2_FAMDataIn_30,hls::stream<axis_data_64> &stage2_FAMDataIn_31,
    hls::stream<axis_data_64> &stage2_FAMDataIn_32,hls::stream<axis_data_64> &stage2_FAMDataIn_33,hls::stream<axis_data_64> &stage2_FAMDataIn_34,hls::stream<axis_data_64> &stage2_FAMDataIn_35,
    hls::stream<axis_data_64> &stage2_FAMDataIn_36,hls::stream<axis_data_64> &stage2_FAMDataIn_37,hls::stream<axis_data_64> &stage2_FAMDataIn_38,hls::stream<axis_data_64> &stage2_FAMDataIn_39,
    hls::stream<axis_data_64> &stage2_FAMDataIn_40,hls::stream<axis_data_64> &stage2_FAMDataIn_41,hls::stream<axis_data_64> &stage2_FAMDataIn_42,hls::stream<axis_data_64> &stage2_FAMDataIn_43,
    hls::stream<axis_data_64> &stage2_FAMDataIn_44,hls::stream<axis_data_64> &stage2_FAMDataIn_45,hls::stream<axis_data_64> &stage2_FAMDataIn_46,hls::stream<axis_data_64> &stage2_FAMDataIn_47,
    hls::stream<axis_data_64> &stage2_FAMDataIn_48,hls::stream<axis_data_64> &stage2_FAMDataIn_49,hls::stream<axis_data_64> &stage2_FAMDataIn_50,hls::stream<axis_data_64> &stage2_FAMDataIn_51,
    hls::stream<axis_data_64> &stage2_FAMDataIn_52,hls::stream<axis_data_64> &stage2_FAMDataIn_53,hls::stream<axis_data_64> &stage2_FAMDataIn_54,hls::stream<axis_data_64> &stage2_FAMDataIn_55,
    hls::stream<axis_data_64> &stage2_FAMDataIn_56,hls::stream<axis_data_64> &stage2_FAMDataIn_57,hls::stream<axis_data_64> &stage2_FAMDataIn_58,hls::stream<axis_data_64> &stage2_FAMDataIn_59,
    hls::stream<axis_data_64> &stage2_FAMDataIn_60,hls::stream<axis_data_64> &stage2_FAMDataIn_61,hls::stream<axis_data_64> &stage2_FAMDataIn_62,hls::stream<axis_data_64> &stage2_FAMDataIn_63,
    hls::stream<axis_data_64> &stage2_FAMDataIn_64,hls::stream<axis_data_64> &stage2_FAMDataIn_65,hls::stream<axis_data_64> &stage2_FAMDataIn_66,hls::stream<axis_data_64> &stage2_FAMDataIn_67,
    hls::stream<axis_data_64> &stage2_FAMDataIn_68,hls::stream<axis_data_64> &stage2_FAMDataIn_69,hls::stream<axis_data_64> &stage2_FAMDataIn_70,hls::stream<axis_data_64> &stage2_FAMDataIn_71,
    hls::stream<axis_data_64> &stage2_FAMDataIn_72,hls::stream<axis_data_64> &stage2_FAMDataIn_73,hls::stream<axis_data_64> &stage2_FAMDataIn_74,hls::stream<axis_data_64> &stage2_FAMDataIn_75,
    hls::stream<axis_data_64> &stage2_FAMDataIn_76,hls::stream<axis_data_64> &stage2_FAMDataIn_77,hls::stream<axis_data_64> &stage2_FAMDataIn_78,hls::stream<axis_data_64> &stage2_FAMDataIn_79,
    hls::stream<axis_data_64> &stage2_FAMDataIn_80,hls::stream<axis_data_64> &stage2_FAMDataIn_81,hls::stream<axis_data_64> &stage2_FAMDataIn_82,hls::stream<axis_data_64> &stage2_FAMDataIn_83,
    hls::stream<axis_data_64> &stage2_FAMDataIn_84,hls::stream<axis_data_64> &stage2_FAMDataIn_85,hls::stream<axis_data_64> &stage2_FAMDataIn_86,hls::stream<axis_data_64> &stage2_FAMDataIn_87,
    hls::stream<axis_data_64> &stage2_FAMDataIn_88,hls::stream<axis_data_64> &stage2_FAMDataIn_89,hls::stream<axis_data_64> &stage2_FAMDataIn_90,hls::stream<axis_data_64> &stage2_FAMDataIn_91,
    hls::stream<axis_data_64> &stage2_FAMDataIn_92,hls::stream<axis_data_64> &stage2_FAMDataIn_93,hls::stream<axis_data_64> &stage2_FAMDataIn_94,hls::stream<axis_data_64> &stage2_FAMDataIn_95,
    hls::stream<axis_data_64> &stage2_FAMDataIn_96,hls::stream<axis_data_64> &stage2_FAMDataIn_97,hls::stream<axis_data_64> &stage2_FAMDataIn_98,hls::stream<axis_data_64> &stage2_FAMDataIn_99,
    hls::stream<axis_data_64> &stage2_FAMDataIn_100,hls::stream<axis_data_64> &stage2_FAMDataIn_101,hls::stream<axis_data_64> &stage2_FAMDataIn_102,hls::stream<axis_data_64> &stage2_FAMDataIn_103,
    hls::stream<axis_data_64> &stage2_FAMDataIn_104,hls::stream<axis_data_64> &stage2_FAMDataIn_105,hls::stream<axis_data_64> &stage2_FAMDataIn_106,hls::stream<axis_data_64> &stage2_FAMDataIn_107,
    hls::stream<axis_data_64> &stage2_FAMDataIn_108,hls::stream<axis_data_64> &stage2_FAMDataIn_109,hls::stream<axis_data_64> &stage2_FAMDataIn_110,hls::stream<axis_data_64> &stage2_FAMDataIn_111,
    hls::stream<axis_data_64> &stage2_FAMDataIn_112,hls::stream<axis_data_64> &stage2_FAMDataIn_113,hls::stream<axis_data_64> &stage2_FAMDataIn_114,hls::stream<axis_data_64> &stage2_FAMDataIn_115,
    hls::stream<axis_data_64> &stage2_FAMDataIn_116,hls::stream<axis_data_64> &stage2_FAMDataIn_117,hls::stream<axis_data_64> &stage2_FAMDataIn_118,hls::stream<axis_data_64> &stage2_FAMDataIn_119,
    hls::stream<axis_data_64> &stage2_FAMDataIn_120,hls::stream<axis_data_64> &stage2_FAMDataIn_121,hls::stream<axis_data_64> &stage2_FAMDataIn_122,hls::stream<axis_data_64> &stage2_FAMDataIn_123,
    hls::stream<axis_data_64> &stage2_FAMDataIn_124,hls::stream<axis_data_64> &stage2_FAMDataIn_125,hls::stream<axis_data_64> &stage2_FAMDataIn_126,hls::stream<axis_data_64> &stage2_FAMDataIn_127,
    
    
    
    hls::stream<axis_data_64> &stage2_FAMOut0,hls::stream<axis_data_64> &stage2_FAMOut1,hls::stream<axis_data_64> &stage2_FAMOut2,hls::stream<axis_data_64> &stage2_FAMOut3,
    hls::stream<axis_data_64> &stage2_FAMOut4,hls::stream<axis_data_64> &stage2_FAMOut5,hls::stream<axis_data_64> &stage2_FAMOut6,hls::stream<axis_data_64> &stage2_FAMOut7,
    hls::stream<axis_data_64> &stage2_FAMOut8,hls::stream<axis_data_64> &stage2_FAMOut9,hls::stream<axis_data_64> &stage2_FAMOut10,hls::stream<axis_data_64> &stage2_FAMOut11,
    hls::stream<axis_data_64> &stage2_FAMOut12,hls::stream<axis_data_64> &stage2_FAMOut13,hls::stream<axis_data_64> &stage2_FAMOut14,hls::stream<axis_data_64> &stage2_FAMOut15,
    hls::stream<axis_data_64> &stage2_FAMOut16,hls::stream<axis_data_64> &stage2_FAMOut17,hls::stream<axis_data_64> &stage2_FAMOut18,hls::stream<axis_data_64> &stage2_FAMOut19,
    hls::stream<axis_data_64> &stage2_FAMOut20,hls::stream<axis_data_64> &stage2_FAMOut21,hls::stream<axis_data_64> &stage2_FAMOut22,hls::stream<axis_data_64> &stage2_FAMOut23,
    hls::stream<axis_data_64> &stage2_FAMOut24,hls::stream<axis_data_64> &stage2_FAMOut25,hls::stream<axis_data_64> &stage2_FAMOut26,hls::stream<axis_data_64> &stage2_FAMOut27,
    hls::stream<axis_data_64> &stage2_FAMOut28,hls::stream<axis_data_64> &stage2_FAMOut29,hls::stream<axis_data_64> &stage2_FAMOut30,hls::stream<axis_data_64> &stage2_FAMOut31,
    hls::stream<axis_data_64> &stage2_FAMOut32,hls::stream<axis_data_64> &stage2_FAMOut33,hls::stream<axis_data_64> &stage2_FAMOut34,hls::stream<axis_data_64> &stage2_FAMOut35,
    hls::stream<axis_data_64> &stage2_FAMOut36,hls::stream<axis_data_64> &stage2_FAMOut37,hls::stream<axis_data_64> &stage2_FAMOut38,hls::stream<axis_data_64> &stage2_FAMOut39,
    hls::stream<axis_data_64> &stage2_FAMOut40,hls::stream<axis_data_64> &stage2_FAMOut41,hls::stream<axis_data_64> &stage2_FAMOut42,hls::stream<axis_data_64> &stage2_FAMOut43,
    hls::stream<axis_data_64> &stage2_FAMOut44,hls::stream<axis_data_64> &stage2_FAMOut45,hls::stream<axis_data_64> &stage2_FAMOut46,hls::stream<axis_data_64> &stage2_FAMOut47,
    hls::stream<axis_data_64> &stage2_FAMOut48,hls::stream<axis_data_64> &stage2_FAMOut49,hls::stream<axis_data_64> &stage2_FAMOut50,hls::stream<axis_data_64> &stage2_FAMOut51,
    hls::stream<axis_data_64> &stage2_FAMOut52,hls::stream<axis_data_64> &stage2_FAMOut53,hls::stream<axis_data_64> &stage2_FAMOut54,hls::stream<axis_data_64> &stage2_FAMOut55,
    hls::stream<axis_data_64> &stage2_FAMOut56,hls::stream<axis_data_64> &stage2_FAMOut57,hls::stream<axis_data_64> &stage2_FAMOut58,hls::stream<axis_data_64> &stage2_FAMOut59,
    hls::stream<axis_data_64> &stage2_FAMOut60,hls::stream<axis_data_64> &stage2_FAMOut61,hls::stream<axis_data_64> &stage2_FAMOut62,hls::stream<axis_data_64> &stage2_FAMOut63,
    hls::stream<axis_data_64> &stage2_FAMOut64,hls::stream<axis_data_64> &stage2_FAMOut65,hls::stream<axis_data_64> &stage2_FAMOut66,hls::stream<axis_data_64> &stage2_FAMOut67,
    hls::stream<axis_data_64> &stage2_FAMOut68,hls::stream<axis_data_64> &stage2_FAMOut69,hls::stream<axis_data_64> &stage2_FAMOut70,hls::stream<axis_data_64> &stage2_FAMOut71,
    hls::stream<axis_data_64> &stage2_FAMOut72,hls::stream<axis_data_64> &stage2_FAMOut73,hls::stream<axis_data_64> &stage2_FAMOut74,hls::stream<axis_data_64> &stage2_FAMOut75,
    hls::stream<axis_data_64> &stage2_FAMOut76,hls::stream<axis_data_64> &stage2_FAMOut77,hls::stream<axis_data_64> &stage2_FAMOut78,hls::stream<axis_data_64> &stage2_FAMOut79,
    hls::stream<axis_data_64> &stage2_FAMOut80,hls::stream<axis_data_64> &stage2_FAMOut81,hls::stream<axis_data_64> &stage2_FAMOut82,hls::stream<axis_data_64> &stage2_FAMOut83,
    hls::stream<axis_data_64> &stage2_FAMOut84,hls::stream<axis_data_64> &stage2_FAMOut85,hls::stream<axis_data_64> &stage2_FAMOut86,hls::stream<axis_data_64> &stage2_FAMOut87,
    hls::stream<axis_data_64> &stage2_FAMOut88,hls::stream<axis_data_64> &stage2_FAMOut89,hls::stream<axis_data_64> &stage2_FAMOut90,hls::stream<axis_data_64> &stage2_FAMOut91,
    hls::stream<axis_data_64> &stage2_FAMOut92,hls::stream<axis_data_64> &stage2_FAMOut93,hls::stream<axis_data_64> &stage2_FAMOut94,hls::stream<axis_data_64> &stage2_FAMOut95,
    hls::stream<axis_data_64> &stage2_FAMOut96,hls::stream<axis_data_64> &stage2_FAMOut97,hls::stream<axis_data_64> &stage2_FAMOut98,hls::stream<axis_data_64> &stage2_FAMOut99,
    hls::stream<axis_data_64> &stage2_FAMOut100,hls::stream<axis_data_64> &stage2_FAMOut101,hls::stream<axis_data_64> &stage2_FAMOut102,hls::stream<axis_data_64> &stage2_FAMOut103,
    hls::stream<axis_data_64> &stage2_FAMOut104,hls::stream<axis_data_64> &stage2_FAMOut105,hls::stream<axis_data_64> &stage2_FAMOut106,hls::stream<axis_data_64> &stage2_FAMOut107,
    hls::stream<axis_data_64> &stage2_FAMOut108,hls::stream<axis_data_64> &stage2_FAMOut109,hls::stream<axis_data_64> &stage2_FAMOut110,hls::stream<axis_data_64> &stage2_FAMOut111,
    hls::stream<axis_data_64> &stage2_FAMOut112,hls::stream<axis_data_64> &stage2_FAMOut113,hls::stream<axis_data_64> &stage2_FAMOut114,hls::stream<axis_data_64> &stage2_FAMOut115,
    hls::stream<axis_data_64> &stage2_FAMOut116,hls::stream<axis_data_64> &stage2_FAMOut117,hls::stream<axis_data_64> &stage2_FAMOut118,hls::stream<axis_data_64> &stage2_FAMOut119,
    hls::stream<axis_data_64> &stage2_FAMOut120,hls::stream<axis_data_64> &stage2_FAMOut121,hls::stream<axis_data_64> &stage2_FAMOut122,hls::stream<axis_data_64> &stage2_FAMOut123,
    hls::stream<axis_data_64> &stage2_FAMOut124,hls::stream<axis_data_64> &stage2_FAMOut125,hls::stream<axis_data_64> &stage2_FAMOut126,hls::stream<axis_data_64> &stage2_FAMOut127,
    
    ap_uint<512>* memout
    
) {
#pragma HLS DATAFLOW
    // Note: Functions under dataflow will be executed concurrently, as long as they are connected by a stream

    plTranspose_to_aie2(
        memtrans,
        stage2_FAMDataIn_0, stage2_FAMDataIn_1,stage2_FAMDataIn_2, stage2_FAMDataIn_3,stage2_FAMDataIn_4, stage2_FAMDataIn_5,stage2_FAMDataIn_6, stage2_FAMDataIn_7,
        stage2_FAMDataIn_8, stage2_FAMDataIn_9,stage2_FAMDataIn_10, stage2_FAMDataIn_11,stage2_FAMDataIn_12, stage2_FAMDataIn_13,stage2_FAMDataIn_14, stage2_FAMDataIn_15,
        stage2_FAMDataIn_16, stage2_FAMDataIn_17,stage2_FAMDataIn_18, stage2_FAMDataIn_19,stage2_FAMDataIn_20, stage2_FAMDataIn_21,stage2_FAMDataIn_22, stage2_FAMDataIn_23,
        stage2_FAMDataIn_24, stage2_FAMDataIn_25,stage2_FAMDataIn_26, stage2_FAMDataIn_27,stage2_FAMDataIn_28, stage2_FAMDataIn_29,stage2_FAMDataIn_30, stage2_FAMDataIn_31,
        stage2_FAMDataIn_32, stage2_FAMDataIn_33,stage2_FAMDataIn_34, stage2_FAMDataIn_35,stage2_FAMDataIn_36, stage2_FAMDataIn_37,stage2_FAMDataIn_38, stage2_FAMDataIn_39,
        stage2_FAMDataIn_40, stage2_FAMDataIn_41,stage2_FAMDataIn_42, stage2_FAMDataIn_43,stage2_FAMDataIn_44, stage2_FAMDataIn_45,stage2_FAMDataIn_46, stage2_FAMDataIn_47,
        stage2_FAMDataIn_48, stage2_FAMDataIn_49,stage2_FAMDataIn_50, stage2_FAMDataIn_51,stage2_FAMDataIn_52, stage2_FAMDataIn_53,stage2_FAMDataIn_54, stage2_FAMDataIn_55,
        stage2_FAMDataIn_56, stage2_FAMDataIn_57,stage2_FAMDataIn_58, stage2_FAMDataIn_59,stage2_FAMDataIn_60, stage2_FAMDataIn_61,stage2_FAMDataIn_62, stage2_FAMDataIn_63,
        stage2_FAMDataIn_64, stage2_FAMDataIn_65,stage2_FAMDataIn_66, stage2_FAMDataIn_67,stage2_FAMDataIn_68, stage2_FAMDataIn_69,stage2_FAMDataIn_70, stage2_FAMDataIn_71,
        stage2_FAMDataIn_72, stage2_FAMDataIn_73,stage2_FAMDataIn_74, stage2_FAMDataIn_75,stage2_FAMDataIn_76, stage2_FAMDataIn_77,stage2_FAMDataIn_78, stage2_FAMDataIn_79,
        stage2_FAMDataIn_80, stage2_FAMDataIn_81,stage2_FAMDataIn_82, stage2_FAMDataIn_83,stage2_FAMDataIn_84, stage2_FAMDataIn_85,stage2_FAMDataIn_86, stage2_FAMDataIn_87,
        stage2_FAMDataIn_88, stage2_FAMDataIn_89,stage2_FAMDataIn_90, stage2_FAMDataIn_91,stage2_FAMDataIn_92, stage2_FAMDataIn_93,stage2_FAMDataIn_94, stage2_FAMDataIn_95,
        stage2_FAMDataIn_96, stage2_FAMDataIn_97,stage2_FAMDataIn_98, stage2_FAMDataIn_99,stage2_FAMDataIn_100, stage2_FAMDataIn_101,stage2_FAMDataIn_102, stage2_FAMDataIn_103,
        stage2_FAMDataIn_104, stage2_FAMDataIn_105,stage2_FAMDataIn_106, stage2_FAMDataIn_107,stage2_FAMDataIn_108, stage2_FAMDataIn_109,stage2_FAMDataIn_110, stage2_FAMDataIn_111,
        stage2_FAMDataIn_112, stage2_FAMDataIn_113,stage2_FAMDataIn_114, stage2_FAMDataIn_115,stage2_FAMDataIn_116, stage2_FAMDataIn_117,stage2_FAMDataIn_118, stage2_FAMDataIn_119,
        stage2_FAMDataIn_120, stage2_FAMDataIn_121,stage2_FAMDataIn_122, stage2_FAMDataIn_123,stage2_FAMDataIn_124, stage2_FAMDataIn_125,stage2_FAMDataIn_126, stage2_FAMDataIn_127
        
    );

    aie2_to_ddr(
        stage2_FAMOut0, stage2_FAMOut1,stage2_FAMOut2, stage2_FAMOut3,stage2_FAMOut4, stage2_FAMOut5,stage2_FAMOut6, stage2_FAMOut7,
        stage2_FAMOut8, stage2_FAMOut9,stage2_FAMOut10, stage2_FAMOut11,stage2_FAMOut12, stage2_FAMOut13,stage2_FAMOut14, stage2_FAMOut15,
        stage2_FAMOut16, stage2_FAMOut17,stage2_FAMOut18, stage2_FAMOut19,stage2_FAMOut20, stage2_FAMOut21,stage2_FAMOut22, stage2_FAMOut23,
        stage2_FAMOut24, stage2_FAMOut25,stage2_FAMOut26, stage2_FAMOut27,stage2_FAMOut28, stage2_FAMOut29,stage2_FAMOut30, stage2_FAMOut31,
        stage2_FAMOut32, stage2_FAMOut33,stage2_FAMOut34, stage2_FAMOut35,stage2_FAMOut36, stage2_FAMOut37,stage2_FAMOut38, stage2_FAMOut39,
        stage2_FAMOut40, stage2_FAMOut41,stage2_FAMOut42, stage2_FAMOut43,stage2_FAMOut44, stage2_FAMOut45,stage2_FAMOut46, stage2_FAMOut47,
        stage2_FAMOut48, stage2_FAMOut49,stage2_FAMOut50, stage2_FAMOut51,stage2_FAMOut52, stage2_FAMOut53,stage2_FAMOut54, stage2_FAMOut55,
        stage2_FAMOut56, stage2_FAMOut57,stage2_FAMOut58, stage2_FAMOut59,stage2_FAMOut60, stage2_FAMOut61,stage2_FAMOut62, stage2_FAMOut63,
        stage2_FAMOut64, stage2_FAMOut65,stage2_FAMOut66, stage2_FAMOut67,stage2_FAMOut68, stage2_FAMOut69,stage2_FAMOut70, stage2_FAMOut71,
        stage2_FAMOut72, stage2_FAMOut73,stage2_FAMOut74, stage2_FAMOut75,stage2_FAMOut76, stage2_FAMOut77,stage2_FAMOut78, stage2_FAMOut79,
        stage2_FAMOut80, stage2_FAMOut81,stage2_FAMOut82, stage2_FAMOut83,stage2_FAMOut84, stage2_FAMOut85,stage2_FAMOut86, stage2_FAMOut87,
        stage2_FAMOut88, stage2_FAMOut89,stage2_FAMOut90, stage2_FAMOut91,stage2_FAMOut92, stage2_FAMOut93,stage2_FAMOut94, stage2_FAMOut95,
        stage2_FAMOut96, stage2_FAMOut97,stage2_FAMOut98, stage2_FAMOut99,stage2_FAMOut100, stage2_FAMOut101,stage2_FAMOut102, stage2_FAMOut103,
        stage2_FAMOut104, stage2_FAMOut105,stage2_FAMOut106, stage2_FAMOut107,stage2_FAMOut108, stage2_FAMOut109,stage2_FAMOut110, stage2_FAMOut111,
        stage2_FAMOut112, stage2_FAMOut113,stage2_FAMOut114, stage2_FAMOut115,stage2_FAMOut116, stage2_FAMOut117,stage2_FAMOut118, stage2_FAMOut119,
        stage2_FAMOut120, stage2_FAMOut121,stage2_FAMOut122, stage2_FAMOut123,stage2_FAMOut124, stage2_FAMOut125,stage2_FAMOut126, stage2_FAMOut127,

        memout     
    );
}


//-------------------------------------
// Final top-level dma_hls()
//-------------------------------------
extern "C"
void dma_hls(
    // DDR input
    ap_uint<64> *memin0,ap_uint<64> *memin1,ap_uint<64> *memin2,ap_uint<64> *memin3,ap_uint<64> *memin4,ap_uint<64> *memin5,ap_uint<64> *memin6,ap_uint<64> *memin7,
    ap_uint<64> *memin8,ap_uint<64> *memin9,ap_uint<64> *memin10,ap_uint<64> *memin11,ap_uint<64> *memin12,ap_uint<64> *memin13,ap_uint<64> *memin14,ap_uint<64> *memin15,
    
    // Intermediate buffer
    ap_uint<64> *memtrans,
    // Final result buffer
    ap_uint<512> *memout,

    // AIE stage1 input/output port
    hls::stream<axis_data_64> &stage1_FAMDataIn_0,hls::stream<axis_data_64> &stage1_FAMDataIn_1,hls::stream<axis_data_64> &stage1_FAMDataIn_2,hls::stream<axis_data_64> &stage1_FAMDataIn_3,
    hls::stream<axis_data_64> &stage1_FAMDataIn_4,hls::stream<axis_data_64> &stage1_FAMDataIn_5,hls::stream<axis_data_64> &stage1_FAMDataIn_6,hls::stream<axis_data_64> &stage1_FAMDataIn_7,
    hls::stream<axis_data_64> &stage1_FAMDataIn_8,hls::stream<axis_data_64> &stage1_FAMDataIn_9,hls::stream<axis_data_64> &stage1_FAMDataIn_10,hls::stream<axis_data_64> &stage1_FAMDataIn_11,
    hls::stream<axis_data_64> &stage1_FAMDataIn_12,hls::stream<axis_data_64> &stage1_FAMDataIn_13,hls::stream<axis_data_64> &stage1_FAMDataIn_14,hls::stream<axis_data_64> &stage1_FAMDataIn_15,


    hls::stream<axis_data_64> &stage1_FAMOut0,hls::stream<axis_data_64> &stage1_FAMOut1,hls::stream<axis_data_64> &stage1_FAMOut2,hls::stream<axis_data_64> &stage1_FAMOut3,
    hls::stream<axis_data_64> &stage1_FAMOut4,hls::stream<axis_data_64> &stage1_FAMOut5,hls::stream<axis_data_64> &stage1_FAMOut6,hls::stream<axis_data_64> &stage1_FAMOut7,
    hls::stream<axis_data_64> &stage1_FAMOut8,hls::stream<axis_data_64> &stage1_FAMOut9,hls::stream<axis_data_64> &stage1_FAMOut10,hls::stream<axis_data_64> &stage1_FAMOut11,
    hls::stream<axis_data_64> &stage1_FAMOut12,hls::stream<axis_data_64> &stage1_FAMOut13,hls::stream<axis_data_64> &stage1_FAMOut14,hls::stream<axis_data_64> &stage1_FAMOut15,

    
    // 128 PL TRANSPONSE to AIE stage2
    hls::stream<axis_data_64> &stage2_FAMDataIn_0,hls::stream<axis_data_64> &stage2_FAMDataIn_1,hls::stream<axis_data_64> &stage2_FAMDataIn_2,hls::stream<axis_data_64> &stage2_FAMDataIn_3,
    hls::stream<axis_data_64> &stage2_FAMDataIn_4,hls::stream<axis_data_64> &stage2_FAMDataIn_5,hls::stream<axis_data_64> &stage2_FAMDataIn_6,hls::stream<axis_data_64> &stage2_FAMDataIn_7,
    hls::stream<axis_data_64> &stage2_FAMDataIn_8,hls::stream<axis_data_64> &stage2_FAMDataIn_9,hls::stream<axis_data_64> &stage2_FAMDataIn_10,hls::stream<axis_data_64> &stage2_FAMDataIn_11,
    hls::stream<axis_data_64> &stage2_FAMDataIn_12,hls::stream<axis_data_64> &stage2_FAMDataIn_13,hls::stream<axis_data_64> &stage2_FAMDataIn_14,hls::stream<axis_data_64> &stage2_FAMDataIn_15,
    hls::stream<axis_data_64> &stage2_FAMDataIn_16,hls::stream<axis_data_64> &stage2_FAMDataIn_17,hls::stream<axis_data_64> &stage2_FAMDataIn_18,hls::stream<axis_data_64> &stage2_FAMDataIn_19,
    hls::stream<axis_data_64> &stage2_FAMDataIn_20,hls::stream<axis_data_64> &stage2_FAMDataIn_21,hls::stream<axis_data_64> &stage2_FAMDataIn_22,hls::stream<axis_data_64> &stage2_FAMDataIn_23,
    hls::stream<axis_data_64> &stage2_FAMDataIn_24,hls::stream<axis_data_64> &stage2_FAMDataIn_25,hls::stream<axis_data_64> &stage2_FAMDataIn_26,hls::stream<axis_data_64> &stage2_FAMDataIn_27,
    hls::stream<axis_data_64> &stage2_FAMDataIn_28,hls::stream<axis_data_64> &stage2_FAMDataIn_29,hls::stream<axis_data_64> &stage2_FAMDataIn_30,hls::stream<axis_data_64> &stage2_FAMDataIn_31,
    hls::stream<axis_data_64> &stage2_FAMDataIn_32,hls::stream<axis_data_64> &stage2_FAMDataIn_33,hls::stream<axis_data_64> &stage2_FAMDataIn_34,hls::stream<axis_data_64> &stage2_FAMDataIn_35,
    hls::stream<axis_data_64> &stage2_FAMDataIn_36,hls::stream<axis_data_64> &stage2_FAMDataIn_37,hls::stream<axis_data_64> &stage2_FAMDataIn_38,hls::stream<axis_data_64> &stage2_FAMDataIn_39,
    hls::stream<axis_data_64> &stage2_FAMDataIn_40,hls::stream<axis_data_64> &stage2_FAMDataIn_41,hls::stream<axis_data_64> &stage2_FAMDataIn_42,hls::stream<axis_data_64> &stage2_FAMDataIn_43,
    hls::stream<axis_data_64> &stage2_FAMDataIn_44,hls::stream<axis_data_64> &stage2_FAMDataIn_45,hls::stream<axis_data_64> &stage2_FAMDataIn_46,hls::stream<axis_data_64> &stage2_FAMDataIn_47,
    hls::stream<axis_data_64> &stage2_FAMDataIn_48,hls::stream<axis_data_64> &stage2_FAMDataIn_49,hls::stream<axis_data_64> &stage2_FAMDataIn_50,hls::stream<axis_data_64> &stage2_FAMDataIn_51,
    hls::stream<axis_data_64> &stage2_FAMDataIn_52,hls::stream<axis_data_64> &stage2_FAMDataIn_53,hls::stream<axis_data_64> &stage2_FAMDataIn_54,hls::stream<axis_data_64> &stage2_FAMDataIn_55,
    hls::stream<axis_data_64> &stage2_FAMDataIn_56,hls::stream<axis_data_64> &stage2_FAMDataIn_57,hls::stream<axis_data_64> &stage2_FAMDataIn_58,hls::stream<axis_data_64> &stage2_FAMDataIn_59,
    hls::stream<axis_data_64> &stage2_FAMDataIn_60,hls::stream<axis_data_64> &stage2_FAMDataIn_61,hls::stream<axis_data_64> &stage2_FAMDataIn_62,hls::stream<axis_data_64> &stage2_FAMDataIn_63,
    hls::stream<axis_data_64> &stage2_FAMDataIn_64,hls::stream<axis_data_64> &stage2_FAMDataIn_65,hls::stream<axis_data_64> &stage2_FAMDataIn_66,hls::stream<axis_data_64> &stage2_FAMDataIn_67,
    hls::stream<axis_data_64> &stage2_FAMDataIn_68,hls::stream<axis_data_64> &stage2_FAMDataIn_69,hls::stream<axis_data_64> &stage2_FAMDataIn_70,hls::stream<axis_data_64> &stage2_FAMDataIn_71,
    hls::stream<axis_data_64> &stage2_FAMDataIn_72,hls::stream<axis_data_64> &stage2_FAMDataIn_73,hls::stream<axis_data_64> &stage2_FAMDataIn_74,hls::stream<axis_data_64> &stage2_FAMDataIn_75,
    hls::stream<axis_data_64> &stage2_FAMDataIn_76,hls::stream<axis_data_64> &stage2_FAMDataIn_77,hls::stream<axis_data_64> &stage2_FAMDataIn_78,hls::stream<axis_data_64> &stage2_FAMDataIn_79,
    hls::stream<axis_data_64> &stage2_FAMDataIn_80,hls::stream<axis_data_64> &stage2_FAMDataIn_81,hls::stream<axis_data_64> &stage2_FAMDataIn_82,hls::stream<axis_data_64> &stage2_FAMDataIn_83,
    hls::stream<axis_data_64> &stage2_FAMDataIn_84,hls::stream<axis_data_64> &stage2_FAMDataIn_85,hls::stream<axis_data_64> &stage2_FAMDataIn_86,hls::stream<axis_data_64> &stage2_FAMDataIn_87,
    hls::stream<axis_data_64> &stage2_FAMDataIn_88,hls::stream<axis_data_64> &stage2_FAMDataIn_89,hls::stream<axis_data_64> &stage2_FAMDataIn_90,hls::stream<axis_data_64> &stage2_FAMDataIn_91,
    hls::stream<axis_data_64> &stage2_FAMDataIn_92,hls::stream<axis_data_64> &stage2_FAMDataIn_93,hls::stream<axis_data_64> &stage2_FAMDataIn_94,hls::stream<axis_data_64> &stage2_FAMDataIn_95,
    hls::stream<axis_data_64> &stage2_FAMDataIn_96,hls::stream<axis_data_64> &stage2_FAMDataIn_97,hls::stream<axis_data_64> &stage2_FAMDataIn_98,hls::stream<axis_data_64> &stage2_FAMDataIn_99,
    hls::stream<axis_data_64> &stage2_FAMDataIn_100,hls::stream<axis_data_64> &stage2_FAMDataIn_101,hls::stream<axis_data_64> &stage2_FAMDataIn_102,hls::stream<axis_data_64> &stage2_FAMDataIn_103,
    hls::stream<axis_data_64> &stage2_FAMDataIn_104,hls::stream<axis_data_64> &stage2_FAMDataIn_105,hls::stream<axis_data_64> &stage2_FAMDataIn_106,hls::stream<axis_data_64> &stage2_FAMDataIn_107,
    hls::stream<axis_data_64> &stage2_FAMDataIn_108,hls::stream<axis_data_64> &stage2_FAMDataIn_109,hls::stream<axis_data_64> &stage2_FAMDataIn_110,hls::stream<axis_data_64> &stage2_FAMDataIn_111,
    hls::stream<axis_data_64> &stage2_FAMDataIn_112,hls::stream<axis_data_64> &stage2_FAMDataIn_113,hls::stream<axis_data_64> &stage2_FAMDataIn_114,hls::stream<axis_data_64> &stage2_FAMDataIn_115,
    hls::stream<axis_data_64> &stage2_FAMDataIn_116,hls::stream<axis_data_64> &stage2_FAMDataIn_117,hls::stream<axis_data_64> &stage2_FAMDataIn_118,hls::stream<axis_data_64> &stage2_FAMDataIn_119,
    hls::stream<axis_data_64> &stage2_FAMDataIn_120,hls::stream<axis_data_64> &stage2_FAMDataIn_121,hls::stream<axis_data_64> &stage2_FAMDataIn_122,hls::stream<axis_data_64> &stage2_FAMDataIn_123,
    hls::stream<axis_data_64> &stage2_FAMDataIn_124,hls::stream<axis_data_64> &stage2_FAMDataIn_125,hls::stream<axis_data_64> &stage2_FAMDataIn_126,hls::stream<axis_data_64> &stage2_FAMDataIn_127,

    // 128 AIE stage2 to DDR
    hls::stream<axis_data_64> &stage2_FAMOut0,hls::stream<axis_data_64> &stage2_FAMOut1,hls::stream<axis_data_64> &stage2_FAMOut2,hls::stream<axis_data_64> &stage2_FAMOut3,
    hls::stream<axis_data_64> &stage2_FAMOut4,hls::stream<axis_data_64> &stage2_FAMOut5,hls::stream<axis_data_64> &stage2_FAMOut6,hls::stream<axis_data_64> &stage2_FAMOut7,
    hls::stream<axis_data_64> &stage2_FAMOut8,hls::stream<axis_data_64> &stage2_FAMOut9,hls::stream<axis_data_64> &stage2_FAMOut10,hls::stream<axis_data_64> &stage2_FAMOut11,
    hls::stream<axis_data_64> &stage2_FAMOut12,hls::stream<axis_data_64> &stage2_FAMOut13,hls::stream<axis_data_64> &stage2_FAMOut14,hls::stream<axis_data_64> &stage2_FAMOut15,
    hls::stream<axis_data_64> &stage2_FAMOut16,hls::stream<axis_data_64> &stage2_FAMOut17,hls::stream<axis_data_64> &stage2_FAMOut18,hls::stream<axis_data_64> &stage2_FAMOut19,
    hls::stream<axis_data_64> &stage2_FAMOut20,hls::stream<axis_data_64> &stage2_FAMOut21,hls::stream<axis_data_64> &stage2_FAMOut22,hls::stream<axis_data_64> &stage2_FAMOut23,
    hls::stream<axis_data_64> &stage2_FAMOut24,hls::stream<axis_data_64> &stage2_FAMOut25,hls::stream<axis_data_64> &stage2_FAMOut26,hls::stream<axis_data_64> &stage2_FAMOut27,
    hls::stream<axis_data_64> &stage2_FAMOut28,hls::stream<axis_data_64> &stage2_FAMOut29,hls::stream<axis_data_64> &stage2_FAMOut30,hls::stream<axis_data_64> &stage2_FAMOut31,
    hls::stream<axis_data_64> &stage2_FAMOut32,hls::stream<axis_data_64> &stage2_FAMOut33,hls::stream<axis_data_64> &stage2_FAMOut34,hls::stream<axis_data_64> &stage2_FAMOut35,
    hls::stream<axis_data_64> &stage2_FAMOut36,hls::stream<axis_data_64> &stage2_FAMOut37,hls::stream<axis_data_64> &stage2_FAMOut38,hls::stream<axis_data_64> &stage2_FAMOut39,
    hls::stream<axis_data_64> &stage2_FAMOut40,hls::stream<axis_data_64> &stage2_FAMOut41,hls::stream<axis_data_64> &stage2_FAMOut42,hls::stream<axis_data_64> &stage2_FAMOut43,
    hls::stream<axis_data_64> &stage2_FAMOut44,hls::stream<axis_data_64> &stage2_FAMOut45,hls::stream<axis_data_64> &stage2_FAMOut46,hls::stream<axis_data_64> &stage2_FAMOut47,
    hls::stream<axis_data_64> &stage2_FAMOut48,hls::stream<axis_data_64> &stage2_FAMOut49,hls::stream<axis_data_64> &stage2_FAMOut50,hls::stream<axis_data_64> &stage2_FAMOut51,
    hls::stream<axis_data_64> &stage2_FAMOut52,hls::stream<axis_data_64> &stage2_FAMOut53,hls::stream<axis_data_64> &stage2_FAMOut54,hls::stream<axis_data_64> &stage2_FAMOut55,
    hls::stream<axis_data_64> &stage2_FAMOut56,hls::stream<axis_data_64> &stage2_FAMOut57,hls::stream<axis_data_64> &stage2_FAMOut58,hls::stream<axis_data_64> &stage2_FAMOut59,
    hls::stream<axis_data_64> &stage2_FAMOut60,hls::stream<axis_data_64> &stage2_FAMOut61,hls::stream<axis_data_64> &stage2_FAMOut62,hls::stream<axis_data_64> &stage2_FAMOut63,
    hls::stream<axis_data_64> &stage2_FAMOut64,hls::stream<axis_data_64> &stage2_FAMOut65,hls::stream<axis_data_64> &stage2_FAMOut66,hls::stream<axis_data_64> &stage2_FAMOut67,
    hls::stream<axis_data_64> &stage2_FAMOut68,hls::stream<axis_data_64> &stage2_FAMOut69,hls::stream<axis_data_64> &stage2_FAMOut70,hls::stream<axis_data_64> &stage2_FAMOut71,
    hls::stream<axis_data_64> &stage2_FAMOut72,hls::stream<axis_data_64> &stage2_FAMOut73,hls::stream<axis_data_64> &stage2_FAMOut74,hls::stream<axis_data_64> &stage2_FAMOut75,
    hls::stream<axis_data_64> &stage2_FAMOut76,hls::stream<axis_data_64> &stage2_FAMOut77,hls::stream<axis_data_64> &stage2_FAMOut78,hls::stream<axis_data_64> &stage2_FAMOut79,
    hls::stream<axis_data_64> &stage2_FAMOut80,hls::stream<axis_data_64> &stage2_FAMOut81,hls::stream<axis_data_64> &stage2_FAMOut82,hls::stream<axis_data_64> &stage2_FAMOut83,
    hls::stream<axis_data_64> &stage2_FAMOut84,hls::stream<axis_data_64> &stage2_FAMOut85,hls::stream<axis_data_64> &stage2_FAMOut86,hls::stream<axis_data_64> &stage2_FAMOut87,
    hls::stream<axis_data_64> &stage2_FAMOut88,hls::stream<axis_data_64> &stage2_FAMOut89,hls::stream<axis_data_64> &stage2_FAMOut90,hls::stream<axis_data_64> &stage2_FAMOut91,
    hls::stream<axis_data_64> &stage2_FAMOut92,hls::stream<axis_data_64> &stage2_FAMOut93,hls::stream<axis_data_64> &stage2_FAMOut94,hls::stream<axis_data_64> &stage2_FAMOut95,
    hls::stream<axis_data_64> &stage2_FAMOut96,hls::stream<axis_data_64> &stage2_FAMOut97,hls::stream<axis_data_64> &stage2_FAMOut98,hls::stream<axis_data_64> &stage2_FAMOut99,
    hls::stream<axis_data_64> &stage2_FAMOut100,hls::stream<axis_data_64> &stage2_FAMOut101,hls::stream<axis_data_64> &stage2_FAMOut102,hls::stream<axis_data_64> &stage2_FAMOut103,
    hls::stream<axis_data_64> &stage2_FAMOut104,hls::stream<axis_data_64> &stage2_FAMOut105,hls::stream<axis_data_64> &stage2_FAMOut106,hls::stream<axis_data_64> &stage2_FAMOut107,
    hls::stream<axis_data_64> &stage2_FAMOut108,hls::stream<axis_data_64> &stage2_FAMOut109,hls::stream<axis_data_64> &stage2_FAMOut110,hls::stream<axis_data_64> &stage2_FAMOut111,
    hls::stream<axis_data_64> &stage2_FAMOut112,hls::stream<axis_data_64> &stage2_FAMOut113,hls::stream<axis_data_64> &stage2_FAMOut114,hls::stream<axis_data_64> &stage2_FAMOut115,
    hls::stream<axis_data_64> &stage2_FAMOut116,hls::stream<axis_data_64> &stage2_FAMOut117,hls::stream<axis_data_64> &stage2_FAMOut118,hls::stream<axis_data_64> &stage2_FAMOut119,
    hls::stream<axis_data_64> &stage2_FAMOut120,hls::stream<axis_data_64> &stage2_FAMOut121,hls::stream<axis_data_64> &stage2_FAMOut122,hls::stream<axis_data_64> &stage2_FAMOut123,
    hls::stream<axis_data_64> &stage2_FAMOut124,hls::stream<axis_data_64> &stage2_FAMOut125,hls::stream<axis_data_64> &stage2_FAMOut126,hls::stream<axis_data_64> &stage2_FAMOut127,

    
    int iter
    
)
{
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS INTERFACE s_axilite bundle=control port=memin0
#pragma HLS INTERFACE s_axilite bundle=control port=memin1
#pragma HLS INTERFACE s_axilite bundle=control port=memin2
#pragma HLS INTERFACE s_axilite bundle=control port=memin3
#pragma HLS INTERFACE s_axilite bundle=control port=memin4
#pragma HLS INTERFACE s_axilite bundle=control port=memin5
#pragma HLS INTERFACE s_axilite bundle=control port=memin6
#pragma HLS INTERFACE s_axilite bundle=control port=memin7
#pragma HLS INTERFACE s_axilite bundle=control port=memin8
#pragma HLS INTERFACE s_axilite bundle=control port=memin9
#pragma HLS INTERFACE s_axilite bundle=control port=memin10
#pragma HLS INTERFACE s_axilite bundle=control port=memin11
#pragma HLS INTERFACE s_axilite bundle=control port=memin12
#pragma HLS INTERFACE s_axilite bundle=control port=memin13
#pragma HLS INTERFACE s_axilite bundle=control port=memin14
#pragma HLS INTERFACE s_axilite bundle=control port=memin15
#pragma HLS INTERFACE s_axilite bundle=control port=memtrans
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
#pragma HLS interface m_axi offset=slave bundle=gmem8 port=memin8 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem9 port=memin9 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem10 port=memin10 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem11 port=memin11 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem12 port=memin12 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem13 port=memin13 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem14 port=memin14 max_read_burst_length=16 num_read_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem15 port=memin15 max_read_burst_length=16 num_read_outstanding=64

#pragma HLS interface m_axi offset=slave bundle=gmem16 port=memtrans \
                           max_read_burst_length=16  max_write_burst_length=16 \
                           num_read_outstanding=64   num_write_outstanding=64
#pragma HLS interface m_axi offset=slave bundle=gmem17 port=memout max_write_burst_length=16 num_write_outstanding=64


//  AXI4-Stream interface: 16 inputs
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_0
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_1
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_2
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_3
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_4
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_5
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_6
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_7
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_8
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_9
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_10
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_11
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_12
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_13
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_14
#pragma HLS INTERFACE axis port=stage1_FAMDataIn_15
// AXI4-Stream interface: 16 outputs
#pragma HLS INTERFACE axis port=stage1_FAMOut0
#pragma HLS INTERFACE axis port=stage1_FAMOut1
#pragma HLS INTERFACE axis port=stage1_FAMOut2
#pragma HLS INTERFACE axis port=stage1_FAMOut3
#pragma HLS INTERFACE axis port=stage1_FAMOut4
#pragma HLS INTERFACE axis port=stage1_FAMOut5
#pragma HLS INTERFACE axis port=stage1_FAMOut6
#pragma HLS INTERFACE axis port=stage1_FAMOut7
#pragma HLS INTERFACE axis port=stage1_FAMOut8
#pragma HLS INTERFACE axis port=stage1_FAMOut9
#pragma HLS INTERFACE axis port=stage1_FAMOut10
#pragma HLS INTERFACE axis port=stage1_FAMOut11
#pragma HLS INTERFACE axis port=stage1_FAMOut12
#pragma HLS INTERFACE axis port=stage1_FAMOut13
#pragma HLS INTERFACE axis port=stage1_FAMOut14
#pragma HLS INTERFACE axis port=stage1_FAMOut15

// AXI4-Stream interface: 128 inputs
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_0
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_1
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_2
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_3
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_4
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_5
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_6
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_7
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_8
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_9
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_10
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_11
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_12
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_13
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_14
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_15
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_16
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_17
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_18
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_19
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_20
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_21
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_22
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_23
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_24
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_25
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_26
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_27
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_28
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_29
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_30
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_31
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_32
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_33
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_34
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_35
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_36
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_37
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_38
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_39
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_40
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_41
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_42
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_43
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_44
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_45
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_46
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_47
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_48
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_49
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_50
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_51
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_52
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_53
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_54
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_55
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_56
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_57
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_58
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_59
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_60
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_61
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_62
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_63
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_64
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_65
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_66
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_67
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_68
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_69
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_70
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_71
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_72
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_73
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_74
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_75
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_76
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_77
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_78
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_79
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_80
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_81
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_82
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_83
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_84
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_85
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_86
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_87
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_88
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_89
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_90
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_91
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_92
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_93
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_94
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_95
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_96
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_97
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_98
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_99
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_100
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_101
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_102
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_103
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_104
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_105
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_106
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_107
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_108
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_109
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_110
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_111
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_112
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_113
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_114
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_115
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_116
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_117
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_118
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_119
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_120
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_121
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_122
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_123
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_124
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_125
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_126
#pragma HLS INTERFACE axis port=stage2_FAMDataIn_127

// AXI4-Stream interface: 128 outputs
#pragma HLS INTERFACE axis port=stage2_FAMOut0
#pragma HLS INTERFACE axis port=stage2_FAMOut1
#pragma HLS INTERFACE axis port=stage2_FAMOut2
#pragma HLS INTERFACE axis port=stage2_FAMOut3
#pragma HLS INTERFACE axis port=stage2_FAMOut4
#pragma HLS INTERFACE axis port=stage2_FAMOut5
#pragma HLS INTERFACE axis port=stage2_FAMOut6
#pragma HLS INTERFACE axis port=stage2_FAMOut7
#pragma HLS INTERFACE axis port=stage2_FAMOut8
#pragma HLS INTERFACE axis port=stage2_FAMOut9
#pragma HLS INTERFACE axis port=stage2_FAMOut10
#pragma HLS INTERFACE axis port=stage2_FAMOut11
#pragma HLS INTERFACE axis port=stage2_FAMOut12
#pragma HLS INTERFACE axis port=stage2_FAMOut13
#pragma HLS INTERFACE axis port=stage2_FAMOut14
#pragma HLS INTERFACE axis port=stage2_FAMOut15
#pragma HLS INTERFACE axis port=stage2_FAMOut16
#pragma HLS INTERFACE axis port=stage2_FAMOut17
#pragma HLS INTERFACE axis port=stage2_FAMOut18
#pragma HLS INTERFACE axis port=stage2_FAMOut19
#pragma HLS INTERFACE axis port=stage2_FAMOut20
#pragma HLS INTERFACE axis port=stage2_FAMOut21
#pragma HLS INTERFACE axis port=stage2_FAMOut22
#pragma HLS INTERFACE axis port=stage2_FAMOut23
#pragma HLS INTERFACE axis port=stage2_FAMOut24
#pragma HLS INTERFACE axis port=stage2_FAMOut25
#pragma HLS INTERFACE axis port=stage2_FAMOut26
#pragma HLS INTERFACE axis port=stage2_FAMOut27
#pragma HLS INTERFACE axis port=stage2_FAMOut28
#pragma HLS INTERFACE axis port=stage2_FAMOut29
#pragma HLS INTERFACE axis port=stage2_FAMOut30
#pragma HLS INTERFACE axis port=stage2_FAMOut31
#pragma HLS INTERFACE axis port=stage2_FAMOut32
#pragma HLS INTERFACE axis port=stage2_FAMOut33
#pragma HLS INTERFACE axis port=stage2_FAMOut34
#pragma HLS INTERFACE axis port=stage2_FAMOut35
#pragma HLS INTERFACE axis port=stage2_FAMOut36
#pragma HLS INTERFACE axis port=stage2_FAMOut37
#pragma HLS INTERFACE axis port=stage2_FAMOut38
#pragma HLS INTERFACE axis port=stage2_FAMOut39
#pragma HLS INTERFACE axis port=stage2_FAMOut40
#pragma HLS INTERFACE axis port=stage2_FAMOut41
#pragma HLS INTERFACE axis port=stage2_FAMOut42
#pragma HLS INTERFACE axis port=stage2_FAMOut43
#pragma HLS INTERFACE axis port=stage2_FAMOut44
#pragma HLS INTERFACE axis port=stage2_FAMOut45
#pragma HLS INTERFACE axis port=stage2_FAMOut46
#pragma HLS INTERFACE axis port=stage2_FAMOut47
#pragma HLS INTERFACE axis port=stage2_FAMOut48
#pragma HLS INTERFACE axis port=stage2_FAMOut49
#pragma HLS INTERFACE axis port=stage2_FAMOut50
#pragma HLS INTERFACE axis port=stage2_FAMOut51
#pragma HLS INTERFACE axis port=stage2_FAMOut52
#pragma HLS INTERFACE axis port=stage2_FAMOut53
#pragma HLS INTERFACE axis port=stage2_FAMOut54
#pragma HLS INTERFACE axis port=stage2_FAMOut55
#pragma HLS INTERFACE axis port=stage2_FAMOut56
#pragma HLS INTERFACE axis port=stage2_FAMOut57
#pragma HLS INTERFACE axis port=stage2_FAMOut58
#pragma HLS INTERFACE axis port=stage2_FAMOut59
#pragma HLS INTERFACE axis port=stage2_FAMOut60
#pragma HLS INTERFACE axis port=stage2_FAMOut61
#pragma HLS INTERFACE axis port=stage2_FAMOut62
#pragma HLS INTERFACE axis port=stage2_FAMOut63
#pragma HLS INTERFACE axis port=stage2_FAMOut64
#pragma HLS INTERFACE axis port=stage2_FAMOut65
#pragma HLS INTERFACE axis port=stage2_FAMOut66
#pragma HLS INTERFACE axis port=stage2_FAMOut67
#pragma HLS INTERFACE axis port=stage2_FAMOut68
#pragma HLS INTERFACE axis port=stage2_FAMOut69
#pragma HLS INTERFACE axis port=stage2_FAMOut70
#pragma HLS INTERFACE axis port=stage2_FAMOut71
#pragma HLS INTERFACE axis port=stage2_FAMOut72
#pragma HLS INTERFACE axis port=stage2_FAMOut73
#pragma HLS INTERFACE axis port=stage2_FAMOut74
#pragma HLS INTERFACE axis port=stage2_FAMOut75
#pragma HLS INTERFACE axis port=stage2_FAMOut76
#pragma HLS INTERFACE axis port=stage2_FAMOut77
#pragma HLS INTERFACE axis port=stage2_FAMOut78
#pragma HLS INTERFACE axis port=stage2_FAMOut79
#pragma HLS INTERFACE axis port=stage2_FAMOut80
#pragma HLS INTERFACE axis port=stage2_FAMOut81
#pragma HLS INTERFACE axis port=stage2_FAMOut82
#pragma HLS INTERFACE axis port=stage2_FAMOut83
#pragma HLS INTERFACE axis port=stage2_FAMOut84
#pragma HLS INTERFACE axis port=stage2_FAMOut85
#pragma HLS INTERFACE axis port=stage2_FAMOut86
#pragma HLS INTERFACE axis port=stage2_FAMOut87
#pragma HLS INTERFACE axis port=stage2_FAMOut88
#pragma HLS INTERFACE axis port=stage2_FAMOut89
#pragma HLS INTERFACE axis port=stage2_FAMOut90
#pragma HLS INTERFACE axis port=stage2_FAMOut91
#pragma HLS INTERFACE axis port=stage2_FAMOut92
#pragma HLS INTERFACE axis port=stage2_FAMOut93
#pragma HLS INTERFACE axis port=stage2_FAMOut94
#pragma HLS INTERFACE axis port=stage2_FAMOut95
#pragma HLS INTERFACE axis port=stage2_FAMOut96
#pragma HLS INTERFACE axis port=stage2_FAMOut97
#pragma HLS INTERFACE axis port=stage2_FAMOut98
#pragma HLS INTERFACE axis port=stage2_FAMOut99
#pragma HLS INTERFACE axis port=stage2_FAMOut100
#pragma HLS INTERFACE axis port=stage2_FAMOut101
#pragma HLS INTERFACE axis port=stage2_FAMOut102
#pragma HLS INTERFACE axis port=stage2_FAMOut103
#pragma HLS INTERFACE axis port=stage2_FAMOut104
#pragma HLS INTERFACE axis port=stage2_FAMOut105
#pragma HLS INTERFACE axis port=stage2_FAMOut106
#pragma HLS INTERFACE axis port=stage2_FAMOut107
#pragma HLS INTERFACE axis port=stage2_FAMOut108
#pragma HLS INTERFACE axis port=stage2_FAMOut109
#pragma HLS INTERFACE axis port=stage2_FAMOut110
#pragma HLS INTERFACE axis port=stage2_FAMOut111
#pragma HLS INTERFACE axis port=stage2_FAMOut112
#pragma HLS INTERFACE axis port=stage2_FAMOut113
#pragma HLS INTERFACE axis port=stage2_FAMOut114
#pragma HLS INTERFACE axis port=stage2_FAMOut115
#pragma HLS INTERFACE axis port=stage2_FAMOut116
#pragma HLS INTERFACE axis port=stage2_FAMOut117
#pragma HLS INTERFACE axis port=stage2_FAMOut118
#pragma HLS INTERFACE axis port=stage2_FAMOut119
#pragma HLS INTERFACE axis port=stage2_FAMOut120
#pragma HLS INTERFACE axis port=stage2_FAMOut121
#pragma HLS INTERFACE axis port=stage2_FAMOut122
#pragma HLS INTERFACE axis port=stage2_FAMOut123
#pragma HLS INTERFACE axis port=stage2_FAMOut124
#pragma HLS INTERFACE axis port=stage2_FAMOut125
#pragma HLS INTERFACE axis port=stage2_FAMOut126
#pragma HLS INTERFACE axis port=stage2_FAMOut127

    //=============================
    // Steps 1 and 2: sequential implementation first
    //=============================
    phase1(
        memin0, memin1, memin2, memin3, memin4, memin5, memin6, memin7, memin8, memin9, memin10, memin11, memin12, memin13, memin14, memin15,
        stage1_FAMDataIn_0, stage1_FAMDataIn_1, stage1_FAMDataIn_2, stage1_FAMDataIn_3, 
        stage1_FAMDataIn_4, stage1_FAMDataIn_5, stage1_FAMDataIn_6, stage1_FAMDataIn_7, 
        stage1_FAMDataIn_8, stage1_FAMDataIn_9, stage1_FAMDataIn_10, stage1_FAMDataIn_11, 
        stage1_FAMDataIn_12, stage1_FAMDataIn_13, stage1_FAMDataIn_14, stage1_FAMDataIn_15,
        stage1_FAMOut0, stage1_FAMOut1, stage1_FAMOut2, stage1_FAMOut3, 
        stage1_FAMOut4, stage1_FAMOut5, stage1_FAMOut6, stage1_FAMOut7, 
        stage1_FAMOut8, stage1_FAMOut9, stage1_FAMOut10, stage1_FAMOut11, 
        stage1_FAMOut12, stage1_FAMOut13, stage1_FAMOut14, stage1_FAMOut15,
        memtrans
    );

    //=============================
    // Steps 3 and 4: parallelization execution
    //=============================

    phase2(
    memtrans,
    stage2_FAMDataIn_0, stage2_FAMDataIn_1,stage2_FAMDataIn_2, stage2_FAMDataIn_3,stage2_FAMDataIn_4, stage2_FAMDataIn_5,stage2_FAMDataIn_6, stage2_FAMDataIn_7,  
    stage2_FAMDataIn_8, stage2_FAMDataIn_9,stage2_FAMDataIn_10, stage2_FAMDataIn_11,stage2_FAMDataIn_12, stage2_FAMDataIn_13,stage2_FAMDataIn_14, stage2_FAMDataIn_15,
    stage2_FAMDataIn_16, stage2_FAMDataIn_17,stage2_FAMDataIn_18, stage2_FAMDataIn_19,stage2_FAMDataIn_20, stage2_FAMDataIn_21,stage2_FAMDataIn_22, stage2_FAMDataIn_23,  
    stage2_FAMDataIn_24, stage2_FAMDataIn_25,stage2_FAMDataIn_26, stage2_FAMDataIn_27,stage2_FAMDataIn_28, stage2_FAMDataIn_29,stage2_FAMDataIn_30, stage2_FAMDataIn_31,
    stage2_FAMDataIn_32, stage2_FAMDataIn_33,stage2_FAMDataIn_34, stage2_FAMDataIn_35,stage2_FAMDataIn_36, stage2_FAMDataIn_37,stage2_FAMDataIn_38, stage2_FAMDataIn_39,  
    stage2_FAMDataIn_40, stage2_FAMDataIn_41,stage2_FAMDataIn_42, stage2_FAMDataIn_43,stage2_FAMDataIn_44, stage2_FAMDataIn_45,stage2_FAMDataIn_46, stage2_FAMDataIn_47,  
    stage2_FAMDataIn_48, stage2_FAMDataIn_49,stage2_FAMDataIn_50, stage2_FAMDataIn_51,stage2_FAMDataIn_52, stage2_FAMDataIn_53,stage2_FAMDataIn_54, stage2_FAMDataIn_55,
    stage2_FAMDataIn_56, stage2_FAMDataIn_57,stage2_FAMDataIn_58, stage2_FAMDataIn_59,stage2_FAMDataIn_60, stage2_FAMDataIn_61,stage2_FAMDataIn_62, stage2_FAMDataIn_63,
    stage2_FAMDataIn_64, stage2_FAMDataIn_65,stage2_FAMDataIn_66, stage2_FAMDataIn_67,stage2_FAMDataIn_68, stage2_FAMDataIn_69,stage2_FAMDataIn_70, stage2_FAMDataIn_71,
    stage2_FAMDataIn_72, stage2_FAMDataIn_73,stage2_FAMDataIn_74, stage2_FAMDataIn_75,stage2_FAMDataIn_76, stage2_FAMDataIn_77,stage2_FAMDataIn_78, stage2_FAMDataIn_79,
    stage2_FAMDataIn_80, stage2_FAMDataIn_81,stage2_FAMDataIn_82, stage2_FAMDataIn_83,stage2_FAMDataIn_84, stage2_FAMDataIn_85,stage2_FAMDataIn_86, stage2_FAMDataIn_87,
    stage2_FAMDataIn_88, stage2_FAMDataIn_89,stage2_FAMDataIn_90, stage2_FAMDataIn_91,stage2_FAMDataIn_92, stage2_FAMDataIn_93,stage2_FAMDataIn_94, stage2_FAMDataIn_95,
    stage2_FAMDataIn_96, stage2_FAMDataIn_97,stage2_FAMDataIn_98, stage2_FAMDataIn_99,stage2_FAMDataIn_100, stage2_FAMDataIn_101,stage2_FAMDataIn_102, stage2_FAMDataIn_103,
    stage2_FAMDataIn_104, stage2_FAMDataIn_105,stage2_FAMDataIn_106, stage2_FAMDataIn_107,stage2_FAMDataIn_108, stage2_FAMDataIn_109,stage2_FAMDataIn_110, stage2_FAMDataIn_111,
    stage2_FAMDataIn_112, stage2_FAMDataIn_113,stage2_FAMDataIn_114, stage2_FAMDataIn_115,stage2_FAMDataIn_116, stage2_FAMDataIn_117,stage2_FAMDataIn_118, stage2_FAMDataIn_119,
    stage2_FAMDataIn_120, stage2_FAMDataIn_121,stage2_FAMDataIn_122, stage2_FAMDataIn_123,stage2_FAMDataIn_124, stage2_FAMDataIn_125,stage2_FAMDataIn_126, stage2_FAMDataIn_127,
   

    stage2_FAMOut0, stage2_FAMOut1,stage2_FAMOut2, stage2_FAMOut3,stage2_FAMOut4, stage2_FAMOut5,stage2_FAMOut6, stage2_FAMOut7,  
    stage2_FAMOut8, stage2_FAMOut9,stage2_FAMOut10, stage2_FAMOut11,stage2_FAMOut12, stage2_FAMOut13,stage2_FAMOut14, stage2_FAMOut15,
    stage2_FAMOut16, stage2_FAMOut17,stage2_FAMOut18, stage2_FAMOut19,stage2_FAMOut20, stage2_FAMOut21,stage2_FAMOut22, stage2_FAMOut23,  
    stage2_FAMOut24, stage2_FAMOut25,stage2_FAMOut26, stage2_FAMOut27,stage2_FAMOut28, stage2_FAMOut29,stage2_FAMOut30, stage2_FAMOut31,
    stage2_FAMOut32, stage2_FAMOut33,stage2_FAMOut34, stage2_FAMOut35,stage2_FAMOut36, stage2_FAMOut37,stage2_FAMOut38, stage2_FAMOut39,
    stage2_FAMOut40, stage2_FAMOut41,stage2_FAMOut42, stage2_FAMOut43,stage2_FAMOut44, stage2_FAMOut45,stage2_FAMOut46, stage2_FAMOut47,
    stage2_FAMOut48, stage2_FAMOut49,stage2_FAMOut50, stage2_FAMOut51,stage2_FAMOut52, stage2_FAMOut53,stage2_FAMOut54, stage2_FAMOut55,
    stage2_FAMOut56, stage2_FAMOut57,stage2_FAMOut58, stage2_FAMOut59,stage2_FAMOut60, stage2_FAMOut61,stage2_FAMOut62, stage2_FAMOut63,
    stage2_FAMOut64, stage2_FAMOut65,stage2_FAMOut66, stage2_FAMOut67,stage2_FAMOut68, stage2_FAMOut69,stage2_FAMOut70, stage2_FAMOut71,
    stage2_FAMOut72, stage2_FAMOut73,stage2_FAMOut74, stage2_FAMOut75,stage2_FAMOut76, stage2_FAMOut77,stage2_FAMOut78, stage2_FAMOut79,
    stage2_FAMOut80, stage2_FAMOut81,stage2_FAMOut82, stage2_FAMOut83,stage2_FAMOut84, stage2_FAMOut85,stage2_FAMOut86, stage2_FAMOut87,
    stage2_FAMOut88, stage2_FAMOut89,stage2_FAMOut90, stage2_FAMOut91,stage2_FAMOut92, stage2_FAMOut93,stage2_FAMOut94, stage2_FAMOut95,
    stage2_FAMOut96, stage2_FAMOut97,stage2_FAMOut98, stage2_FAMOut99,stage2_FAMOut100, stage2_FAMOut101,stage2_FAMOut102, stage2_FAMOut103,
    stage2_FAMOut104, stage2_FAMOut105,stage2_FAMOut106, stage2_FAMOut107,stage2_FAMOut108, stage2_FAMOut109,stage2_FAMOut110, stage2_FAMOut111,
    stage2_FAMOut112, stage2_FAMOut113,stage2_FAMOut114, stage2_FAMOut115,stage2_FAMOut116, stage2_FAMOut117,stage2_FAMOut118, stage2_FAMOut119,
    stage2_FAMOut120, stage2_FAMOut121,stage2_FAMOut122, stage2_FAMOut123,stage2_FAMOut124, stage2_FAMOut125,stage2_FAMOut126, stage2_FAMOut127,
    
    memout
    );

}
