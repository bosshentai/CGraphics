
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
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



const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
        uniformSpecularIntensity = 0, uniformShininess = 0,
        uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;
//
Texture brickTexture;
//Texture dirtTexture;
//Texture plainTexture;
//Texture testeTexture;

Material shinyMaterial;
//Material dullMaterial;


DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex shader
static const char* vShader = "../assets/shader/shader.vert";

// Fragment shader
static const char* fShader = "../assets/shader/shader.frag";

//// Vertex directional Shadow map
//static const char* vDirecShadowMap = "./assets/directional_shadow_map.vert";
//
//// Fragment directional Shadow map
//static const char* fDirectShadowMap = "./assets/directional_shadow_map.frag";
//
//// Vertex omni Shadow map
//static const char* vOmniShadowMap = "./assets/omni_shadow_map.vert";
//
//// Geometrix omni Shadow map
//static const char* gOmniShadowMap = "./assets/omni_shadow_map.geom";
//
//// Fragment omni Shadow map
//static const char* fOmniShadowMap = "./assets/omni_shadow_map.frag";

void CreateObjects(){
    unsigned int indices[] = {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
    };

    GLfloat vertices[] = {
            //	x      y      z			u	  v			nx	  ny    nz
            -1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] = {
            0, 2, 1,
            1, 2, 3
    };

    GLfloat floorVertices[] = {
            -10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
            -10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
            10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
    };


    Mesh *obj1 = new Mesh();
    obj1->createMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->createMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);

    Mesh *obj3 = new Mesh();
    obj3->createMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(obj3);
}

void AddLights(){
    mainLight = DirectionalLight(2048, 2048,
                                 1.0f, 0.53f, 0.3f,
                                 0.1f, 0.9f,
                                 -10.0f, -12.0f, 18.5f);

    pointLights[0] = PointLight(1024, 1024,
                                0.01f, 100.0f,
                                0.0f, 0.0f, 1.0f,
                                0.0f, 1.0f,
                                1.0f, 2.0f, 0.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;
    pointLights[1] = PointLight(1024, 1024,
                                0.01f, 100.0f,
                                0.0f, 1.0f, 0.0f,
                                0.0f, 1.0f,
                                -4.0f, 3.0f, 0.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;


    spotLights[0] = SpotLight(1024, 1024,
                              0.01f, 100.0f,
                              1.0f, 1.0f, 1.0f,
                              0.0f, 2.0f,
                              0.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              20.0f);
    spotLightCount++;
    spotLights[1] = SpotLight(1024, 1024,
                              0.01f, 100.0f,
                              1.0f, 1.0f, 1.0f,
                              0.0f, 1.0f,
                              0.0f, -1.5f, 0.0f,
                              -100.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              20.0f);
    spotLightCount++;
}


void CreateShader(){
    Shader* shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);

    directionalShadowShader = Shader();
    directionalShadowShader.createFromFiles("../assets/shader/directional_shadow_map.vert","../assets/shader/directional_shadow_map.frag");

    omniShadowShader = Shader();
    omniShadowShader.createFromFiles("../assets/shader/omni_shadow_map.vert","../assets/shader/omni_shadow_map.geom","../assets/shader/omni_shadow_map.frag");
}

void RenderScene(){
    glm::mat4 model(1.0f);

    //triangle
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.useTexture();
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->renderMesh();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
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


void OmniShadowMapPass(PointLight* light)
{
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


void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    glViewport(0, 0, 1920, 1080);

    // clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    skybox.DrawSkybox(viewMatrix, projectionMatrix);

    shaderList[0].useShader();

    uniformModel = shaderList[0].getModelLocation();
    uniformProjection = shaderList[0].getProjectionLocation();
    uniformView = shaderList[0].getViewLocation();
    uniformEyePosition = shaderList[0].getEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
    uniformShininess = shaderList[0].getShininessLocation();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

    glm::mat4 lightTransform = mainLight.CalculateLightTransform();

    shaderList[0].setDirectionalLight(&mainLight);
    shaderList[0].setPointLights(pointLights, pointLightCount, 3, 0);
    shaderList[0].setSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
    shaderList[0].setDirectionalLightTransform(&lightTransform);

    mainLight.getShadowMap()->Read(GL_TEXTURE2);
    shaderList[0].setTexture(1);
    shaderList[0].setDirectionalShadowMap(2);

    glm::vec3 lowerLight = camera.getCameraPosition();
    lowerLight.y -= 0.0f;
    /*spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());*/

    shaderList[0].validate();

    RenderScene();
}

int main(void)
{
    mainWindow = Window(1024,800);
    mainWindow.Initialise();

    CreateObjects();
    CreateShader();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f),
                    -60.0f, 0.0f, 5.0f, 0.5f);



    AddLights();

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
            uniformSpecularIntensity = 0, uniformShininess = 0;
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),(float) mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.f);



    /* Loop until the user closes the window */
    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.keyControl(mainWindow.getkeys(), deltaTime);
        camera.mouseControl(mainWindow.getDeltaMouseX(), mainWindow.getDeltaMouseY());

        brickTexture = Texture("../assets/Texture/teste3.png");
        brickTexture.LoadTextureA();
/*
		if (mainWindow.getkeys()[GLFW_KEY_E])
		{
			spotLights[0].Tooggle();
			mainWindow.getkeys()[GLFW_KEY_E] = false;
		}
*/
        DirectionalShadowMapPass(&mainLight);
        for (size_t i = 0; i < pointLightCount; i++)
        {
            OmniShadowMapPass(&pointLights[i]);
        }
        for (size_t i = 0; i < spotLightCount; i++)
        {
            OmniShadowMapPass(&spotLights[i]);
        }
        RenderPass(projection, camera.calculateViewMatrix());

        glUseProgram(0);

        mainWindow.swapBuffers();

    }

//    glfwTerminate();
    return 0;
}

