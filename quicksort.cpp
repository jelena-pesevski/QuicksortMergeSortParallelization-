// Quicksort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <omp.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int random() {
	return (rand() << 16) + rand();
}

void generateArray(int* arr,int* arr2, int size) {
	for (int i = 0; i < size; i++) {
		arr2[i]=arr[i] = random();
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

void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

int partition(int* elements, int left, int right)
{
	int pivot = elements[right];

	int i = left;
	int j = right - 1;
	while (i < j)
	{
		//pronadji prvi element koji je veci ili jednak pivotu
		while (elements[i] < pivot)
		{
			i++;
		}

		// pronadji prvi element zdesna koji je manji od pivota
		while (j > left && elements[j] >= pivot)
		{
			j--;
		}

		// ako je veci element lijevo od manjeg elementa zamijeni ih
		if (i < j)
		{
			int temp = elements[i];
			elements[i] = elements[j];
			elements[j] = temp;
			i++;
			j--;
		}
	}

	// i == j znaci da jos uvijek nismo provjerili ovaj element
	//pomjeri i tako da pokazuje na pocetak desnog niza
	if (i == j && elements[i] < pivot)
	{
		i++;
	}

	// pomjeri pivot na finalnu poziciju
	if (elements[i] != pivot)
	{
		int temp = elements[i];
		elements[i] = elements[right];
		elements[right] = temp;
	}
	return i;
}

void quicksort(int* array, int left, int right) {

	if (left<right) {
		int p = partition(array, left, right);

		quicksort(array, left, p - 1);
	
		quicksort(array, p + 1, right);
		
	}
}

void quicksortOpenMP(int* array, int left, int right) {

	if (right - left >= 2000) {
		int p = partition(array, left, right);
#pragma omp task 	
		{ 
			quicksortOpenMP(array, left, p - 1);
		}
//#pragma omp task 	
		{ 
			quicksortOpenMP(array, p + 1, right); 
		}
//#pragma omp taskwait
	}
	else {
		quicksort(array, left, right);
	}
}


int main()
{
	srand(time(NULL));
	int n = 1000000;

	int* arr = (int*)malloc(n * sizeof(int));
	int* arr2 = (int*)malloc(n * sizeof(int));

	if (!arr || !arr2) {
		if (arr)free(arr);
		if (arr2)free(arr2);
		return(EXIT_FAILURE);
	}

	generateArray(arr,arr2, n);

//	printArray(arr2, n);

	double startTime = omp_get_wtime();
	quicksort(arr2, 0, n-1);
	double endTime = omp_get_wtime();
	printf("Time: %f\n", endTime - startTime);

	assert(1 == isSorted(arr2, n));

//	printArray(arr2, n);

	startTime = omp_get_wtime();
#pragma omp parallel
	{
#pragma omp single 
		quicksortOpenMP(arr, 0, n-1);
	}
	endTime = omp_get_wtime();
	printf("Time: %f\n", endTime - startTime);


	assert(1 == isSorted(arr, n));
	//printArray(arr, n);

	free(arr);
	free(arr2);

	return 0;
}
