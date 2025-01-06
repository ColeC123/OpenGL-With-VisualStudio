#ifndef WINDOWING_H
#define WINDOWING_H
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdio.h>

typedef struct Window {
	char* title;
	int width;
	int height;
	GLFWwindow* windPtr;
	int glVersionMajor;
	int glVersionMinor;
	int glProfile;
	void (*frame_buffer_callback_pointer)(GLFWwindow*, int, int);
} Window;

int windowInitialize(Window* window);

#endif