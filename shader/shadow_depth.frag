#version 460
out vec4 FragColor;

in vec3 ClipPos;
void main()
{
    // gl_FragDepth = gl_FragCoord.z;
    //FragColor = vec4(ClipPos, 1.0);
    FragColor = vec4(vec3(ClipPos.z), 1.0);
}