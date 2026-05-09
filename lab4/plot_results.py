#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

os.makedirs('plots/functions', exist_ok=True)
os.makedirs('plots/tables', exist_ok=True)

def plot_histogram(csv_file, title, output_file, xlim=None, ylim=None):
    try:
        df = pd.read_csv(csv_file)
        
        plt.figure(figsize=(12, 5))
        
        if xlim:
            df = df[df['bucket'] <= xlim]
        
        plt.bar(df['bucket'], df['count'], width=0.8, edgecolor='black', alpha=0.7)
        plt.xlabel('Bucket index (hash % 1000)')
        plt.ylabel('Number of keys')
        plt.title(title)
        if ylim:
            plt.ylim(ylim)
        plt.tight_layout()
        plt.savefig(output_file, dpi=150)
        plt.close()
        print(f"Saved: {output_file}")
    except Exception as e:
        print(f"Error plotting {csv_file}: {e}")

# ========== Uint ==========
for name in ['mod', 'bitwise', 'knuth']:
    plot_histogram(f'results/functions/uint_{name}.csv',
                   f'Uint hash: {name.capitalize()}',
                   f'plots/functions/uint_{name}_hist.png')

# ========== Float ==========
for name in ['tobits', 'bitwise', 'mantissa', 'exponent', 'mantexp']:
    plot_histogram(f'results/functions/float_{name}.csv',
                   f'Float hash: {name}',
                   f'plots/functions/float_{name}_hist.png')

# ========== String ==========
plot_histogram('results/functions/string_len.csv', 'String hash: Length', 
               'plots/functions/string_len_hist.png', xlim=25)
plot_histogram('results/functions/string_sum.csv', 'String hash: Sum',
               'plots/functions/string_sum_hist.png')
plot_histogram('results/functions/string_poly.csv', 'String hash: Polynomial',
               'plots/functions/string_poly_hist.png')
plot_histogram('results/functions/string_crc32.csv', 'String hash: CRC32',
               'plots/functions/string_crc32_hist.png')

# ========== Load factor ==========
def plot_load_factor():
    try:
        df = pd.read_csv('results/tables/load_factor_results.csv')
        
        plt.figure(figsize=(10, 6))
        
        for col in ['chaining', 'linear', 'quadratic', 'double']:
            if col in df.columns:
                data = df[df[col] != 'N/A'][col].astype(float)
                plt.step(df['load_factor'][:len(data)], data, where='mid', 
                        label=col.capitalize(), linewidth=2)
        
        if 'cuckoo' in df.columns:
            cuckoo_df = df[df['cuckoo'] != 'N/A']
            if not cuckoo_df.empty:
                plt.step(cuckoo_df['load_factor'], cuckoo_df['cuckoo'].astype(float), 
                        where='mid', label='Cuckoo', linewidth=2, linestyle='--')
        
        plt.xlabel('Load Factor')
        plt.ylabel('Time (ms)')
        plt.title('Hash Table Performance vs Load Factor')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.savefig('plots/tables/load_factor_comparison.png', dpi=150)
        plt.close()
        print("Saved: plots/tables/load_factor_comparison.png")
        
    except Exception as e:
        print(f"Error: {e}")

# ========== Operations (0.5/0.25/0.25) ==========
def plot_operations():
    try:
        df = pd.read_csv('results/tables/operations_results.csv')
        
        plt.figure(figsize=(12, 7))
        
        for col in ['chaining', 'linear', 'quadratic', 'double']:
            if col in df.columns:
                plt.plot(df['size'], df[col], label=col.capitalize(), linewidth=2)
        
        # Кукушка отдельно (если есть данные)
        if 'cuckoo' in df.columns:
            cuckoo_df = df[df['cuckoo'] != 'N/A']
            if not cuckoo_df.empty:
                plt.plot(cuckoo_df['size'], cuckoo_df['cuckoo'].astype(float), 
                        label='Cuckoo', linewidth=2, linestyle='--')
        
        plt.xlabel('Number of operations')
        plt.ylabel('Time (ms)')
        plt.title('Mixed Operations (insert=0.5, search=0.25, delete=0.25)')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.savefig('plots/tables/operations_comparison.png', dpi=150)
        plt.close()
        print("Saved: plots/tables/operations_comparison.png")
        
    except Exception as e:
        print(f"Error plotting operations: {e}")

