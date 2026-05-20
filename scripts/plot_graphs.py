#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import os

plt.style.use('seaborn-v0_8-darkgrid')

def load_csv(filepath):
    try:
        sizes = []
        times = []
        with open(filepath, 'r') as f:
            lines = f.readlines()
            if len(lines) < 2:
                return None, None
            for line in lines[1:]:
                if line.strip():
                    parts = line.strip().split(',')
                    if len(parts) >= 2:
                        try:
                            sizes.append(float(parts[0]))
                            times.append(float(parts[1]))
                        except:
                            pass
        return np.array(sizes), np.array(times)
    except:
        return None, None

def plot_point1():
    plt.figure(figsize=(12, 8))
    
    sorts = [
        ('insertion_small.csv', 'Insertion Sort', '#1f77b4'),
        ('bubble_small.csv', 'Bubble Sort', '#ff7f0e'),
        ('selection_small.csv', 'Selection Sort', '#2ca02c'),
        ('shell_small.csv', 'Shell Sort', '#d62728')
    ]
    
    for file, label, color in sorts:
        x, y = load_csv(f'results/{file}')
        if x is not None and len(x) > 0:
            plt.plot(x, y, label=label, color=color, linewidth=1.8)
    
    plt.xlabel('Размер массива', fontsize=12)
    plt.ylabel('Время (мс)', fontsize=12)
    plt.title('Пункт 1: Квадратичные (и не совсем) сортировки', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plots/point1_quadratic.png', dpi=150, bbox_inches='tight')
    plt.close()

def plot_point2():
    plt.figure(figsize=(12, 8))
    
    colors = [
        '#e41a1c', '#377eb8', '#4daf4a', '#984ea3', '#ff7f00',
        '#ffff33', '#a65628', '#f781bf', '#999999'
    ]
    
    for i, k in enumerate(range(2, 11)):
        x, y = load_csv(f'results/heap_k{k}_big.csv')
        if x is not None and len(x) > 0:
            plt.plot(x, y, label=f'k = {k}', color=colors[i], linewidth=1.8)
    
    plt.xlabel('Размер массива', fontsize=12)
    plt.ylabel('Время (мс)', fontsize=12)
    plt.title('Пункт 2: Пирамидальные сортировки', fontsize=14)
    plt.legend(fontsize=9, ncol=2)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plots/point2_heap.png', dpi=150, bbox_inches='tight')
    plt.close()

def plot_point3():
    plt.figure(figsize=(12, 8))
    
    x1, y1 = load_csv('results/merge_recursive_big.csv')
    x2, y2 = load_csv('results/merge_iterative_big.csv')
    
    if x1 is not None and len(x1) > 0:
        plt.plot(x1, y1, label='Рекурсивная', color='#1f77b4', linewidth=1.8)
    if x2 is not None and len(x2) > 0:
        plt.plot(x2, y2, label='Итеративная', color='#ff7f0e', linewidth=1.8)
    
    plt.xlabel('Размер массива', fontsize=12)
    plt.ylabel('Время (мс)', fontsize=12)
    plt.title('Пункт 3: Сортировки слиянием', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plots/point3_merge.png', dpi=150, bbox_inches='tight')
    plt.close()

def plot_point4():
    fig, axes = plt.subplots(1, 2, figsize=(15, 6))
    
    sorts = [
        ('quick_lomuto_big.csv', 'Ломуто', '#1f77b4'),
        ('quick_hoare_big.csv', 'Хоара', '#ff7f0e'),
        ('quick_3way_big.csv', '3-way', '#2ca02c')
    ]
    
    for file, label, color in sorts:
        x, y = load_csv(f'results/{file}')
        if x is not None and len(x) > 0:
            axes[0].plot(x, y, label=label, color=color, linewidth=1.8)
    
    axes[0].set_xlabel('Размер массива', fontsize=12)
    axes[0].set_ylabel('Время (мс)', fontsize=12)
    axes[0].set_title('Обычные данные', fontsize=12)
    axes[0].legend(fontsize=10)
    axes[0].grid(True, alpha=0.3)
    
    for file, label, color in sorts:
        dub_file = file.replace('big', 'dub')
        x, y = load_csv(f'results/{dub_file}')
        if x is not None and len(x) > 0:
            axes[1].plot(x, y, label=label, color=color, linewidth=1.8)
    
    axes[1].set_xlabel('Размер массива', fontsize=12)
    axes[1].set_ylabel('Время (мс)', fontsize=12)
    axes[1].set_title('Данные с повторениями', fontsize=12)
    axes[1].legend(fontsize=10)
    axes[1].grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('plots/point4_quick_partitions.png', dpi=150, bbox_inches='tight')
    plt.close()

def plot_point5():
    plt.figure(figsize=(12, 8))
    
    sorts = [
        ('quick_central_big.csv', 'Центральный элемент', '#1f77b4'),
        ('quick_median3_big.csv', 'Медиана трех', '#ff7f0e'),
        ('quick_random_big.csv', 'Случайный элемент', '#2ca02c'),
        ('quick_median3_random_big.csv', 'Медиана трех случайных', '#d62728')
    ]
    
    for file, label, color in sorts:
        x, y = load_csv(f'results/{file}')
        if x is not None and len(x) > 0:
            plt.plot(x, y, label=label, color=color, linewidth=1.8)
    
    plt.xlabel('Размер массива', fontsize=12)
    plt.ylabel('Время (мс)', fontsize=12)
    plt.title('Пункт 5: Быстрые сортировки (разные стратегии выбора)', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plots/point5_quick_pivot.png', dpi=150, bbox_inches='tight')
    plt.close()

def plot_point6():
    plt.figure(figsize=(12, 8))
    
    thresholds = list(range(5, 51, 5))
    
    colors = plt.cm.tab10(np.linspace(0, 1, len(thresholds)))
    
    for i, t in enumerate(thresholds):
        x, y = load_csv(f'results/introsort_t{t}_big.csv')
        if x is not None and len(x) > 0:
            plt.plot(x, y, label=f'threshold = {t}', color=colors[i], linewidth=1.5, alpha=0.9)
    
    x_base, y_base = load_csv('results/quick_hoare_baseline_big.csv')
    if x_base is not None and len(x_base) > 0:
        plt.plot(x_base, y_base, label='Quick Sort (Hoare)', 
                color='black', linewidth=2.5, linestyle='--')
    
    plt.xlabel('Размер массива', fontsize=12)
    plt.ylabel('Время (мс)', fontsize=12)
    plt.title('Пункт 6: Выбор размера блоков для Introsort', fontsize=14)
    plt.legend(fontsize=8, ncol=2)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plots/point6_introsort_thresholds.png', dpi=150, bbox_inches='tight')
    plt.close()

def plot_point7_c():
    plt.figure(figsize=(12, 8))
    
    c_values = list(range(1, 6))
    colors = plt.cm.viridis(np.linspace(0, 1, len(c_values)))
    
    for i, c in enumerate(c_values):
        x, y = load_csv(f'results/introspective_c{c}_big.csv')
        if x is not None and len(x) > 0:
            plt.plot(x, y, label=f'C = {c}', color=colors[i], linewidth=1.8)
    
    x_base, y_base = load_csv('results/quick_central_baseline_big.csv')
    if x_base is not None and len(x_base) > 0:
        plt.plot(x_base, y_base, label='Quick Sort (central)', 
                color='black', linewidth=2.5, linestyle='--')
    
    plt.xlabel('Размер массива', fontsize=12)
    plt.ylabel('Время (мс)', fontsize=12)
    plt.title('Пункт 7: Introspective Sorting', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plots/point7_introspective_c.png', dpi=150, bbox_inches='tight')
    plt.close()

def plot_point7_optimal():
    plt.figure(figsize=(12, 8))
    
    x_opt, y_opt = load_csv('results/introspective_optimal_big.csv')
    if x_opt is not None and len(x_opt) > 0:
        plt.plot(x_opt, y_opt, label='Introspective Sort', 
                color='#d62728', linewidth=2.5)
    
    x_base, y_base = load_csv('results/quick_central_baseline_big.csv')
    if x_base is not None and len(x_base) > 0:
        plt.plot(x_base, y_base, label='Quick Sort (central)', 
                color='#1f77b4', linewidth=2, linestyle='--')
    
    plt.xlabel('Размер массива', fontsize=12)
    plt.ylabel('Время (мс)', fontsize=12)
    plt.title('Пункт 7: Сравнение Quick Sort и Introspective Sort', fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plots/point7_introspective_optimal.png', dpi=150, bbox_inches='tight')
    plt.close()

def plot_point9():
    plt.figure(figsize=(12, 8))
    
    x1, y1 = load_csv('results/radix_lsd_big.csv')
    x2, y2 = load_csv('results/radix_msd_big.csv')
    
    if x1 is not None and len(x1) > 0:
        plt.plot(x1, y1, label='LSD Radix Sort', color='#1f77b4', linewidth=1.8)
    if x2 is not None and len(x2) > 0:
        plt.plot(x2, y2, label='MSD Radix Sort', color='#ff7f0e', linewidth=1.8)
    
    plt.xlabel('Размер массива', fontsize=12)
    plt.ylabel('Время (мс)', fontsize=12)
    plt.title('Пункт 9: Сортировки, основанные не на сравнениях', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plots/point9_radix.png', dpi=150, bbox_inches='tight')
    plt.close()

def plot_point10():
    plt.figure(figsize=(14, 10))
    
    sorts = [
        ('best_shell_big.csv', 'Shell Sort (п.1)', '#1f77b4', '-'),
        ('best_heap_k5_big.csv', 'Heap Sort k = 4 (п.2)', '#ff7f0e', '-'),
        ('best_merge_iterative_big.csv', 'Merge Sort Iterative (п.3)', '#2ca02c', '-'),
        ('best_quick_hoare_big.csv', 'Quick Sort Hoare (п.4)', '#d62728', '-'),
        ('best_quick_central_big.csv', 'Quick Sort Central (п.5)', '#9467bd', '-'),
        ('best_introsort_t40_big.csv', 'Introsort (п.6)', '#8c564b', '-'),
        ('best_introspective_c2_big.csv', 'Introspective Sort (п.7)', '#e377c2', '-'),
        ('best_radix_lsd_big.csv', 'LSD Radix Sort (п.9)', '#7f7f7f', '-'),
        ('qsort_big.csv', 'qsort (стандартная)', 'black', '--')
    ]
    
    for file, label, color, style in sorts:
        x, y = load_csv(f'results/{file}')
        if x is not None and len(x) > 0:
            plt.plot(x, y, label=label, color=color, 
                    linewidth=1.8, linestyle=style)
    
    plt.xlabel('Размер массива', fontsize=12)
    plt.ylabel('Время (мс)', fontsize=12)
    plt.title('Пункт 10: Сравнение лучших алгоритмов сортировки', fontsize=14)
    plt.legend(fontsize=9, ncol=2)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('plots/point10_best_sorts.png', dpi=150, bbox_inches='tight')
    plt.close()

def main():
    os.makedirs('plots', exist_ok=True)
    
    print("Генерация графиков...")
    
    plot_point1()
    print("   Пункт 1:  point1_quadratic.png")
    
    plot_point2()
    print("   Пункт 2:  point2_heap.png")
    
    plot_point3()
    print("   Пункт 3:  point3_merge.png")
    
    plot_point4()
    print("   Пункт 4:  point4_quick_partitions.png")
    
    plot_point5()
    print("   Пункт 5:  point5_quick_pivot.png")
    
    plot_point6()
    print("   Пункт 6:  point6_introsort_thresholds.png")

    plot_point7_c()
    print("   Пункт 7:  point7_introspective_c.png")

    plot_point7_optimal()
    print("   Пункт 7:  point7_introspective_optimal.png")
    
    plot_point9()
    print("   Пункт 9:  point9_radix.png")

    plot_point10()
    print("   Пункт 10: point10_best_sorts.png")
    
    print("\nВсе графики сохранены в папке plots/")

if __name__ == "__main__":
    main()