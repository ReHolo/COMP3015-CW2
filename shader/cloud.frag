#version 460
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;  
uniform sampler2D depthTexture;
//uniform sampler2D _WorldPosTexture;
uniform sampler2D _PerlinTex;
uniform sampler3D _Worly3DTex;  //control cloud shape

uniform vec3 _LightPos;  
//uniform vec3 _LightColor;

uniform vec3 cameraPos; 
uniform mat4 _InverseProjectionMatrix;
uniform mat4 _InverseViewMatrix;
uniform float _Time;
#define BAYER_FACTOR 1.0/16.0
const float _DensityScale = 1.0;
vec3 _LightColor = vec3(1.0,1.0,1.0);
const float PI = 3.141592653589793238462643383279502884197169;
uniform float bayerFilter[16u] = float[]
(
	0.0*BAYER_FACTOR, 8.0*BAYER_FACTOR, 2.0*BAYER_FACTOR, 10.0*BAYER_FACTOR,
	12.0*BAYER_FACTOR, 4.0*BAYER_FACTOR, 14.0*BAYER_FACTOR, 6.0*BAYER_FACTOR,
	3.0*BAYER_FACTOR, 11.0*BAYER_FACTOR, 1.0*BAYER_FACTOR, 9.0*BAYER_FACTOR,
	15.0*BAYER_FACTOR, 7.0*BAYER_FACTOR, 13.0*BAYER_FACTOR, 5.0*BAYER_FACTOR
);

float random(vec2 seed) {
    return fract (sin(dot(seed, vec2(12.9898, 78.233)) * 43758.5453));
}

vec3 generateRandomDirection(vec2 fragCoord, float time) {
    // 生成两个随机数作为球坐标参数
    float u = random(fragCoord + time);
    float v = random(fragCoord - time + 0.123);

    // 将u映射到[-1,1]保证cos(theta)均匀分布
    float theta = acos(2.0 * u - 1.0);  // 极角 [0, π]
    float phi = 2.0 * 3.141592653589793 * v; // 方位角 [0, 2π]
    
    float sinTheta = sin(theta);
    return vec3(
    sinTheta * cos(phi),
    sinTheta * sin(phi),
    cos(theta)
    );
}
#define bottom 20   
#define top   70    
#define width 400     

#define baseBright  vec3(1.26,1.25,1.29)    
#define baseDark    vec3(0.31,0.31,0.32)    
#define lightBright vec3(1.29, 1.17, 1.05)  
#define lightDark   vec3(0.7,0.75,0.8)

/* util function */
float HeightFraction(vec3 pos){
    return (pos.y - bottom) / (top - bottom);
}
float Perturbance(vec2 coords, float scale)
{
    float noise = texture(_PerlinTex, coords).r;
    noise = (noise - 0.5) * 2.0;   //[-1, 1]
    return  noise * scale;
}
vec2 RayBoxIntersect(vec3 rayOri, vec3 invRayDir)
{
    vec3 boundsMin = vec3(-width, bottom, -width);
    vec3 boundsMax = vec3(width, top, width);

    vec3 t0 = (boundsMin - rayOri) * invRayDir;
    vec3 t1 = (boundsMax - rayOri) * invRayDir;
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);

    float dstA = max(max(tmin.x, tmin.y), tmin.z);
    float dstB = min(tmax.x, min(tmax.y, tmax.z));

    float dstToBox = max(0, dstA);
    float dstInsideBox = max(0, dstB - dstToBox);
    return vec2(dstToBox, dstInsideBox);
}

float phase(float cosTheta, float g) {
    float g2 = g*g;
    return (1.0 - g2) / (4.0 * PI * pow(1.0 + g2 - 2.0*g*cosTheta, 1.5))
    + 0.2 * (1.0 - g2); // 添加各向同性基底
}
vec4 GetWorldSpacePosition(float depth, vec2 uv)
{
    vec4 srcPos = vec4(2.0 * uv - 1.0, depth, 1.0);
    vec4 view_vector = _InverseProjectionMatrix * srcPos;
    view_vector.xyz /= view_vector.w;
    vec4 world_vector = _InverseViewMatrix * vec4(view_vector.xyz, 1);
    return world_vector;
}

