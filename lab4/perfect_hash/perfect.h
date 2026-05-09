/**
 * @file perfect_hash.h
 * @brief Идеальное хеширование.
 */

#ifndef PERFECT_HASH_H
#define PERFECT_HASH_H

/**
 * @brief Вторичная хеш-таблица для одной корзины.
 */
typedef struct
{
    int* keys;      ///< Массив ключей
    int  size;      ///< Размер вторичной таблицы
} PerfectSecondary;

/**
 * @brief Таблица идеального хеширования.
 */
typedef struct
{
    PerfectSecondary* table;   ///< Массив вторичных таблиц
    int* hash_params;          ///< Параметры для каждой вторичной таблицы
    int size;                  ///< Количество корзин
} PerfectHashTable;

/**
 * @brief Строит таблицу идеального хеширования из массива ключей.
 * 
 * @param keys  Массив ключей
 * @param count Количество ключей
 * @return Указатель на таблицу или NULL при ошибке
 */
PerfectHashTable* perfect_create(int* keys, int count);

/**
 * @brief Выполняет поиск ключа в таблице идеального хеширования.
 * 
 * @param ht  Указатель на таблицу
 * @param key Искомый ключ
 * @return 1 если ключ найден, 0 если нет
 */
int perfect_search(PerfectHashTable* ht, int key);

/**
 * @brief Уничтожает таблицу и освобождает память.
 * 
 * @param ht Указатель на таблицу
 */
void perfect_destroy(PerfectHashTable* ht);

#endif // PERFECT_HASH_H