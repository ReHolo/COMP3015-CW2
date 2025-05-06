#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform bool DebugMode = false; 
uniform int DisplayChannel = 0; 


uniform struct LightInfo {
    vec3 Position;
    vec3 L;   
    vec3 La;  
} Light;


uniform struct MaterialInfo {
    vec3 Ka; 
    vec3 Kd; 
    vec3 Ks; 
    float Shininess;
} Material;

uniform sampler2D BaseColorMap;    // 0
uniform sampler2D MetallicMap;     // 1
uniform sampler2D NormalMap;       // 2
uniform sampler2D OcclusionMap;    // 3
uniform sampler2D RoughnessMap;    // 4

vec3 blinnPhong(vec3 pos, vec3 n)
{
    vec3 lightDir = normalize(Light.Position - pos);
    vec3 ambient = Light.La * Material.Ka;

    float sDotN = max(dot(lightDir, n), 0.0);
    vec3 diffuse = Material.Kd * sDotN * Light.L;

    vec3 spec = vec3(0.0);
    if (sDotN > 0.0) {
        vec3 viewDir = normalize(-pos);
        vec3 halfDir = normalize(lightDir + viewDir);
        float hDotN = max(dot(halfDir, n), 0.0);
        spec = Material.Ks * pow(hDotN, Material.Shininess) * Light.L;
    }

    float distance = length(Light.Position - pos);
    float attenuation = 1.0 / (distance * distance);
    return (ambient + diffuse + spec) * attenuation;
}

vec3 debugDisplay(vec2 tc) {
    vec3 color;
    switch(DisplayChannel) {
        case 1: 
            color = texture(MetallicMap, tc).rrr;
            break;
        case 2: 
            color = texture(RoughnessMap, tc).rrr;
            break;
        case 3: 
            color = texture(OcclusionMap, tc).rrr;
            break;
        default: 
            color = blinnPhong(Position, normalize(Normal));
            color = mix(color, color * texture(OcclusionMap, tc).r, 0.5);
    }
    return color;
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 normalTex = texture(NormalMap, TexCoord).rgb * 2.0 - 1.0;
    vec3 baseColor = texture(BaseColorMap, TexCoord).rgb;
    float metallic = texture(MetallicMap, TexCoord).r;
    float roughness = texture(RoughnessMap, TexCoord).r;
    float ao = texture(OcclusionMap, TexCoord).r;

    MaterialInfo mat = Material;
    mat.Kd = baseColor;

    vec3 color = blinnPhong(Position, N);
    color = mix(color, color * ao, 0.75);

    vec3 finalColor = debugDisplay(TexCoord);
    FragColor = vec4(finalColor, 1.0);
    FragColor = vec4(color, 1.0);
}
