/**
 * @file linear.h
 * @brief Хеш-таблица с линейным пробированием.
 */

#ifndef LINEAR_H
#define LINEAR_H

#include "common.h"

/**
 * @brief Хеш-таблица с линейным пробированием.
 */
typedef struct
{
    int*  keys;                       ///< Массив ключей
    CellStatus* status;               ///< Массив статусов
    int   size;                       ///< Размер таблицы
    int   count;                      ///< Количество элементов
    float load_factor_threshold;      ///< Порог для рехеширования
} LinearHashTable;

/**
 * @brief Создаёт новую таблицу.
 * 
 * @param initial_size Начальный размер таблицы
 * @param threshold    Порог load factor для рехеширования
 * @return Указатель на созданную таблицу или NULL при ошибке
 */
LinearHashTable* linear_create(int initial_size, float threshold);

/**
 * @brief Вставляет ключ в таблицу.
 * 
 * @param ht  Указатель на таблицу
 * @param key Вставляемый ключ
 */
void linear_insert(LinearHashTable* ht, int key);

/**
 * @brief Ищет ключ в таблице.
 * 
 * @param ht  Указатель на таблицу
 * @param key Искомый ключ
 * @return 1 если ключ найден, 0 если нет
 */
int linear_search(LinearHashTable* ht, int key);

/**
 * @brief Удаляет ключ из таблицы.
 * 
 * @param ht  Указатель на таблицу
 * @param key Удаляемый ключ
 * @return 1 если ключ удалён, 0 если не найден
 */
int linear_delete(LinearHashTable* ht, int key);

/**
 * @brief Уничтожает таблицу и освобождает память.
 * 
 * @param ht Указатель на таблицу
 */
void linear_destroy(LinearHashTable* ht);

#endif // LINEAR_H