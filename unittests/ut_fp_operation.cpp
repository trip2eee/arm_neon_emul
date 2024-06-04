#include <stdio.h>
#include "unittest.h"
#include "../neon_emul/fp32_operation.h"
#include "../neon_emul/fp16_operation.h"

void test_conversion()
{
    printf("test_conversion\n");
    float32_t f32A = 10.25F;
    float32_t f32B = -1.12345F;

    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    FP32::FP32_t stB = FP32::float32_to_fp32(f32B);

    printf("sizeof float32_t: %d\n", sizeof(stA));

    float32_t a = FP32::fp32_to_float32(stA);
    printf("a = %f\n", a);
    EXPECT_FLOAT_EQ(f32A, a);

    float32_t b = FP32::fp32_to_float32(stB);
    printf("b = %f\n", b);
    EXPECT_FLOAT_EQ(f32B, b);
}

void test_IsNaN()
{
    printf("test_IsNaN\n");
    float32_t f32A = 0.0F / 0.0F;
    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    
    EXPECT_EQ(IsNaN(stA), true);
}

void test_IsInf()
{
    printf("test_IsInf\n");
    float32_t f32A = 1.0F / 0.0F;
    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    
    EXPECT_EQ(IsInf(stA), true);
}

void test_add1()
{
    printf("test_add1\n");
    float32_t f32A = 1000.5F;
    float32_t f32B = 2.5F;

    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    FP32::FP32_t stB = FP32::float32_to_fp32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add2()
{
    printf("test_add2\n");
    float32_t f32A = 1.0F;
    float32_t f32B = 0.75F;

    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    FP32::FP32_t stB = FP32::float32_to_fp32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add3()
{
    printf("test_add3\n");
    float32_t f32A = 1.0F;
    float32_t f32B = -0.75F;

    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    FP32::FP32_t stB = FP32::float32_to_fp32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add4()
{
    printf("test_add4\n");
    float32_t f32A = 10.0F;
    float32_t f32B =  -1.0F;

    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    FP32::FP32_t stB = FP32::float32_to_fp32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add5()
{
    printf("test_add5\n");
    float32_t f32A = 10.0F;
    float32_t f32B = -10.0F;

    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    FP32::FP32_t stB = FP32::float32_to_fp32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}


void test_add6()
{
    printf("test_add6\n");
    float32_t f32A = 3.14159265F;
    float32_t f32B = 1000.0F;

    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    FP32::FP32_t stB = FP32::float32_to_fp32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add7()
{
    printf("test_add7\n");
    float32_t f32A = 3.14159265F;
    float32_t f32B = 1000000.0F;

    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    FP32::FP32_t stB = FP32::float32_to_fp32(f32B);
    FP32::FP32_t stC = FP32::add(stA, stB);
    
    float32_t f32C = FP32::fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}


void test_sub()
{
    printf("test_sub\n");
    float32_t f32A = 10.0F;
    float32_t f32B = 5.0F;

    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    FP32::FP32_t stB = FP32::float32_to_fp32(f32B);
    FP32::FP32_t stC = FP32::sub(stA, stB);
    
    float32_t f32C = FP32::fp32_to_float32(stC);
    printf("%f - %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A - f32B, f32C);
}

void test_mul()
{
    printf("test_mul\n");

    float32_t f32A = 10000.1234F;
    // float32_t f32B = 0.01256789F;
    float32_t f32B = 0.01256789F;

    FP32::FP32_t stA = FP32::float32_to_fp32(f32A);
    FP32::FP32_t stB = FP32::float32_to_fp32(f32B);

    FP32::FP32_t stC = FP32::mul(stA, stB);

    float32_t f32C = FP32::fp32_to_float32(stC);
    printf("%f * %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A * f32B, f32C);
}

void test_conversion_fp16()
{
    printf("test_conversion_fp16\n");
}

void test_add1_fp16()
{
    printf("test_add1_fp16\n");
}

int main()
{
    printf("fp operation\n");

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
    test_mul();

    test_conversion_fp16();
    test_add1_fp16();

    return 0;
}


