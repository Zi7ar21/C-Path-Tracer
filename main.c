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
#define samples 512U

// Resolution
#define resolutionx 640U
#define resolutiony 360U

// Maximum Ray-Marches
#define maxmarches 1024U

// Maximum Path Bounces
#define maxbounces 32U

// Maximum Sample Brightness (Should be the light brightness)
#define maxluminance 1.0f

// Distance counted as a collision
#define collisiondist 1e-2f
#define lightcollisiondist 0.1f

// Ray-March Step Multiplier
#define marchprecision 1.0f

// Maximum Distance from the Origin
#define scenesize 4.0f

// FBM Noise Octaves
#define octaves 4U

/*// Mandelbulb DE Parameters
#define iterations 4U
#define bailout 4.0f
#define power 8.0f

// Mandelbulb Distance Estimator
// (Ported from:http://blog.hvidtfeldts.net/index.php/2011/09/distance-estimated-3d-fractals-v-the-mandelbulb-different-de-approximations/)
float mandelbulb(vec3 pos){
    if(vec3length(pos) > 1.5f){
        return vec3length(pos)-1.35f;
    }
    vec3 z = float3(pos.x, pos.z, pos.y);
    float dr = 1.0f;
    float r = 0.0f;
    for(unsigned int i = 0U; i < iterations; i++) {
        r = vec3length(z);
        if(r > bailout){break;}
        // Convert to Polar Coordinates
        float theta = acosf(z.z/r);
        float phi = atan2f(z.y, z.x);
        dr = powf(r, power-1.0f)*power*dr+1.0f;
        // scale and rotate the point
        float zr = powf(r, power);
        theta = theta*power;
        phi = phi*power;
        // Convert back to cartesian coordinates
        z = vec3Multf(float3(sinf(theta)*cosf(phi), sinf(phi)*sinf(theta), cosf(theta)), zr);
        z = vec3Add(z, pos);
    }
    return 0.5f*logf(r)*r/dr;
}*/

// 3D FBM: https://www.shadertoy.com/view/3dSBRh
float fbm(vec3 x){
    float v = 0.0f;
    float a = 0.5f;
    for(unsigned int i = 0U; i < octaves; i++){
        v += a*noise(x);
        x = vec3Multf(x, 2.0f);
        a *= 0.5f;
    }
    return v;
}

// Planet Distance Estimator
float planetDE(vec3 pos){
    float vectorLength = vec3length(pos);
    // If the ray is far enough, we can skip the expensive noise computation
    if(vectorLength-0.7f > 0.0f){
        return vectorLength-0.6f;
    }
    float surfaceHeight = 0.1f*(fbm(vec3Multf(normalize(pos), 2.0f))-0.025f);
    float displacement = surfaceHeight;
    if(surfaceHeight < 0.0f){
        displacement = 0.0f;
    }
    return vectorLength-0.5f-displacement;
}

// Scene Distance Estimator
float distestimator(vec3 pos){
    // Find the distance to all the objects in the scene
    float DE0 = planetDE(pos);

    // Return the nearest object's distance
    return DE0;
}

// Calculate SDF Normals (Tetrahedron Technique Ported from: https://www.iquilezles.org/www/articles/normalsSDF/normalsSDF.htm)
vec3 calcNormal(vec3 p){
    const vec2 k = float2(1.0f, -1.0f);
    return normalize(vec3Add(
    vec3Add(vec3Multf(float3 (k.x, k.y, k.y), distestimator(vec3Add(p, vec3Multf(float3 (k.x, k.y, k.y), collisiondist)))),
    vec3Multf(        float3 (k.y, k.y, k.x), distestimator(vec3Add(p, vec3Multf(float3 (k.y, k.y, k.x), collisiondist))))),
    vec3Add(vec3Multf(float3 (k.y, k.x, k.y), distestimator(vec3Add(p, vec3Multf(float3 (k.y, k.x, k.y), collisiondist)))),
    vec3Multf(        floatf3(k.x          ), distestimator(vec3Add(p, vec3Multf(floatf3(k.x          ), collisiondist)))))));
}

// Material Lookup
material getMaterial(vec3 pos, vec3 normal){
    // Find the object the Ray collided with
    //float DE0 = planetDE(pos);

    // Return the nearest object's distance
    //float minimumde = DE0;

    // Return the corresponding material
    // Planet
    //if(minimumde == DE0){
    if(fbm(vec3Multf(normalize(pos), 2.0f))-0.025f < 0.0f){
        return mat(float3(0.8f, 0.8f, 1.0f), floatf3(0.4f), normal, 0.5f);
    }
    return mat(float3(0.6f, 1.0f, 0.6f), floatf3(0.8f), normal, 1.0f);

    // Return the undefined material (shouldn't happen, only to be on the safe side)
    return mat(floatf3(0.0f), normal, floatf3(0.0f), 0.0f);
}

// Light Distance Estimator
float light(vec3 pos){
    return vec3length(vec3Sub(pos, float3(-1.0f, 1.0f, -1.0f)))-0.5f;
}

// Scene Path-Tracing Function
vec3 pathtrace(vec3 raydir, vec3 rayori){
    // Set-Up Variables
    unsigned int bounces = 0U;
    vec3 raypos = rayori, lastpos = rayori, outCol = floatf3(1.0f), normal;
    material objectMat;
    float distest;

    // Perform Ray-Marching
    for(unsigned int i = 0U; i < maxmarches; i++){
        // Check if our Ray escaped the scene
        if(vec3length(raypos) > scenesize){break;}

        // Check if our Ray bounced too many times
        if(bounces > maxbounces){return floatf3(0.0f);}

        // Check if we reached the light source
        if(light(raypos) < lightcollisiondist){
            return vec3Multf(outCol, 8.0f);
        }

        // Find the distance from our Ray to the scene
        distest = distestimator(raypos);

        // If our Ray hit something, perform neccesary functions
        if(distest < collisiondist){
            normal    = calcNormal(raypos);
            objectMat = getMaterial(raypos, normal);
            outCol    = vec3Mult(outCol, vec3Mult(fresnel(raydir, objectMat.norm, objectMat.spec), objectMat.col));
            raydir    = reflect(raydir, normalize(nrand3(objectMat.rough, objectMat.norm)));
            raypos    = lastpos;
            bounces++;
        }

        // If our Ray didn't hit anything, move it forward as usual
        else{
            lastpos = raypos;
            raypos = vec3Add(raypos, vec3Multf(raydir, marchprecision*min(distest, light(raypos))));
        }
    }

    // Return the sample illuminated by the background color
    return vec3Multf(outCol, 0.125f);
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
    // Inverted Y, Because Targa Sucks...
    for(unsigned int y = resolutiony; y > 0U; y--){
    for(unsigned int x = 0U; x < resolutionx; x++){
        // Monte-Carlo Sampling
        for(unsigned int sample = 0U; sample < samples; sample++){
            INIT_RNG;
            uv = vec2Divf(vec2Multf(vec2Sub(nrand2(0.5f, vec2Addf(float2(x, y), 0.5f)), vec2Multf(resolutionxy, 0.5f)), 2.0f), resolutionmax);
            raydir = normalize(float3(uv.x*camfov, uv.y*camfov, 1.0f));
            outCol = vec3Add(outCol, pathtrace(raydir, float3(0.0f, 0.0f, -2.0f)));
        }

        // Divide the sum to get the converged sample, also Gamma-Correct it
        outCol = vec3pow(vec3Divf(outCol, samples), 1.0f/2.2f);

        // Add the pixels to the buffer
        pixels[pixel] = vec3clampf(outCol, 0.0f, maxluminance);

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
