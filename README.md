# C-Path-Tracer

## Introduction

This is the repository for my [Path Tracer](https://en.wikipedia.org/wiki/Path_tracing) written in C. It currently runs on the CPU, single-threaded, and saves the render in the [Targa Image File Format (`.tga`)](https://en.wikipedia.org/wiki/Truevision_TGA)

***

## Sample Render

![Path-Traced rendering of a Red Sphere.](https://repository-images.githubusercontent.com/342092160/21fa9100-793c-11eb-9d6d-89c986825b63)

***

## TODO

- Rendering Solid Surface and Volumetric objects in the same scene
- Physically Accurate Rendering (There is currently no BRDF's and so this path tracer is considered biased.)
- Multithreading
- A better output image format (OpenEXR would be epic)
