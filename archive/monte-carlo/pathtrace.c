// Include Standard Headers that we Need
#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Include Custom Utilities
#include "targa.h"
#include "rng.h"
#include "vectormath.h"

#define samples     (unsigned int) 1024U
#define resolutionx (unsigned int) 320U
#define resolutiony (unsigned int) 240U
#define maxmarches  (unsigned int) 128U
#define collisiondist 0.01
#define scenesize 32.0
#define camfov 1.0

// Classic "Unsafe" Minimum and Maximum Functions
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

float distestimator(vec3 pos){
    return vector3Length(pos)-0.5;
}

float light(vec3 pos){
    return vector3Length(vec3Sub(pos, float3(-2.0, 2.0, -2.0)))-0.5;
}

vec3 pathtrace(vec3 raydir, vec3 rayori){
    vec3 lastpos = rayori, raypos = rayori;
    float distest;
    for(unsigned int i = 0U; i < maxmarches; i++){
        if(vector3Length(raypos) > scenesize){break;}
        if(light(raypos) < collisiondist){
            return floatf3(1.0);
        }
        distest = distestimator(raypos);
        if(distest < collisiondist){
            // TODO: Spherically Uniform Random- Right now this is Basically a Cube which is Incorrect
            raydir = normalize(float3(rand()-0.5, rand()-0.5, rand()-0.5));
            raypos = lastpos;
        }
        lastpos = raypos;
        raypos = vec3Add(raypos, vec3Mult(raydir, floatf3(0.5*MIN(distest, light(raypos)))));
    }
    return floatf3(0.0);
}

int main(){
    // Set-Up Variables
    unsigned int resolutionmax = MAX(resolutionx, resolutiony), byte = 0U;
    vec3 raydir, outCol;
    float uvx, uvy;
    uint8_t imageBuffer[resolutionx * resolutiony * 3U] = {0U};

    // Execute Rendering
    for(unsigned int y = 0U; y < resolutiony; y++){
    for(unsigned int x = 0U; x < resolutionx; x++){
        // Monte-Carlo Rendering
        for(unsigned int sample = 0U; sample < samples; sample++){
            INIT_RNG;
            uvx = 2.0*((x+0.5)-0.5*resolutionx)/resolutionmax;
            uvy = 2.0*((y+0.5)-0.5*resolutiony)/resolutionmax;
            raydir = normalize(float3(uvx*camfov, uvy*camfov, 1.0));
            outCol = vec3Add(outCol, vec3Mult(pathtrace(raydir, float3(0.0, 0.0, -2.0)), floatf3(32.0)));
        }
        outCol = vec3Div(outCol, floatf3(samples));
        imageBuffer[byte   ] = MAX(0U, MIN(255U, round(outCol.x*255U)));
        imageBuffer[byte+1U] = MAX(0U, MIN(255U, round(outCol.y*255U)));
        imageBuffer[byte+2U] = MAX(0U, MIN(255U, round(outCol.z*255U)));
        byte += 3;
    }
    }

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