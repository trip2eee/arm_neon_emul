#include <cmath>

#define EXPECT_EQ(x, y) __test_equal(__FILE__, __LINE__, x, y);
#define EXPECT_FLOAT_EQ(x, y) __test_float_equal(__FILE__, __LINE__, x, y);

bool __test_equal(const char* const file, const int line, const double x, const double y)
{
    bool bEqual;
    const double err = x - y;
    if(fabs(err) <= 0.0) {
        bEqual = true;        
    } else {
        bEqual = false;
        printf("\n%s:%d - FAIL!\n", file, line);
        printf("%f != %f\n\n", x, y);
    }
    return bEqual;
}

bool __test_float_equal(const char* const file, const int line, const double x, const double y, const double eps=1e-6)
{
    bool bEqual;
    const double err = x - y;
    if(fabs(err) <= eps) {
        bEqual = true;
    } else {
        bEqual = false;
        printf("\n%s:%d - FAIL!\n", file, line);
        printf("%f != %f\n\n", x, y);
    }
    return bEqual;
}
