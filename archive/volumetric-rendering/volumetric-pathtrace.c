// Include Standard Headers that we Need
#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Include Custom Utilities
#include "targa.h"
#include "custommath.h"

// Parameters
// Camera Field of View
#define camfov 1.0f

// Number of Path-Traced Samples
#define samples 1024U

// Resolution
#define resolutionx 640U
#define resolutiony 360U

// Maximum Ray-Marches
#define maxmarches 1024U

// Maximum Path Bounces
#define maxbounces 32U

// Maximum Sample Brightness
#define maxluminance 10.0f

// Ray-March Step Multiplier
#define stepsize 0.125f

// Maximum Distance from the Origin
#define scenesize 4.0f

// Volumetric Density Function
vec4 densityfunction(vec3 pos){
    return float4(1.0f, 1.0f, 1.0f, max((0.5f-vec3length(pos))*128.0f, 0.0f));
}

// Light
float light(vec3 pos){
    return vec3length(vec3Sub(pos, float3(-1.0f, 1.0f, 1.0f)))-0.5f;
}

// Scene Path-Tracing Function
vec3 pathtrace(vec3 raydir, vec3 rayori){
    // Set-Up Variables
    unsigned int bounces = 0U;
    float absorbance, distancetravelled;
    vec3 raypos = vec3Add(rayori, vec3Multf(raydir, stepsize*random())), attenuation = floatf3(1.0f);
    vec4 density;

    // Perform Ray-Marching
    for(unsigned int i = 0U; i < maxmarches; i++){
        // Check if we reached the light source
        if(light(raypos) < 0.0f){
            return vec3Multf(attenuation, 8.0f);
        }

        // Check if our Ray escaped the scene
        if(vec3length(raypos) > scenesize){break;}

        // Check if our Ray bounced too many times
        if(bounces > maxbounces){return floatf3(0.0f);}

        // Find the density of the point
        density = densityfunction(raypos);
        absorbance = expf(-density.w*stepsize);

        // If our Ray decides to bounce, perform neccesary functions
        if(absorbance < random()){
            attenuation = vec3Mult(attenuation, float3(clamp(density.x*32.0f, 0.0f, 1.0f), clamp(density.y*32.0f, 0.0f, 1.0f), clamp(density.z*32.0f, 0.0f, 1.0f)));
            raydir = normalize(nrand3(1.0f, floatf3(0.0f)));
        }

        raypos = vec3Add(raypos, vec3Multf(raydir, stepsize));
    }

    // Return the sample illuminated by the background color
    return floatf3(0.0f);
}

int main(){
    // Set-Up Variables
    const unsigned int resolutionmax = max(resolutionx, resolutiony);
    unsigned int pixel = 0U;
    const vec2 resolutionxy = float2(resolutionx, resolutiony);
    vec2 uv;
    vec3 raydir, normal, outCol;

    // Image Buffers
    vec3 pixels[resolutionx*resolutiony] = {floatf3(0.0f)};
    uint8_t imageBuffer[resolutionx*resolutiony*3U] = {0U};

    // Execute Rendering
    // Inverted Y, Because Targa Sucks... This originally used PPM but I was convinced otherwise, oh how foolish of me... lol
    for(unsigned int y = resolutiony; y > 0U; y--){
    for(unsigned int x = 0U; x < resolutionx; x++){
        // Monte-Carlo Sampling
        for(unsigned int sample = 0U; sample < samples; sample++){
            INIT_RNG;
            uv = vec2Divf(vec2Multf(vec2Sub(nrand2(0.5f, vec2Addf(float2(x, y), 0.5f)), vec2Multf(resolutionxy, 0.5f)), 2.0f), resolutionmax);
            raydir = normalize(float3(uv.x*camfov, uv.y*camfov, 1.0f));
            outCol = vec3Add(outCol, pathtrace(raydir, float3(0.0f, 0.0f, -2.0f)));
        }

        // Divide the sum to get the converged sample
        outCol = vec3Divf(outCol, samples);
        // Add the pixels to the buffer
        pixels[pixel] = float3(clamp(outCol.x, 0.0f, maxluminance), clamp(outCol.y, 0.0f, maxluminance), clamp(outCol.z, 0.0f, maxluminance));

        // Update progress display every n Pixels
        if(0U == pixel % 10U){printf("\r%d%%", pixel/(resolutionx*resolutiony/100U));}
        pixel++;
    }
    }

    // Convert our High-Precision Float Buffer to Dumpable Bytes
    for(pixel = 0U; pixel < resolutionx*resolutiony; pixel++){
        imageBuffer[pixel*3U   ] = clamp(round(pixels[pixel].z*255U), 0U, 255U);
        imageBuffer[pixel*3U+1U] = clamp(round(pixels[pixel].y*255U), 0U, 255U);
        imageBuffer[pixel*3U+2U] = clamp(round(pixels[pixel].x*255U), 0U, 255U);
    }

    // Save the Image in the Targa format
    FILE * imageFile;
    imageFile = fopen ("volumerender.tga", "wb");

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