#version 460
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out vec4 FragPosLightSpace;

uniform mat4 _Projection;
uniform mat4 _View;
uniform mat4 _Model;
uniform mat3 _NormalMatrix;
uniform mat4 _LightSpaceMatrix;

void main()
{
    TexCoords = texCoord;
    WorldPos = (_Model * vec4(position, 1.0)).xyz;
    Normal = normalize(_NormalMatrix * normal);
    FragPosLightSpace = _LightSpaceMatrix * vec4(WorldPos, 1.0);
    gl_Position = _Projection * _View * vec4(WorldPos, 1.0);

}