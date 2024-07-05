#include "fp16_operation.h"
#include "fp32_operation.h"
#include <stdio.h>
#include <string.h>

const uint32_t FP16_MANTISSA_BITS = 10U;
const uint32_t FP16_EXP_BIAS = 15U;

namespace FP16
{
    float32_t to_float32(const FP16_t& stFP16)
    {       
        float32_t f32Value;
        if(IsZero(stFP16))
        {
            f32Value = 0.0F;
        }
        else
        {
            f32Value = 1.0F;
            float32_t f32F = 1.0F;
            for(uint32_t u32IdxF = 0U; u32IdxF < FP16_MANTISSA_BITS; u32IdxF++)
            {
                f32F *= 0.5F;
                if((stFP16.mantissa >> (FP16_MANTISSA_BITS - 1U - u32IdxF) & 0x01U) == 1U)
                {
                    f32Value += f32F;
                }
            }

            if(stFP16.exp >= FP16_EXP_BIAS)
            {
                f32Value *= static_cast<float32_t>(1U << (stFP16.exp - FP16_EXP_BIAS));
            }
            else
            {
                f32Value /= static_cast<float32_t>(1U << (FP16_EXP_BIAS - stFP16.exp));
            }
        }

        if(stFP16.sign == 1)
        {
            f32Value *= -1.0F;
        }

        return f32Value;
    }

    FP16_t from_float32(const float32_t f32Val)
    {
        FP32::FP32_t stFP32;
        (void)memcpy(&stFP32, &f32Val, sizeof(f32Val));

        FP16_t stFP16;
        stFP16.sign = stFP32.sign;
        
        // handle special cases.
        if(FP32::IsNaN(stFP32))
        {
            stFP16.exp = 0x1FU;
        }
        else if(FP32::IsInf(stFP32))
        {
            stFP16.exp = 0x1FU;
        }
        else if(FP32::IsZero(stFP32))
        {
            stFP16.exp = 0;
            stFP16.mantissa = 0;
            stFP16.sign = stFP32.sign;
        }
        else
        {
            const int32_t s32Exp = static_cast<int32_t>(stFP32.exp) - 127 + 15;
            stFP16.exp = static_cast<uint16_t>(s32Exp);
        }        

        stFP16.mantissa = stFP32.mantissa >> 13;

        // rounding
        if(((stFP32.mantissa >> 12U) & 0x01U) != 0U)
        {
            stFP16.mantissa += 1;
        }
        
        return stFP16;
    }

