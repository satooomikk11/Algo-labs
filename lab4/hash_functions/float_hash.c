#include "float_hash.h"
#include <string.h>

// преобразование к int
uint32_t hash_float_to_int_bits(float key)
{
    uint32_t bits = 0;
    memcpy(&bits, &key, sizeof(float));
    return bits;
}

// битовые сдвиги
uint32_t hash_float_bitwise(float key)
{
    uint32_t bits = hash_float_to_int_bits(key);
    bits ^= (bits >> 16);
    bits ^= (bits >> 8);
    return bits;
}

// мантисса
uint32_t hash_float_mantissa(float key)
{
    uint32_t bits = hash_float_to_int_bits(key);
    return bits & 0x7FFFFF;  // младшие 23 бита
}

// экспонента 
uint32_t hash_float_exponent(float key)
{
    uint32_t bits = hash_float_to_int_bits(key);
    return (bits >> 23) & 0xFF;  // биты 23-30
}

// произведение мантиссы на экспоненту
uint32_t hash_float_mantissa_exp_product(float key)
{
    uint32_t mant = hash_float_mantissa(key);
    uint32_t exp  = hash_float_exponent(key);
    return mant * (exp + 1);  // +1 чтоб не обнулялось
}