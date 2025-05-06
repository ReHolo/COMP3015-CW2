#include "VolumeCloud.h"
#include <iostream>
#include "light.h"
#include "Camera.h"
#include "objmesh.h"

VolumeCloud::VolumeCloud()
{
	try {
		cloudShader.compileShader("shader/cloud.vert");
		cloudShader.compileShader("shader/cloud.frag");
		cloudShader.link();

		worly3DShader.compileShader("shader/worly3D.comp", GLSLShader::COMPUTE);
		worly3DShader.link();
	}
	catch (GLSLProgramException& e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	perlin2D = Texture::loadTexture("media/texture/cloud/noise.png");
	worly3D = Texture::create3D(256, 256, 256);
	worly3DShader.use();
	worly3DShader.setUniform("u_resolution", glm::vec3(256, 256, 256));
	std::cout << "computing worley 3D!" << std::endl;
	glDispatchCompute((GLuint)256, (GLuint)256, (GLuint)256);
	std::cout << "computed!!" << std::endl;
}

void VolumeCloud::Draw(std::unique_ptr<Framebuffer>& inFBO, Light &light, Camera &camera)
{
	cloudShader.use();
	cloudShader.setSampler2D("screenTexture", inFBO->GetColorAttachmentRendererID());
	cloudShader.setSampler2D("depthTexture", inFBO->GetDepthAttachmentRendererID());
	cloudShader.setSampler2D("_PerlinTex", perlin2D);
	cloudShader.setSampler3D("_Worly3DTex", worly3D);

	cloudShader.setUniform("cameraPos", camera.Position);
	cloudShader.setUniform("_LightPos", light.Pos);
	cloudShader.setUniform("_InverseViewMatrix", glm::inverse(camera.getViewMatrix()));
	cloudShader.setUniform("_InverseProjectionMatrix", glm::inverse(camera.Projection));
	cloudShader.setUniform("_Time", (float)glfwGetTime());

	glBindVertexArray(CommonMesh::GetQuad());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
