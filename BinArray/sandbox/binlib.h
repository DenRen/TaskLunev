#ifndef BINLIB_H
#define BINLIB_H

#include <stdint.h>
#include <stdlib.h>

void print_byte (uint8_t byte);
void print_byte_nl (uint8_t byte);
void bprint (uint8_t* bytes, size_t num_bytes);

#endif /* BINLIB_H */