#include "fp32_operation.h"
#include <stdio.h>
#include <string.h>

const uint32_t FP32_MANTISSA_BITS = 23U;
const uint32_t FP16_EXP_BIAS = 127U;

namespace FP32
{
    float32_t to_float32(const FP32_t& stFP32)
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

        if(stFP32.exp >= FP16_EXP_BIAS)
        {
            f32Value *= static_cast<float32_t>(1U << (stFP32.exp - FP16_EXP_BIAS));
        }
        else
        {
            f32Value /= static_cast<float32_t>(1U << (FP16_EXP_BIAS - stFP32.exp));
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
        if((0xFFU == stFP32.exp) && (0U != stFP32.mantissa))
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
        if((0xFFU == stFP32.exp) && (0U == stFP32.mantissa))
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
        if((0U == stFP32.exp) && (0U == stFP32.mantissa))
        {
            return true;
        }
        else
        {
            return false;
        }
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

    FP32_t from_float32(const float32_t f32Val)
    {
        FP32_t stFP32;
        (void)memcpy(&stFP32, &f32Val, sizeof(f32Val));
        return stFP32;
    }

    FP32_t add(const FP32_t stA, const FP32_t stB)
    {
        const uint32_t u32SignA = stA.sign;
        const uint32_t u32SignB = stB.sign;

        uint32_t u32ExpA = stA.exp;
        uint32_t u32ExpB = stB.exp;
        
        // Prepend implicit 1 to mantissas
        // (1 << 23U) = 0x800000
        uint32_t u32SignificandA = (1U << FP32_MANTISSA_BITS) + stA.mantissa;
        uint32_t u32SignificandB = (1U << FP32_MANTISSA_BITS) + stB.mantissa;

        u32SignificandA <<= 7U; // 7: 8(exponent bits) - 1
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
            if(u32SignificandA > u32SignificandB) {
                u32SignC = u32SignA;
                u32SignificandC = u32SignificandA - u32SignificandB;
            } else if(u32SignificandA < u32SignificandB) {
                u32SignC = u32SignB;
                u32SignificandC = u32SignificandB - u32SignificandA;
            } else {
                u32SignC = 0U;
                u32SignificandC = 0U;
            }
        }

        FP32_t stResult;
        if(0U == u32SignificandC)
        {
            stResult.exp = 0U;
            stResult.mantissa = 0U;
            stResult.sign = u32SignC;
        }
        else
        { 
            uint32_t u32ExpC = u32ExpA;

            // Normalize
            int32_t s32NumSigBits = 31; // (24 + 7) bits. (23-bit + 23-bit) <= 24-bit
            while((((u32SignificandC >> s32NumSigBits) & 0x01U) == 0U) && (s32NumSigBits > 0))
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

                if((1U == u32Round) && (1U == u32Guard)) {
                    u32SignificandC += 1U;
                } else if((0U == u32Round) && (1U == u32Guard)) {
                    u32SignificandC += 1U;
                } else if((1U == u32Guard) && (1U == u32LSB)) {
                    u32SignificandC += 1U;
                }

                // (23-bit + 23-bit) <= 24-bit
                if((u32SignificandC >> 24U) > 0) {
                    u32ExpC --;
                }
            }
            else if(s32NumSigBits < FP32_MANTISSA_BITS)
            {
                u32ExpC -= (FP32_MANTISSA_BITS - s32NumSigBits);
                u32SignificandC <<= (FP32_MANTISSA_BITS - s32NumSigBits);
            }

            u32ExpC -= 7U;  // 7: 8(exponent bits) - 1

            stResult.sign = u32SignC;
            stResult.exp = u32ExpC;
            stResult.mantissa = (u32SignificandC & 0x7FFFFFU);// - (1U << 24U);
        }
        return stResult;
    }

    FP32_t sub(const FP32_t stA, const FP32_t stB)
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
        const FP32_t stC = add(stA, stNegB);

        return stC;
    }

    FP32_t mul(const FP32_t stA, const FP32_t stB)
    {
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

        FP32_t stResult;
        if(IsZero(stA) || IsZero(stB))
        {
            stResult.exp = 0U;
            stResult.mantissa = 0U;
            stResult.sign = u32SignC;
        }
        else
        {        
            uint32_t u32ExpA = stA.exp;
            uint32_t u32ExpB = stB.exp;

            // Prepend implicit 1 to mantissas
            // (1 << 23U) = 0x800000
            uint32_t u32SignificandA = (1U << FP32_MANTISSA_BITS) + stA.mantissa;
            uint32_t u32SignificandB = (1U << FP32_MANTISSA_BITS) + stB.mantissa;
            
            u32SignificandA <<= 7U; // 7 + 8 = 15, 24 + 24 + 15 = 63
            u32SignificandB <<= 8U;

            // (expC + bias) = (expA + bias) + (expC + bias) - bias
            uint32_t u32ExpC = (u32ExpA + u32ExpB) - FP16_EXP_BIAS;
            uint64_t u64SignificandC = static_cast<uint64_t>(u32SignificandA) * static_cast<uint64_t>(u32SignificandB);
            // u64SignificandC >>= FP32_MANTISSA_BITS; // (24+7) + (24+8)
              
            // Normalize
            int32_t s32NumSigBits = 63; // (24 + 24 + 15)
            while((((u64SignificandC >> s32NumSigBits) & 0x01U) == 0U) && (s32NumSigBits > 0U))
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
                else if(0U == u32Round && 1U == u32Guard)
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

            u32ExpC -= 15U;
            u32ExpC -= FP32_MANTISSA_BITS;

            stResult.sign = u32SignC;
            stResult.exp = u32ExpC;
            stResult.mantissa = (u64SignificandC & 0x7FFFFF);// - (1U << 24U);
        }

        return stResult;
    }
}




