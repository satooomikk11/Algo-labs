/**
 * @file double_hash.h
 * @brief Хеш-таблица с двойным хешированием.
 */

#ifndef DOUBLE_HASH_H
#define DOUBLE_HASH_H

#include "common.h"

/**
 * @brief Хеш-таблица с двойным хешированием.
 */
typedef struct
{
    int*  keys;                       ///< Массив ключей
    CellStatus* status;               ///< Массив статусов
    int   size;                       ///< Размер таблицы
    int   count;                      ///< Количество элементов
    float load_factor_threshold;      ///< Порог для рехеширования
} DoubleHashTable;

/**
 * @brief Создаёт новую таблицу.
 * 
 * @param initial_size Начальный размер
 * @param threshold    Порог load factor
 * @return Указатель на таблицу или NULL при ошибке
 */
DoubleHashTable* double_create(int initial_size, float threshold);

/**
 * @brief Вставляет ключ в таблицу.
 * 
 * @param ht  Указатель на таблицу
 * @param key Вставляемый ключ
 */
void double_insert(DoubleHashTable* ht, int key);

/**
 * @brief Ищет ключ в таблице.
 * 
 * @param ht  Указатель на таблицу
 * @param key Искомый ключ
 * @return 1 если найден, 0 если нет
 */
int double_search(DoubleHashTable* ht, int key);

/**
 * @brief Удаляет ключ из таблицы.
 * 
 * @param ht  Указатель на таблицу
 * @param key Удаляемый ключ
 * @return 1 если удалён, 0 если не найден
 */
int double_delete(DoubleHashTable* ht, int key);

/**
 * @brief Уничтожает таблицу и освобождает память.
 * 
 * @param ht Указатель на таблицу
 */
void double_destroy(DoubleHashTable* ht);

#endif // DOUBLE_HASH_H