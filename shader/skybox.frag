#version 460

out vec4 FragColor;

in vec2 TexCoord;
in vec3 outVertexPosition;

uniform samplerCube skyboxMap;

void main()
{
    // ���ݶ���λ����ȷ����������
    vec3 dir = outVertexPosition;
    dir = normalize(dir);

    FragColor = texture(skyboxMap, dir);
}