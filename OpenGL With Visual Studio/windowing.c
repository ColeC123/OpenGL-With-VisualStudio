#include "windowing.h"s

//Initializes a window and also sets up all of the glad function pointers for opengl functions
int windowInitialize(Window* window) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, window->glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, window->glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, window->glProfile);

	window->windPtr = glfwCreateWindow(window->width, window->height, "OpenGL Program", NULL, NULL);
	if (window->windPtr == NULL) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window->windPtr);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize glad\n");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window->windPtr, window->frame_buffer_callback_pointer);
}