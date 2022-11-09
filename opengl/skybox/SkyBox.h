//
// Created by bosshentai on 11/9/22.
//

#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "../utils/CommonValues.h"

#include "../mesh/Mesh.h"
#include "../shader/Shader.h"


class SkyBox{
public:
    SkyBox();

    SkyBox(std::vector<std::string> faceLoactions);

    void DrawSkyBox(glm:: mat4 viewMatrix,glm::mat4 projectionMatrix);

    ~SkyBox();

private:
    Mesh* skyMesh;
    Shader* skyShader;

    GLuint textureId;
    GLuint uniformProjection, uniformView;
};