    bool IsNaN(const FP16_t& stFP16)
    {
        // NaN 0x7FFFFFFF
        if((0x01FU == stFP16.exp) && (0U != stFP16.mantissa))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool IsInf(const FP16_t& stFP16)
    {
        if((0x1FU == stFP16.exp) && (0U == stFP16.mantissa))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool IsZero(const FP16_t& stFP16)
    {
        if((0U == stFP16.exp) && (0U == stFP16.mantissa))
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
        if(u32Exp >= 0x1FU)
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

    FP16_t add(const FP16_t stA, const FP16_t stB)
    {
        const uint32_t u32SignA = stA.sign;
        const uint32_t u32SignB = stB.sign;

        uint32_t u32ExpA = stA.exp;
        uint32_t u32ExpB = stB.exp;
        
        // Prepend implicit 1 to mantissas
        uint32_t u32SignificandA = (1U << FP16_MANTISSA_BITS) + stA.mantissa;
        uint32_t u32SignificandB = (1U << FP16_MANTISSA_BITS) + stB.mantissa;

        u32SignificandA <<= 4U; // 4: 5(exponent bit) - 1
        u32SignificandB <<= 4U;

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

        FP16_t stResult;
        if(0U == u32SignificandC)
        {
            // special case: zero
            stResult.exp = 0U;
            stResult.mantissa = 0U;
            stResult.sign = u32SignC;
        }
        else
        {
            uint32_t u32ExpC = u32ExpA;

            // Normalize
            int32_t s32NumSigBits = 15; // (11 + 4) bits. (10-bit + 10-bit) <= 11-bit
            while((((u32SignificandC >> s32NumSigBits) & 0x01U) == 0U) && (s32NumSigBits > 0))
            {
                s32NumSigBits--;
            }

            if(s32NumSigBits > FP16_MANTISSA_BITS)
            {
                u32ExpC += (s32NumSigBits - FP16_MANTISSA_BITS);

                // Round
                uint32_t u32Round;            
                if((s32NumSigBits - FP16_MANTISSA_BITS) >= 2U) {
                    u32Round = (u32SignificandC >> (s32NumSigBits - FP16_MANTISSA_BITS - 2U)) & 0x01U;
                } else {
                    u32Round = 0U;
                }

                uint32_t u32Guard;
                if((s32NumSigBits - FP16_MANTISSA_BITS) >= 1U) {
                    u32Guard = (u32SignificandC >> (s32NumSigBits - FP16_MANTISSA_BITS - 1U)) & 0x01U;
                } else {
                    u32Guard = 0U;
                }

                u32SignificandC >>= (s32NumSigBits - FP16_MANTISSA_BITS);

                const uint32_t u32LSB = u32SignificandC & 0x01U;

                if((1U == u32Round) && (1U == u32Guard)) {
                    u32SignificandC += 1U;
                } else if((0U == u32Round) && (1U == u32Guard) && (1U == u32LSB)) {
                    u32SignificandC += 1U;
                } else if((1U == u32Guard) && (1U == u32LSB)) {
                    u32SignificandC += 1U;
                }

                // (10-bit + 10-bit) <= 11-bit
                if((u32SignificandC >> 11U) > 0) {
                    u32ExpC --;
                }
            }
            else if(s32NumSigBits < FP16_MANTISSA_BITS)
            {
                u32ExpC -= (FP16_MANTISSA_BITS - s32NumSigBits);
                u32SignificandC <<= (FP16_MANTISSA_BITS - s32NumSigBits);
            }

            u32ExpC -= 4U;  // 4: 5(exponent bit) - 1

            stResult.sign = u32SignC;
            stResult.exp = u32ExpC;
            stResult.mantissa = (u32SignificandC & 0x3FFU);// - (1U << 11U);
        }

        return stResult;
    }

    FP16_t sub(const FP16_t stA, const FP16_t stB)
    {
        FP16_t stNegB = stB;
        if(0U == stNegB.sign)
        {
            stNegB.sign = 1U;
        }
        else
        {
            stNegB.sign = 0U;
        }
        const FP16_t stC = add(stA, stNegB);

        return stC;
    }

    FP16_t mul(const FP16_t stA, const FP16_t stB)
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

        FP16_t stResult;
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
            uint32_t u32SignificandA = (1U << FP16_MANTISSA_BITS) + stA.mantissa;
            uint32_t u32SignificandB = (1U << FP16_MANTISSA_BITS) + stB.mantissa;
            
            u32SignificandA <<= 4U; // 4 + 5 = 9, 11 + 11 + 9 = 31
            u32SignificandB <<= 5U;

            // (expC + bias) = (expA + bias) + (expC + bias) - bias
            uint32_t u32ExpC = (u32ExpA + u32ExpB) - FP16_EXP_BIAS;
            uint32_t u32SignificandC = u32SignificandA * u32SignificandB;

            // Normalize
            int32_t s32NumSigBits = 31; // (11 + 11 + 9)
            while((((u32SignificandC >> s32NumSigBits) & 0x01U) == 0U) && (s32NumSigBits > 0U))
            {
                s32NumSigBits--;
            }

            if(s32NumSigBits > FP16_MANTISSA_BITS)
            {
                u32ExpC += (s32NumSigBits - FP16_MANTISSA_BITS);

                uint32_t u32Round;
                if((s32NumSigBits - FP16_MANTISSA_BITS) >= 2U)
                {
                    u32Round = (u32SignificandC >> (s32NumSigBits - FP16_MANTISSA_BITS - 2U)) & 0x01U;
                }
                else
                {
                    u32Round = 0U;
                }

                uint32_t u32Guard;
                if((s32NumSigBits - FP16_MANTISSA_BITS) >= 1U)
                {
                    u32Guard = (u32SignificandC >> (s32NumSigBits - FP16_MANTISSA_BITS - 1U)) & 0x01U;
                }
                else
                {
                    u32Guard = 0U;
                }

                u32SignificandC >>= (s32NumSigBits - FP16_MANTISSA_BITS);

                const uint32_t u32LSB = u32SignificandC & 0x01U;

                if(1U == u32Round && 1U == u32Guard)
                {
                    u32SignificandC += 1U;
                }
                else if(0U == u32Round && 1U == u32Guard)
                {
                    u32SignificandC += 1U;
                }
                else if(1U == u32Guard && 1U == u32LSB)
                {
                    u32SignificandC += 1U;
                }

                if((u32SignificandC >> 24U) > 0)
                {
                    u32ExpC --;
                }
            }
            else if(s32NumSigBits < FP16_MANTISSA_BITS)
            {
                u32ExpC -= (FP16_MANTISSA_BITS - s32NumSigBits);
                u32SignificandC <<= (FP16_MANTISSA_BITS - s32NumSigBits);
            }

            u32ExpC -= 9U;
            u32ExpC -= FP16_MANTISSA_BITS;

            stResult.sign = u32SignC;
            stResult.exp = u32ExpC;
            stResult.mantissa = (u32SignificandC & 0x3FF);// - (1U << 24U);
        }
        
        return stResult;
    }

    FP16_t mla(const FP16_t stA, const FP16_t stB, const FP16_t stC)
    {
        // a + (b * c)
        return add(stA, mul(stB, stC));
    }
}




