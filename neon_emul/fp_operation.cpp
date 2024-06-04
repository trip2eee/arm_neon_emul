#include "fp_operation.h"
#include <stdio.h>
#include <string.h>

const uint32_t FP32_MANTISSA_BITS = 23U;
const uint32_t FP32_EXP_BIAS = 127U;

namespace FP32
{
    float32_t fp32_to_float32(const FP32_t& stFP32)
    {       
        float32_t f32Value = 1.0F;

        float32_t f32F = 1.0F;
        for(uint32_t u32IdxF = 0U; u32IdxF < FP32_MANTISSA_BITS; u32IdxF++)
        {
            f32F *= 0.5F;
            if((stFP32.mantissa >> (FP32_MANTISSA_BITS - 1U - u32IdxF) & 0x01U) == 1U)
            {
                f32Value += f32F;
            }
        }

        if(stFP32.exp >= FP32_EXP_BIAS)
        {
            f32Value *= static_cast<float32_t>(1U << (stFP32.exp - FP32_EXP_BIAS));
        }
        else
        {
            f32Value /= static_cast<float32_t>(1U << (FP32_EXP_BIAS - stFP32.exp));
        }
        
        if(stFP32.sign == 1)
        {
            f32Value *= -1.0F;
        }

        return f32Value;
    }

