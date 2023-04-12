#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
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

void countSort(uint32_t array[], int exp)
{
    int output[NUM];
    int i, count[10] = { 0 };
 
    for (i = 0; i < NUM; i++){
        count[(array[i] / exp) % 10]++; // заполняем вспомогательный массив с индексами от 0 до 9 текущим разрядом(единицы, десятки, сотни...)
    }
    
    for (i = 1; i < 10; i++){
        count[i] += count[i - 1]; 
    }
 
    for (i = NUM - 1; i >= 0; i--) {
        output[count[(array[i] / exp) % 10] - 1] = array[i]; // присваиваем output[] по индексу разряда -1 значение в сортируемом массиве 
        count[(array[i] / exp) % 10]--;
    }

    for (i = 0; i < NUM; i++){
        array[i] = output[i];
    }
}
 
void radixsort(uint32_t array[], int max)
{
    for (int exp = 1; max / exp > 0; exp *= 10){
        countSort(array, exp);
    }
}

void print(uint32_t array[])
{
    for (int i = 0; i < NUM; i++){
        printf("%d\t", array[i]);
    }
    printf("\n");
}


int main()
{
    double start, end;
    start = wtime();
    uint32_t array[NUM];
    int max = 0;
    for(int i = 0; i < NUM; i++){
        array[i] = getrand(0, 100001);
        if (max < array[i]){
            max = array[i];
        }
    }
 
    //printf("max = %d\n", max);
    radixsort(array, max);
    //print(array);
    end = wtime();
    printf("%f\n", end - start);

    return 0;
}