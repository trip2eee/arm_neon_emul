#ifndef __FP16_OPERATION_H__
#define __FP16_OPERATION_H__

#include "typedef.h"

namespace FP16
{
    #pragma pack(push, 1)
    struct FP16_t
    {
        uint16_t mantissa: 10;      // mantissa
        uint16_t exp:       5;      // exponent
        uint16_t sign:      1;      // sign
    };
    #pragma pack(pop)

    bool IsNaN(const FP16_t& stFP32);
    bool IsInf(const FP16_t& stFP32);
    bool IsZero(const FP16_t& stFP32);
    bool CheckOverflow(const uint32_t u32Exp);
    bool CheckUnderflow(const uint32_t u32Exp);

    float32_t to_float32(const FP16_t& stFP32);
    FP16_t from_float32(const float32_t f32Val);
    
    FP16_t add(const FP16_t stA, const FP16_t stB);
    FP16_t sub(const FP16_t stA, const FP16_t stB);
    FP16_t mul(const FP16_t stA, const FP16_t stB);

    /**
     * @brief This function computes a + (b * c)
     * @param stA [in] operand a
     * @param stB [in] operand b
     * @param stC [in] operand c
     * @return a + (b * c)
    */
    FP16_t mla(const FP16_t stA, const FP16_t stB, const FP16_t stC);
}

#endif


