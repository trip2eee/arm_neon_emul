#ifndef __FP32_OPERATION_H__
#define __FP32_OPERATION_H__

#include "typedef.h"

namespace FP32
{
    struct FP32_t
    {
        uint32_t mantissa: 23;      // mantissa
        uint32_t exp:       8;      // exponent
        uint32_t sign:      1;      // sign
    };

    bool IsNaN(const FP32_t& stFP32);
    bool IsInf(const FP32_t& stFP32);
    bool IsZero(const FP32_t& stFP32);
    bool CheckOverflow(const uint32_t u32Exp);
    bool CheckUnderflow(const uint32_t u32Exp);

    float32_t to_float32(const FP32_t& stFP32);
    FP32_t from_float32(const float32_t f32Val);
    
    FP32_t add(const FP32_t stA, const FP32_t stB);
    FP32_t sub(const FP32_t stA, const FP32_t stB);
    FP32_t mul(const FP32_t stA, const FP32_t stB);
}

#endif


