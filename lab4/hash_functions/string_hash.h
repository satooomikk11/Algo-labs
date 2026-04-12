#ifndef STRING_HASH_H
#define STRING_HASH_H

#include <stdint.h>

uint32_t hash_string_len       (const char* s);
uint32_t hash_string_sum       (const char* s);
uint32_t hash_string_polynomial(const char* s);
uint32_t hash_string_crc32     (const char* s);

#endif // STRING_HASH_H