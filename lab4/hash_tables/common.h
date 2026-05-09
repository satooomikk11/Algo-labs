/**
 * @file common.h
 * @brief Общие определения для хеш-таблиц.
 */

#ifndef COMMON_H
#define COMMON_H

/**
 * @brief Статус ячейки таблицы.
 */
typedef enum
{
    CELL_EMPTY    =  0,  ///< Ячейка пуста
    CELL_OCCUPIED =  1,  ///< Ячейка занята
    CELL_DELETED  = -1   ///< Ячейка помечена как удалённая
} CellStatus;

#endif // COMMON_H