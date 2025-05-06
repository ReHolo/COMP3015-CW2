#version 460

layout(location = 0) in vec3 position;   
layout(location = 1) in vec3 normal;     
layout(location = 2) in vec2 texCoord;  

out vec3 Position;   
out vec3 Normal;     
out vec2 TexCoord;   

// Uniforms
uniform mat4 ModelViewMatrix;  
uniform mat4 MVP;              
uniform mat3 NormalMatrix;     

void main()
{
    gl_Position = MVP * vec4(position, 1.0);

    
    Position = (ModelViewMatrix * vec4(position, 1.0)).xyz;
    Normal = normalize(NormalMatrix * normal);
    TexCoord = texCoord;
}
