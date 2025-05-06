#include "texture.h"
#include "stb/stb_image.h"
#include "glutils.h"
#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <cstdint>
#include <algorithm>

/*static*/
GLuint Texture::loadTexture( const std::string & fName ) {
    int width, height;
    unsigned char * data = Texture::loadPixels(fName, width, height);
	GLuint tex = 0;
    if( data != nullptr ) {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

        Texture::deletePixels(data);
    }

    return tex;
}

GLuint Texture::loadTexture(const std::string& fName, int channels) {
    int width, height;
    unsigned char* data = Texture::loadPixels(fName, width, height, 1, channels);
    GLuint tex = 0;
    if (data != nullptr) {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Texture::deletePixels(data);
    }

    return tex;
}

void Texture::deletePixels(unsigned char *data) {
    stbi_image_free(data);
}


GLuint Texture::createTexture(unsigned int w, unsigned h)
{
    unsigned int tex_output;
    glGenTextures(1, &tex_output);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_output);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    return tex_output;
}

GLuint Texture::createBlackTexture()
{
    // 初始化默认纹理
    GLuint defaultTexture;
    glGenTextures(1, &defaultTexture);
    glBindTexture(GL_TEXTURE_2D, defaultTexture);
    unsigned char pixel[] = { 0, 0, 0, 255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return defaultTexture;
}

GLuint Texture::create3D(unsigned int w, unsigned int h, unsigned int d)
{
    unsigned int tex_output;
    glGenTextures(1, &tex_output);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, tex_output);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexStorage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, w, h, d);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, w, h, d, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    return tex_output;
}

void Texture::save(std::string file, GLuint id)
{
    glBindTexture(GL_TEXTURE_2D, id);
    GLint width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    std::vector<unsigned char> pixels(width * height * 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    stbi_write_png(file.c_str(), width, height, 4, pixels.data(), width * 4);
}

void Texture::saveWorldPos(std::string file, GLuint id)
{
    glBindTexture(GL_TEXTURE_2D, id);
    GLint width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    std::vector<float> worldPosData(width * height * 3);
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    //glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB32F, GL_FLOAT, worldPosData.data());
    glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, worldPosData.data());
    float minVal = FLT_MAX, maxVal = -FLT_MAX;
    for (float val : worldPosData) {
        if (val < minVal) minVal = val;
        if (val > maxVal) maxVal = val;
    }

    float maxDistance = maxVal - minVal;
    float halfDis = maxDistance / 2.0;
    std::vector<uint8_t> imageData(width * height * 3);
    for (int i = 0; i < width * height * 3; i++) {
        float value = worldPosData[i];

        // 归一化方法（根据实际坐标范围调整）
        // 假设坐标范围在 [-10.0, 10.0]，映射到 [0.0, 1.0]
        float normalized = (value + halfDis) / maxDistance;

        // 限制到 [0, 1] 并转换为 8-bit
        //normalized = std::clamp(normalized, 0.0f, 1.0f);
        imageData[i] = static_cast<uint8_t>(normalized * 255.0f);
    }
    stbi_write_png(file.c_str(), width, height, 3, imageData.data(), width * 3);
}
void Texture::save3D(std::string file, GLuint id)
{
    // 获取纹理尺寸和格式
    GLint width, height, depth;
    glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_DEPTH, &depth);

    // 根据纹理格式分配内存
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    size_t bytesPerPixel = 4; // GL_RGBA8: 4 bytes per pixel

    std::vector<uint8_t> data(width * height * depth * bytesPerPixel);

    // 设置像素存储对齐（避免数据错位）
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    // 读取纹理数据
    glGetTexImage(GL_TEXTURE_3D, 0, format, type, data.data());
    for (int z = 0; z < depth; ++z) {
        std::string filename = file + "_z" + std::to_string(z) + ".png";
        stbi_write_png(
            filename.c_str(),
            width, height,
            4, // RGBA
            data.data() + z * width * height * 4, // 当前层的起始地址
            width * 4 // 每行字节数
        );
    }
}

unsigned char *Texture::loadPixels(const std::string &fName, int & width, int & height, bool flip) {
    int bytesPerPix;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(fName.c_str(), &width, &height, &bytesPerPix, 4);
    return data;
}

unsigned char* Texture::loadPixels(const std::string& fName, int& width, int& height, bool flip, int channels) {
    int bytesPerPix;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(fName.c_str(), &width, &height, &bytesPerPix, channels);
    return data;
}

GLuint Texture::loadCubeMap(const std::string &baseName, const std::string &extension) {
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    const char * suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
    GLint w, h;

    // Load the first one to get width/height
    std::string texName = baseName + "_" + suffixes[0] + extension;
    GLubyte * data = Texture::loadPixels(texName, w, h, false);

    // Allocate immutable storage for the whole cube map texture
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, w, h);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    // Load the other 5 cube-map faces
    for( int i = 1; i < 6; i++ ) {
        std::string texName = baseName + "_" + suffixes[i] + extension;
        data = Texture::loadPixels(texName, w, h, false);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texID;
}

GLuint Texture::loadHdrCubeMap(const std::string &baseName) {
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    const char * suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
    GLint w, h;

    // Load the first one to get width/height
    std::string texName = baseName + "_" + suffixes[0] + ".hdr";
    float * data = stbi_loadf(texName.c_str(), &w, &h, NULL, 3);

    // Allocate immutable storage for the whole cube map texture
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGB32F, w, h);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, w, h, GL_RGB, GL_FLOAT, data);
    stbi_image_free(data);

    // Load the other 5 cube-map faces
    for( int i = 1; i < 6; i++ ) {
        std::string texName = baseName + "_" + suffixes[i] + ".hdr";
        data = stbi_loadf(texName.c_str(), &w, &h, NULL, 3);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, w, h, GL_RGB, GL_FLOAT, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texID;
}


