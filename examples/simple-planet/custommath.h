#include <math.h>

// ----- Constants -----
// Yummy Pi!
#define pi 3.1415926535897932384626433832795028841971693993751058209749445923078164
#define twopi pi*2.0

// ----- Math Utilities -----
// Classic "Unsafe" Minimum and Maximum Functions
// People on Stack Exchange said it was bad but it works Fine for me...
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

// Clamp Function
#define clamp(n, a, b) max(a, min(b, n))

// ----- Vector Utilities -----
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

// 4D Vector
typedef struct{
    float x;
    float y;
    float z;
    float w;
} vec4;

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

// Convert 4 Floating-Point Numbers to a 4D Vector
vec4 float4(float x, float y, float z, float w){
    vec4 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    vec.w = w;
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

// Convert a Floating-Point Number to a 4D Vector
vec4 floatf4(float x){
    vec4 vec;
    vec.x = x;
    vec.y = x;
    vec.z = x;
    vec.w = x;
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

// Add a Float to a 3D Vector
vec3 vec3Addf(vec3 x, float y){
    vec3 vec;
    vec.x = x.x+y;
    vec.y = x.y+y;
    vec.z = x.z+y;
    return vec;
}

// Subtract Two 2D Vectors
vec2 vec2Sub(vec2 x, vec2 y){
    vec2 vec;
    vec.x = x.x-y.x;
    vec.y = x.y-y.y;
    return vec;
}

// Subtract a Float from a 2D Vector
vec2 vec2Subf(vec2 x, float y){
    vec2 vec;
    vec.x = x.x-y;
    vec.y = x.y-y;
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

// Subtract a Float from a 3D Vector
vec3 vec3Subf(vec3 x, float y){
    vec3 vec;
    vec.x = x.x-y;
    vec.y = x.y-y;
    vec.z = x.z-y;
    return vec;
}

// Clamp a 3D Vector by 2 3D Vectors
vec3 vec3clamp(vec3 x, vec3 y, vec3 z){
    vec3 vec;
    vec.x = clamp(x.x, y.x, z.x);
    vec.y = clamp(x.y, y.y, z.y);
    vec.z = clamp(x.z, y.z, z.z);
    return vec;
}

// Clamp a 3D Vector by a Float
vec3 vec3clampf(vec3 x, float y, float z){
    vec3 vec;
    vec.x = clamp(x.x, y, z);
    vec.y = clamp(x.y, y, z);
    vec.z = clamp(x.z, y, z);
    return vec;
}

// Find the Square Root of a 3D Vector
vec3 vec3sqrt(vec3 vec){
    vec.x = sqrtf(vec.x);
    vec.y = sqrtf(vec.y);
    vec.z = sqrtf(vec.z);
    return vec;
}

// Find the Logarithm of a 3D Vector
vec3 vec3log(vec3 vec){
    vec.x = logf(vec.x);
    vec.y = logf(vec.y);
    vec.z = logf(vec.z);
    return vec;
}

// Find the Dot-Product of Two 3D Vectors
float vec3dotp(vec3 x, vec3 y){
    vec3 z = vec3Mult(x, y);
    return z.x+z.y+z.z;
}

// Find the Floor of a 3D Vector
vec3 vec3floor(vec3 vec){
    vec.x = floorf(vec.x);
    vec.y = floorf(vec.y);
    vec.z = floorf(vec.z);
    return vec;
}

// Find the Fract of a 3D Vector
vec3 vec3fract(vec3 vec){
    vec3 vecFloor = vec3floor(vec);
    return float3(vec.x-vecFloor.x, vec.y-vecFloor.y, vec.z-vecFloor.z);
}

// Find the Length of a Vector
float vec3length(vec3 vec){
    return sqrtf(vec3dotp(vec, vec));
}

// Raise a 3D Vector to a Power
vec3 vec3pow(vec3 vec, float nthpower){
    vec.x = powf(vec.x, nthpower);
    vec.y = powf(vec.y, nthpower);
    vec.z = powf(vec.z, nthpower);
    return vec;
}

// Normalize a 3D Vector
vec3 normalize(vec3 vec){
    float length = vec3length(vec);
    return float3(vec.x/length, vec.y/length, vec.z/length);
}

// Reflect a 3D Vector along a Normal
vec3 reflect(vec3 vector, vec3 normal){
    return vec3Sub(vector, vec3Multf(normal, 2.0f*vec3dotp(vector, normal)));
}

// ----- Material Utilities -----
// Material Data Type
typedef struct{
    vec3 col;
    vec3 spec;
    vec3 norm;
    float rough;
} material;

// Create a Material
material mat(vec3 color, vec3 specular, vec3 normal, float roughness){
    material materialProperties;
    materialProperties.col   = color;
    materialProperties.spec  = specular;
    materialProperties.norm  = normal;
    materialProperties.rough = roughness;
    return materialProperties;
};

// Fresnel Reflectance
vec3 fresnel(vec3 raydir, vec3 normal, vec3 F0){
    return vec3Add(F0, vec3Multf(vec3Sub(floatf3(1.0f), F0), powf(1.0f-vec3dotp(vec3Multf(raydir, -1.0f), normal), 5.0f)));
}

// ----- RNG Utilities -----
// Hash: https://www.shadertoy.com/view/4djSRW
vec3 hash(vec3 pos){
    pos = vec3fract(vec3Mult(pos, float3(0.1031f, 0.1030f, 0.0973f)));
    pos = vec3Addf(pos, vec3dotp(pos, vec3Addf(float3(pos.y, pos.x, pos.z), 33.33f)));
    return vec3fract(vec3Mult(vec3Add(float3(pos.x, pos.x, pos.y), float3(pos.y, pos.x, pos.x)), float3(pos.z, pos.y, pos.x)));
}

// Gradient Noise: https://www.iquilezles.org/www/articles/gradientnoise/gradientnoise.htm
float noise(vec3 x){
    vec3 p   = vec3floor(x);
    vec3 w   = vec3fract(x);
    vec3 u   = vec3Mult(vec3Mult(vec3Mult(w, w), w), vec3Addf(vec3Mult(w, vec3Subf(vec3Multf(w, 6.0f), 15.0f)), 10.0f));
    vec3 ga  = hash(vec3Add(p, float3(0.0f, 0.0f, 0.0f)));
    vec3 gb  = hash(vec3Add(p, float3(1.0f, 0.0f, 0.0f)));
    vec3 gc  = hash(vec3Add(p, float3(0.0f, 1.0f, 0.0f)));
    vec3 gd  = hash(vec3Add(p, float3(1.0f, 1.0f, 0.0f)));
    vec3 ge  = hash(vec3Add(p, float3(0.0f, 0.0f, 1.0f)));
    vec3 gf  = hash(vec3Add(p, float3(1.0f, 0.0f, 1.0f)));
    vec3 gg  = hash(vec3Add(p, float3(0.0f, 1.0f, 1.0f)));
    vec3 gh  = hash(vec3Add(p, float3(1.0f, 1.0f, 1.0f)));
    float va = vec3dotp(ga, vec3Sub(w, float3(0.0f, 0.0f, 0.0f)));
    float vb = vec3dotp(gb, vec3Sub(w, float3(1.0f, 0.0f, 0.0f)));
    float vc = vec3dotp(gc, vec3Sub(w, float3(0.0f, 1.0f, 0.0f)));
    float vd = vec3dotp(gd, vec3Sub(w, float3(1.0f, 1.0f, 0.0f)));
    float ve = vec3dotp(ge, vec3Sub(w, float3(0.0f, 0.0f, 1.0f)));
    float vf = vec3dotp(gf, vec3Sub(w, float3(1.0f, 0.0f, 1.0f)));
    float vg = vec3dotp(gg, vec3Sub(w, float3(0.0f, 1.0f, 1.0f)));
    float vh = vec3dotp(gh, vec3Sub(w, float3(1.0f, 1.0f, 1.0f)));
    return va+
    u.x*(vb-va)+
    u.y*(vc-va)+
    u.z*(ve-va)+
    u.x*u.y*(va-vb-vc+vd)+
    u.y*u.z*(va-vc-ve+vg)+
    u.z*u.x*(va-vb-ve+vf)+
    u.x*u.y*u.z*(-va+vb+vc-vd+ve-vf-vg+vh);
}

// High-Quality Random Numbers from Michael0884 (Ported from: https://www.shadertoy.com/view/wltcRS)
unsigned int ns;
#define INIT_RNG ns = 185730U * sample + (x + y * resolutionx);

// The PRNG Algorithm
void pcg(){
    unsigned int state = ns*747796405U+2891336453U;
    unsigned int word = ((state >> ((state >> 28U) + 4U)) ^ state) * 277803737U;
    ns = (word >> 22U) ^ word;
}

// Random Float between 0.0 and 1.0
float random(){
    pcg();
    return (float)ns/(float)0xffffffffU;
}

// Random Vectors
vec2 rand2(){return float2(random(), random());}
vec3 rand3(){return float3(random(), random(), random());}
vec4 rand4(){return float4(random(), random(), random(), random());}

// Sphereically Uniform Random Functions (Ported from: https://www.shadertoy.com/view/WttyWX)
vec2 nrand2(float sigma, vec2 mean){
    vec2 Z = rand2();
    return vec2Add(mean, vec2Multf(float2(cos(twopi*Z.y), sin(twopi*Z.y)), sigma*sqrtf(-2.0f*logf(Z.x))));
}

vec3 nrand3(float sigma, vec3 mean){
    vec4 Z = rand4();
    vec3 trig = float3(cos(twopi*Z.z), sin(twopi*Z.z), cos(twopi*Z.w));
    vec3 swiz = float3(Z.x, Z.x, Z.y);
    return vec3Add(mean, vec3Mult(vec3Multf(vec3sqrt(vec3Multf(vec3log(swiz), -2.0f)), sigma), trig));
}