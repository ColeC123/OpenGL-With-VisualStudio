#version 460 core

#define PI 3.14159265359
#define HALF_PI 1.57079632679
#define TAU 6.28318530718

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float movement;
uniform float time;

out vec3 pos;

out vec3 center;

out vec3 normals;

out float fragTime;

//out float yOffset;

float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 randomUnitVec(vec2 seed) {
    float random = TAU * rand(seed);
    //Have to do square root of random seed for more uniform distribution across the circle
    return sqrt(rand(seed.yx)) * vec2(cos(random), sin(random));
}

float easingFunction(float value) {
    return (3 * value * value) - (2 * value * value * value);
}

float perlinNoise(vec2 seed) {
    vec2 seedClamped = floor(seed);

    vec2 blRand = randomUnitVec(seedClamped);
    float bl = dot(blRand, seed - seedClamped);

    vec2 brRand = randomUnitVec(seedClamped + vec2(1.0, 0.0));
    float br = dot(brRand, seed - (seedClamped + vec2(1.0, 0.0)));

    vec2 tlRand = randomUnitVec(seedClamped + vec2(0.0, 1.0));
    float tl = dot(tlRand, seed - (seedClamped + vec2(0.0, 1.0)));

    vec2 trRand = randomUnitVec(seedClamped + vec2(1.0, 1.0));
    float tr = dot(trRand, seed - (seedClamped + vec2(1.0, 1.0)));

    float horizontal = bl + (easingFunction(seed.x - seedClamped.x) * (br - bl));
    float vertical = tl + (easingFunction(seed.x - seedClamped.x) * (tr - tl));

    return horizontal + easingFunction(seed.y - seedClamped.y) * (vertical - horizontal);
}

void main() {
	//vec3 adder = 5 * sin(0.1 * distance(position, vec3(0.0, 0.0, 0.0)) - time) * vec3(0.0, 1.0, 0.0);
    float height1 = 100 * perlinNoise(0.01 * position.xy) + 50 * perlinNoise(0.02 * position.zy) + 25 * perlinNoise(0.04 * position.zx);

    vec3 height = height1 * vec3(0.0, 1.0, 0.0);
	gl_Position = projection * view * model * vec4(position + height, 1.0);
	pos = position;
	center = vec4(view * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	normals = (model * vec4(normal, 1.0)).xyz;
	fragTime = time;
	//yOffset = adder.y;
}