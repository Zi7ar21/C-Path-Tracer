#include <math.h>

// 3D Vector
typedef struct{
    float x;
    float y;
    float z;
} vec3;

// Convert 3 Floating-Point Numbers to a 3D Vector
vec3 float3(float x, float y, float z){
    vec3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
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