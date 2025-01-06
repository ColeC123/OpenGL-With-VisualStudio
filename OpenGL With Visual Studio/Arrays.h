#ifndef ARRAYS_H
#define ARRAYS_H

//------------------------------------------------------------------------------------------------------------------

typedef struct IntArray {
	//The pointer to the integer array
	int* arr;
	//How many currently initialized elements there are in the list
	unsigned int size;
	//The actual number of elements allocated. In order to get the memory size in bytes this would be multiplied by sizeof(int)
	unsigned int memsize;
} IntArray;

#define INT_ARRAY_DEFAULT (IntArray){.arr = NULL, .size = 0, .memsize = 1}

#define int_array_free(array) \
free(array.arr); \
array = INT_ARRAY_DEFAULT;

void int_array_append(IntArray* arr, int val);

//------------------------------------------------------------------------------------------------------------------

typedef struct FloatArray {
	//The pointer to the float array
	float* arr;
	//How many currently initialized elements there are in the list
	unsigned int size;
	//The actual number of elements allocated
	unsigned int memsize;
} FloatArray;

#define FLOAT_ARRAY_DEFAULT (FloatArray){.arr = NULL, .size = 0, .memsize = 1}

#define float_array_free(array) \
free(array.arr); \
array = FLOAT_ARRAY_DEFAULT;

void float_array_append(FloatArray* arr, float val);

//------------------------------------------------------------------------------------------------------------------

typedef struct UIntArray {
	//The pointer to the integer array
	unsigned int* arr;
	//How many currently initialized elements there are in the list
	unsigned int size;
	//The actual number of elements allocated. In order to get the memory size in bytes this would be multiplied by sizeof(int)
	unsigned int memsize;
} UIntArray;

#define UINT_ARRAY_DEFAULT (UIntArray){.arr = NULL, .size = 0, .memsize = 1}

#define uint_array_free(array) \
free(array.arr); \
array = UINT_ARRAY_DEFAULT;

void uint_array_append(UIntArray* arr, unsigned int val);

//------------------------------------------------------------------------------------------------------------------

typedef struct CharArray {
	//The pointer to the integer array
	unsigned int* arr;
	//How many currently initialized elements there are in the list
	unsigned int size;
	//The actual number of elements allocated. In order to get the memory size in bytes this would be multiplied by sizeof(int)
	unsigned int memsize;
} CharArray;

#define CHAR_ARRAY_DEFAULT (CharArray){.arr = NULL, .size = 0, .memsize = 1}

#define char_array_free(array) \
free(array.arr); \
array = CHAR_ARRAY_DEFAULT;

void char_array_append(CharArray* arr, char val);

//------------------------------------------------------------------------------------------------------------------


#endif