#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float movement;

out vec3 pos;

out vec3 center;

out vec3 normals;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
	pos = position;
	center = vec4(view * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	normals = (model * vec4(normal, 1.0)).xyz;
}