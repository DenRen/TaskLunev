#include <gtest/gtest.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include "../lib/hpc.h"


bool IsEqual (double first, double second, double eps = 1e-3) {
    return fabs (first - second) < eps;
}

TEST (HPC_Integral, SimpleIntegral) {
    #define TEST_SIMPLE_INTEGRAL(a, b, func, eps, answer)                           \
    do {                                                                            \
        double res = SimpleIntegral (a, b, func);                                   \
        ASSERT_TRUE (IsEqual (res, answer, eps)) << "result: " << res << std::endl; \
    } while (0)

    TEST_SIMPLE_INTEGRAL (0, M_PI,       sin, 1e-3, 2);
    TEST_SIMPLE_INTEGRAL (0, M_PI_2,     sin, 1e-3, 1);
    TEST_SIMPLE_INTEGRAL (0, M_PI_4,     sin, 1e-1, 1 - 1 / sqrt (2));
    TEST_SIMPLE_INTEGRAL (0, M_PI_4 / 2, sin, 1e-3, 2 * sin (M_PI / 16) * sin (M_PI / 16));

    TEST_SIMPLE_INTEGRAL (0, M_PI + M_PI,       sin, 1e-3, 0);
    TEST_SIMPLE_INTEGRAL (0, M_PI + M_PI_2,     sin, 1e-3, 1);
    TEST_SIMPLE_INTEGRAL (0, M_PI + M_PI_4,     sin, 1e-1, 1 + 1 / sqrt (2));
    TEST_SIMPLE_INTEGRAL (0, M_PI + M_PI_4 / 2, sin, 1e-3, 2 *(1 - sin (M_PI / 16) * sin (M_PI / 16)));

    TEST_SIMPLE_INTEGRAL (M_PI, M_PI,       sin, 1e-3,  0);
    TEST_SIMPLE_INTEGRAL (M_PI, M_PI_2,     sin, 1e-3, -1);
    TEST_SIMPLE_INTEGRAL (M_PI, M_PI_4,     sin, 1e-1, -1 - 1 / sqrt (2));
    TEST_SIMPLE_INTEGRAL (M_PI, M_PI_4 / 2, sin, 1e-3, -2 * (1 - sin (M_PI / 16) * sin (M_PI / 16)));
}