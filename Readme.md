# COMP3015 CW2 – Real-Time Graphics Project

Author: Sirun Wu  
Module: COMP3015 – CW2  
Academic Year: 2024–2025

## Project Overview

This project demonstrates multiple advanced real-time rendering techniques using OpenGL and GLSL. The scene includes physically based materials, volumetric clouds, shadow mapping, and post-processing effects. The purpose is to showcase shader knowledge and graphics programming as part of the CW2 submission for COMP3015.

## Implemented Features

### Shader Techniques

- Shadow Mapping – Real-time shadow rendering using depth framebuffer and directional lighting.
- Physically Based Rendering (PBR) – Implemented using Cook-Torrance BRDF with albedo, metallic, roughness, normal, and AO textures.
- 3D Worley Noise – Procedural 3D noise generation used to control cloud shape and density.
- Volumetric Cloud Rendering – Based on real-time rendering techniques from SIGGRAPH 2015, using raymarching and atmospheric light scattering.
- Post-Processing Effects – Includes:
  - Gaussian blur (applied to alpha-masked areas)
  - HDR tone mapping
  - Gamma correction
  - Contrast adjustment
  - Subtle blue tint for atmospheric effect

## Requirements

- Windows 10/11  
- OpenGL 4.0 or higher  
- Visual Studio 2022  
- Dependencies:
  - GLFW (window and input handling)
  - GLAD (OpenGL function loader)
  - stb_image (texture loading)
  - GLM (OpenGL math library)

All dependencies are included in the project or preconfigured via local headers/libs.

## Demonstration Video

Link: [https://www.youtube.com/link](https://www.youtube.com/link)

The video demonstrates:
- Volumetric cloud rendering
- Shadow casting and dynamic lighting
- Procedural noise affecting cloud structure
- Post-processing enhancements and visual style

## References

- Schneider A, Vos N. *The Real-Time Volumetric Cloudscapes of Horizon: Zero Dawn*. SIGGRAPH 2015.
- Worley, S. *A Cellular Texture Basis Function*. 1996.

## Feature Summary for CW2

- Shadow Mapping  
- PBR  
- 3D Noise  
- Volumetric Cloud (Research-based)  
- Post-Processing Effects  
- Git Repository with source code  
- Demonstration video

## Academic Statement

This submission is my own original work. All shaders and techniques implemented were created or adapted by me and are properly cited when inspired by external sources.
