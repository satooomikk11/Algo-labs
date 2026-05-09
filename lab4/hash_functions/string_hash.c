#include "string_hash.h"
#include <string.h>

// длина строки
uint32_t hash_string_len(const char* s)
{
    return strlen(s);
}

// сумма букв
uint32_t hash_string_sum(const char* s)
{
    uint32_t sum = 0; 
    while (*s)
    {
        sum += (unsigned char)(*s);
        s++;
    }
    return sum;
}

// полиномиальный хеш (p = 131)
uint32_t hash_string_polynomial(const char* s)
{
    uint32_t hash = 0;
    const uint32_t p = 131;
    while (*s)
    {
        hash = hash * p + (unsigned char)(*s);
        s++;
    }
    return hash;
}

// crc32
uint32_t hash_string_crc32(const char* s)
{
    uint32_t crc = 0xFFFFFFFF;
    while (*s)
    {
        crc ^= (unsigned char)(*s);
        for (int i = 0; i < 8; i++)
        {
            crc = (crc >> 1) ^ ((crc & 1) ? 0xEDB88320 : 0);
        }
        s++;
    }
    return ~crc;
}