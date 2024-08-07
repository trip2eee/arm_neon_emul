#include <stdio.h>
#include "unittest.h"
#include "../neon_emul/fp32_operation.h"
#include "../neon_emul/fp16_operation.h"
#include <arm_neon.h>

void test_conversion()
{
    printf("test_conversion\n");
    float32_t f32A = 10.25F;
    float32_t f32B = -1.12345F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);

    printf("    sizeof float32_t: %ld\n", sizeof(stA));

    float32_t a = FP32::to_float32(stA);
    printf("    a = %f\n", a);
    EXPECT_FLOAT_EQ(f32A, a);

    float32_t b = FP32::to_float32(stB);
    printf("    b = %f\n", b);
    EXPECT_FLOAT_EQ(f32B, b);
}

void test_IsNaN()
{
    printf("test_IsNaN\n");
    float32_t f32A = 0.0F / 0.0F;
    FP32::FP32_t stA = FP32::from_float32(f32A);
    
    EXPECT_EQ(IsNaN(stA), true);
}

void test_IsInf()
{
    printf("test_IsInf\n");
    float32_t f32A = 1.0F / 0.0F;
    FP32::FP32_t stA = FP32::from_float32(f32A);
    
    EXPECT_EQ(IsInf(stA), true);
}

void test_add1()
{
    printf("test_add1\n");
    float32_t f32A = 1000.5F;
    float32_t f32B = 2.5F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::to_float32(stC);
    printf("    %f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add2()
{
    printf("test_add2\n");
    float32_t f32A = 1.0F;
    float32_t f32B = 0.75F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = to_float32(stC);
    printf("    %f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add3()
{
    printf("test_add3\n");
    float32_t f32A = 1.0F;
    float32_t f32B = -0.75F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::to_float32(stC);
    printf("    %f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add4()
{
    printf("test_add4\n");
    float32_t f32A = 10.0F;
    float32_t f32B =  -1.0F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::to_float32(stC);
    printf("    %f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add5()
{
    printf("test_add5\n");
    float32_t f32A = 10.0F;
    float32_t f32B = -10.0F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::to_float32(stC);
    printf("    %f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}


void test_add6()
{
    printf("test_add6\n");
    float32_t f32A = 3.14159265F;
    float32_t f32B = 3000000.0F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::to_float32(stC);
    printf("    %f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add7()
{
    printf("test_add7\n");
    float32_t f32A = 3.14159265F;
    float32_t f32B = 1000000.0F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::to_float32(stC);
    printf("    %f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}


void test_sub()
{
    printf("test_sub\n");
    float32_t f32A = 5.0F;
    float32_t f32B = 5.0F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);
    FP32::FP32_t stC = FP32::sub(stA, stB);
    
    float32_t f32C = FP32::to_float32(stC);
    printf("    %f - %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A - f32B, f32C);
}

void test_mul1()
{
    printf("test_mul1\n");
    float32_t f32A = 10000.1234F;
    float32_t f32B = 0.01256789F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);

    FP32::FP32_t stC = FP32::mul(stA, stB);

    float32_t f32C = FP32::to_float32(stC);
    printf("    %f * %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A * f32B, f32C);
}

void test_mul2()
{
    printf("test_mul2\n");
    float32_t f32A = -0.0F;
    float32_t f32B = 0.01256789F;

    FP32::FP32_t stA = FP32::from_float32(f32A);
    FP32::FP32_t stB = FP32::from_float32(f32B);

    FP32::FP32_t stC = FP32::mul(stA, stB);

    float32_t f32C = FP32::to_float32(stC);
    printf("    %f * %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A * f32B, f32C);
}


void test_conversion_fp16()
{
    printf("test_conversion_fp16\n");

    // compute test input and expected output.
    float32_t arf32A[4U] = {10.25F, -1.1234F, 1.1234F, 3.14F};
    float32_t arf32B[4U];
    float32x4_t vf32A = vld1q_f32(arf32A);
    float16x4_t vf16A = vcvt_f16_f32(vf32A);
    float32x4_t vf32B = vcvt_f32_f16(vf16A);
    vst1q_f32(arf32B, vf32B);

    FP16::FP16_t stA;
    printf("    sizeof float16_t: %ld\n", sizeof(stA));
    printf("    FP32,     NEON(FP16) EMUL(FP16)\n");

    for(int i = 0; i < 4; i++)
    {
        stA = FP16::from_float32(arf32A[i]);
        float32_t f32B = FP16::to_float32(stA);

        printf("    %f, %f, %f\n", arf32A[i], arf32B[i], f32B);
        EXPECT_FLOAT_EQ(arf32B[i], f32B);
    }
}

void test_IsNaN_fp16()
{
    printf("test_IsNaN_fp16\n");
    float32_t f32A = 0.0F / 0.0F;
    FP16::FP16_t stA = FP16::from_float32(f32A);
    
    EXPECT_EQ(FP16::IsNaN(stA), true);
}

void test_IsInf_fp16()
{
    printf("test_IsInf_fp16\n");
    float32_t f32A = 1.0F / 0.0F;
    FP16::FP16_t stA = FP16::from_float32(f32A);
    
    EXPECT_EQ(FP16::IsInf(stA), true);
}

void test_IsZero_fp16()
{
    printf("test_IsZero_fp16\n");

    float32_t f32A = 0.0F;
    FP16::FP16_t stA = FP16::from_float32(f32A);
    EXPECT_EQ(FP16::IsZero(stA), true);
}

void test_add1_fp16()
{
    printf("test_add1_fp16\n");

    float32_t arf32A[4U] = {10.25F, -1.1234F, 1.1234F, 3.14F};
    float32_t arf32B[4U] = {-10.25F, 2.5F, 1000.0F, 13.14F};
    float32_t arf32C[4U];

    float32x4_t vf32A = vld1q_f32(arf32A);
    float32x4_t vf32B = vld1q_f32(arf32B);

    float16x4_t vf16A = vcvt_f16_f32(vf32A);
    float16x4_t vf16B = vcvt_f16_f32(vf32B);
    float16x4_t vf16C = vadd_f16(vf16A, vf16B);

    float32x4_t vf32C = vcvt_f32_f16(vf16C);
    vst1q_f32(arf32C, vf32C);
    
    for(int i = 0; i < 4; i++)
    {
        FP16::FP16_t stA = FP16::from_float32(arf32A[i]);
        FP16::FP16_t stB = FP16::from_float32(arf32B[i]);
        FP16::FP16_t stC = FP16::add(stA, stB);
        float32_t f32C = FP16::to_float32(stC);

        printf("    %f + %f = %f, %f\n", arf32A[i], arf32B[i], arf32C[i], f32C);

        EXPECT_FLOAT_EQ(arf32C[i], f32C);
    }
}

void test_sub_fp16()
{
    printf("test_sub_fp16\n");
    
    float32_t arf32A[4U] = {10.25F, -1.1234F, 1.1234F, 3.14F};
    float32_t arf32B[4U] = {-10.25F, 2.5F, 1000.0F, 13.141592F};
    float32_t arf32C[4U];

    float32x4_t vf32A = vld1q_f32(arf32A);
    float32x4_t vf32B = vld1q_f32(arf32B);

    float16x4_t vf16A = vcvt_f16_f32(vf32A);
    float16x4_t vf16B = vcvt_f16_f32(vf32B);
    float16x4_t vf16C = vsub_f16(vf16A, vf16B);

    float32x4_t vf32C = vcvt_f32_f16(vf16C);
    vst1q_f32(arf32C, vf32C);
    
    for(int i = 0; i < 4; i++)
    {
        FP16::FP16_t stA = FP16::from_float32(arf32A[i]);
        FP16::FP16_t stB = FP16::from_float32(arf32B[i]);
        FP16::FP16_t stC = FP16::sub(stA, stB);
        float32_t f32C = FP16::to_float32(stC);

        printf("    %f - %f = %f, %f\n", arf32A[i], arf32B[i], arf32C[i], f32C);

        EXPECT_FLOAT_EQ(arf32C[i], f32C);
    }
}

void test_mul_fp16()
{
    printf("test_mul_fp16\n");

    float32_t arf32A[4U] = {0.0F, -1.1234F, 1.1234F, 3.14F};
    float32_t arf32B[4U] = {10.25F, 0.0F, 1000.0F, 13.14F};
    float32_t arf32C[4U];

    float32x4_t vf32A = vld1q_f32(arf32A);
    float32x4_t vf32B = vld1q_f32(arf32B);

    float16x4_t vf16A = vcvt_f16_f32(vf32A);
    float16x4_t vf16B = vcvt_f16_f32(vf32B);
    float16x4_t vf16C = vmul_f16(vf16A, vf16B);

    float32x4_t vf32C = vcvt_f32_f16(vf16C);
    vst1q_f32(arf32C, vf32C);
    
    for(int i = 0; i < 4; i++)
    {
        FP16::FP16_t stA = FP16::from_float32(arf32A[i]);
        FP16::FP16_t stB = FP16::from_float32(arf32B[i]);
        FP16::FP16_t stC = FP16::mul(stA, stB);
        float32_t f32C = FP16::to_float32(stC);

        printf("    %f * %f = %f, %f\n", arf32A[i], arf32B[i], arf32C[i], f32C);

        EXPECT_FLOAT_EQ(arf32C[i], f32C);
    }
}

void test_mul_add_fp16()
{
    printf("test_mul_add_fp16\n");

    float32_t arf32A[4U] = {0.0F, -1.1234F, 1.4321F, 3.14159265F};
    float32_t arf32B[4U] = {10.25F, 0.0F, 1000.0F, 13.145F};
    float32_t arf32C[4U] = {1.0F, 2.0F, -3.12F, 4.0F};
    float32_t arf32D[4U];

    float32x4_t vf32A = vld1q_f32(arf32A);
    float32x4_t vf32B = vld1q_f32(arf32B);
    float32x4_t vf32C = vld1q_f32(arf32C);

    float16x4_t vf16A = vcvt_f16_f32(vf32A);
    float16x4_t vf16B = vcvt_f16_f32(vf32B);
    float16x4_t vf16C = vcvt_f16_f32(vf32C);
    float16x4_t vf16D = vfma_f16(vf16A, vf16B, vf16C);

    float32x4_t vf32D = vcvt_f32_f16(vf16D);
    vst1q_f32(arf32D, vf32D);
    
    for(int i = 0; i < 4; i++)
    {
        FP16::FP16_t stA = FP16::from_float32(arf32A[i]);
        FP16::FP16_t stB = FP16::from_float32(arf32B[i]);
        FP16::FP16_t stC = FP16::from_float32(arf32C[i]);

        FP16::FP16_t stD = FP16::mla(stA, stB, stC);
        float32_t f32D = FP16::to_float32(stD);

        printf("    %f + (%f * %f) = %f, %f\n", arf32A[i], arf32B[i], arf32C[i], arf32D[i], f32D);

        EXPECT_FLOAT_EQ(arf32D[i], f32D);        
    } 
}

int main()
{
    printf("FP operation\n");

    test_conversion();
    test_IsNaN();
    test_IsInf();
    test_add1();
    test_add2();
    test_add3();
    test_add4();
    test_add5();
    test_add6();
    test_add7();
    test_sub();
    test_mul1();
    test_mul2();

    test_conversion_fp16();
    test_IsNaN_fp16();
    test_IsInf_fp16();
    test_IsZero_fp16();
    test_add1_fp16();
    test_sub_fp16();
    test_mul_fp16();
    test_mul_add_fp16();

    return 0;
}


