// High-Quality Random Numbers from Michael0884, ported from: https://www.shadertoy.com/view/wltcRS
unsigned int ns;
#define INIT_RNG ns = 185730U*sample+(x+y*resolutionx);

void pcg(){
    unsigned int state = ns*747796405U+2891336453U;
    unsigned int word = ((state >> ((state >> 28U) + 4U)) ^ state)*277803737U;
    ns = (word >> 22U) ^ word;
}

float rand(){
    pcg();
    return (float)ns/(float)0xffffffffU;
}