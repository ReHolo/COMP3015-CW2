#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glslprogram.h"
#include <vector>
#include <string>
class Light
{
public:
	glm::vec3 Pos;
	glm::vec3 Color;
    float Near = 0.1f;
    float Far = 1000.0f;


    glm::mat4 GetLightMatrix()
    {
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
        glm::mat4 lightView = glm::lookAt(Pos, glm::vec3(1.0), glm::vec3(0.0, 1.0, 0.0));
        return lightProjection * lightView;
    }

};

