#version 460
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;

const float xOff = 1.0 / 1280.0;
const float yOff = 1.0 / 720.0;

const float exposure = 1.0;
const float gamma = 2.2;
const float contrast = 1.15;  
const vec3 tint = vec3(0.98, 1.00, 1.05);

float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16
);

void main()
{
    vec4 col = texture(screenTexture, TexCoords);
    vec3 color = col.rgb;
    float mask = col.a;

    
    if (mask > 0.0)
    {
        vec4 blurColor = vec4(0.0);
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                vec2 offset = vec2(i * xOff, j * yOff);
                vec2 uv = TexCoords + offset * 2.0;
                blurColor += texture(screenTexture, uv) * kernel[(i + 1) * 3 + (j + 1)];
            }
        }
        color = blurColor.rgb;
    }

    //  HDR Tone Mapping
    color = vec3(1.0) - exp(-color * exposure);

    //  Gamma Correction
    color = pow(color, vec3(1.0 / gamma));

    // Contrast Boost
    color = (color - 0.5) * contrast + 0.5;

    // Color Tint
    color *= tint;

    FragColor = vec4(color, 1.0);
}
