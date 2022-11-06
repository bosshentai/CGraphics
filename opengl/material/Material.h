//
// Created by bosshentai on 11/6/22.
//

#pragma once

#include <GL/glew.h>

class Material
{
public:
    Material();
    Material(GLfloat sIntensity, GLfloat shine);

    void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

    ~Material();

private:
    GLfloat specularIntensity;
    GLfloat shininess;
};
