#version 460 core

#define PI 3.14159265359
#define HALF_PI 1.57079632679
#define TAU 6.28318530718

in vec3 pos;
in vec3 center;
in vec3 normals;

//in float yOffset;
in float fragTime;

out vec4 FragColor;

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
    float p1, p2, p3;
    p1 = 0.5 * perlinNoise(0.1 * pos.xy) + 0.5 + 0.1 * perlinNoise(pos.yx);
    p2 = 0.5 * perlinNoise(0.1 * pos.xz) + 0.5 + 0.1 * perlinNoise(pos.zx);
    p3 = 0.5 * perlinNoise(0.1 * pos.yz) + 0.5 + 0.1 * perlinNoise(pos.zy);
	FragColor = vec4(p1, p2, p3, 1.0);
}
