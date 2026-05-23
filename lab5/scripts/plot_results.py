import matplotlib.pyplot as plt
import numpy as np
import os

# Получаем путь к директории скрипта
script_dir = os.path.dirname(os.path.abspath(__file__))
project_dir = os.path.dirname(script_dir)
results_dir = os.path.join(project_dir, 'results')
plots_dir = os.path.join(project_dir, 'plots')

# Создание директории для графиков
os.makedirs(plots_dir, exist_ok=True)

# Список файлов для чтения
tree_names = ['AVL', 'Treap', 'Splay', 'RB', 'B', 'SkipList']
data = {}

for name in tree_names:
    filename = os.path.join(results_dir, f"{name}_results.txt")
    if os.path.exists(filename):
        sizes = []
        inserts = []
        deletes = []
        
        with open(filename, 'r') as f:
            lines = f.readlines()
            start_line = 0
            if lines and 'size' in lines[0].lower():
                start_line = 1
            
            for line in lines[start_line:]:
                parts = line.strip().split()
                if len(parts) >= 3:
                    sizes.append(int(parts[0]))
                    inserts.append(float(parts[1]))
                    deletes.append(float(parts[2]))
        
        if sizes:
            data[name] = {
                'sizes': sizes,
                'inserts': inserts,
                'deletes': deletes
            }
        else:
            print(f"  No valid data in {filename}")
    else:
        print(f"File {filename} not found")

# Чтение данных для AVL с отсортированным массивом
avl_sorted_sizes = []
avl_sorted_inserts = []
avl_sorted_deletes = []

sorted_filename = os.path.join(results_dir, "avl_sorted_results.txt")
if os.path.exists(sorted_filename):
    with open(sorted_filename, 'r') as f:
        lines = f.readlines()
        start_line = 0
        if lines and 'size' in lines[0].lower():
            start_line = 1
        for line in lines[start_line:]:
            parts = line.strip().split()
            if len(parts) >= 3:
                avl_sorted_sizes.append(int(parts[0]))
                avl_sorted_inserts.append(float(parts[1]))
                avl_sorted_deletes.append(float(parts[2]))

if not data:
    print("\nNo data files found in results directory!")
    exit(1)

# Цвета для деревьев
colors = {
    'AVL': 'blue',
    'Treap': 'green',
    'Splay': 'red',
    'RB': 'purple',
    'B': 'orange',
    'SkipList': 'brown'
}

# ============================================
# 1. AVL график с 4 линиями (случайные vs отсортированные)
# ============================================
if 'AVL' in data and avl_sorted_sizes:
    plt.figure(figsize=(12, 8))
    
    plt.plot(data['AVL']['sizes'], data['AVL']['inserts'], 
             color='blue', linestyle='-', linewidth=1, label='Insert (random)')
    plt.plot(data['AVL']['sizes'], data['AVL']['deletes'], 
             color='red', linestyle='-', linewidth=1, label='Delete (random)')
    plt.plot(avl_sorted_sizes, avl_sorted_inserts, 
             color='green', linestyle='--', linewidth=1, label='Insert (sorted)')
    plt.plot(avl_sorted_sizes, avl_sorted_deletes, 
             color='orange', linestyle='--', linewidth=1, label='Delete (sorted)')
    
    plt.xlabel('Number of Elements', fontsize=12)
    plt.ylabel('Time (ms)', fontsize=12)
    plt.title('AVL Tree: Random and Sorted Data', fontsize=14)
    plt.legend(loc='upper left', fontsize=11)
    plt.grid(True, alpha=0.3, linestyle='--')
    plt.tight_layout()
    plt.savefig(os.path.join(plots_dir, 'avl_comparison.png'), dpi=150, bbox_inches='tight')
    print("Saved: avl_comparison.png")

# ============================================
# 2. Графики для каждого дерева (Treap, Splay, RB, B, SkipList)
# ============================================
for name in ['Treap', 'Splay', 'RB', 'B', 'SkipList']:
    if name in data:
        plt.figure(figsize=(10, 6))
        plt.plot(data[name]['sizes'], data[name]['inserts'], 
                 color='blue', linestyle='-', linewidth=1, label='Insert')
        plt.plot(data[name]['sizes'], data[name]['deletes'], 
                 color='red', linestyle='-', linewidth=1, label='Delete')
        plt.xlabel('Number of Elements', fontsize=12)
        plt.ylabel('Time (ms)', fontsize=12)
        plt.title(f'{name} Tree Performance', fontsize=14)
        plt.legend(fontsize=12)
        plt.grid(True, alpha=0.3, linestyle='--')
        plt.tight_layout()
        plt.savefig(os.path.join(plots_dir, f'{name.lower()}_performance.png'), dpi=150, bbox_inches='tight')
        print(f"Saved: {name.lower()}_performance.png")

# ============================================
# 3. Сравнительный график (вставки и удаления на одном полотне) для всех деревьев
# ============================================
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

for name in data:
    ax1.plot(data[name]['sizes'], data[name]['inserts'], 
             color=colors.get(name, 'gray'), linestyle='-', linewidth=1, label=name)
    ax2.plot(data[name]['sizes'], data[name]['deletes'], 
             color=colors.get(name, 'gray'), linestyle='-', linewidth=1, label=name)

ax1.set_xlabel('Number of Elements', fontsize=12)
ax1.set_ylabel('Time (ms)', fontsize=12)
ax1.set_title('Insertion Time', fontsize=14)
ax1.legend(loc='upper left', fontsize=9)
ax1.grid(True, alpha=0.3, linestyle='--')

ax2.set_xlabel('Number of Elements', fontsize=12)
ax2.set_ylabel('Time (ms)', fontsize=12)
ax2.set_title('Deletion Time', fontsize=14)
ax2.legend(loc='upper left', fontsize=9)
ax2.grid(True, alpha=0.3, linestyle='--')

plt.tight_layout()
plt.savefig(os.path.join(plots_dir, 'comparison.png'), dpi=150, bbox_inches='tight')
print("Saved: comparison.png")