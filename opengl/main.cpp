
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
//#define STBI_FAILURE
//#include "stbImage/stb_image.h"
#include "./texture/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <assimp/Importer.hpp>

#include "./utils/CommonValues.h"
#include "./window/Window.h"
#include "./mesh/Mesh.h"
#include "./shader/Shader.h"
#include "./camera/Camera.h"
#include "./light/DirectionalLight.h"
#include "./light/PointLight.h"
#include "./light/SpotLight.h"
#include "./material/Material.h"
#include "./model/Model.h"

#include "./skybox/SkyBox.h"

const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
        uniformSpecularIntensity = 0, uniformShininess = 0,
        uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;
//
Texture brickTexture;
//Texture dirtTexture;
//Texture plainTexture;
//Texture testeTexture;
Texture terraTexture;
Texture monitorTexture;

Material shinyMaterial;
Material dullMaterial;

Model table;
Model monitor;
Model floorDX;
Model chair;
Model wall;
Model door;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

SkyBox skyBox;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex shader
static const char *vShader = "../assets/shader/shader.vert";

// Fragment shader
static const char *fShader = "../assets/shader/shader.frag";

//// Vertex directional Shadow map
static const char *vDirecShadowMap = "../assets/shader/directional_shadow_map.vert";
//
//// Fragment directional Shadow map
static const char *fDirectShadowMap = "../assets/shader/directional_shadow_map.frag";
//
//// Vertex omni Shadow map
static const char *vOmniShadowMap = "../assets/shader/omni_shadow_map.vert";
//
//// Geometrix omni Shadow map
static const char *gOmniShadowMap = "../assets/shader/omni_shadow_map.geom";
//
//// Fragment omni Shadow map
static const char *fOmniShadowMap = "../assets/shader/omni_shadow_map.frag";


static const char *skyBoxBottom = "../assets/Texture/SkyBox/skyrender0006.tga";
static const char *skyBoxTop = "../assets/Texture/SkyBox/skyrender0005.tga";
static const char *skyBoxFront = "../assets/Texture/SkyBox/skyrender0004.tga";
static const char *skyBoxLeft = "../assets/Texture/SkyBox/skyrender0003.tga";
static const char *skyBoxRight = "../assets/Texture/SkyBox/skyrender0001.tga";
static const char *skyBoxBack = "../assets/Texture/SkyBox/skyrender0002.tga";

void CreateObjects() {
    unsigned int indices[] = {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
    };

    GLfloat vertices[] = {
            //	x      y      z			u	  v			nx	  ny    nz
            -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] = {
            0, 2, 1,
            1, 2, 3
    };

    GLfloat floorVertices[] = {
            -10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            -10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f
    };


//    Mesh *obj1 = new Mesh();
//    obj1->createMesh(vertices, indices, 32, 12);
//    meshList.push_back(obj1);

//    Mesh *obj2 = new Mesh();
//    obj2->createMesh(vertices, indices, 32, 12);
//    meshList.push_back(obj2);

//    Mesh *obj3 = new Mesh();
//    obj3->createMesh(floorVertices, floorIndices, 32, 6);
//    meshList.push_back(obj3);
}

void AddLights() {
//    mainLight = DirectionalLight(1024, 1024,    //resolution
//                                 1.0f, 0.53f, 0.3f, //color
//                                 0.1f, 0.9f, //intensity 0.1 0.5
//                                 -10.0f, -12.0f, -18.5f); //location

    mainLight = DirectionalLight(2048, 2048,    //resolution
                                 1.0f, 1.0f, 1.0f, //color
                                 0.5f, 0.1f, //intensity 0.1 0.5
                                 0.0f, 0.0f, -50.5f); //location



    pointLights[0] = PointLight(1024, 1024,       //width height needs to be square
                                0.1f, 100.0f,     //near and far plane
                                1.0f, 1.0f, 1.0f, //color
                                0.1f, 0.1f,          // aIntensity dIntensity
                                0.0f, 0.0f, 0.0f, // x y z coords
                                0.4f, 0.2f, 0.2f);//con lin exp
    pointLightCount++;

//    pointLights[0] = PointLight(2048, 2048,       //width height needs to be square
//                                0.1f, 100.0f,     //near and far plane
//                                0.0f, 0.0f, 1.0f, //color
//                                0.4f, 0.4f,          // aIntensity dIntensity
//                                2.0f, 2.0f, 2.0f, // x y z coords
//                                0.3f, 0.1f, 0.1f);//con lin exp
//    pointLightCount++;

    spotLights[0] = SpotLight(1024, 1024,     //width height needs to be square
                              0.1f, 50.0f,   //near and far plane
                              1.0f, 1.0f, 1.0f,
                              0.0f, 0.5f, //brightness
                              0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f,
                              1.0f, 0.0f, 0.0f, //attenuation values
                              0.0f); //angle of edge
    spotLightCount++;

//
//    spotLights[0] = SpotLight(2048, 2048,     //width height needs to be square
//                              0.1f, 100.0f,   //near and far plane
//                              1.0f, 1.0f, 1.0f,
//                              0.0f, 4.0f, //brightness
//                              0.0f, 0.0f, 0.0f,
//                              0.0f, -1.0f, 0.0f,
//                              1.0f, 0.0f, 0.0f, //attenuation values
//                              20.0f); //angle of edge
//    spotLightCount++;

}

