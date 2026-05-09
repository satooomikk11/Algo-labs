/**
 * @file cuckoo.h
 * @brief Хеш-таблица с кукушечным хешированием.
 */

#ifndef CUCKOO_H
#define CUCKOO_H

/**
 * @brief Хеш-таблица с кукушечным хешированием.
 */
typedef struct
{
    int*  table1;                     ///< Первая таблица
    int*  table2;                     ///< Вторая таблица
    int   size;                       ///< Размер каждой таблицы
    int   count;                      ///< Общее количество элементов
    float load_factor_threshold;      ///< Порог для рехеширования
} CuckooHashTable;

/**
 * @brief Создаёт новую таблицу.
 * 
 * @param initial_size Начальный размер
 * @param threshold    Порог load factor
 * @return Указатель на таблицу или NULL при ошибке
 */
CuckooHashTable* cuckoo_create(int initial_size, float threshold);

/**
 * @brief Вставляет ключ.
 * 
 * @param ht  Указатель на таблицу
 * @param key Вставляемый ключ
 */
void cuckoo_insert(CuckooHashTable* ht, int key);

/**
 * @brief Ищет ключ в таблице.
 * 
 * @param ht  Указатель на таблицу
 * @param key Искомый ключ
 * @return 1 если найден, 0 если нет
 */
int cuckoo_search(CuckooHashTable* ht, int key);

/**
 * @brief Удаляет ключ из таблицы.
 * 
 * @param ht  Указатель на таблицу
 * @param key Удаляемый ключ
 * @return 1 если удалён, 0 если не найден
 */
int cuckoo_delete(CuckooHashTable* ht, int key);

/**
 * @brief Уничтожает таблицу и освобождает память.
 * 
 * @param ht Указатель на таблицу
 */
void cuckoo_destroy(CuckooHashTable* ht);

#endif // CUCKOO_H