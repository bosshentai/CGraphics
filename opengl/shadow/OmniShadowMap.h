//
// Created by bosshentai on 11/6/22.
//

#pragma once
#include "ShadowMap.h"
class OmniShadowMap:
        public ShadowMap
{
public:
    OmniShadowMap();

    bool Init(GLuint width, GLuint height);

    void Write();

    void Read(GLenum textureUnit);

    ~OmniShadowMap();
};

