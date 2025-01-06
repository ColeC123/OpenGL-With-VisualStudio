#ifndef MESHLOADERS_H
#define MESHLOADERS_H

#include "Arrays.h"

//TODO: add support for vertex normals and vertex colors at a later time
//Note: contents of vertices and indices will be wiped before loading in the data from the obj file
void loadWaveFrontOBJ(FloatArray* vertices, UIntArray* indices, char* filePath);

#endif