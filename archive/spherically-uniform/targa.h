#include <stdio.h>

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