    bool IsNaN(const FP32_t& stFP32)
    {
        // NaN 0x7FFFFFFF
        if((255U == stFP32.exp) && (0U != stFP32.mantissa))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool IsInf(const FP32_t& stFP32)
    {
        if((255U == stFP32.exp) && (0U == stFP32.mantissa))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool IsZero(const FP32_t& stFP32)
    {
        return true;
    }
    
    bool CheckOverflow(const uint32_t u32Exp)
    {
        if(u32Exp >= 0xFFU)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool CheckUnderflow(const uint32_t u32Exp)
    {
        if(u32Exp >= 0x800000FEU)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    FP32_t float32_to_fp32(const float32_t f32Val)
    {
        FP32_t stFP32;
        (void)memcpy(&stFP32, &f32Val, sizeof(f32Val));
        return stFP32;
    }

    FP32_t add_fp32(const FP32_t stA, const FP32_t stB)
    {
        const uint32_t u32SignA = stA.sign;
        const uint32_t u32SignB = stB.sign;

        uint32_t u32ExpA = stA.exp;
        uint32_t u32ExpB = stB.exp;
        
        // Prepend implicit 1 to mantissas
        // (1 << 23U) = 0x800000
        uint32_t u32SignificandA = (1U << FP32_MANTISSA_BITS) + stA.mantissa;
        uint32_t u32SignificandB = (1U << FP32_MANTISSA_BITS) + stB.mantissa;

        u32SignificandA <<= 7U;
        u32SignificandB <<= 7U;

        if(u32ExpA > u32ExpB)
        {
            u32SignificandB >>= (u32ExpA - u32ExpB);
            u32ExpB += (u32ExpA - u32ExpB);
        }
        else if(u32ExpA < u32ExpB)
        {
            u32SignificandA >>= (u32ExpB - u32ExpA);
            u32ExpA += (u32ExpB - u32ExpA);
        }

        // Add
        uint32_t u32SignificandC;
        uint32_t u32SignC;
        
        if(u32SignA == u32SignB)
        {
            u32SignificandC = u32SignificandA + u32SignificandB;
            u32SignC = u32SignA;
        }
        else
        {
            if(u32SignificandA > u32SignificandB)
            {
                u32SignC = u32SignA;
                u32SignificandC = u32SignificandA - u32SignificandB;
            }
            else if(u32SignificandA < u32SignificandB)
            {
                u32SignC = u32SignB;
                u32SignificandC = u32SignificandB - u32SignificandA;
            }
            else
            {
                u32SignC = 0U;
                u32SignificandC = 0U;
            }
        }
        
        uint32_t u32ExpC = u32ExpA;

        // Normalize
        int32_t s32NumSigBits = 31;
        while(((u32SignificandC >> s32NumSigBits) == 0) && (s32NumSigBits > 0))
        {
            s32NumSigBits--;
        }
        
        if(s32NumSigBits > FP32_MANTISSA_BITS)
        {
            u32ExpC += (s32NumSigBits - FP32_MANTISSA_BITS);

            // Round
            uint32_t u32Round;            
            if((s32NumSigBits - FP32_MANTISSA_BITS) >= 2U) {
                u32Round = (u32SignificandC >> (s32NumSigBits - FP32_MANTISSA_BITS - 2U)) & 0x01U;
            } else {
                u32Round = 0U;
            }

            uint32_t u32Guard;
            if((s32NumSigBits - FP32_MANTISSA_BITS) >= 1U) {
                u32Guard = (u32SignificandC >> (s32NumSigBits - FP32_MANTISSA_BITS - 1U)) & 0x01U;
            } else {
                u32Guard = 0U;
            }

            u32SignificandC >>= (s32NumSigBits - FP32_MANTISSA_BITS);

            const uint32_t u32LSB = u32SignificandC & 0x01U;

            if(1U == u32Round && 1U == u32Guard) {
                u32SignificandC += 1U;
            } else if(1U == u32Guard && 1U == u32LSB) {
                u32SignificandC += 1U;
            }

            if((u32SignificandC >> 24U) > 0) {
                u32ExpC --;
            }
        }
        else if(s32NumSigBits < FP32_MANTISSA_BITS)
        {
            u32ExpC -= (FP32_MANTISSA_BITS - s32NumSigBits);
            u32SignificandC <<= (FP32_MANTISSA_BITS - s32NumSigBits);
        }

        u32ExpC -= 7U;
        // u32SignificandC >>= 7U;

        FP32_t stResult;
        stResult.sign = u32SignC;
        stResult.exp = u32ExpC;
        stResult.mantissa = (u32SignificandC & 0x7FFFFF);// - (1U << 24U);

        return stResult;
    }

    FP32_t sub_fp32(const FP32_t stA, const FP32_t stB)
    {
        FP32_t stNegB = stB;
        if(0U == stNegB.sign)
        {
            stNegB.sign = 1U;
        }
        else
        {
            stNegB.sign = 0U;
        }
        const FP32_t stC = add_fp32(stA, stNegB);

        return stC;
    }

    FP32_t mul_fp32(const FP32_t stA, const FP32_t stB)
    {
        FP32_t stC;

        const uint32_t u32SignA = stA.sign;
        const uint32_t u32SignB = stB.sign;
        uint32_t u32SignC;
        if(u32SignA == u32SignB)
        {
            u32SignC = 0U;
        }
        else
        {
            u32SignC = 1U;
        }

        uint32_t u32ExpA = stA.exp;
        uint32_t u32ExpB = stB.exp;

        // Prepend implicit 1 to mantissas
        // (1 << 23U) = 0x800000
        uint32_t u32SignificandA = (1U << FP32_MANTISSA_BITS) + stA.mantissa;
        uint32_t u32SignificandB = (1U << FP32_MANTISSA_BITS) + stB.mantissa;
        
        // (expC + bias) = (expA + bias) + (expC + bias) - bias
        uint32_t u32ExpC = (u32ExpA + u32ExpB) - FP32_EXP_BIAS;
        uint64_t u64SignificandC = static_cast<uint64_t>(u32SignificandA) * static_cast<uint64_t>(u32SignificandB);
        // u64SignificandC >>= FP32_MANTISSA_BITS;

        // Normalize
        int32_t s32NumSigBits = 63;
        while(((u64SignificandC >> s32NumSigBits) == 0) && (s32NumSigBits > 0))
        {
            s32NumSigBits--;
        }

        if(s32NumSigBits > FP32_MANTISSA_BITS)
        {
            u32ExpC += (s32NumSigBits - FP32_MANTISSA_BITS);

            uint32_t u32Round;            
            if((s32NumSigBits - FP32_MANTISSA_BITS) >= 2U)
            {
                u32Round = (u64SignificandC >> (s32NumSigBits - FP32_MANTISSA_BITS - 2U)) & 0x01U;
            }
            else
            {
                u32Round = 0U;
            }

            uint32_t u32Guard;
            if((s32NumSigBits - FP32_MANTISSA_BITS) >= 1U)
            {
                u32Guard = (u64SignificandC >> (s32NumSigBits - FP32_MANTISSA_BITS - 1U)) & 0x01U;
            }
            else
            {
                u32Guard = 0U;
            }

            u64SignificandC >>= (s32NumSigBits - FP32_MANTISSA_BITS);

            const uint32_t u32LSB = u64SignificandC & 0x01U;

            if(1U == u32Round && 1U == u32Guard)
            {
                u64SignificandC += 1U;
            }
            else if(1U == u32Guard && 1U == u32LSB)
            {
                u64SignificandC += 1U;
            }

            if((u64SignificandC >> 24U) > 0)
            {
                u32ExpC --;
            }
        }
        else if(s32NumSigBits < FP32_MANTISSA_BITS)
        {
            u32ExpC -= (FP32_MANTISSA_BITS - s32NumSigBits);
            u64SignificandC <<= (FP32_MANTISSA_BITS - s32NumSigBits);
        }

        u32ExpC -= FP32_MANTISSA_BITS;

        FP32_t stResult;
        stResult.sign = u32SignC;
        stResult.exp = u32ExpC;
        stResult.mantissa = (u64SignificandC & 0x7FFFFF);// - (1U << 24U);

        return stResult;
    }
}




