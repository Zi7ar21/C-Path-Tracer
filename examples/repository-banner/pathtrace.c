// Include Standard Headers that we Need
#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Include Custom Utilities
#include "targa.h"
#include "custommath.h"

// Parameters
// Number of Path-Traced Samples
#define samples 512U

// Resolution
#define resolutionx 1280U
#define resolutiony 640U

// Maximum Ray-Marches
#define maxmarches 8192U

// Maximum Path Bounces
#define maxbounces 16U

// Distance counted as a collision
#define collisiondist 0.01f
#define lightcollisiondist 0.25f

// Ray-March Step Multiplier
#define marchprecision 0.5f

// Maximum Distance from the Origin
#define scenesize 4.0f

// Camera Field of View
#define camfov 1.0f

// Sphere Distance Estimator
float sphde(vec3 pos, vec3 sphpos, float sphrad){
    return vec3length(vec3Sub(pos, sphpos))-sphrad;
}

// Infinite Plane Distance Estimator
float planede(vec3 pos, float height){
    return pos.y-height;
}

// Scene Distance Estimator
float distestimator(vec3 pos){
    // Find the distance to all the objects in the scene
    float sphere = sphde(pos, floatf3(0.0f), 0.5f);
    float plane  = planede(pos, -0.5f);

    // Return the nearest object's distance
    return min(sphere, plane);
}

// Object Normal Lookup
vec3 normal(vec3 pos){
    // Find the object the Ray collided with
    float sphere = sphde(pos, floatf3(0.0f), 0.5f);
    float plane  = planede(pos, -0.5f);
    float minimumde = min(sphere, plane);

    // Return the corresponding normal
    if(minimumde == sphere){
        return normalize(pos);
    }

    if(minimumde == plane){
        return float3(0.0f, 1.0f, 0.0f);
    }

    // Return the undefined Normal (shouldn't happen, only to be on the safe side)
    return floatf3(0.0f);
}

// Material Lookup
vec4 material(vec3 pos){
    // Find the object the Ray collided with
    float sphere = sphde(pos, floatf3(0.0f), 0.5f);
    float plane  = planede(pos, -0.5f);
    float minimumde = min(sphere, plane);

    // Return the corresponding material
    if(minimumde == sphere){
        return float4(1.0f, 0.2f, 0.2f, 0.5f);
    }

    if(minimumde == plane){
        return float4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    // Return the undefined material (shouldn't happen, only to be on the safe side)
    return floatf4(0.0f);
}

// Fresnel Reflectance
vec3 fresnel(vec3 raydir, vec3 normal){
    // Specularity
    vec3 F0 = floatf3(0.8f);
    return vec3Add(F0, vec3Mult(vec3Sub(floatf3(1.0f), F0), floatf3(powf(1.0f-vec3dotp(vec3Mult(raydir, floatf3(-1.0f)), normal), 5.0f))));
}

// Light Distance Estimator
float light(vec3 pos){
    return vec3length(float3(pos.x*0.25f, 1.0f-pos.y, pos.z*0.25f))-0.125f;
}

// Scene Path-Tracing Function
vec3 pathtrace(vec3 raydir, vec3 rayori){
    // Set-Up Variables
    unsigned int bounces = 0U;
    vec3 raypos = rayori, lastpos = rayori, outCol = floatf3(1.0f), normals;
    vec4 materialprops;
    float distest;

    // Perform Ray-Marching
    for(unsigned int i = 0U; i < maxmarches; i++){
        // Check if our Ray escaped the scene
        if(vec3length(raypos) > scenesize){break;}

        // Check if our Ray bounced too many times
        if(bounces > maxbounces){return floatf3(0.0f);}

        // Check if we reached the light source
        if(light(raypos) < lightcollisiondist){
            return outCol;
        }

        // Find the distance from our Ray to the scene
        distest = distestimator(raypos);

        // If our Ray hit something, perform neccesary functions
        if(distest < collisiondist){
            normals = normal(raypos);
            materialprops = material(raypos);
            outCol = vec3Mult(outCol, vec3Mult(fresnel(raydir, normals), float3(materialprops.x, materialprops.y, materialprops.z)));
            // TODO: Spherically Uniform Random- Right now this is Basically a Cube which is Incorrect
            raydir = reflect(raydir, normalize(nrand3(materialprops.w, normals)));
            raypos = lastpos;
            bounces++;
        }

        // If our Ray didn't hit anything, move it forward as usual
        else{
            lastpos = raypos;
            raypos = vec3Add(raypos, vec3Mult(raydir, floatf3(marchprecision*min(distest, light(raypos)))));
        }
    }

    // Return the sample illuminated by the background color
    return vec3Mult(outCol, floatf3(0.5f));
}

int main(){
    // Set-Up Variables
    unsigned int resolutionmax = max(resolutionx, resolutiony), byte = 0U;
    vec3 raydir, normal, outCol;
    vec2 uv, ditheroffset;
    uint8_t imageBuffer[resolutionx * resolutiony * 3U] = {0U};

    // Execute Rendering
    // Inverted Y, Because Targa Sucks... This originally used PPM but I was convinced otherwise, oh how foolish of me... lol
    for(unsigned int y = resolutiony; y > 0U; y--){
    for(unsigned int x = 0U; x < resolutionx; x++){
        // Monte-Carlo Sampling
        for(unsigned int sample = 0U; sample < samples; sample++){
            INIT_RNG;
            ditheroffset = nrand2(0.5f, floatf2(0.0f));
            uv = float2(2.0f*(((x+0.5f)+ditheroffset.x)-0.5f*resolutionx)/resolutionmax, 2.0f*(((y+0.5f)+ditheroffset.y)-0.5f*resolutiony)/resolutionmax);
            raydir = normalize(float3(uv.x*camfov, uv.y*camfov, 1.0f));
            outCol = vec3Add(outCol, pathtrace(raydir, float3(0.0f, 0.0f, -2.0f)));
        }

        // Divide the sum to get the converged sample
        outCol = vec3Div(outCol, floatf3(samples));

        // Add the pixels to the buffer
        imageBuffer[byte   ] = clamp(round(outCol.z*255U), 0U, 255U);
        imageBuffer[byte+1U] = clamp(round(outCol.y*255U), 0U, 255U);
        imageBuffer[byte+2U] = clamp(round(outCol.x*255U), 0U, 255U);
        byte += 3U;

        // Update Progress Display Every 300 Bytes
        if(0U == byte % 300U){printf("\r%d", byte/((resolutionx*resolutiony*3U)/100U));}
    }
    }

    // Save the Image in the Targa format
    FILE * imageFile;
    imageFile = fopen ("render.tga", "wb");

    // Prepare the Targa Image Header
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

    // Write the Header to the File on the Disk
    write_header(header, imageFile);

    // Dump the Pixels to the File on the Disk
    fwrite(imageBuffer, sizeof(uint8_t), sizeof(imageBuffer), imageFile);
    fclose(imageFile);

    // Print that the Program Finished
    printf("\rDone!\n");

    // Return Code 0 which should mean that the program ran without a hitch
    return 0;
}