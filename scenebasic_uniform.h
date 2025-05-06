#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/plane.h"
#include "helper/objmesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "helper/Camera.h"
#include "helper/skybox.h"
#include "helper/light.h"
#include "helper/Material.h"
#include "helper/FrameBuffer.h"
#include "helper/VolumeCloud.h"

class SceneBasic_Uniform : public Scene {
private:
    std::unique_ptr<Framebuffer> m_PostFB;
    std::unique_ptr<Framebuffer> m_cloudFB; 
    std::unique_ptr<Framebuffer> m_shadowFB;


    //object
    std::unique_ptr<ObjMesh> sword;
    std::unique_ptr<VolumeCloud> m_volumeCloud;

    std::unique_ptr<PbrMaterial> swordMat;
    std::unique_ptr<PbrMaterial> planeMat;
    GLuint defaultTex;  //¿Õ°×ÎÆÀí£¬ÖµÎª0

    Camera camera;
    std::vector<Light> lights;

    GLSLProgram shadowDepthShader;
    GLSLProgram pbrShadowShader;
    GLSLProgram screenShader;

    
    float angle;

    float tPrev;
    float rotSpeed;

    GLSLProgram progSkybox;
    SkyBox skybox;
    GLuint skyboxTex;       



    void compile();
    void setMatrices(GLSLProgram& shader);
    GLuint loadSkybox();
    void drawSkybox();

public:
    SceneBasic_Uniform();
    void initScene();
    void update(float t);
    void render();
    void resize(int, int);

    virtual void handleMouse(double xpos, double ypos) override;
};

#endif
