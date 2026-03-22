#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <size> <max_value>\n", argv[0]);
        return 1;
    }
    
    size_t size = atoi(argv[1]);
    int max_val = atoi(argv[2]);
    
    srand(time(NULL));
    
    printf("%zu\n", size);
    for (size_t i = 0; i < size; i++)
    {
        printf("%d ", rand() % (max_val + 1));
    }
    
    return 0;
}