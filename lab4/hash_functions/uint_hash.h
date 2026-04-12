#ifndef UINT_HASH_H
#define UINT_HASH_H

#include <stdint.h>

uint32_t hash_uint_mod           (uint32_t key);
uint32_t hash_uint_bitwise       (uint32_t key);
uint32_t hash_uint_multiply_knuth(uint32_t key);

#endif // UINT_HASH_H