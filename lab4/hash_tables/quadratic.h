/**
 * @file quadratic.h
 * @brief Хеш-таблица с квадратичным пробированием.
 */

#ifndef QUADRATIC_H
#define QUADRATIC_H

#include "common.h"

/**
 * @brief Хеш-таблица с квадратичным пробированием.
 */
typedef struct
{
    int*  keys;                       ///< Массив ключей
    CellStatus* status;               ///< Массив статусов ячеек
    int   size;                       ///< Размер таблицы
    int   count;                      ///< Количество элементов
    float load_factor_threshold;      ///< Порог для рехеширования
} QuadraticHashTable;

/**
 * @brief Создаёт новую таблицу.
 * 
 * @param initial_size Начальный размер таблицы
 * @param threshold    Порог load factor для рехеширования
 * @return Указатель на созданную таблицу или NULL при ошибке
 */
QuadraticHashTable* quadratic_create(int initial_size, float threshold);

/**
 * @brief Вставляет ключ в таблицу.
 * 
 * @param ht  Указатель на таблицу
 * @param key Вставляемый ключ
 */
void quadratic_insert(QuadraticHashTable* ht, int key);

/**
 * @brief Ищет ключ в таблице.
 * 
 * @param ht  Указатель на таблицу
 * @param key Искомый ключ
 * @return 1 если ключ найден, 0 если нет
 */
int quadratic_search(QuadraticHashTable* ht, int key);

/**
 * @brief Удаляет ключ из таблицы.
 * 
 * @param ht  Указатель на таблицу
 * @param key Удаляемый ключ
 * @return 1 если ключ удалён, 0 если не найден
 */
int quadratic_delete(QuadraticHashTable* ht, int key);

/**
 * @brief Уничтожает таблицу и освобождает память.
 * 
 * @param ht Указатель на таблицу
 */
void quadratic_destroy(QuadraticHashTable* ht);

#endif // QUADRATIC_H