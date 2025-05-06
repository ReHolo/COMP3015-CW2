#version 460
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
out vec3 ClipPos;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
    ClipPos = gl_Position.xyz;
}