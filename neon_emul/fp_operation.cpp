#include "fp_operation.h"
#include <stdio.h>
#include <string.h>

const uint32_t FP32_FRACTION_BITS = 23U;
const uint32_t FP32_EXP_BIAS = 127U;

namespace FPOP
{
    float32_t fp32_to_float32(const FP32_t& stFP32)
    {       
        float32_t f32Value = 1.0F;

        float32_t f32F = 1.0F;
        for(uint32_t u32IdxF = 0U; u32IdxF < FP32_FRACTION_BITS; u32IdxF++)
        {
            f32F *= 0.5F;
            if((stFP32.fraction >> (FP32_FRACTION_BITS - 1U - u32IdxF) & 0x01U) == 1U)
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
        if((255U == stFP32.exp) && (0U != stFP32.fraction))
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
        if((255U == stFP32.exp) && (0U == stFP32.fraction))
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
        uint32_t u32SignificandA = (1U << 23U) + stA.fraction;
        uint32_t u32SignificandB = (1U << 23U) + stB.fraction;

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
        int32_t s32NumSig = 31;
        while(((u32SignificandC >> s32NumSig) == 0) && (s32NumSig >= 0))
        {
            s32NumSig--;
        }

        if(s32NumSig > FP32_FRACTION_BITS)
        {
            u32ExpC += (s32NumSig - FP32_FRACTION_BITS);
            u32SignificandC >>= (s32NumSig - FP32_FRACTION_BITS);
        }
        else if(s32NumSig < FP32_FRACTION_BITS)
        {
            u32ExpC -= (FP32_FRACTION_BITS - s32NumSig);
            u32SignificandC <<= (FP32_FRACTION_BITS - s32NumSig);
        }

        FP32_t stResult;
        stResult.sign = u32SignC;
        stResult.exp = u32ExpC;
        stResult.fraction = (u32SignificandC & 0x7FFFFF);// - (1U << 24U);

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

    }
}


// int main()
// {
//     printf("fp operation\n");



//     return 0;
// }


