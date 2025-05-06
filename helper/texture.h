#pragma once
#include <glad/glad.h>
#include <string>

class Texture {
public:
    static GLuint loadTexture( const std::string & fName );
    static GLuint loadTexture(const std::string& fName, int channels);
    static GLuint loadCubeMap(const std::string & baseName, const std::string & extention = ".png");
    static GLuint loadHdrCubeMap( const std::string & baseName );
    static unsigned char * loadPixels( const std::string & fName, int & w, int & h, bool flip = true );
    static unsigned char* loadPixels(const std::string& fName, int& width, int& height, bool flip, int channels);
    static void deletePixels( unsigned char * );
    static GLuint createTexture(unsigned int w, unsigned h);
    static GLuint createBlackTexture();

    static GLuint create3D(unsigned int w, unsigned int h, unsigned int d);
    static void save(std::string file, GLuint id);
    static void saveWorldPos(std::string file, GLuint id);
    static void save3D(std::string file, GLuint id);

};