#version 460

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexCoord; // ��������

out vec2 TexCoord;
out vec3 outVertexPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 MVP;

void main()
{
    outVertexPosition = VertexPosition;

    TexCoord = VertexTexCoord; // ������������
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}  