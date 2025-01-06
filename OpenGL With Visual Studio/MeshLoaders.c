#include "MeshLoaders.h"
#include <stdlib.h>
#include <stdio.h>

void loadWaveFrontOBJ(FloatArray* vertices, UIntArray* indices, char* filePath) {
	//If there is any data already in vertices or indices, it will be lost when this function is called
	float_array_free((*vertices));
	uint_array_free((*indices));

	FILE* fptr = fopen(filePath, "rt");

	fseek(fptr, 0L, SEEK_END);
	unsigned int fileSize = ftell(fptr);
	rewind(fptr);

	char* fileString = (char*)malloc(fileSize);
	if (fileString == NULL) {
		printf("Failed to allocate memory for fileString when loading wavefront obj file\n");
		exit(-1);
	}
	fread(fileString, sizeof(char), fileSize, fptr);


	//--------------------------------------Extracts Vertices to vertices Array-------------------------------------------

	IntArray vertexIndices = INT_ARRAY_DEFAULT;

	char prevchar = (char)-1;
	for (int i = 0; i < fileSize - 1; i++) {
		if (fileString[i] == 'v' && (prevchar == (char)-1 || prevchar == '\n') && fileString[i+1] == ' ') {
			int_array_append(&vertexIndices, i);
		}
		prevchar = fileString[i];
	}

	for (int i = 0; i < vertexIndices.size; i++) {
		char buff[20];
		int buffCount = 0;

		int spaceIndices[3];
		int spaceIndicesCount = 0;

		for (int j = vertexIndices.arr[i]; j < fileSize && fileString[j] != '\n' && spaceIndicesCount < 3 && fileString[j] != '\0'; j++) {
			if (fileString[j] == ' ') {
				spaceIndices[spaceIndicesCount] = j;
				spaceIndicesCount++;
			}
		}

		float x, y, z;

		for (int j = spaceIndices[0] + 1; j < fileSize && j < spaceIndices[1] && buffCount < 19; j++) {
			buff[buffCount] = fileString[j];
			buffCount++;
		}
		buff[buffCount] = '\0';
		x = (float)atof(buff);

		buffCount = 0;
		for (int j = spaceIndices[1] + 1; j < fileSize && j < spaceIndices[2] && buffCount < 19; j++) {
			buff[buffCount] = fileString[j];
			buffCount++;
		}
		buff[buffCount] = '\0';
		y = (float)atof(buff);

		buffCount = 0;
		for (int j = spaceIndices[2] + 1; j < fileSize && buffCount < 19 && fileString[j] != '\n' && fileString[j] != '\0'; j++) {
			buff[buffCount] = fileString[j];
			buffCount++;
		}
		buff[buffCount] = '\0';
		z = (float)atof(buff);

		float_array_append(vertices, x);
		float_array_append(vertices, y);
		float_array_append(vertices, z);
	}

	int_array_free(vertexIndices);

	//------------------------------------------------------------------------------------------------------------------------

	//-----------------------------------Extract Triangle Data---------------------------------------------------------------
	IntArray faceIndices = INT_ARRAY_DEFAULT;

	prevchar = (char)-1;
	for (int i = 0; i < fileSize - 1; i++) {
		if (fileString[i] == 'f' && (prevchar == (char)-1 || prevchar == '\n') && fileString[i + 1] == ' ') {
			int_array_append(&faceIndices, i);
		}
		prevchar = fileString[i];
	}

	for (int i = 0; i < faceIndices.size; i++) {
		/*The number of spaces will be equal to the number of vertices in that particular face.
		The obj file format allows for any number of vertices per face as long as they form a closed polygon, so I have set 
		the buffer to hold a maximum of 100 vertices per face. This may need to be increased later on, but so far I haven't
		seen a model that would need more than 100 vertices for a single face, but that could change.*/
		int spaceIndices[100];
		int spaceCount = 0;

		for (int j = faceIndices.arr[i]; j < fileSize && spaceCount < 100 && fileString[j] != '\n' && fileString[j] != '\0'; j++) {
			if (fileString[j] == ' ') {
				spaceIndices[spaceCount] = j;
				spaceCount++;
			}
		}

		unsigned int* extractedIndices = (unsigned int*)malloc(spaceCount * sizeof(unsigned int));
		if (extractedIndices == NULL) {
			printf("Failed to allocate memory for extracted indices in wavefrontobj loader\n");
			exit(-1);
		}

		for (int j = 0; j < spaceCount; j++) {
			char buff[20];
			int buffCount = 0;

			for (int k = spaceIndices[j] + 1; k < fileSize && buffCount < 19 && fileString[k] != '/' && fileString[k] != '\n' && fileString[k] != '\0'; k++) {
				buff[buffCount] = fileString[k];
				buffCount++;
			}

			buff[buffCount] = '\0';
			//Have to subtract one since obj file uses indices starting at 1 instead of 0
			extractedIndices[j] = (unsigned int)atoi(buff) - 1;
		}

		for (int j = 0; j < spaceCount - 2; j++) {
			uint_array_append(indices, extractedIndices[0]);
			uint_array_append(indices, extractedIndices[j + 1]);
			uint_array_append(indices, extractedIndices[j + 2]);
		}


		free(extractedIndices);
	}
	int_array_free(faceIndices);

	//------------------------------------------------------------------------------------------------------------------------

	free(fileString);
	fclose(fptr);
}