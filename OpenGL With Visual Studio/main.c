#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "windowing.h"
#include "Shaders.h"
#include <stdio.h>
#include <stdbool.h>
#include "GLBuffers.h"
#include "Arrays.h"
#include "MeshLoaders.h"
#include "cglm.h"
#define _USE_MATH_DEFINES
#include <math.h>

void frame_buffer_size_callback(GLFWwindow* wind, int width, int height);

void generate_surface_normals(FloatArray* vertices, UIntArray* triangles, FloatArray* normals);

static Window window;

int main(void) {
	window = (Window){
	.width = 800,
	.height = 600,
	.glVersionMajor = 4,
	.glVersionMinor = 6,
	.glProfile = GLFW_OPENGL_CORE_PROFILE,
	.windPtr = NULL,
	.frame_buffer_callback_pointer = frame_buffer_size_callback
	};
	windowInitialize(&window);
	glfwSwapInterval(0);

	Shader shader;

	shaderProgramCreate(
		&shader, 
		"C:\\Users\\colec\\C Programs\\OpenGL With Visual Studio\\OpenGL With Visual Studio\\vert.glsl",
		"C:\\Users\\colec\\C Programs\\OpenGL With Visual Studio\\OpenGL With Visual Studio\\frag.glsl"
	);

	BufferObject obj;
	bufferObjectInit(obj);

	FloatArray vertices = FLOAT_ARRAY_DEFAULT;
	UIntArray indices = UINT_ARRAY_DEFAULT;
	loadWaveFrontOBJ(&vertices, &indices, "C:\\Users\\colec\\Blender Models\\torus.obj");

	FloatArray normals = FLOAT_ARRAY_DEFAULT;
	generate_surface_normals(&vertices, &indices, &normals);
	unsigned int normal_vbo;
	glGenBuffers(1, &normal_vbo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size * sizeof(unsigned int), indices.arr, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size * sizeof(float), vertices.arr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size * sizeof(float), normals.arr, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glUseProgram(shader);

	float currTime = 0, prevTime = 0, deltaTime = 0;

	mat4 model = GLM_MAT4_IDENTITY_INIT;
	mat4 view = GLM_MAT4_IDENTITY_INIT;
	mat4 projection;

	unsigned int modelLocation = glGetUniformLocation(shader, "model");
	unsigned int viewLocation = glGetUniformLocation(shader, "view");
	glm_translate(view, (vec3) { 0.0f, -12.0f, -44.0f });
	unsigned int projectionLocation = glGetUniformLocation(shader, "projection");

	glEnable(GL_DEPTH_TEST);

	float speed = 100.0f;

	while (!glfwWindowShouldClose(window.windPtr)) {
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;

		glm_perspective(M_PI / 4.0f, (float)window.width / (float)window.height, 0.1f, 1000.0f, projection);

		if (glfwGetKey(window.windPtr, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			glm_rotate(model, deltaTime, (vec3) { 0.0f, 1.0f, 0.0f });
		}
		else if (glfwGetKey(window.windPtr, GLFW_KEY_LEFT) == GLFW_PRESS) {
			glm_rotate(model, -deltaTime, (vec3) { 0.0f, 1.0f, 0.0f });
		}

		if (glfwGetKey(window.windPtr, GLFW_KEY_W) == GLFW_PRESS) {
			glm_translate(view, (vec3) { 0.0f, 0.0f, -speed * deltaTime });
		}
		else if (glfwGetKey(window.windPtr, GLFW_KEY_S) == GLFW_PRESS) {
			glm_translate(view, (vec3) { 0.0f, 0.0f, speed * deltaTime});
		}

		if (glfwGetKey(window.windPtr, GLFW_KEY_A) == GLFW_PRESS) {
			glm_translate(view, (vec3) { -speed * deltaTime, 0.0f, 0.0f });
		}
		else if (glfwGetKey(window.windPtr, GLFW_KEY_D) == GLFW_PRESS) {
			glm_translate(view, (vec3) { speed * deltaTime, 0.0f, 0.0f });
		}

		if (glfwGetKey(window.windPtr, GLFW_KEY_UP) == GLFW_PRESS) {
			glm_rotate(model, deltaTime, (vec3){1.0f, 0.0f, 0.0f});
		}
		else if (glfwGetKey(window.windPtr, GLFW_KEY_DOWN) == GLFW_PRESS) {
			glm_rotate(model, -deltaTime, (vec3) { 1.0f, 0.0f, 0.0f });
		}

		if (glfwGetKey(window.windPtr, GLFW_KEY_SPACE) == GLFW_PRESS) {
			glm_translate(view, (vec3) { 0.0f, speed* deltaTime, 0.0f });
		}
		else if (glfwGetKey(window.windPtr, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			glm_translate(view, (vec3) { 0.0f, -speed * deltaTime, 0.0f });
		}

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, (float*)model);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, (float*)view);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, (float*)projection);

		glClearColor(0.1, 0.5, 0.7, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window.windPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window.windPtr, true);
		}

		glDrawElements(GL_TRIANGLES, indices.size, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window.windPtr);
		glfwPollEvents();

		prevTime = currTime;
	}

	bufferObjectDelete(obj);
	glDeleteBuffers(1, &normal_vbo);
	glDeleteProgram(shader);

	return 0;
}

