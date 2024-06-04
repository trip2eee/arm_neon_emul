#include <stdio.h>
#include <cmath>
#include "../neon_emul/fp_operation.h"

using namespace FPOP;

#define EXPECT_EQ(x, y) __test_equal(__FILE__, __LINE__, x, y);
#define EXPECT_FLOAT_EQ(x, y) __test_float_equal(__FILE__, __LINE__, x, y);

bool __test_equal(const char* const file, const int line, const float64_t x, const float64_t y)
{
    bool bEqual;
    const float32_t err = x - y;
    if(abs(err) <= 0.0)
    {
        bEqual = true;        
    } else {
        bEqual = false;
        printf("%s:%d - FAIL!\n", file, line);
    }
    return bEqual;
}

bool __test_float_equal(const char* const file, const int line, const float64_t x, const float64_t y, const float64_t eps=1e-6)
{
    bool bEqual;
    const float32_t err = x - y;
    if(abs(err) <= eps)
    {
        bEqual = true;        
    } else {
        bEqual = false;
        printf("%s:%d - FAIL!\n", file, line);
    }
    return bEqual;
}

void test_conversion()
{
    printf("test_conversion\n");
    float32_t f32A = 10.25F;
    float32_t f32B = -1.12345F;

    FPOP::FP32_t stA = FPOP::float32_to_fp32(f32A);
    FPOP::FP32_t stB = FPOP::float32_to_fp32(f32B);

    printf("sizeof float32_t: %d\n", sizeof(stA));

    float32_t a = fp32_to_float32(stA);
    printf("a = %f\n", a);
    EXPECT_FLOAT_EQ(f32A, a);

    float32_t b = fp32_to_float32(stB);
    printf("b = %f\n", b);
    EXPECT_FLOAT_EQ(f32B, b);
}

void test_IsNaN()
{
    printf("test_IsNaN\n");
    float32_t f32A = 0.0F / 0.0F;
    FP32_t stA = float32_to_fp32(f32A);
    
    EXPECT_EQ(IsNaN(stA), true);
}

void test_IsInf()
{
    printf("test_IsInf\n");
    float32_t f32A = 1.0F / 0.0F;
    FP32_t stA = float32_to_fp32(f32A);
    
    EXPECT_EQ(IsInf(stA), true);
}

void test_add1()
{
    printf("test_add1\n");
    float32_t f32A = 1000.5F;
    float32_t f32B = 2.5F;

    FP32_t stA = float32_to_fp32(f32A);
    FP32_t stB = float32_to_fp32(f32B);
    FP32_t stC = add_fp32(stA, stB);
    
    float32_t f32C = fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add2()
{
    printf("test_add2\n");
    float32_t f32A = 1.0F;
    float32_t f32B = 0.75F;

    FP32_t stA = float32_to_fp32(f32A);
    FP32_t stB = float32_to_fp32(f32B);
    FP32_t stC = add_fp32(stA, stB);
    
    float32_t f32C = fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add3()
{
    printf("test_add3\n");
    float32_t f32A = 1.0F;
    float32_t f32B = -0.75F;

    FP32_t stA = float32_to_fp32(f32A);
    FP32_t stB = float32_to_fp32(f32B);
    FP32_t stC = add_fp32(stA, stB);
    
    float32_t f32C = fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add4()
{
    printf("test_add4\n");
    float32_t f32A = 10.0F;
    float32_t f32B =  -1.0F;

    FP32_t stA = float32_to_fp32(f32A);
    FP32_t stB = float32_to_fp32(f32B);
    FP32_t stC = add_fp32(stA, stB);
    
    float32_t f32C = fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_add5()
{
    printf("test_add5\n");
    float32_t f32A = 10.0F;
    float32_t f32B = -10.0F;

    FP32_t stA = float32_to_fp32(f32A);
    FP32_t stB = float32_to_fp32(f32B);
    FP32_t stC = add_fp32(stA, stB);
    
    float32_t f32C = fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A + f32B, f32C);
}

void test_sub()
{
    printf("test_add5\n");
    float32_t f32A = 10.0F;
    float32_t f32B = 5.0F;

    FP32_t stA = float32_to_fp32(f32A);
    FP32_t stB = float32_to_fp32(f32B);
    FP32_t stC = sub_fp32(stA, stB);
    
    float32_t f32C = fp32_to_float32(stC);
    printf("%f + %f = %f\n", f32A, f32B, f32C);

    EXPECT_FLOAT_EQ(f32A - f32B, f32C);
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
    test_sub();

    return 0;
}


