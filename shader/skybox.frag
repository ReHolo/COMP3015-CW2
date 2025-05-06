#version 460

out vec4 FragColor;

in vec2 TexCoord;
in vec3 outVertexPosition;

uniform samplerCube skyboxMap;

void main()
{
    // 根据顶点位置来确定采样方向
    vec3 dir = outVertexPosition;
    dir = normalize(dir);

    FragColor = texture(skyboxMap, dir);
}