// Include Standard Headers that we Need
#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Include Custom Utilities
#include "targa.h"
#include "custommath.h"

// Parameters
#define samples     256U
#define resolutionx 640U
#define resolutiony 360U
#define maxmarches  128U
#define collisiondist 0.01
#define scenesize 32.0
#define camfov 1.0

float distestimator(vec3 pos){
    return vector3Length(pos)-0.5;
}

float light(vec3 pos){
    return vector3Length(vec3Sub(pos, float3(-2.0, 2.0, -2.0)))-0.5;
}

vec3 pathtrace(vec3 raydir, vec3 rayori){
    vec3 raypos = rayori, lastpos = rayori, normal;
    float distest;
    for(unsigned int i = 0U; i < maxmarches; i++){
        if(vector3Length(raypos) > scenesize){break;}
        if(light(raypos) < collisiondist){
            return floatf3(1.0);
        }
        distest = distestimator(raypos);
        if(distest < collisiondist){
            normal = normalize(raypos);
            // TODO: Spherically Uniform Random- Right now this is Basically a Cube which is Incorrect
            raydir = reflect(raydir, normalize(nrand3(1.0, normal)));
            raypos = lastpos;
        }
        lastpos = raypos;
        raypos = vec3Add(raypos, vec3Mult(raydir, floatf3(min(distest, light(raypos)))));
    }
    return floatf3(0.0);
}

int main(){
    // Set-Up Variables
    unsigned int resolutionmax = max(resolutionx, resolutiony), byte = 0U;
    vec3 raydir, normal, outCol;
    vec2 uv, ditheroffset;
    uint8_t imageBuffer[resolutionx * resolutiony * 3U] = {0U};

    // Execute Rendering
    for(unsigned int y = 0U; y < resolutiony; y++){
    for(unsigned int x = 0U; x < resolutionx; x++){
        // Monte-Carlo Rendering
        for(unsigned int sample = 0U; sample < samples; sample++){
            INIT_RNG;
            ditheroffset = nrand2(0.5, floatf2(0.0));
            uv = float2(2.0*(((x+0.5)+ditheroffset.x)-0.5*resolutionx)/resolutionmax, 2.0*(((y+0.5)+ditheroffset.y)-0.5*resolutiony)/resolutionmax);
            raydir = normalize(float3(uv.x*camfov, uv.y*camfov, 1.0));
            outCol = vec3Add(outCol, vec3Mult(pathtrace(raydir, float3(0.0, 0.0, -2.0)), floatf3(32.0)));
        }
        outCol = vec3Div(outCol, floatf3(samples));
        imageBuffer[byte   ] = clamp(round(outCol.z*255U), 0U, 255U);
        imageBuffer[byte+1U] = clamp(round(outCol.y*255U), 0U, 255U);
        imageBuffer[byte+2U] = clamp(round(outCol.x*255U), 0U, 255U);
        byte += 3U;
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