/**************cloud function**************/
float getDensity3D(sampler3D noisetex, vec3 pos) {
    // 高度衰减
    float mid = (bottom + top) / 2.0;
    float h = top - bottom;
    float weight = 1.0 - 2.0 * abs(mid - pos.y) / h;
    if(weight > 0.) weight = pow(weight, 0.5);
    //uv scale
    float uvScale = 1.0 / width;
    vec3 str = pos;
    str.y -= bottom;
    str *= uvScale ;
    str += vec3(0.5, 0.0, 0.5);     // [0.0 - 1.0]
    //str *= 0.08;
    
    vec4 noise = texture(noisetex, str);
    float res = noise.r * weight;
    if(res < 0.4) return 0.0;
    return res;
}
// get cloud color
vec4 getCloud(vec3 worldPos, vec3 cameraPos, vec3 lightPos) {
    vec3 rayDir = normalize(worldPos - cameraPos);   
    float lenEyeToObj = length(worldPos - cameraPos); 
    vec2 rayIntersectRes = RayBoxIntersect(cameraPos, 1.0 / rayDir); //res.x: distance to box, res.y: distance inside box
    float lenEyeToCloud = rayIntersectRes.x; // distance to box
    float BoxInsideLen = rayIntersectRes.y; // distance inside box
    //float distanceLimit = min(lenEyeToObj - lenEyeToCloud, lenBoxInside); // distance limit for ray marching
    // no intersect
    if( BoxInsideLen <= 0.0) {
        return vec4(0.0); 
    }
    if( lenEyeToObj < lenEyeToCloud) return vec4(0.0);
    //startPos add perturbation
    vec3 startPos = cameraPos + rayDir * (lenEyeToCloud + Perturbance(TexCoords, 1.0)); 
    //march data
    float stepDis = 0.5 + Perturbance(TexCoords, 0.2);
    vec3 stepV = stepDis * rayDir ;
    float nSteps = floor(BoxInsideLen / stepDis);
    
    vec3 curPoint = startPos; 
    vec4 colorSum = vec4(0.0);
    float transmittanceSum = 0.0;
    // ray marching
    for(int i=0; i<nSteps; i++) {
        float density =  getDensity3D(_Worly3DTex, curPoint) ;
        //light
        vec3 L = normalize(lightPos - curPoint);                       // 光源方向
        float lightDensity = getDensity3D(_Worly3DTex, curPoint + L );       // 向光源方向采样一次 获取密度
        float delta = clamp(density - lightDensity, 0.0, 1.0);      // 两次采样密度差
        // 控制透明度
        density *= 0.5;
        // 颜色计算
        vec3 base = mix(baseBright, baseDark, density) * density;   // 基础颜色
        vec3 light = mix(lightDark, lightBright, delta);            // 光照对颜色影响
        // 混合
        vec4 color = vec4(base*light, density);                     // 当前点的最终颜色
        colorSum = color * (1.0 - colorSum.a) + colorSum;           // 与累积的颜色混合
        
        curPoint += stepV;
    }
    return colorSum; 
}
vec4 getCloud2(vec3 worldPos, vec3 cameraPos, vec3 lightPos) {
    vec3 rayDir = normalize(worldPos - cameraPos);
    vec2 rayIntersectRes = RayBoxIntersect(cameraPos, 1.0 / rayDir);
    float lenEyeToCloud = rayIntersectRes.x;
    float BoxInsideLen = rayIntersectRes.y;

    if(BoxInsideLen <= 0.0 || length(worldPos - cameraPos) < lenEyeToCloud) {
        return vec4(0.0);
    }

    // 初始化随机偏移
    float jitter = texture(_PerlinTex, TexCoords * 10.0 + _Time.xx).r;
    vec3 startPos = cameraPos + rayDir * (lenEyeToCloud + jitter * 0.5);

    // 光照参数
    const float _ScatteringCoeff = 1.2;    // 提高散射强度 
    const float _AbsorptionCoeff = 0.08;   // 降低吸收系数
    const float _Anisotropy = 0.3;         // 减弱前向散射
    const int MAX_STEPS = 128;             // 最大步数
    const float MIN_TRANSMITTANCE = 0.01;  // 提前终止阈值

    // 步进参数
    float stepDis = BoxInsideLen / float(MAX_STEPS) * 0.5;
    vec3 stepV = rayDir * stepDis;

    // 光照计算变量
    vec3 transmittance = vec3(1.0); // 透射率
    vec3 scatteringLight = vec3(0.0); // 散射光累积
    vec3 curPoint = startPos;
    
    for(int i=0; i<MAX_STEPS; ++i){
        // 获取当前点密度
        float density = getDensity3D(_Worly3DTex, curPoint);

        if(density > 0.0) {
            // 计算光照方向
            vec3 L = normalize(lightPos - curPoint);
            vec3 V = -rayDir; // 观察方向
            float cosTheta = dot(L, V);

            // 相位函数计算
            float phaseVal = phase(cosTheta, _Anisotropy);

            // 光照衰减计算
            float lightTransmittance = exp(-_AbsorptionCoeff * density * stepDis);
            vec3 lightEnergy = _LightColor * phaseVal * _ScatteringCoeff;

            // 散射贡献
            vec3 scattering = density * stepDis * lightEnergy * (transmittance + 0.5 * transmittance * transmittance);
            scatteringLight += scattering;

            // 透射率更新
            transmittance *= exp(-(_AbsorptionCoeff + _ScatteringCoeff) * density * stepDis);
        }

        // 提前终止
        if(transmittance.r < MIN_TRANSMITTANCE &&
        transmittance.g < MIN_TRANSMITTANCE &&
        transmittance.b < MIN_TRANSMITTANCE) {
            break;
        }

        curPoint += stepV;
    }

    // 颜色混合（考虑HDR）
    // 调整alpha计算方式
    float alpha = 1.0 - smoothstep(0.0, 0.2, max(max(transmittance.r, transmittance.g), transmittance.b));
    return vec4(scatteringLight * 2.0, alpha); // 提升亮度
}

void main()
{
    float depth = texture(depthTexture, TexCoords).r;
    if(depth < 0.0001) depth = 1.0;
    vec4 worldPos = GetWorldSpacePosition(depth, TexCoords);
    vec4 cloud = getCloud(worldPos.xyz, cameraPos, _LightPos); 
    vec3 oriColor = texture(screenTexture, TexCoords).rgb;
    vec3 finalCol = oriColor.rgb*(1.0 - cloud.a) + cloud.rgb;  
    if(cloud.a == 0.0) FragColor = vec4(finalCol, 0.0); //no cloud
    else FragColor = vec4(finalCol, 1.0);   //has cloud
    
} 