void DrawSky() {
    std::vector<std::string> skyFaces;

    // left
    // right
    // top
    // bottom
    // back
    // front
    skyFaces.push_back(skyBoxLeft);
    skyFaces.push_back(skyBoxRight);
    skyFaces.push_back(skyBoxTop);
    skyFaces.push_back(skyBoxBottom);
    skyFaces.push_back(skyBoxBack);
    skyFaces.push_back(skyBoxFront);

    skyBox = SkyBox(skyFaces);

}


void CreateShader() {
    Shader *shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);

    directionalShadowShader = Shader();
    directionalShadowShader.createFromFiles(vDirecShadowMap,
                                            fDirectShadowMap);

    omniShadowShader = Shader();
    omniShadowShader.createFromFiles(vOmniShadowMap, gOmniShadowMap,
                                     fOmniShadowMap);
}

void RenderScene() {

//    glm::mat4 model = glm::mat4(1.0);
//    model = glm::translate(model,glm:: vec3(0.f,0.f,-2.5f));
//
//    glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
//
////    dullMaterial.UseMaterial(uniformSpecularIntensity,uniformShininess);
//    meshList[0]->renderMesh();


// table
    // primera fila
    glm::mat4 modelTable = glm::mat4(1.0);
    modelTable = glm::translate(modelTable, glm::vec3(7.0f, -4.75f, -23.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
//    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(-4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(-4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(-4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(-4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
//
    modelTable = glm::translate(modelTable, glm::vec3(-4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
//
//    // segunda fila
    modelTable = glm::translate(modelTable, glm::vec3(0.0f, 0.0f, -4.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
//    // terceira fila
    modelTable = glm::translate(modelTable, glm::vec3(0.0f, 0.0f, -4.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(-4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(-4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(-4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(-4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(-4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
//    // quarta fila
//
    modelTable = glm::translate(modelTable, glm::vec3(-0.0f, 0.0f, -4.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
    modelTable = glm::translate(modelTable, glm::vec3(4.0f, 0.0f, -0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTable));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    table.renderModel();
//
//    // monitor
    glm::mat4 modelMonitor = glm::mat4(1.0);
    modelMonitor = glm::translate(modelMonitor, glm::vec3(7.0f, -3.68f, -23.2f));
    modelMonitor = glm::rotate(modelMonitor, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMonitor = glm::scale(modelMonitor, glm::vec3(.2f, .2f, .2f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    monitor.renderModel();
////
////
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, -20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, -20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, -20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, -20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, -20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
//    // segunda fila
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(20.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, 20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, 20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, 20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, 20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, 20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
//    // terceira fila
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(20.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, -20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, -20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, -20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, -20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, -20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
//    // quarta fila
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(20.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
    modelMonitor = glm::translate(modelMonitor, glm::vec3(0.0f, 0.0f, 20.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
    monitor.renderModel();
//
//
//    // chair
    glm::mat4 modelChair = glm:: mat4(1.0);
    modelChair = glm::translate(modelChair, glm::vec3(7.0f, -4.7f, -22.0f));
    modelChair = glm::scale(modelChair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    modelChair = glm::rotate(modelChair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelChair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();

    // chair 2
    glm::mat4 model2Chair = glm:: mat4(1.0);
    model2Chair = glm::translate(model2Chair,glm::vec3(3.0f,-4.7f,-22.0f));
    model2Chair = glm::scale(model2Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model2Chair = glm::rotate(model2Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();

    // chair 3
    glm::mat4 model3Chair = glm:: mat4(1.0);
    model3Chair = glm::translate(model3Chair,glm::vec3(-1.0f,-4.7f,-22.0f));
    model3Chair = glm::scale(model3Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model3Chair = glm::rotate(model3Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model3Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();


    // chair 4
    glm::mat4 model4Chair = glm:: mat4(1.0);
    model4Chair = glm::translate(model4Chair,glm::vec3(-5.0f,-4.7f,-22.0f));
    model4Chair = glm::scale(model4Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model4Chair = glm::rotate(model4Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model4Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();

    // chair 5
    glm::mat4 model5Chair = glm:: mat4(1.0);
    model5Chair = glm::translate(model5Chair,glm::vec3(-9.0f,-4.7f,-22.0f));
    model5Chair = glm::scale(model5Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model5Chair = glm::rotate(model5Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model5Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();

    // chair 6
    glm::mat4 model6Chair = glm:: mat4(1.0);
    model6Chair = glm::translate(model6Chair,glm::vec3(-13.0f,-4.7f,-22.0f));
    model6Chair = glm::scale(model6Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model6Chair = glm::rotate(model6Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model6Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();


    // segunda fila
    // chair 7
    glm::mat4 model7Chair = glm:: mat4(1.0);
    model7Chair = glm::translate(model7Chair, glm::vec3(7.0f, -4.7f, -26.0f));
    model7Chair = glm::scale(model7Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model7Chair = glm::rotate(model7Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model7Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();

    // chair 8
    glm::mat4 model8Chair = glm:: mat4(1.0);
    model8Chair = glm::translate(model8Chair,glm::vec3(3.0f,-4.7f,-26.0f));
    model8Chair = glm::scale(model8Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model8Chair = glm::rotate(model8Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model8Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();


    // chair 9
    glm::mat4 model9Chair = glm:: mat4(1.0);
    model9Chair = glm::translate(model9Chair,glm::vec3(-1.0f,-4.7f,-26.0f));
    model9Chair = glm::scale(model9Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model9Chair = glm::rotate(model9Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model9Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();


    // chair 10
    glm::mat4 model10Chair = glm:: mat4(1.0);
    model10Chair = glm::translate(model10Chair,glm::vec3(-5.0f,-4.7f,-26.0f));
    model10Chair = glm::scale(model10Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model10Chair = glm::rotate(model10Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model10Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();

    // chair 11
    glm::mat4 model11Chair = glm:: mat4(1.0);
    model11Chair = glm::translate(model11Chair,glm::vec3(-9.0f,-4.7f,-26.0f));
    model11Chair = glm::scale(model11Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model11Chair = glm::rotate(model11Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model11Chair));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();


    // chair 12
    glm::mat4 model12Chair = glm:: mat4(1.0);
    model12Chair = glm::translate(model12Chair,glm::vec3(-13.0f,-4.7f,-26.0f));
    model12Chair = glm::scale(model12Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model12Chair = glm::rotate(model12Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model12Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();

    // terceira fila
    // chair 13
    glm::mat4 model13Chair = glm:: mat4(1.0);
    model13Chair = glm::translate(model13Chair, glm::vec3(7.0f, -4.7f, -30.0f));
    model13Chair = glm::scale(model13Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model13Chair = glm::rotate(model13Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model13Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();

    // chair 14
    glm::mat4 model14Chair = glm:: mat4(1.0);
    model14Chair = glm::translate(model14Chair,glm::vec3(3.0f,-4.7f,-30.0f));
    model14Chair = glm::scale(model14Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model14Chair = glm::rotate(model14Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model14Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();


    // chair 15
    glm::mat4 model15Chair = glm:: mat4(1.0);
    model15Chair = glm::translate(model15Chair,glm::vec3(-1.0f,-4.7f,-30.0f));
    model15Chair = glm::scale(model15Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model15Chair = glm::rotate(model15Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model15Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();


    // chair 16
    glm::mat4 model16Chair = glm:: mat4(1.0);
    model16Chair = glm::translate(model16Chair,glm::vec3(-5.0f,-4.7f,-30.0f));
    model16Chair = glm::scale(model16Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model16Chair = glm::rotate(model16Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model16Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();

    // chair 17
    glm::mat4 model17Chair = glm:: mat4(1.0);
    model17Chair = glm::translate(model17Chair,glm::vec3(-9.0f,-4.7f,-30.0f));
    model17Chair = glm::scale(model17Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model17Chair = glm::rotate(model17Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model17Chair));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();


    // chair 18
    glm::mat4 model18Chair = glm:: mat4(1.0);
    model18Chair = glm::translate(model18Chair,glm::vec3(-13.0f,-4.7f,-30.0f));
    model18Chair = glm::scale(model18Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model18Chair = glm::rotate(model18Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model18Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();


    // 4

    // quarta fila

    // chair 19
    glm::mat4 model19Chair = glm:: mat4(1.0);
    model19Chair = glm::translate(model19Chair,glm::vec3(-9.0f,-4.7f,-34.0f));
    model19Chair = glm::scale(model19Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model19Chair = glm::rotate(model19Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model19Chair));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();


    // chair 20
    glm::mat4 model20Chair = glm:: mat4(1.0);
    model20Chair = glm::translate(model20Chair,glm::vec3(-13.0f,-4.7f,-34.0f));
    model20Chair = glm::scale(model20Chair, glm::vec3(0.0015f, 0.0015f, 0.0015f));
    model20Chair = glm::rotate(model20Chair,glm::radians(-180.0f),glm::vec3(0.0f,1.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model20Chair));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    chair.renderModel();




////floor
    glm::mat4 modelfloor = glm::mat4(1.0);
    modelfloor = glm::translate(modelfloor, glm::vec3(-2.0f, -5.0f, -30.0f));
    modelfloor = glm::scale(modelfloor, glm::vec3(10.0f, 0.2f, 10.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelfloor));
//    shinyMaterial.UseMaterial(uniformSpecularIntensity,uniformShininess);
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    floorDX.renderModel();
//
//
//    // Wall
//
//    // left wall
    glm::mat4 modelwall = glm::mat4 (1.0);
    modelwall = glm::translate(modelwall,glm::vec3(18.5f,-2.3f,-30.0f));
    modelwall = glm::scale(modelwall,glm::vec3(0.2f,0.3f,1.43f));
    modelwall = glm::rotate(modelwall,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
    modelwall = glm::rotate(modelwall,glm::radians(90.0f),glm::vec3(0.0f,0.0f,1.0f));
    dullMaterial.UseMaterial(uniformSpecularIntensity,uniformShininess);
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelwall));
    wall.renderModel();
//
//    // right wall
    modelwall = glm::translate(modelwall,glm::vec3(0.0f,202.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelwall));
    wall.renderModel();
//
//    // bottom wall
    modelwall = glm::translate(modelwall,glm::vec3(-9.0f,-100.0f,0.0f));
    modelwall = glm::scale(modelwall,glm::vec3(0.2f,10.8f,1.0f));
    modelwall = glm::rotate(modelwall,glm::radians(90.0f),glm::vec3(0.0f,0.0f,1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelwall));
    wall.renderModel();
//
//    // top wall
    modelwall = glm::translate(modelwall,glm::vec3(0.0f,-44.0f,-10.0f));
    modelwall = glm::scale(modelwall,glm::vec3(1.0f,5.6f,1.0f));
    modelwall = glm::rotate(modelwall,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelwall));
    wall.renderModel();


    // front wall
    modelwall = glm::translate(modelwall,glm::vec3(0.0f,9.7f,8.5f));
    modelwall = glm::scale(modelwall,glm::vec3(1.0f,1.0f,0.1f));
    modelwall = glm::rotate(modelwall,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelwall));
    wall.renderModel();

    // door
    glm::mat4 modelDoor = glm::mat4 (1.0);
    modelDoor = glm::translate(modelDoor,glm::vec3(10.0f,-4.8f,-16.5f));
    modelDoor = glm::scale(modelDoor,glm::vec3(1.5f,1.5f,1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelDoor));
    door.renderModel();
}

void DirectionalShadowMapPass(DirectionalLight *light) {
    directionalShadowShader.useShader();

    glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

    light->getShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::mat4 lightTransform = light->CalculateLightTransform();


    uniformModel = directionalShadowShader.getModelLocation();
    directionalShadowShader.setDirectionalLightTransform(&lightTransform);

    directionalShadowShader.validate();

    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void OmniShadowMapPass(PointLight *light) {
    omniShadowShader.useShader();

    glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

    light->getShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = omniShadowShader.getModelLocation();
    uniformOmniLightPos = omniShadowShader.getOmniLightPosLocation();
    uniformFarPlane = omniShadowShader.getFarPlaneLocation();

    glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
    glUniform1f(uniformFarPlane, light->GetFarPlane());
    omniShadowShader.setLightMatrices(light->CalculateLightTransform());

    omniShadowShader.validate();

    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {


    glViewport(0, 0, 1920, 1080);
//        glViewport(0, 0, 1920, 1080);

    // clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skyBox.DrawSkyBox(viewMatrix, projectionMatrix);


    shaderList[0].useShader();
    uniformModel = shaderList[0].getModelLocation();
    uniformProjection = shaderList[0].getProjectionLocation();
    uniformView = shaderList[0].getViewLocation();
    uniformEyePosition = shaderList[0].getEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
    uniformShininess = shaderList[0].getShininessLocation();


    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y,
                camera.getCameraPosition().z);

    glm::mat4 lightTransform = mainLight.CalculateLightTransform();

    shaderList[0].setDirectionalLight(&mainLight);
    shaderList[0].setPointLights(pointLights, pointLightCount, 3, 0);
    shaderList[0].setSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
    shaderList[0].setDirectionalLightTransform(&lightTransform);

    mainLight.getShadowMap()->Read(GL_TEXTURE2);
    shaderList[0].setTexture(1);
    shaderList[0].setDirectionalShadowMap(2);

//    glm::vec3 lowerLight = camera.getCameraPosition();
//    lowerLight.y -= 0.0f;
    /*spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());*/

    shaderList[0].validate();

    RenderScene();
}

int main() {
    mainWindow = Window(1424, 800);
    mainWindow.Initialise();

//    glfwSetInputMode(mainWindow,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);




    CreateObjects();
    CreateShader();

    camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f),
                    -60.0f, 0.0f, 5.0f, 0.5f);


//    terra = Model();
//    terra.loadModel("../assets/Models/Terra/Earth2K.fbx");
//    terraTexture = Texture("../assets/Texture/Diffuse_2K.png");
//    terraTexture.LoadTextureA();

//     monitor
    monitor = Model();
    monitor.loadModel("../assets/Models/MoniTest/Lowpoly_Notebook_2.obj");
//    monitor.loadModel("../assets/Models/Monitor/MonitorTeste.obj");


//     table
    table = Model();
    table.loadModel("../assets/Models/Table/standing_desk.obj");

//     floor
    floorDX = Model();
    floorDX.loadModel("../assets/Models/Floor/floor.obj");

//     chair
    chair = Model();
    chair.loadModel("../assets/Models/Chair/Office_chair.obj");

//     wall
    wall = Model();
    wall.loadModel("../assets/Models/Wall/wall.obj");

    // door
    door = Model();
    door.loadModel("../assets/Models/Door/Door_Component_BI3.obj");




     terraTexture.LoadTextureA();

    shinyMaterial = Material(50.0f, 100);
    dullMaterial = Material(0.5f, 1);

    AddLights();

    DrawSky();

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
            uniformSpecularIntensity = 0, uniformShininess = 0;
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
                                            (float) mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f,
                                            100.f);



    /* Loop until the user closes the window */
    while (!mainWindow.getShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.keyControl(mainWindow.getkeys(), deltaTime);
        camera.mouseControl(mainWindow.getDeltaMouseX(), mainWindow.getDeltaMouseY());



        DirectionalShadowMapPass(&mainLight);
        for (size_t i = 0; i < pointLightCount; i++) {
            OmniShadowMapPass(&pointLights[i]);
        }
        for (size_t i = 0; i < spotLightCount; i++) {
            OmniShadowMapPass(&spotLights[i]);
        }
        RenderPass(projection, camera.calculateViewMatrix());

//        glUseProgram(0);

        mainWindow.swapBuffers();

    }

//    glfwTerminate();
    return 0;
}

