#include <cstdio>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "AddFunc.h"

int IsSameFile (int fd1, int fd2);

int main () {
    const char firstFileName[]  = "../_link";
    const char secondFileName[] = "../_link2";

    int fd1 = open (firstFileName, O_RDONLY);
    CHECK_ERROR (fd1);
    int fd2 = open (secondFileName, O_RDONLY);
    CHECK_ERROR (fd2);

    int result = IsSameFile (fd1, fd2);
    if (result == 1) {
        printf ("This is THE SAME file\n");
    } else if (result == 0) {
        printf ("They are NOT THE SAME file!\n");
    } else {
        printf ("Check failed\n");
    }

    close (2, fd1, fd2);
}

int IsSameFile (int fd1, int fd2) {
    struct stat fdStat = {0};
    CHECK_ERROR (fstat (fd1, &fdStat));
    int inode = fdStat.st_ino;
    CHECK_ERROR (fstat (fd2, &fdStat));

    return inode == fdStat.st_ino;
}