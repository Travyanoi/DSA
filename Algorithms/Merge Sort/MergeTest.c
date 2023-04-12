#include <stdio.h>
#include <stdlib.h>
#include "libMerge/Merge.h"

void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main()
{
    int array[] = { 12, 11, 13, 5, 6, 7 };
    int arr_size = sizeof(array) / sizeof(array[0]);
  
    printf("Given array is \n");
    printArray(array, arr_size);

    MergeSort(array, 0, arr_size - 1);
  
    printf("\nSorted array is \n");
    printArray(array, arr_size);
    return 0;
}