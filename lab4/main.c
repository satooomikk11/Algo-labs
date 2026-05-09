#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void test_uint_hashes  ();
void test_float_hashes ();
void test_string_hashes();
void test_hash_tables_load_factor();
void test_hash_tables_operations ();
void test_perfect_hashing();

void generate_all_data()
{
    srand(42);
    system("mkdir -p data");
    
    // uint
    FILE* f = fopen("data/uint_keys.bin", "wb");
    unsigned int* uint_keys = calloc(1000000, sizeof(unsigned int));
    for (int i = 0; i < 1000000; i++) uint_keys[i] = rand();
    fwrite(uint_keys, sizeof(unsigned int), 1000000, f);
    fclose(f);
    free(uint_keys);
    
    // float
    f = fopen("data/float_keys.bin", "wb");
    float* float_keys = calloc(1000000, sizeof(float));
    for (int i = 0; i < 1000000; i++)
    {
        float_keys[i] = -10.0 + (rand() / (float)RAND_MAX) * 20.0;
    }
    fwrite(float_keys, sizeof(float), 1000000, f);
    fclose(f);
    free(float_keys);
    
    // strings
    f = fopen("data/string_keys.txt", "w");
    for (int i = 0; i < 1000000; i++)
    {
        int len = 5 + rand() % 16;
        for (int j = 0; j < len; j++)
        {
            fputc('a' + rand() % 26, f);
        }
        fputc('\n', f);
    }
    fclose(f);

    // int для таблиц
    f = fopen("data/int_keys.bin", "wb");
    int* int_keys = calloc(1000000, sizeof(int));
    for (int i = 0; i < 1000000; i++) int_keys[i] = rand();
    fwrite(int_keys, sizeof(int), 1000000, f);
    fclose(f);
    free(int_keys);
    
    printf("\nAll data generated!\n");
}

int main()
{
    generate_all_data();
    
    test_uint_hashes  ();
    test_float_hashes ();
    test_string_hashes();
    test_hash_tables_load_factor();
    test_hash_tables_operations_uniform();
    test_hash_tables_operations ();
    test_perfect_hashing();
    
    return 0;
}