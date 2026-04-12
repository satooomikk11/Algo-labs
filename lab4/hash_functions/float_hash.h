#ifndef FLOAT_HASH_H
#define FLOAT_HASH_H

#include <stdint.h>

uint32_t hash_float_to_int_bits(float key);
uint32_t hash_float_bitwise    (float key);
uint32_t hash_float_mantissa   (float key);
uint32_t hash_float_exponent   (float key);
uint32_t hash_float_mantissa_exp_product(float key);

#endif // FLOAT_HASH_H