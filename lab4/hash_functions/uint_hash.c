#include "uint_hash.h"

// остаток от деления в тестах
uint32_t hash_uint_mod(uint32_t key)
{
    return key;
}

// битовое представление
uint32_t hash_uint_bitwise(uint32_t key)
{
    key ^= (key >> 16);
    key ^= (key >> 8);
    key ^= (key >> 4);
    key ^= (key >> 2);
    key ^= (key >> 1);
    return key;
}

// функция Кнута
uint32_t hash_uint_multiply_knuth(uint32_t key)
{
    return (key * 2654435761u) >> 16;
}