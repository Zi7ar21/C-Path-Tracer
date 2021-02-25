#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define resolutionx (unsigned int) 1920
#define resolutiony (unsigned int) 1080
#define maxmarches  (unsigned int) 128
#define collisiondist 0.01
#define scenesize 32.0
#define camfov 1.0

// Classic "Unsafe" Minimum and Maximum Functions
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

// Endian-ifier
int little_endianify(int number){return number % 256;}
int big_endianify(int number){return number / 256;}

// Targa Image Format File Header
typedef struct{
    char id_length;
    char map_type;
    char image_type;
    int map_first;
    int map_length;
    char map_entry_size;
    int x;
    int y;
    int width;
    int height;
    char bits_per_pixel;
    char misc;
} targaHeader;

// Write the File Header
void write_header(targaHeader header, FILE *tga){
    fputc(header.id_length, tga);
    fputc(header.map_type, tga);
    fputc(header.image_type, tga);
    fputc(little_endianify(header.map_first), tga);
    fputc(big_endianify(header.map_first), tga);
    fputc(little_endianify(header.map_length), tga);
    fputc(big_endianify(header.map_length), tga);
    fputc(header.map_entry_size, tga);
    fputc(little_endianify(header.x), tga);
    fputc(big_endianify(header.x), tga);
    fputc(little_endianify(header.y), tga);
    fputc(big_endianify(header.y), tga);
    fputc(little_endianify(header.width), tga);
    fputc(big_endianify(header.width), tga);
    fputc(little_endianify(header.height), tga);
    fputc(big_endianify(header.height), tga);
    fputc(header.bits_per_pixel, tga);
    fputc(header.misc, tga);
}

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

// Add Two 3D Vectors
vec3 vec3Add(vec3 x, vec3 y){
    vec3 vec;
    vec.x = x.x+y.x;
    vec.y = x.y+y.y;
    vec.z = x.z+y.z;
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

float distestimator(vec3 pos){
    return vector3Length(pos)-0.5;
}

float shade(vec3 norm){
    return 0.5*vec3dotp(normalize(float3(-1.0, -1.0, 1.0)), norm)+0.5;
}

vec3 pathtrace(vec3 raydir, vec3 rayori){
    vec3 raypos = rayori;
    float distest;
    for(unsigned int i = 0; i < maxmarches; i++){
        if(vector3Length(raypos) > scenesize){break;}
        distest = distestimator(raypos);
        if(distest < collisiondist){
            return floatf3(shade(normalize(raypos)));
        }
        raypos = vec3Add(raypos, vec3Mult(raydir, floatf3(distest)));
    }
    return floatf3(0.0);
}

int main(){
    // Set-Up Variables
    unsigned int resolutionmax = MAX(resolutionx, resolutiony), byte = 0;
    vec3 raydir, outCol;
    float uvx, uvy;
    uint8_t imageBuffer[resolutionx * resolutiony * 3] = {0};

    // Execute Rendering
    for(unsigned int y = 0; y < resolutiony; y++){
        uvy = 2.0*((y+0.5)-0.5*resolutiony)/resolutionmax;
    for(unsigned int x = 0; x < resolutionx; x++){
        uvx = 2.0*((x+0.5)-0.5*resolutionx)/resolutionmax;
        raydir = normalize(float3(uvx*camfov, uvy*camfov, 1.0));
        outCol = pathtrace(raydir, float3(0.0, 0.0, -2.0));
        imageBuffer[byte  ] = MAX(0, MIN(255, round(outCol.x*255)));
        imageBuffer[byte+1] = MAX(0, MIN(255, round(outCol.y*255)));
        imageBuffer[byte+2] = MAX(0, MIN(255, round(outCol.z*255)));
        byte += 3;
    }
    }

    // Print that the Rendering is Complete
    printf("Done Rendering! Saving...\n");

    // Save the Image
    FILE * imageFile;
    imageFile = fopen ("render.tga", "wb");
    targaHeader header;
    header.id_length = 0;
    header.map_type = 0;
    header.image_type = 2;
    header.map_first = 0;
    header.map_length = 0;
    header.map_entry_size = 0;
    header.x = 0;
    header.y = 0;
    header.width = resolutionx;
    header.height = resolutiony;
    header.bits_per_pixel = 24;
    header.misc = 0x20;
    write_header(header, imageFile);
    fwrite(imageBuffer, sizeof(uint8_t), sizeof(imageBuffer), imageFile);
    fclose(imageFile);

    // Print that the Image is Saved
    printf("Done!\n");

    // Return Code 0
    return 0;
}