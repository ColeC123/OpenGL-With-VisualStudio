#version 460 core

#define PI 3.14159265359
#define HALF_PI 1.57079632679
#define TAU 6.28318530718
#define PHI 1.61803398874

in vec3 pos;
in vec3 center;
in vec3 normals;

//in float yOffset;
in float fragTime;
in flat int fragSeed;

out vec4 FragColor;

//Function retrieved from the link below
//https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
//Note: the seed is an integer because floating point imprecision results in weird flickering. 
//This is becuase the float for seed may be close to what the seed actually is, but not quite equal to it, and the small change can cause
//inconsistencies that result in the output of rand changing a lot
float rand(vec2 xy, int seed) {
    //I had to add the modulus operator inside of the tangent function because it would cause errors if the number became too large
    return fract(tan(mod(distance(xy*PHI, xy)*seed, 500))*xy.x);
}

vec2 randomUnitVec(vec2 xy, int seed) {
    float random = TAU * rand(xy, seed);
    //Have to do square root of random seed for more uniform distribution across the circle
    return sqrt(rand(xy.yx, seed)) * vec2(cos(random), sin(random));
}

float easingFunction(float value) {
    value = clamp(value, 0.0, 1.0);
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
    float p1 = ( perlinNoise(pos.yx * 0.05 - fragTime, fragSeed) + perlinNoise(pos.xz * 0.05 + fragTime, fragSeed) + 2.0) / 4.0 + 0.35;

    float p2 = ((perlinNoise(pos.zx * 0.4 - 1.5 * fragTime, fragSeed) + perlinNoise(pos.xy * 0.4 + 1.5 * fragTime, fragSeed) + 2.0) / 4.0) * easingFunction(pos.y * 0.05);
    
	FragColor.rgb = p2 * vec3(1.0, 1.0, 1.0) + p1 * vec3(0.0, 0.98, 1.0);
    FragColor.a = 1.0;
    FragColor = clamp(FragColor, 0.0, 1.0);
}
