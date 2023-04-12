#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

void counting_sort(uint32_t*, uint32_t, int);

int getrand(int min, int max)
{
    return (double)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main()
{
    int N;
    printf("Введите длину массива\n");
    scanf("%d", &N);

    double start_time = wtime();
    uint32_t arr[N];
    int i;
    uint32_t max = 0;

    for (i = 0; i < N; i++) {
        arr[i] = getrand(0, 100001);
    }

    for (i = 0; i < N; i++) {
        if (max < arr[i]) {
            max = arr[i];
        }
    }
    counting_sort(arr, max, N);
    double end_time = wtime();

    printf("%f\n", end_time - start_time);

    return 0;
}

void counting_sort(uint32_t* arr, uint32_t max, int N)
{
    int i, j;
    uint32_t add[max + 1];

    for (i = 0; i < max + 1; i++) {
        add[i] = 0;
    }

    for (i = 0; i < N; i++) {
        add[arr[i]]++;
    }

    for (i = 0; i < N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    for (i = 0; i < max + 1; i++) {
        if (add[i] != 0) {
            for (j = 0; j < add[i]; j++) {
                printf("%d ", i);
            }
        }
    }
    printf("\n");
}