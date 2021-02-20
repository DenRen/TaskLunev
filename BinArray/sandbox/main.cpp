#include "Tutorials.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "binlib.h"

#include <string.h>
#include <sstream>
#include <iostream>
#include <unistd.h>

int main () {
    //Tutorial_BinShift_Casting ();
    //Tutorial_BinShift ();

    fflush (stdout);
    FILE* save_stdout = stdout;
    const char nameTempFile[] = "test_Secondary_Functions.tmp"; 
    stdout = (FILE*) fopen (nameTempFile, "w+");
    if (stdout == nullptr) {
        perror ("fopen");
        return -1;
    }
    
    char buf[128] = "";

    uint8_t byte = 0b00000000;
    print_byte (byte);
    fflush (stdout);

    fseek (stdout, 0, SEEK_SET);
    fread (buf, sizeof (char), 8, stdout);

    fclose (stdout);
    remove (nameTempFile);

    stdout = save_stdout;

    printf ("a: %s\n", buf);
    printf ("cmp: %\n", strncmp (buf, "00000000", 8));

}