void frame_buffer_size_callback(GLFWwindow* wind, int width, int height) {
	glViewport(0, 0, width, height);
	window.height = height;
	window.width = width;
}

void generate_surface_normals(FloatArray* vertices, UIntArray* triangles, FloatArray* normals) {
	float_array_free((*normals));

	for (int i = 0; i < vertices->size; i++) {
		float_array_append(normals, 0.0f);
	}

	for (int i = 0; i < triangles->size; i += 3) {
		vec3 p1 = {
			vertices->arr[3 * triangles->arr[i]],
			vertices->arr[3 * triangles->arr[i] + 1],
			vertices->arr[3 * triangles->arr[i] + 2]
		};

		vec3 p2 = {
			vertices->arr[3 * triangles->arr[i + 1]],
			vertices->arr[3 * triangles->arr[i + 1] + 1],
			vertices->arr[3 * triangles->arr[i + 1] + 2]
		};

		vec3 p3 = {
			vertices->arr[3 * triangles->arr[i + 2]],
			vertices->arr[3 * triangles->arr[i + 2] + 1],
			vertices->arr[3 * triangles->arr[i + 2] + 2]
		};

		vec3 u, v;
		glm_vec3_sub(p2, p1, u);
		glm_vec3_sub(p3, p1, v);

		vec3 surface_normal = {
			(u[1] * v[2]) - (u[2] * v[1]),
			(u[2] * v[0]) - (u[0] * v[2]),
			(u[0] * v[1]) - (u[1] * v[0])
		};

		normals->arr[3 * triangles->arr[i]] += surface_normal[0];
		normals->arr[3 * triangles->arr[i] + 1] += surface_normal[1];
		normals->arr[3 * triangles->arr[i] + 2] += surface_normal[2];

		normals->arr[3 * triangles->arr[i + 1]] += surface_normal[0];
		normals->arr[3 * triangles->arr[i + 1] + 1] += surface_normal[1];
		normals->arr[3 * triangles->arr[i + 1] + 2] += surface_normal[2];

		normals->arr[3 * triangles->arr[i + 2]] += surface_normal[0];
		normals->arr[3 * triangles->arr[i + 2] + 1] += surface_normal[1];
		normals->arr[3 * triangles->arr[i + 2] + 2] += surface_normal[2];
	}

	for (int i = 0; i < triangles->size; i++) {
		vec3 normalized = { normals->arr[3 * triangles->arr[i]], normals->arr[3 * triangles->arr[i] + 1], normals->arr[3 * triangles->arr[i] + 2] };
		glm_normalize(normalized);
		normals->arr[3 * triangles->arr[i]] = normalized[0];
		normals->arr[3 * triangles->arr[i] + 1] = normalized[1];
		normals->arr[3 * triangles->arr[i] + 2] = normalized[2];
	}
}