#version 460 core

#define PI 3.14159265359
#define HALF_PI 1.57079632679
#define TAU 6.28318530718

in vec3 pos;
in vec3 center;
in vec3 normals;

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
    /*float p1, p2, p3;
    p1 = 0.5 * perlinNoise(pos.xy) + 0.5;
    p2 = 0.5 * perlinNoise(pos.xz) + 0.5;
    p3 = 0.5 * perlinNoise(pos.yz) + 0.5;
	FragColor = vec4(p1, p2, p3, 1.0);*/
    vec3 posNormals = abs(normals);
    float temp = (posNormals.x + posNormals.y + posNormals.z) / 3.0 + 0.5;
    temp = pow(temp, 20);
    temp = clamp(temp, 0.0, 1.0);
    FragColor = clamp(temp * vec4(0.45098039215, 0.36470588235, 0.64705882352, 0.0) + (1.0 - temp) * vec4(0.82745098039, 0.7725490196, 0.89803921568, 0.0), 0.0, 1.0);
    FragColor.a = 1.0;
}
