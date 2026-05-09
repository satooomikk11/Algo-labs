/**
 * @file chaining.h
 * @brief Хеш-таблица с разрешением коллизий методом цепочек.
 */

#ifndef CHAINING_H
#define CHAINING_H

/**
 * @brief Узел связного списка.
 */
typedef struct ChainingNode
{
    int key;                       ///< Ключ
    struct ChainingNode* next;     ///< Указатель на следующий узел
} ChainingNode;

/**
 * @brief Хеш-таблица с цепочками.
 */
typedef struct
{
    ChainingNode** buckets;        ///< Массив указателей на списки
    int size;                      ///< Размер таблицы
    int count;                     ///< Количество элементов в таблице
    float load_factor_threshold;   ///< Порог для рехеширования
} ChainingHashTable;

/**
 * @brief Создаёт новую таблицу.
 * 
 * @param initial_size Начальный размер
 * @param threshold    Порог load factor для рехеширования
 * @return Указатель на таблицу или NULL при ошибке
 */
ChainingHashTable* chaining_create(int initial_size, float threshold);

/**
 * @brief Вставляет ключ в таблицу.
 * 
 * @param ht  Указатель на таблицу
 * @param key Вставляемый ключ
 */
void chaining_insert(ChainingHashTable* ht, int key);

/**
 * @brief Ищет ключ в таблице.
 * 
 * @param ht  Указатель на таблицу
 * @param key Искомый ключ
 * @return 1 если ключ найден, 0 если нет
 */
int chaining_search(ChainingHashTable* ht, int key);

/**
 * @brief Удаляет ключ из таблицы.
 * 
 * @param ht  Указатель на таблицу
 * @param key Удаляемый ключ
 * @return 1 если удалён, 0 если не найден
 */
int chaining_delete(ChainingHashTable* ht, int key);

/**
 * @brief Уничтожает таблицу и освобождает память.
 * 
 * @param ht Указатель на таблицу
 */
void chaining_destroy(ChainingHashTable* ht);

/**
 * @brief Возвращает текущий load factor.
 * 
 * @param ht Указатель на таблицу
 * @return load_factor = count / size
 */
float chaining_get_load_factor(ChainingHashTable* ht);

#endif // CHAINING_H