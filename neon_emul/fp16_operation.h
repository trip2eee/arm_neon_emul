#ifndef __FP16_OPERATION_H__
#define __FP16_OPERATION_H__

#include "typedef.h"

namespace FP16
{
    struct FP16_t
    {
        uint32_t mantissa: 23;      // mantissa
        uint32_t exp:       8;      // exponent
        uint32_t sign:      1;      // sign
    };

    bool IsNaN(const FP16_t& stFP32);
    bool IsInf(const FP16_t& stFP32);
    bool IsZero(const FP16_t& stFP32);
    bool CheckOverflow(const uint32_t u32Exp);
    bool CheckUnderflow(const uint32_t u32Exp);

    float32_t fp32_to_float32(const FP16_t& stFP32);
    FP16_t float32_to_fp32(const float32_t f32Val);
    
    FP16_t add_fp32(const FP16_t stA, const FP16_t stB);
    FP16_t sub_fp32(const FP16_t stA, const FP16_t stB);
    FP16_t mul_fp32(const FP16_t stA, const FP16_t stB);
}

#endif


