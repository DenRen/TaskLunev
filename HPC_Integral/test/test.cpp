#include <gtest/gtest.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include "../lib/hpc.h"
#include "../lib/cpu_topology.h"
#include "../lib/cpu_topology_dev.h"

/*
TODO: тесты GETNumebrCPU
*/

bool IsEqual (double first, double second, double eps = 1e-3) {
    return fabs (first - second) < eps;
}

TEST (CPU_TOPOLOGY, _parseString2SumNumber) {

    #define TEST_PARSER_STR_2_SUM_NUM(str, answer)       \
    do {                                                 \
        char buf[128] = str;                             \
        ASSERT_EQ (_parseString2SumNumber (buf), answer);\
    } while (0)

    TEST_PARSER_STR_2_SUM_NUM ("0", 1);
    TEST_PARSER_STR_2_SUM_NUM ("1", 1);
    TEST_PARSER_STR_2_SUM_NUM ("2", 1);
    TEST_PARSER_STR_2_SUM_NUM ("0-1", 2);
    TEST_PARSER_STR_2_SUM_NUM ("3-15", 13);
    TEST_PARSER_STR_2_SUM_NUM ("3,4-5", 3);
    TEST_PARSER_STR_2_SUM_NUM ("1,2,3,5-9,13,14-19", 15);
}

TEST (HPC_Integral, SimpleIntegral) {
    #define TEST_SIMPLE_INTEGRAL(a, b, func, answer)                            \
    do {                                                                        \
        double res = SimpleIntegral (a, b, func);                               \
        ASSERT_TRUE (IsEqual (res, answer)) << "result: " << res << std::endl;  \
    } while (0)

    TEST_SIMPLE_INTEGRAL (0, M_PI,       sin, 2);
    TEST_SIMPLE_INTEGRAL (0, M_PI_2,     sin, 1);
    TEST_SIMPLE_INTEGRAL (0, M_PI_4,     sin, 1 - 1 / sqrt (2));
    TEST_SIMPLE_INTEGRAL (0, M_PI_4 / 2, sin, 2 * sin (M_PI / 16) * sin (M_PI / 16));

    TEST_SIMPLE_INTEGRAL (0, M_PI + M_PI,       sin, 0);
    TEST_SIMPLE_INTEGRAL (0, M_PI + M_PI_2,     sin, 1);
    TEST_SIMPLE_INTEGRAL (0, M_PI + M_PI_4,     sin, 1 + 1 / sqrt (2));
    TEST_SIMPLE_INTEGRAL (0, M_PI + M_PI_4 / 2, sin, 2 *(1 - sin (M_PI / 16) * sin (M_PI / 16)));

    TEST_SIMPLE_INTEGRAL (M_PI, M_PI,       sin,  0);
    TEST_SIMPLE_INTEGRAL (M_PI, M_PI_2,     sin, -1);
    TEST_SIMPLE_INTEGRAL (M_PI, M_PI_4,     sin, -1 - 1 / sqrt (2));
    TEST_SIMPLE_INTEGRAL (M_PI, M_PI_4 / 2, sin, -2 * (1 - sin (M_PI / 16) * sin (M_PI / 16)));

    TEST_SIMPLE_INTEGRAL (M_PI, M_PI + M_PI,       sin, -2);
    TEST_SIMPLE_INTEGRAL (M_PI, M_PI + M_PI_2,     sin, -1);
    TEST_SIMPLE_INTEGRAL (M_PI, M_PI + M_PI_4,     sin, -1 + 1 / sqrt (2));
    TEST_SIMPLE_INTEGRAL (M_PI, M_PI + M_PI_4 / 2, sin, -2 * sin (M_PI / 16) * sin (M_PI / 16));

    #undef TEST_SIMPLE_INTEGRAL
}

TEST (HPC_Integral, Integral) {
    #define TEST_INTEGRAL(num_threads, a, b, func, answer)                          \
    do {                                                                            \
        double res = Integral (a, b, func, num_threads);                            \
        ASSERT_TRUE (IsEqual (res, answer))                                         \
            << "result: " << res << "\nnum_threads: " << num_threads << std::endl; \
    } while (0)

    const int max_num_threads = 10;
    for (int i = 1; i < max_num_threads; ++i) {
        TEST_INTEGRAL (i, 0, M_PI,       sin, 2);
        TEST_INTEGRAL (i, 0, M_PI_2,     sin, 1);
        TEST_INTEGRAL (i, 0, M_PI_4,     sin, 1 - 1 / sqrt (2));
        TEST_INTEGRAL (i, 0, M_PI_4 / 2, sin, 2 * sin (M_PI / 16) * sin (M_PI / 16));

        TEST_INTEGRAL (i, 0, M_PI + M_PI,       sin, 0);
        TEST_INTEGRAL (i, 0, M_PI + M_PI_2,     sin, 1);
        TEST_INTEGRAL (i, 0, M_PI + M_PI_4,     sin, 1 + 1 / sqrt (2));
        TEST_INTEGRAL (i, 0, M_PI + M_PI_4 / 2, sin, 2 *(1 - sin (M_PI / 16) * sin (M_PI / 16)));

        TEST_INTEGRAL (i, M_PI, M_PI,       sin,  0);
        TEST_INTEGRAL (i, M_PI, M_PI_2,     sin, -1);
        TEST_INTEGRAL (i, M_PI, M_PI_4,     sin, -1 - 1 / sqrt (2));
        TEST_INTEGRAL (i, M_PI, M_PI_4 / 2, sin, -2 * (1 - sin (M_PI / 16) * sin (M_PI / 16)));

        TEST_INTEGRAL (i, M_PI, M_PI + M_PI,       sin, -2);
        TEST_INTEGRAL (i, M_PI, M_PI + M_PI_2,     sin, -1);
        TEST_INTEGRAL (i, M_PI, M_PI + M_PI_4,     sin, -1 + 1 / sqrt (2));
        TEST_INTEGRAL (i, M_PI, M_PI + M_PI_4 / 2, sin, -2 * sin (M_PI / 16) * sin (M_PI / 16));
    }
    #undef TEST_INTEGRAL
}