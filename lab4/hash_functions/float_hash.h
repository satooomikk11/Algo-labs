/**
 * @file float_hash.h
 * @brief Хеш-функции для чисел с плавающей точкой.
 */

#ifndef FLOAT_HASH_H
#define FLOAT_HASH_H

#include <stdint.h>

/**
 * @brief Преобразует float в целое число.
 * 
 * @param key Исходное число с плавающей точкой.
 * @return uint32_t Битовое представление числа как целого.
 */
uint32_t hash_float_to_int_bits(float key);

/**
 * @brief Хеш-функция с перемешиванием битов.
 * 
 * Берёт битовое представление числа и применяет xor со сдвигами
 * для улучшения равномерности распределения.
 * 
 * @param key Исходное число с плавающей точкой.
 * @return uint32_t Перемешанное битовое представление.
 */
uint32_t hash_float_bitwise(float key);

/**
 * @brief Извлекает только мантиссу числа.
 * 
 * @param key Исходное число с плавающей точкой.
 * @return uint32_t Мантисса числа.
 */
uint32_t hash_float_mantissa(float key);

/**
 * @brief Извлекает только экспоненту числа.
 * 
 * @param key Исходное число с плавающей точкой.
 * @return uint32_t Экспонента числа.
 */
uint32_t hash_float_exponent(float key);

/**
 * @brief Произведение мантиссы на экспоненту.
 * 
 * @param key Исходное число с плавающей точкой.
 * @return uint32_t Результат умножения матиссы на экспоненту.
 */
uint32_t hash_float_mantissa_exp_product(float key);

#endif // FLOAT_HASH_H