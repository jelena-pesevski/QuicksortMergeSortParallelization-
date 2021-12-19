// ProjekatAR2_mergeSort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <omp.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int random() {
	return (rand() << 16) + rand();
}

void generateArray(int* arr,int* arr2, int size) {
	for (int i = 0; i < size; i++)
		arr2[i] = arr[i] = random();
}


void merge(int* arr, int n, int* tmp) {
	int i = 0;
	int j = n / 2;
	int k = 0;

	while (i < n / 2 && j < n) {
		if (arr[i] < arr[j]) {
			tmp[k++] = arr[i++];
		}
		else {
			tmp[k++] = arr[j++];
		}
	}

	while (i < n / 2) {
		tmp[k++] = arr[i++];
	}

	while (j < n) {
		tmp[k++] = arr[j++];
	}
	memcpy(arr, tmp, n * sizeof(int));
}

void mergeSort(int* arr, int n, int* tmp) {
	if (n < 2) return;

	mergeSort(arr, n / 2, tmp);

	mergeSort(arr + (n / 2), n - (n / 2), tmp + (n / 2));

	merge(arr, n, tmp);

}

void mergeSortOpenMP(int* arr, int n, int* tmp) {
	if (n  < 2000) mergeSort(arr, n, tmp);
	else {
#pragma omp task 
		{
			mergeSortOpenMP(arr, n / 2, tmp);
		}
//#pragma omp task //shared(arr)
		mergeSortOpenMP(arr + (n / 2), n - (n / 2), tmp + (n / 2));

#pragma omp taskwait
		merge(arr, n, tmp);
	}
}

int isSorted(int* a, int size) {
	for (int i = 0; i < size - 1; i++)
		if (a[i] > a[i + 1])
			return 0;
	return 1;
}

void printArray(int* arr, int size) {
	for (int i = 0; i < size; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

int main()
{
	srand(time(NULL));
	int n = 10000000;
	int numOfThreads = 4;

	int* arr = (int*)malloc(n * sizeof(int));
	int* arr2 = (int*)malloc(n * sizeof(int));
	
	int* tmp = (int*)malloc(n * sizeof(int));

	omp_set_dynamic(0);
	omp_set_num_threads(numOfThreads);

	if (!arr || !tmp || !arr2) {
		if(arr)free(arr);
		if (tmp)free(tmp);
		if (arr2)free(arr2);
		return(EXIT_FAILURE);

	}

	generateArray(arr,arr2,  n);

	double startTime = omp_get_wtime();
	mergeSort(arr2, n, tmp);
	double endTime = omp_get_wtime();
	printf("Time: %f\n", endTime - startTime);

	assert(1 == isSorted(arr2, n));

	free(tmp);
	tmp = (int*)malloc(n * sizeof(int));
	startTime = omp_get_wtime();
#pragma omp parallel
	{
#pragma omp single 
		mergeSortOpenMP(arr, n, tmp);
	}
	 endTime= omp_get_wtime();
	printf("Time: %f\n", endTime - startTime);


	assert(1 == isSorted(arr, n));
	//printArray(arr, n);

	free(arr);
	free(arr2);

	return 0;


}
