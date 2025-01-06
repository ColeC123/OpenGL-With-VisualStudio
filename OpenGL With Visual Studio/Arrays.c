#include "Arrays.h"
#include <stdlib.h>
#include <stdio.h>

//------------------------------------------------------------------------------------------------------------------

void int_array_append(IntArray* arr, int val) {
	if (arr->size + 1 >= arr->memsize) {
		arr->memsize *= 2;
		int* temp = (int*)realloc(arr->arr, arr->memsize * sizeof(int));

		if (temp != NULL) {
			arr->arr = temp;
		} else {
			printf("Failed to reallocate memory for IntArray type\n");
			exit(-1);
		}
	}

	arr->arr[arr->size] = val;
	arr->size++;
}

//------------------------------------------------------------------------------------------------------------------

void float_array_append(FloatArray* arr, float val) {
	if (arr->size + 1 >= arr->memsize) {
		arr->memsize *= 2;
		float* temp = (float*)realloc(arr->arr, arr->memsize * sizeof(float));

		if (temp != NULL) {
			arr->arr = temp;
		} else {
			printf("Failed to reallocate memory for FloatArray type\n");
			exit(-1);
		}
	}

	arr->arr[arr->size] = val;
	arr->size++;
}

//------------------------------------------------------------------------------------------------------------------

void uint_array_append(UIntArray* arr, unsigned int val) {
	if (arr->size + 1 >= arr->memsize) {
		arr->memsize *= 2;
		unsigned int* temp = (unsigned int*)realloc(arr->arr, arr->memsize * sizeof(unsigned int));

		if (temp != NULL) {
			arr->arr = temp;
		}
		else {
			printf("Failed to reallocate memory for IntArray type\n");
			exit(-1);
		}
	}

	arr->arr[arr->size] = val;
	arr->size++;
}

//------------------------------------------------------------------------------------------------------------------

void char_array_append(CharArray* arr, char val) {
	if (arr->size + 1 >= arr->memsize) {
		arr->memsize *= 2;
		char* temp = (char*)realloc(arr->arr, arr->memsize * sizeof(char));

		if (temp != NULL) {
			arr->arr = temp;
		}
		else {
			printf("Failed to reallocate memory for IntArray type\n");
			exit(-1);
		}
	}

	arr->arr[arr->size] = val;
	arr->size++;
}

//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------

