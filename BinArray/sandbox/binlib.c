#include "binlib.h"

#include <stdio.h>

void print_byte (uint8_t byte) {
    char str[9] = "";
    for (uint8_t i = 0; i < 8; ++i)
        str[i] = '0' + ((byte & ((1U << 7) >> i)) != 0);

    str[8] = '\0';
    printf ("%s", str);
}

void print_byte_nl (uint8_t byte) {
    print_byte (byte);
    printf ("\n");
}

void bprint (uint8_t* bytes, size_t num_bytes) {
    for (int i = 0; i < num_bytes; ++i) {
        print_byte (bytes[i]);
        
        if (i % 8 == 0)
            printf (" ");
    }
}