# ========== Uniform Operations (0.33/0.33/0.33) ==========
def plot_operations_uniform():
    try:
        df = pd.read_csv('results/tables/operations_uniform_results.csv')
        
        plt.figure(figsize=(12, 7))
        
        for col in ['chaining', 'linear', 'quadratic', 'double']:
            if col in df.columns:
                plt.plot(df['size'], df[col], label=col.capitalize(), linewidth=2)
        
        if 'cuckoo' in df.columns:
            cuckoo_df = df[df['cuckoo'] != 'N/A']
            if not cuckoo_df.empty:
                plt.plot(cuckoo_df['size'], cuckoo_df['cuckoo'].astype(float), 
                        label='Cuckoo', linewidth=2, linestyle='--')
        
        plt.xlabel('Number of operations')
        plt.ylabel('Time (ms)')
        plt.title('Uniform Operations (insert=33%, search=33%, delete=33%)')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.savefig('plots/tables/operations_uniform_comparison.png', dpi=150)
        plt.close()
        print("Saved: plots/tables/operations_uniform_comparison.png")
        
    except Exception as e:
        print(f"Error plotting uniform operations: {e}")

# ========== Perfect hash ==========
def plot_perfect_hash():
    try:
        df = pd.read_csv('results/tables/perfect_hash_results.csv')
        
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
        
        colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
        table_labels = ['Chaining', 'Quadratic', 'Cuckoo', 'Perfect hashing']
        table_keys = ['chaining', 'quadratic', 'cuckoo', 'perfect_hashing']
        
        build_times = []
        search_times = []
        
        for key in table_keys:
            row = df[df['table_type'] == key]
            if not row.empty:
                build_times.append(row['build_ms'].values[0])
                search_times.append(row['search_ms'].values[0])
            else:
                build_times.append(0)
                search_times.append(0)
        
        # Build time 
        bars1 = ax1.bar(table_labels, build_times, color=colors)
        ax1.set_xlabel('Hash table type')
        ax1.set_ylabel('Build time (ms)')
        ax1.set_title('Build Time')
        ax1.tick_params(axis='x', rotation=45)
        
        for bar, val in zip(bars1, build_times):
            if val > 0:
                ax1.text(bar.get_x() + bar.get_width()/2, val + 100, 
                        str(val), ha='center', va='bottom', fontsize=9)
        
        # Search time
        bars2 = ax2.bar(table_labels, search_times, color=colors)
        ax2.set_xlabel('Hash table type')
        ax2.set_ylabel('Search time (ms)')
        ax2.set_title('Search Time')
        ax2.tick_params(axis='x', rotation=45)
        
        for bar, val in zip(bars2, search_times):
            if val > 0:
                ax2.text(bar.get_x() + bar.get_width()/2, val + 5, 
                        str(val), ha='center', va='bottom', fontsize=9)
        
        plt.tight_layout()
        plt.savefig('plots/tables/perfect_hash_comparison.png', dpi=150)
        plt.close()
        print("Saved: plots/tables/perfect_hash_comparison.png")
        
    except Exception as e:
        print(f"Error plotting perfect hash: {e}")

# ========== Дисперсии ==========
def plot_variances():
    try:
        uint_data = {'Modulo': 913.38, 'Bitwise': 974.75, 'Knuth': 1101.02}
        float_data = {'To int bits': 298269, 'Bitwise': 973, 'Mantissa': 1003688, 
                      'Exponent': 251898236, 'Mant*Exp': 2898839}
        string_data = {'Length': 61500707, 'Sum': 289176, 'Polynomial': 1012, 'CRC32': 1013}
        
        fig, axes = plt.subplots(1, 3, figsize=(15, 5))
        
        # Uint
        axes[0].bar(uint_data.keys(), uint_data.values(), color='skyblue')
        axes[0].set_yscale('log')
        axes[0].set_ylabel('Variance (log scale)')
        axes[0].set_title('Uint Hash Functions Variance')
        axes[0].tick_params(axis='x', rotation=45)
        axes[0].axhline(y=1000, color='r', linestyle='--', label='Ideal (1000)')
        axes[0].legend()
        
        # Float
        axes[1].bar(float_data.keys(), float_data.values(), color='lightgreen')
        axes[1].set_yscale('log')
        axes[1].set_title('Float Hash Functions Variance')
        axes[1].tick_params(axis='x', rotation=45)
        
        # String
        axes[2].bar(string_data.keys(), string_data.values(), color='salmon')
        axes[2].set_yscale('log')
        axes[2].set_title('String Hash Functions Variance')
        axes[2].tick_params(axis='x', rotation=45)
        
        plt.tight_layout()
        plt.savefig('plots/functions/variance_comparison.png', dpi=150)
        plt.close()
        print("Saved: plots/functions/variance_comparison.png")
        
    except Exception as e:
        print(f"Error plotting variances: {e}")

if __name__ == "__main__":
    plot_load_factor()
    plot_operations()
    plot_operations_uniform()
    plot_perfect_hash()
    plot_variances()
    print("\nAll plots saved!")