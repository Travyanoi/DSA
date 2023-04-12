#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#define NUM 1000000

double wtime()
{
struct timeval t;
gettimeofday(&t, NULL);
return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int getrand(int min, int max)
{
return (double)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

int main()
{
    double start, end;
    start = wtime();
    void odd_even(uint32_t first[]);
    uint32_t first[NUM];
    for(int i = 0; i < NUM; i++){
        first[i] = getrand(0, 100001);
    }
    odd_even(first);
    end = wtime();
    printf("%lf\n", end - start);

    return 0;

}

void swap(uint32_t a[], uint32_t b[])
{
    uint32_t temp = 0;
    temp = *a;
    *a = *b;
    *b = temp;
}

void odd_even(uint32_t first[])
{
    for(int i = 0; i < NUM; i++){
        for(int j = (i % 2) ? 0 : 1; j + 1 < NUM; j+=2){
            if(first[j] > first[j + 1]){
                swap(&first[j], &first[j + 1]);
            }
        }
    }
}