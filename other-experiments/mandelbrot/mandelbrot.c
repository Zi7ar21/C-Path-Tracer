// Include Standard Headers that we Need
#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Include Custom Utilities
#include "targa.h"
#include "custommath.h"

// Parameters

// Number of Samples
#define samples 128U

// Resolution
#define resolutionx 640U
#define resolutiony 480U

// Iterations
#define iterations 32U

vec3 mandelbrot(vec2 c){
    vec2 z2 = floatf2(0.0f);
    for(unsigned int i = 0U; i < iterations; i++){
        z2 = float2(z2.x*z2.x-z2.y*z2.y, z2.y*z2.x+z2.x*z2.y);
        z2 = vec2Add(z2, c);
        if(sqrtf(z2.x*z2.x+z2.y*z2.y) > 4.0f){return floatf3(0.0f);}
    }
    return floatf3(1.0f);
}

int main(){
    // Set-Up Variables
    const unsigned int resolutionmax = max(resolutionx, resolutiony);
    unsigned int pixel = 0U;
    const vec2 resolutionxy = float2(resolutionx, resolutiony);
    vec2 uv, ditheroffset;
    vec3 outCol;

    // Image Buffers
    vec3 pixels[resolutionx*resolutiony] = {floatf3(0.0f)};

    // Execute Rendering
    for(unsigned int y = resolutiony; y > 0U; y--){
    for(unsigned int x = 0U; x < resolutionx; x++){
        // Monte-Carlo Sampling
        for(unsigned int sample = 0U; sample < samples; sample++){
            INIT_RNG;
            ditheroffset = nrand2(0.5f, floatf2(0.0f));
            uv = vec2Divf(vec2Multf(vec2Sub(nrand2(0.5f, vec2Addf(float2(x, y), 0.5f)), vec2Multf(resolutionxy, 0.5f)), 2.0f), resolutionmax);
            outCol = vec3Add(outCol, mandelbrot(vec2Multf(uv, 2.0f)));
        }

        // Divide the sum to get the converged sample
        outCol = vec3Divf(outCol, samples);

        // Add the pixels to the buffer
        pixels[pixel] = float3(max(outCol.x, 0.0f), max(outCol.y, 0.0f), max(outCol.z, 0.0f));

        // Update progress display every n Pixels
        if(0U == pixel % 10U){printf("\r%d%%", pixel/(resolutionx*resolutiony/100U));}
        pixel++;
    }
    }

    // Make our Image Buffer
    uint8_t imageBuffer[resolutionx*resolutiony*3U] = {0U};

    // Convert our High-Precision Float Buffer to Dumpable Bytes
    for(pixel = 0U; pixel < resolutionx*resolutiony; pixel++){
        imageBuffer[pixel*3U   ] = clamp(round(pixels[pixel].z*255U), 0U, 255U);
        imageBuffer[pixel*3U+1U] = clamp(round(pixels[pixel].y*255U), 0U, 255U);
        imageBuffer[pixel*3U+2U] = clamp(round(pixels[pixel].x*255U), 0U, 255U);
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