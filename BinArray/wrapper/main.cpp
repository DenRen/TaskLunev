#include <iostream>
#include "BinArray.hpp"

int main () {
    ba::binarray arr {5}, ne_arr {6};

    (arr[3] = 1) = 0;

    (arr.operator[] (3)).operator= (1);
}