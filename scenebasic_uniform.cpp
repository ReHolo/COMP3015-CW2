#include "scenebasic_uniform.h"
#include "helper/glutils.h"
#include "helper/texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GLFW/glfw3.h>


SceneBasic_Uniform::SceneBasic_Uniform() :
	tPrev(0.0f), angle(0.0f), rotSpeed(glm::pi<float>() / 8.0f)
{

	sword = ObjMesh::loadWithAdjacency("media/models/Rook.obj", true);
	swordMat = std::make_unique<PbrMaterial>(
		"media/texture/sword/Rook_Base_Colour.png",
		"media/texture/sword/Rook_Normal.png",
		"media/texture/sword/Rook_Metallic.png",
		"media/texture/sword/Rook_Roughness.png",
		"media/texture/sword/Rook_Occlusion.png");
	planeMat = std::make_unique<PbrMaterial>(
		"media/ground/diffuse.png",
		"media/ground/normal.png",
		"media/ground/height.png",
		"media/ground/roughness.png",
		"media/ground/ao.png");
	//fbo
	FramebufferSpecification fbSpec;
	fbSpec.Attachments = { FramebufferTextureFormat::RGBA8,FramebufferTextureFormat::RGB32F,  FramebufferTextureFormat::Depth };
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_cloudFB = std::make_unique<Framebuffer>(fbSpec);
	FramebufferSpecification shadowFbSpec;
	shadowFbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
	shadowFbSpec.Width = 1280;
	shadowFbSpec.Height = 720;
	m_shadowFB = std::make_unique<Framebuffer>(shadowFbSpec);
	FramebufferSpecification postFbSpec;
	postFbSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
	postFbSpec.Width = 1280;
	postFbSpec.Height = 720;
	m_PostFB = std::make_unique<Framebuffer>(postFbSpec);

	m_volumeCloud = std::make_unique<VolumeCloud>();
	defaultTex = Texture::createBlackTexture();
	CommonMesh::InitCommonMesh();
}

GLuint SceneBasic_Uniform::loadSkybox()
{

	std::vector<std::string> imagePaths = {
		"media/skybox/right.jpg",
		"media/skybox/left.jpg",
		"media/skybox/bottom.jpg",
		"media/skybox/top.jpg",
		"media/skybox/front.jpg",
		"media/skybox/back.jpg"
	};




	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);


	int width, height, channels;
	for (size_t i = 0; i < imagePaths.size(); i++)
	{
		unsigned char* data = Texture::loadPixels(imagePaths[i], width, height, 1, 3);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			Texture::deletePixels(data);
		}
		else
		{
			Texture::deletePixels(data);
		}
	}


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texId;
}

