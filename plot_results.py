import matplotlib.pyplot as plt
import numpy as np

n_values    = []
array_times = []
list_times  = []

with open('test4_data.txt', 'r') as f:
    next(f)  # пропускаем заголовок
    for line in f:
        n, arr, lst = map(float, line.split())
        n_values.append(n)
        array_times.append(arr)
        list_times.append(lst)

plt.figure(figsize=(12, 8))

plt.plot(n_values, array_times, 'b-', label='Стек на массиве', linewidth=2)
plt.plot(n_values, list_times,  'r-', label='Стек на списке',  linewidth=2)

plt.xlabel('Количество вставок (n)', fontsize=12)
plt.ylabel('Время (секунды)', fontsize=12)
plt.title('Сравнение производительности: стек на массиве vs стек на списке', fontsize=14)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)

plt.tick_params(labelsize=10)
plt.tight_layout()

# сохранение графика
plt.savefig('stack_comparison.png', dpi=300, bbox_inches='tight')
plt.show()