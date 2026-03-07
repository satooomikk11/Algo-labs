#!/bin/bash

gcc -o test_runner test_runner.c dynamic_array_stack.c linked_list_stack.c -lm

if [ $? -ne 0 ]; then
    echo "Ошибка компиляции!"
    exit 1
fi

echo "Запуск тестов"
./test_runner

echo "Построение графиков"
python3 plot_results.py

echo -e "\nРезультаты сохранены:"
echo "test4_data.txt (данные для графика)"
echo "stack_comparison.png (сам график)"