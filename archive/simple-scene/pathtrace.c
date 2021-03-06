// Include Standard Headers that we Need
#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Include Custom Utilities
#include "targa.h"
#include "custommath.h"

// Parameters
#define samples     2048U
#define resolutionx 640U
#define resolutiony 360U
#define maxmarches  512U
#define collisiondist 0.01f
#define scenesize 2.5f
#define camfov 1.0f

float distestimator(vec3 pos){
    float sphere = vector3Length(pos)-0.5f;
    float plane  = pos.y+0.5f;
    return min(sphere, plane);
}

vec3 normal(vec3 pos){
    float sphere = vector3Length(pos)-0.5f;
    float plane  = pos.y+0.5f;
    float minimumde = min(sphere, plane);
    if(minimumde == sphere){
        return normalize(pos);
    }
    if(minimumde == plane){
        return float3(0.0f, 1.0f, 0.0f);
    }
    return floatf3(0.0f);
}

vec4 material(vec3 pos){
    float sphere = vector3Length(pos)-0.5f;
    float plane  = pos.y+0.5f;
    float minimumde = min(sphere, plane);
    if(minimumde == sphere){
        return float4(1.0f, 0.2f, 0.2f, 0.5f);
    }
    if(minimumde == plane){
        return float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    return floatf4(0.0f);
}

vec3 fresnel(vec3 raydir, vec3 normal){
    vec3 F0 = floatf3(0.8f);
    return vec3Add(F0, vec3Mult(vec3Sub(floatf3(1.0f), F0), floatf3(powf(1.0f-vec3dotp(vec3Mult(raydir, floatf3(-1.0f)), normal), 5.0f))));
}

float light(vec3 pos){
    return vector3Length(vec3Sub(pos, float3(-1.0f, 1.0f, -1.0f)))-0.25f;
}

vec3 pathtrace(vec3 raydir, vec3 rayori){
    vec3 raypos = rayori, lastpos = rayori, outCol = floatf3(1.0f), normals;
    vec4 materialprops;
    float distest;
    for(unsigned int i = 0U; i < maxmarches; i++){
        if(vector3Length(raypos) > scenesize){break;}
        if(light(raypos) < collisiondist+0.25f){
            return vec3Mult(outCol, floatf3(8.0f));
        }
        distest = distestimator(raypos);
        if(distest < collisiondist){
            normals = normal(raypos);
            materialprops = material(raypos);
            outCol = vec3Mult(outCol, vec3Mult(fresnel(raydir, normals), float3(materialprops.x, materialprops.y, materialprops.z)));
            // TODO: Spherically Uniform Random- Right now this is Basically a Cube which is Incorrect
            raydir = reflect(raydir, normalize(nrand3(materialprops.w, normals)));
            raypos = lastpos;
        }
        else{
            lastpos = raypos;
            raypos = vec3Add(raypos, vec3Mult(raydir, floatf3(0.5f*min(distest, light(raypos)))));
        }
    }
    return vec3Mult(outCol, floatf3(0.5f));
}

int main(){
    // Set-Up Variables
    unsigned int resolutionmax = max(resolutionx, resolutiony), byte = 0U;
    vec3 raydir, normal, outCol;
    vec2 uv, ditheroffset;
    uint8_t imageBuffer[resolutionx * resolutiony * 3U] = {0U};

    // Execute Rendering
    // Inverted Y, Because Targa Sucks... THis originally used PPM but I was convinced otherwise, oh how foolish of me... lol
    for(unsigned int y = resolutiony; y > 0U; y--){
    for(unsigned int x = 0U; x < resolutionx; x++){
        // Monte-Carlo Rendering
        for(unsigned int sample = 0U; sample < samples; sample++){
            INIT_RNG;
            ditheroffset = nrand2(0.5f, floatf2(0.0f));
            uv = float2(2.0f*(((x+0.5f)+ditheroffset.x)-0.5f*resolutionx)/resolutionmax, 2.0f*(((y+0.5f)+ditheroffset.y)-0.5f*resolutiony)/resolutionmax);
            raydir = normalize(float3(uv.x*camfov, uv.y*camfov, 1.0f));
            outCol = vec3Add(outCol, pathtrace(raydir, float3(0.0f, 0.0f, -2.0f)));
        }
        outCol = vec3Div(outCol, floatf3(samples));
        imageBuffer[byte   ] = clamp(round(outCol.z*255U), 0U, 255U);
        imageBuffer[byte+1U] = clamp(round(outCol.y*255U), 0U, 255U);
        imageBuffer[byte+2U] = clamp(round(outCol.x*255U), 0U, 255U);
        byte += 3U;
        // Update Progress Every 30 Bytes
        if(0U == byte % 30U){printf("\r%d", byte/((resolutionx*resolutiony*3U)/100U));}
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
    printf("\rDone!\n");

    // Return Code 0
    return 0;
}