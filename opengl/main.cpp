
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

Model terra;
Model monitor;
Model table;

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
////
//    Mesh *obj2 = new Mesh();
//    obj2->createMesh(vertices, indices, 32, 12);
//    meshList.push_back(obj2);
////
//    Mesh *obj3 = new Mesh();
//    obj3->createMesh(floorVertices, floorIndices, 32, 6);
//    meshList.push_back(obj3);
}

void AddLights() {
    mainLight = DirectionalLight(2048, 2048,    //resolution
                                 1.0f, 0.53f, 0.3f, //color
                                 0.1f, 0.9f, //intensity 0.1 0.5
                                 -10.0f, -12.0f, -18.5f); //location

    pointLights[0] = PointLight(1024, 1024,       //width height needs to be square
                                0.1f, 100.0f,     //near and far plane
                                0.0f, 0.0f, 1.0f, //color
                                0.4f, 0.4f,          // aIntensity dIntensity
                                2.0f, 2.0f, 2.0f, // x y z coords
                                0.3f, 0.1f, 0.1f);//con lin exp
    pointLightCount++;

    spotLights[0] = SpotLight(1024, 1024,     //width height needs to be square
                              0.1f, 100.0f,   //near and far plane
                              1.0f, 1.0f, 1.0f,
                              0.0f, 2.0f, //brightness
                              0.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f, //attenuation values
                              20.0f); //angle of edge
    spotLightCount++;


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


//    glm::mat4 model = glm::mat4 (1.0f);
//    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

//     terra
//    model = glm::mat4(1.0);
//    glm::mat4 modelTerra = glm::mat4(1.0f);
//    modelTerra = glm::translate(modelTerra, glm::vec3(2.0f, -1.0f, -10.0f));
//    modelTerra = glm::scale(modelTerra, glm::vec3(1.0f, 1.0f, 1.0f));
//    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTerra));
//    terraTexture.useTexture();
//    shinyMaterial.UseMaterial(uniformSpecularIntensity,uniformShininess);
//    terra.renderModel();

    // monitor
    glm::mat4 modelMonitor = glm::mat4(1.0);
    modelMonitor = glm::translate(modelMonitor, glm::vec3(5.0f, -1.0f, -1.0f));
//    modelMonitor = glm::rotate(modelMonitor, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMonitor = glm::scale(modelMonitor, glm::vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMonitor));
//    monitorTexture.useTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    monitor.renderModel();

//table
    glm::mat4 modeltable = glm::mat4(1.0);
    modeltable = glm::translate(modeltable,glm::vec3(2.0f,-1.0f,-3.0f));
//    modeltable = glm::scale(modeltable,glm::vec3(.0f,100.0f,10.00f));
    glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(modeltable));
//    shinyMaterial.UseMaterial(uniformSpecularIntensity,uniformShininess);
    dullMaterial.UseMaterial(uniformSpecularIntensity,uniformShininess);
    table.renderModel();

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

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f),
                    -60.0f, 0.0f, 5.0f, 0.5f);


//    terra = Model();
//    terra.loadModel("../assets/Models/Terra/Earth2K.fbx");
//    terraTexture = Texture("../assets/Texture/Diffuse_2K.png");
//    terraTexture.LoadTextureA();


    monitor = Model();
    monitor.loadModel("../assets/Models/Monitor/MonitorTeste.obj");

//    monitorTexture = Texture((char *) "../assets/Models/Monitor/Monitor_1.png");
//    monitorTexture.LoadTextureA();
//    monitorTexture = Texture((char *) "../assets/Models/Monitor/Monitor_1_Specular.png");
//    monitorTexture.LoadTextureA();
//    monitorTexture = Texture((char *) "../assets/Models/Monitor/Monitor_1_Normal.png");
//    monitorTexture.LoadTextureA();

    table = Model();
    table.loadModel("../assets/Models/Table/table.obj");


//     terraTexture.LoadTextureA();

    shinyMaterial = Material(10.0f, 256);
    dullMaterial = Material(0.3f, 1);

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


        // TESTES

//        brickTexture = Texture("../assets/Texture/teste3.png");
//        brickTexture.LoadTextureA();
/*
		if (mainWindow.getkeys()[GLFW_KEY_E])
		{
			spotLights[0].Tooggle();
			mainWindow.getkeys()[GLFW_KEY_E] = false;
		}
*/
        DirectionalShadowMapPass(&mainLight);
        for (size_t i = 0; i < pointLightCount; i++) {
            OmniShadowMapPass(&pointLights[i]);
        }
        for (size_t i = 0; i < spotLightCount; i++) {
            OmniShadowMapPass(&spotLights[i]);
        }
        RenderPass(projection, camera.calculateViewMatrix());

        glUseProgram(0);

        mainWindow.swapBuffers();

    }

//    glfwTerminate();
    return 0;
}

