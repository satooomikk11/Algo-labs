#!/bin/bash

if [ ! -f "ctest.h" ]; then
    echo "Скачивание ctest.h..."
    wget -q https://raw.githubusercontent.com/AineeJames/ctest.h/main/ctest.h
    if [ $? -ne 0 ]; then
        echo "Ошибка скачивания ctest.h!"
        exit 1
    fi
    echo "ctest.h скачан"
fi

make

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

make clean
