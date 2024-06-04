#ifndef __FP_OPERATION_H__
#define __FP_OPERATION_H__

#include "typedef.h"

namespace FPOP
{
    struct FP32_t
    {
        uint32_t fraction: 23;
        uint32_t exp:       8;        
        uint32_t sign:      1;
    };

    bool IsNaN(const FP32_t& stFP32);
    bool IsInf(const FP32_t& stFP32);
    bool IsZero(const FP32_t& stFP32);

    float32_t fp32_to_float32(const FP32_t& stFP32);
    FP32_t float32_to_fp32(const float32_t f32Val);
    
    FP32_t add_fp32(const FP32_t stA, const FP32_t stB);
    FP32_t sub_fp32(const FP32_t stA, const FP32_t stB);
    FP32_t mul_fp32(const FP32_t stA, const FP32_t stB);
}

#endif


