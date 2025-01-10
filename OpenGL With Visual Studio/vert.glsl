#version 460 core

#define PI 3.14159265359
#define HALF_PI 1.57079632679
#define TAU 6.28318530718
#define PHI 1.61803398874989484820459

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float movement;
uniform float time;
uniform int vertSeed;

out vec3 pos;

out vec3 center;

out vec3 normals;

out float fragTime;

out flat int fragSeed;

//out float yOffset;

//Function retrieved from the link below
//https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
float rand(vec2 xy, int seed) {
    return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

vec2 randomUnitVec(vec2 xy, int seed) {
    float random = TAU * rand(xy, seed);
    //Have to do square root of random seed for more uniform distribution across the circle
    return sqrt(rand(xy.yx, seed)) * vec2(cos(random), sin(random));
}

float easingFunction(float value) {
    return (3 * value * value) - (2 * value * value * value);
}

float perlinNoise(vec2 xy, int seed) {
    vec2 xyClamped = floor(xy);

    vec2 blRand = randomUnitVec(xyClamped, seed);
    float bl = dot(blRand, xy - xyClamped);

    vec2 brRand = randomUnitVec(xyClamped + vec2(1.0, 0.0), seed);
    float br = dot(brRand, xy - (xyClamped + vec2(1.0, 0.0)));

    vec2 tlRand = randomUnitVec(xyClamped + vec2(0.0, 1.0), seed);
    float tl = dot(tlRand, xy - (xyClamped + vec2(0.0, 1.0)));

    vec2 trRand = randomUnitVec(xyClamped + vec2(1.0, 1.0), seed);
    float tr = dot(trRand, xy - (xyClamped + vec2(1.0, 1.0)));

    float horizontal = bl + (easingFunction(xy.x - xyClamped.x) * (br - bl));
    float vertical = tl + (easingFunction(xy.x - xyClamped.x) * (tr - tl));

    return horizontal + easingFunction(xy.y - xyClamped.y) * (vertical - horizontal);
}

void main() {
    float height1 = 100 * perlinNoise(0.01 * position.xy - 13.789, vertSeed) + 50 * perlinNoise(0.02 * position.zy + 7.896, vertSeed) + 25 * perlinNoise(0.04 * position.zx - 78.128321, vertSeed);

    pos = position + height1 * vec3(0.0, 1.0, 0.0);
	gl_Position = projection * view * model * vec4(pos, 1.0);
	center = vec4(view * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	normals = (model * vec4(normal, 1.0)).xyz;
	fragTime = time;
    fragSeed = vertSeed;
}
