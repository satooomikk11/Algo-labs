/**
 * @file uint_hash.h
 * @brief Хеш-функции для целых чисел.
 */

#ifndef UINT_HASH_H
#define UINT_HASH_H

#include <stdint.h>

/**
 * @brief Остаток от деления.
 * 
 * @param key Исходное число.
 * @return key (остаток берётся в тесте).
 */
uint32_t hash_uint_mod(uint32_t key);

/**
 * @brief Перемешивание битов с помощью XOR и сдвигов.
 * 
 * @param key Исходное число.
 * @return Перемешанное значение.
 */
uint32_t hash_uint_bitwise(uint32_t key);

/**
 * @brief Метод умножения Кнута.
 * 
 * @param key Исходное число.
 * @return (key * 2654435761u) >> 16
 */
uint32_t hash_uint_multiply_knuth(uint32_t key);

#endif // UINT_HASH_H