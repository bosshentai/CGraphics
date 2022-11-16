//
// Created by bosshentai on 11/6/22.
//
#pragma once


#include <GL/glew.h>
#include "../utils/CommonValues.h"


class Texture
{
public:

    Texture();
    Texture(const char* filePath);

    bool loadTexture();
    bool LoadTextureA();
    void useTexture();
    void clearTexture();

    ~Texture();

private:
    GLuint textureID;
    int width, height, bitdepth;

    const char* filePath;
};


