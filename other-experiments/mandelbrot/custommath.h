#include <math.h>

// Yummy Pi!
#define pi 3.1415926535897932384626433832795028841971693993751058209749445923078164
#define twopi pi*2.0

// Classic "Unsafe" Minimum and Maximum Functions
// People on Stack Exchange said it was bad but it works Fine for me...
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

// Clamp Function
#define clamp(n, a, b) max(a, min(b, n))

// High-Quality Random Numbers from Michael0884 (Ported from: https://www.shadertoy.com/view/wltcRS)
unsigned int ns;
#define INIT_RNG ns = 185730U*sample+(x+y*resolutionx);

// The PRNG Algorithm
void pcg(){
    unsigned int state = ns*747796405U+2891336453U;
    unsigned int word = ((state >> ((state >> 28U) + 4U)) ^ state)*277803737U;
    ns = (word >> 22U) ^ word;
}

// Random Float between 0.0 and 1.0
float random(){
    pcg();
    return (float)ns/(float)0xffffffffU;
}

// 2D Vector
typedef struct{
    float x;
    float y;
} vec2;

// 3D Vector
typedef struct{
    float x;
    float y;
    float z;
} vec3;

// Double Precision 2D Vector
typedef struct{
    double x;
    double y;
} dvec2;

// Double Precision 3D Vector
typedef struct{
    double x;
    double y;
    double z;
} dvec3;

// Convert 2 Floating-Point Numbers to a 2D Vector
vec2 float2(float x, float y){
    vec2 vec;
    vec.x = x;
    vec.y = y;
    return vec;
};

// Convert 3 Floating-Point Numbers to a 3D Vector
vec3 float3(float x, float y, float z){
    vec3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
};

// Convert a Floating-Point Number to a 2D Vector
vec2 floatf2(float x){
    vec2 vec;
    vec.x = x;
    vec.y = x;
    return vec;
};

// Convert a Floating-Point Number to a 3D Vector
vec3 floatf3(float x){
    vec3 vec;
    vec.x = x;
    vec.y = x;
    vec.z = x;
    return vec;
};

// Multiply Two 2D Vectors
vec2 vec2Mult(vec2 x, vec2 y){
    vec2 vec;
    vec.x = x.x*y.x;
    vec.y = x.y*y.y;
    return vec;
}

// Multiply a 2D Vector by a Float
vec2 vec2Multf(vec2 x, float y){
    vec2 vec;
    vec.x = x.x*y;
    vec.y = x.y*y;
    return vec;
}

// Multiply Two 3D Vectors
vec3 vec3Mult(vec3 x, vec3 y){
    vec3 vec;
    vec.x = x.x*y.x;
    vec.y = x.y*y.y;
    vec.z = x.z*y.z;
    return vec;
}

// Multiply a 3D Vector by a Float
vec3 vec3Multf(vec3 x, float y){
    vec3 vec;
    vec.x = x.x*y;
    vec.y = x.y*y;
    vec.z = x.z*y;
    return vec;
}

// Divide Two 2D Vectors
vec2 vec2Div(vec2 x, vec2 y){
    vec2 vec;
    vec.x = x.x/y.x;
    vec.y = x.y/y.y;
    return vec;
}

// Divide a 2D Vector by a Float
vec2 vec2Divf(vec2 x, float y){
    vec2 vec;
    vec.x = x.x/y;
    vec.y = x.y/y;
    return vec;
}

// Divide Two 3D Vectors
vec3 vec3Div(vec3 x, vec3 y){
    vec3 vec;
    vec.x = x.x/y.x;
    vec.y = x.y/y.y;
    vec.z = x.z/y.z;
    return vec;
}

// Divide a 3D Vector by a Float
vec3 vec3Divf(vec3 x, float y){
    vec3 vec;
    vec.x = x.x/y;
    vec.y = x.y/y;
    vec.z = x.z/y;
    return vec;
}

// Add Two 2D Vectors
vec2 vec2Add(vec2 x, vec2 y){
    vec2 vec;
    vec.x = x.x+y.x;
    vec.y = x.y+y.y;
    return vec;
}

// Add a Float to a 2D Vector
vec2 vec2Addf(vec2 x, float y){
    vec2 vec;
    vec.x = x.x+y;
    vec.y = x.y+y;
    return vec;
}

// Add Two 3D Vectors
vec3 vec3Add(vec3 x, vec3 y){
    vec3 vec;
    vec.x = x.x+y.x;
    vec.y = x.y+y.y;
    vec.z = x.z+y.z;
    return vec;
}

// Subtract Two 2D Vectors
vec2 vec2Sub(vec2 x, vec2 y){
    vec2 vec;
    vec.x = x.x-y.x;
    vec.y = x.y-y.y;
    return vec;
}

// Subtract Two 3D Vectors
vec3 vec3Sub(vec3 x, vec3 y){
    vec3 vec;
    vec.x = x.x-y.x;
    vec.y = x.y-y.y;
    vec.z = x.z-y.z;
    return vec;
}

// Random Vectors
vec2 rand2(){return float2(random(), random());}

// Sphereically Uniform Random Functions (Ported from: https://www.shadertoy.com/view/WttyWX)
vec2 nrand2(float sigma, vec2 mean){
    vec2 Z = rand2();
    return vec2Add(mean, vec2Multf(float2(cos(twopi*Z.y), sin(twopi*Z.y)), sigma*sqrtf(-2.0f*logf(Z.x))));
}