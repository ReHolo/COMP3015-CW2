# OpenGL Rendering Project

## Overview
This project is an OpenGL-based rendering system that demonstrates various graphical effects, including basic shading, skybox rendering, and plane rendering. The system utilizes GLSL shaders to process and display 3D scenes with lighting, textures, and transformations.



## How It Works
### **1. Initialization**
- The application initializes GLFW and OpenGL settings.
- Scene objects, shaders, and textures are loaded in `SceneBasic_Uniform::initScene()`.
- The camera is set up, and input handlers for mouse and keyboard are configured.

### **2. Rendering Pipeline**
- **Skybox Rendering**: 
  - `loadSkybox()` loads cube map textures.
  - `drawSkybox()` renders a large cube with skybox textures.
- **Object Rendering**:
  - The `render()` function sets up the scene, applying transformations.
  - Uses Phong shading model with PBR material textures.
- **Plane Rendering**:
  - Loads a textured ground plane (`plane.vert`, `plane.frag`).
  - Applies normal mapping and roughness for realistic shading.

### **3. Camera System**
- Camera movement is handled in `Camera::processInput()`.
- Supports forward, backward, left, right, up, and down movement.
- Mouse movement controls camera orientation (`Camera::processMouseMovement()`).

### **4. Shader System**
- Shaders are compiled and linked in `SceneBasic_Uniform::compile()`.
- `GLSLProgram` handles GLSL shader programs dynamically.
- Uniforms for lighting and materials are set at runtime.

## Features
- **Basic Object Rendering**: Renders 3D objects using `basic_uniform` shaders.
- **Skybox Implementation**: Uses `skybox.vert` and `skybox.frag` to render a cube-mapped sky.
- **Plane Rendering**: Displays a textured plane with shading effects.
- **Lighting and Texturing**: Implements lighting models and texture mapping.
- **Camera Control**: Allows user interaction to move and rotate the camera.

## Setup & Compilation
### **Requirements**
- OpenGL 4.0+
- GLFW for window management
- GLAD for OpenGL function loading
- stb_image for texture loading

## Controls
- `W/A/S/D` - Move the camera
- `Mouse Movement` - Look around
- `ESC` - Exit the application

## Troubleshooting
- Ensure that all dependencies (GLFW, GLAD, stb_image) are correctly installed.
- Check OpenGL error messages for debugging rendering issues.

## Future Enhancements
- Implement dynamic lighting and shadows.
- Add support for more complex models and animations.
- Improve texture quality and PBR (Physically-Based Rendering) materials.

## License
This project is open-source and available under the MIT License.



## Link

-Youtube:https://youtu.be/y-xbUXui3-I

