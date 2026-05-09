/**
 * @file string_hash.h
 * @brief Хеш-функции для строк.
 */

#ifndef STRING_HASH_H
#define STRING_HASH_H

#include <stdint.h>

/**
 * @brief Хеш-функция, возвращающая длину строки.
 * 
 * @param s Указатель на нуль-терминированную строку.
 * @return uint32_t Длина строки.
 */
uint32_t hash_string_len(const char* s);

/**
 * @brief Хеш-функция, суммирующая коды символов.
 * 
 * @param s Указатель на нуль-терминированную строку.
 * @return uint32_t Сумма ASCII-кодов символов.
 */
uint32_t hash_string_sum(const char* s);

/**
 * @brief Полиномиальный хеш.
 * 
 * @param s Указатель на нуль-терминированную строку.
 * @return uint32_t Полиномиальный хеш строки.
 */
uint32_t hash_string_polynomial(const char* s);

/**
 * @brief Хеш-функция CRC32.
 * 
 * @param s Указатель на нуль-терминированную строку.
 * @return uint32_t CRC32 хеш строки.
 */
uint32_t hash_string_crc32(const char* s);

#endif // STRING_HASH_H