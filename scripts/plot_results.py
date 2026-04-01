import matplotlib.pyplot as plt
import numpy as np
import subprocess
import re

def run_binary_heap_test():
    try:
        result = subprocess.run(['./test_binary_heap'], 
                              capture_output=True, 
                              text=True, 
                              check=True)
        
        sizes = []
        linear_times = []
        insertions_times = []
        
        lines = result.stdout.split('\n')
        for line in lines:
            match = re.match(r'(\d+)\s+(\d+)\s+(\d+)\s+([\d.]+)', line)
            if match:
                sizes.append(int(match.group(1)))
                linear_times.append(int(match.group(2)))
                insertions_times.append(int(match.group(3)))
        
        return sizes, linear_times, insertions_times
    except Exception as e:
        print(f"Ошибка: {e}")
        return None, None, None

def run_binomial_heap_test():
    try:
        result = subprocess.run(['./test_binomial_heap'], 
                              capture_output=True, 
                              text=True, 
                              check=True)
        
        sizes = []
        times = []
        
        lines = result.stdout.split('\n')
        for line in lines:
            match = re.match(r'(\d+)\s+(\d+)', line)
            if match:
                sizes.append(int(match.group(1)))
                times.append(int(match.group(2)))
        
        return sizes, times
    except Exception as e:
        print(f"Ошибка: {e}")
        return None, None

def plot_binary_heap_results(sizes, linear_times, insertions_times):
    plt.figure(figsize=(10, 6))
    
    plt.plot(sizes, linear_times, 'b-', linewidth=2, label='Линейный')
    plt.plot(sizes, insertions_times, 'r-', linewidth=2, label='Вставки')
    
    plt.xlabel('Размер кучи (количество элементов)', fontsize=12)
    plt.ylabel('Время выполнения (микросекунды)', fontsize=12)
    plt.title('Сравнение алгоритмов построения бинарной кучи', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('binary_heap_results.png', dpi=150, bbox_inches='tight')
    plt.show()

def plot_binomial_heap_results(sizes, times):
    plt.figure(figsize=(10, 6))
    
    plt.plot(sizes, times, 'm-', linewidth=2, label='Экспериментальные данные')
    
    if len(sizes) > 1:
        z = np.polyfit(sizes, times, 1)
        p = np.poly1d(z)
        plt.plot(sizes, p(sizes), '--', color='orange', linewidth=2, 
                label=f'Линейная аппроксимация: {z[0]:.2f}·n + {z[1]:.0f}', alpha=0.7)
    
    plt.xlabel('Размер кучи (количество элементов)', fontsize=12)
    plt.ylabel('Время выполнения (микросекунды)', fontsize=12)
    plt.title('Построение биномиальной кучи вставками', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('binomial_heap_results.png', dpi=150, bbox_inches='tight')
    plt.show()

def main():
    # Бинарная куча
    sizes, linear_times, insertions_times = run_binary_heap_test()
    if sizes and linear_times and insertions_times:
        plot_binary_heap_results(sizes, linear_times, insertions_times)
    else:
        print("Не удалось получить результаты")
    
    # Биномиальная куча
    sizes, times = run_binomial_heap_test()
    if sizes and times:
        plot_binomial_heap_results(sizes, times)
    else:
        print("Не удалось получить результаты")
    
    print("\nГрафики сохранены в файлы:")
    print("- binary_heap_results.png")
    print("- binomial_heap_results.png")

if __name__ == "__main__":
    main()