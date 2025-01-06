#ifndef GLBUFFERS_H
#define GLBUFFERS_H
#include "glad/glad.h"

typedef struct BufferObject {
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
} BufferObject;

#define bufferObjectInit(obj) \
glGenVertexArrays(1, &obj.vao); \
glBindVertexArray(obj.vao); \
glGenBuffers(1, &obj.vbo); \
glGenBuffers(1, &obj.ebo);

#define bufferObjectDelete(obj) \
glDeleteVertexArrays(1, &obj.vao); \
glDeleteBuffers(1, &obj.vbo); \
glDeleteBuffers(1, &obj.ebo); 

// Consider adding more functions here later on to make working with buffers nicer

#endif