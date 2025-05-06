#pragma once
#include <vector>
#include <string>
#include <utility>
#include "texture.h"
#include <glad/glad.h>
#include "glslprogram.h"
#include "light.h"
//Specular/Glossiness Workflow


struct TextureInfo
{
	unsigned int glHandle;
	std::string file;
	std::string shaderUniform;
};


class PbrMaterial {
public:
	TextureInfo diffuse;
	TextureInfo normal;
	TextureInfo metallic;
	TextureInfo roughness;
	TextureInfo ao;

	PbrMaterial(std::string diffuse_, std::string normal_, std::string metallic_, std::string roughness_, std::string ao_)
	{
		diffuse = { 0, diffuse_, "albedoMap" };
		normal = {0, normal_, "normalMap"};
		metallic = {0, metallic_, "metallicMap"};	
		roughness = {0, roughness_, "roughnessMap"};
		ao = {0, ao_, "aoMap"};
		diffuse.glHandle = Texture::loadTexture(diffuse.file);
		normal.glHandle = Texture::loadTexture(normal.file);
		metallic.glHandle = Texture::loadTexture(metallic.file);
		roughness.glHandle = Texture::loadTexture(roughness.file);
		ao.glHandle = Texture::loadTexture(ao.file);
	}


	void UpdateSceneData(GLSLProgram &shader, Camera& camera, std::vector<Light> &lights, glm::mat4 model)
	{
		glm::mat4 view = camera.getViewMatrix();
		shader.setUniform("_Model", model);
		shader.setUniform("_View", view);
		shader.setUniform("_Projection", camera.Projection);
		shader.setUniform("_NormalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		shader.setUniform("camPos", camera.Position);

		for (unsigned int i = 0; i < lights.size(); ++i)
		{
			shader.setUniform(std::string("lightPositions[" + std::to_string(i) + "]").c_str(), lights[i].Pos);
			shader.setUniform(std::string("lightColors[" + std::to_string(i) + "]").c_str(), lights[i].Color);
		}
	}

	void UpdateUniform(GLSLProgram &shader)
	{
		shader.use();
		shader.setSampler2D(diffuse.shaderUniform.c_str(), diffuse.glHandle);
		shader.setSampler2D(normal.shaderUniform.c_str(), normal.glHandle);
		shader.setSampler2D(metallic.shaderUniform.c_str(), metallic.glHandle);
		shader.setSampler2D(roughness.shaderUniform.c_str(), roughness.glHandle);
		shader.setSampler2D(ao.shaderUniform.c_str(), ao.glHandle);
	}

	void UnloadTextures()
	{
		glDeleteTextures(1, &diffuse.glHandle);
		glDeleteTextures(1, &normal.glHandle);
		glDeleteTextures(1, &metallic.glHandle);
		glDeleteTextures(1, &roughness.glHandle);
		glDeleteTextures(1, &ao.glHandle);
	}
};



