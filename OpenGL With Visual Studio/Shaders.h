#ifndef SHADERS_H
#define SHADERS_H

#include "glad/glad.h"

typedef unsigned int Shader; //A shader is just an unsigned int because it holds the identifier to the shader program

//Returns string of memory allocated on the heap, so it must be freed later by the reciever of the pointer
char* shaderFileToString(char* path);

// Creates the shader program given a pointer to a Shader struct and the paths to vertex and fragment shaders.
// Note: Add additional shader options later on like tessellation and geometry once more is learned about how those work
// Note: vertex and fragment shaders will always be required anyways
int shaderProgramCreate(Shader* program, char* vertexPath, char* fragmentPath);

#endif