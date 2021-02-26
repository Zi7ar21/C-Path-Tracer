#include <math.h>

// Classic "Unsafe" Minimum and Maximum Functions
// People on Stack Exchange said it was bad but it works Fine for me...
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

// Clamp Function
#define clamp(n, a, b) max(a, min(b, n))

// High-Quality Random Numbers from Michael0884, ported from: https://www.shadertoy.com/view/wltcRS
unsigned int ns;
#define INIT_RNG ns = 185730U*sample+(x+y*resolutionx);

void pcg(){
    unsigned int state = ns*747796405U+2891336453U;
    unsigned int word = ((state >> ((state >> 28U) + 4U)) ^ state)*277803737U;
    ns = (word >> 22U) ^ word;
}

float random(){
    pcg();
    return (float)ns/(float)0xffffffffU;
}

// 3D Vector
typedef struct{
    float x;
    float y;
    float z;
} vec3;

// 2D Vector
typedef struct{
    float x;
    float y;
} vec2;

// Convert 3 Floating-Point Numbers to a 3D Vector
vec3 float3(float x, float y, float z){
    vec3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
};

// Convert 3 Floating-Point Numbers to a 3D Vector
vec2 float2(float x, float y){
    vec2 vec;
    vec.x = x;
    vec.y = y;
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

// Multiply Two 3D Vectors
vec3 vec3Mult(vec3 x, vec3 y){
    vec3 vec;
    vec.x = x.x*y.x;
    vec.y = x.y*y.y;
    vec.z = x.z*y.z;
    return vec;
}

// Subtract Two 3D Vectors
vec3 vec3Div(vec3 x, vec3 y){
    vec3 vec;
    vec.x = x.x/y.x;
    vec.y = x.y/y.y;
    vec.z = x.z/y.z;
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

// Subtract Two 3D Vectors
vec3 vec3Sub(vec3 x, vec3 y){
    vec3 vec;
    vec.x = x.x-y.x;
    vec.y = x.y-y.y;
    vec.z = x.z-y.z;
    return vec;
}

// Find the Dot-Product of Two 3D Vectors
float vec3dotp(vec3 x, vec3 y){
    vec3 z = vec3Mult(x, y);
    return z.x+z.y+z.z;
}

// Find the Length of a Vector
float vector3Length(vec3 vec){
    return sqrtf((vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z));
}

// Normalize a Vector
vec3 normalize(vec3 vec){
    float length = vector3Length(vec);
    return float3(vec.x/length, vec.y/length, vec.z/length);
}

vec3 reflect(vec3 vector, vec3 normal){
    return vec3Sub(vector, vec3Mult(floatf3(2.0*vec3dotp(vector, normal)), normal));
}

// TODO: Sphereically Uniform Random (https://www.shadertoy.com/view/WttyWX)
//vec2 nrand2(float sigma, vec2 mean){vec2 Z = rand2(); return mean+sigma*sqrt(-2.0*log(Z.x))*vec2(cos(twopi*Z.y),sin(twopi*Z.y));}
//vec3 nrand3(float sigma, vec3 mean){vec4 Z = rand4(); return mean+sigma*sqrt(-2.0*log(Z.xxy))*vec3(cos(twopi*Z.z), sin(twopi*Z.z), cos(twopi*Z.w));}