#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define BYTE_RANGE 256

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

int little_endianify(int number){return number % BYTE_RANGE;}
int big_endianify(int number){return number / BYTE_RANGE;}

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

/*typedef struct{
    float x;
    float y;
    float z;
} vec3;

vec3 float3(float x, float y, float z){
    vec3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
};

vec3 normalize(vec3 vec){
    float length = sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
    return float3(vec.x / length, vec.y / length, vec.z / length);
}*/

// Classic "Unsafe" Minimum and Maximum Functions
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

// Test Function
float mandelbrot(float cr, float ci){
    float z2r, z2i, oz2r, oz2i;
    oz2r = 0.0;
    oz2i = 0.0;
    for(unsigned int i = 0; i < 32; i++){
        z2r = (oz2r*oz2r)-(oz2i*oz2i);
        z2i = oz2r*oz2i*2.0;
        z2r += cr;
        z2i += ci;
        if(sqrtf((z2r*z2r)+(z2i*z2i)) > 4.0){
            return 0.0;
        }
        oz2r = z2r;
        oz2i = z2i;
    }
    return 1.0;
}

int main(){
    // Set-Up Variables
    unsigned int resolutionx, resolutiony, resolutionmax, byte;
    float outCol, uvx, uvy;
    byte = 0;

    // Parameters
    resolutionx = 1920;
    resolutiony = 1080;

    // Auto-Defined Parameters
    resolutionmax = MAX(resolutionx, resolutiony);
    uint8_t imageBuffer[1920 * 1080 * 3] = {0};

    // Execute Rendering
    for(unsigned int y = 0; y < resolutiony; y++){
        uvy = 2.0 * ((y + 0.5) - 0.5 * resolutiony) / resolutionmax;
    for(unsigned int x = 0; x < resolutionx; x++){
        uvx = 2.0 * ((x + 0.5) - 0.5 * resolutionx) / resolutionmax;
        outCol = mandelbrot(uvx, uvy);
        imageBuffer[byte] = MAX(0, MIN(255, round(outCol*255)));
        imageBuffer[byte+1] = MAX(0, MIN(255, round(outCol*255)));
        imageBuffer[byte+2] = MAX(0, MIN(255, round(outCol*255)));
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