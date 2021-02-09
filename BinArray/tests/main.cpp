#include <gtest/gtest.h>
#include "BinArray_Tests.cpp"
// g++ main.cpp -o main_test -lgtest -pthread  

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS ();
}