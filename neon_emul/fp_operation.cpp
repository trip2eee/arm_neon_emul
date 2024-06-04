#include "fp_operation.h"
#include <stdio.h>
#include <string.h>

const uint32_t FP32_FRACTION_BITS = 23U;
const uint32_t FP32_EXP_BIAS = 127U;

namespace FPOP
{
    float32_t FP32_to_float32(const FP32_t& stFP32)
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

    FP32_t float32_to_FP32(const float32_t f32Val)
    {
        FP32_t stFP32;
        (void)memcpy(&stFP32, &f32Val, sizeof(f32Val));

        return stFP32;
    }

    FP32_t add_FP32(const FP32_t a, const FP32_t b)
    {
        const uint32_t u32SignA = a.sign;
        const uint32_t u32SignB = b.sign;

        uint32_t u32ExpA = a.exp;
        uint32_t u32ExpB = b.exp;
        
        // Prepend implicit 1 to mantissas
        // (1 << 23U) = 0x800000
        uint32_t u32SignificandA = (1U << 23U) + a.fraction;
        uint32_t u32SignificandB = (1U << 23U) + b.fraction;

        while(u32ExpA > u32ExpB)
        {
            u32ExpB ++;
            u32SignificandB >>= 1U;
        }

        while(u32ExpA < u32ExpB)
        {
            u32ExpA ++;
            u32SignificandA >>= 1U;
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

        // Normalize
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

    FP32_t sub_fp32(const FP32_t a, const FP32_t b)
    {
        FP32_t stResult;

        return stResult;
    }
}


// int main()
// {
//     printf("fp operation\n");



//     return 0;
// }


