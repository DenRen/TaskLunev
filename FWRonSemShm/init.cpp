#include <cstdio>
#include "wrrd.h

int main () {
    const char nameFile[] = "/home/tester/Documents/GitHub/TaskLunev/FWRonSemShm/cmake-build-debug/FWRonSemShm";

    connShm_t connShm = nullptr;
    CHECK_ERROR (InitConnShm (nameFile, &connShm));

    return EXIT_SUCCESS;
}