#include "float_hash.h"
#include <string.h>

// для битовых операций с float
#define FLOAT_MANTISSA_MASK  0x7FFFFF   // 23 бита мантиссы
#define FLOAT_EXPONENT_SHIFT 23
#define FLOAT_EXPONENT_MASK  0xFF

// для битового перемешивания
#define BITWISE_SHIFT_16 16
#define BITWISE_SHIFT_8  8

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
    bits ^= (bits >> BITWISE_SHIFT_16);
    bits ^= (bits >> BITWISE_SHIFT_8);
    return bits;
}

// мантисса
uint32_t hash_float_mantissa(float key)
{
    uint32_t bits = hash_float_to_int_bits(key);
    return bits & FLOAT_MANTISSA_MASK;  // младшие 23 бита
}

// экспонента 
uint32_t hash_float_exponent(float key)
{
    uint32_t bits = hash_float_to_int_bits(key);
    return (bits >> FLOAT_EXPONENT_SHIFT) & FLOAT_EXPONENT_MASK;  // биты 23-30
}

// произведение мантиссы на экспоненту
uint32_t hash_float_mantissa_exp_product(float key)
{
    uint32_t mant = hash_float_mantissa(key);
    uint32_t exp  = hash_float_exponent(key);
    return mant * (exp + 1);  // +1 чтоб не обнулялось
}