void SceneBasic_Uniform::drawSkybox()
{
	glDepthFunc(GL_LEQUAL);
	this->progSkybox.use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2000.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
	glm::mat4 mvp = camera.Projection * view * model;

	progSkybox.setUniform("MVP", mvp);


	glBindVertexArray(skybox.cubeVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}


void SceneBasic_Uniform::initScene()
{
	//shader
	compile();
	//light
	lights = {
		{{-1.0f, 8.0f, -1.0f},  {5.0f, 4.9f,4.95f}},
		{{-3.0f, 4.0f, 3.0f}, {1.0f, 1.0f,1.0f}},
		{{1.0f, 2.0f, -1.0f}, {1.0f, 1.0f,1.0f}},
		{{-3.0f, 4.0f, -3.0f},{1.0f, 1.0f,1.0f}}
	};
	lights[0].Near = 1.0f;
	lights[0].Far = 7.5f;

	//camera
	camera.setPosition(glm::vec3(1.0f, 1.0f, 1.0f));
	camera.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
	view = camera.getViewMatrix();
	projection = glm::mat4(1.0f);

	//windows
	GLFWwindow* win = glfwGetCurrentContext();
	camera.setWindow(win);
	glfwSetWindowUserPointer(win, this);
	glfwSetCursorPosCallback(win, [](GLFWwindow* window, double xpos, double ypos) {
		SceneBasic_Uniform* scene = static_cast<SceneBasic_Uniform*>(glfwGetWindowUserPointer(window));
		if (scene) scene->handleMouse(xpos, ypos);
		});
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	this->progSkybox.use();
	this->skyboxTex = loadSkybox();
	glActiveTexture(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTex);
	this->progSkybox.setUniform("skyboxMap", 0);

	return;
}


void SceneBasic_Uniform::compile()
{
	try {
		progSkybox.compileShader("shader/skybox.vert");
		progSkybox.compileShader("shader/skybox.frag");
		progSkybox.link();

		screenShader.compileShader("shader/screen_shader.vert");
		screenShader.compileShader("shader/screen_shader.frag");
		screenShader.link();

		shadowDepthShader.compileShader("shader/shadow_depth.vert");
		shadowDepthShader.compileShader("shader/shadow_depth.frag");
		shadowDepthShader.link();

		pbrShadowShader.compileShader("shader/pbr_shadow.vert");
		pbrShadowShader.compileShader("shader/pbr_shadow.frag");
		pbrShadowShader.link();


	}
	catch (GLSLProgramException& e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}


void SceneBasic_Uniform::update(float t)
{
	float deltaT = t - tPrev;
	if (tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;


	camera.processInput(deltaT);


	angle += 0.1f * deltaT;
	if (this->m_animate) {
		angle += rotSpeed * deltaT;
		if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
	}
	sword->Pos = glm::vec3(0.0, 3.0, 0.0);
	sword->Rotate.y = angle;
	sword->Scale = glm::vec3(3.0f);
}

void renderScene(GLSLProgram& shader)
{
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0));
	shader.setUniform("model", model);
	glBindVertexArray(CommonMesh::GetPlane());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//// cubes

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setUniform("model", model);
	glBindVertexArray(CommonMesh::GetCube());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.setUniform("model", model);
	glBindVertexArray(CommonMesh::GetCube());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
void SceneBasic_Uniform::render()
{
	//1. render shadow map 
	m_shadowFB->Bind();

	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	shadowDepthShader.use();
	shadowDepthShader.setUniform("lightSpaceMatrix", lights[0].GetLightMatrix());
	shadowDepthShader.setUniform("model", sword->getModel());
	sword->render();

	// 2. render scene
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_cloudFB->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0); 
	glEnable(GL_DEPTH_TEST);

	pbrShadowShader.use();
	//scene
	swordMat->UpdateSceneData(pbrShadowShader, camera, lights, sword->getModel());
	pbrShadowShader.setUniform("_LightSpaceMatrix", lights[0].GetLightMatrix());
	pbrShadowShader.setSampler2D("shadowMap", m_shadowFB->GetDepthAttachmentRendererID());
	//sword
	pbrShadowShader.setUniform("_Model", sword->getModel());
	pbrShadowShader.setUniform("_NormalMatrix", glm::transpose(glm::inverse(glm::mat3(sword->getModel()))));
	pbrShadowShader.setUniform("uvScale", 1.0f);
	swordMat->UpdateUniform(pbrShadowShader);
	sword->render();
	//plane
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0));
	pbrShadowShader.setUniform("_Model", model);
	pbrShadowShader.setUniform("_NormalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
	planeMat->UpdateUniform(pbrShadowShader);
	pbrShadowShader.setSampler2D("metallicMap", defaultTex);		//地面金属度为0
	pbrShadowShader.setUniform("uvScale", 0.2f);
	glBindVertexArray(CommonMesh::GetPlane());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//skybox
	drawSkybox();

	//3. cloud pass
	m_PostFB->Bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	m_volumeCloud->Draw(m_cloudFB, lights[0], camera);
	//4. post process
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	screenShader.use();
	screenShader.setSampler2D("screenTexture", m_PostFB->GetColorAttachmentRendererID());
	glBindVertexArray(CommonMesh::GetQuad());
	glDrawArrays(GL_TRIANGLES, 0, 6);

}



void SceneBasic_Uniform::resize(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	camera.Projection = glm::perspective(glm::radians(50.0f),
		float(w) / float(h),
		0.3f,
		5000.0f);
}

// 针对不同的 GLSLProgram 设置 MVP / NormalMatrix
void SceneBasic_Uniform::setMatrices(GLSLProgram& shader)
{
	glm::mat4 mv = view * model;
	shader.setUniform("ModelViewMatrix", mv);

	glm::mat3 normMat = glm::mat3(glm::vec3(mv[0]),
		glm::vec3(mv[1]),
		glm::vec3(mv[2]));
	shader.setUniform("NormalMatrix", normMat);

	shader.setUniform("MVP", projection * mv);
}

// 处理鼠标移动
void SceneBasic_Uniform::handleMouse(double xpos, double ypos)
{
	camera.processMouseMovement(xpos, ypos);
}
