#pragma once
#include "texture.h"
#include <string>
#include "FrameBuffer.h"
#include "glslprogram.h"
#include <memory>
#include "light.h"
#include "Camera.h"

class VolumeCloud
{
public:
	VolumeCloud();

	void Draw(std::unique_ptr<Framebuffer>& inFBO, Light& light, Camera& camera);
	//unsigned int getNoiseTextureId() { return glNoiseMap; }
public:
	unsigned int glNoiseMap;	//‘Î…˘Œ∆¿Ì
private:
	GLSLProgram cloudShader;
	GLSLProgram worly3DShader;

	GLuint perlin2D;
	GLuint worly3D;

};