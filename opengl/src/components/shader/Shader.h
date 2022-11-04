#pragma once
#ifndef Shader_h
#define Shader_h

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include

#include "./utils/CommonValues.h"

class Shader
{
public:
  Shader();

  void createFromFiles(const char *vertexLocation, const char *fragmentLocation);
  void createFromStrings(const char *vertexCode, const char *fragmentCode);
  void createFromFiles(const char *vertexLocation, const char *geometryLocation, const char *fragmentLocation);

  void validade();


  void useShader();
  void clearShader();

  ~Shader();


private:
  int pointLightCount;
  int spotLightCount;

  GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
      uniformSpecularIntensity, uniformShininess, uniformTexture,
      uniformDirectionalLightTransform, uniformDirectionalShadowMap,
      uniformOmniLightPos, uniformFarPlane;

  GLuint uniformLightMatrices[6];

  struct
  {
    GLuint uniformColour;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;
    GLuint uniformDirection;

  } uniforDirectionalLight;

  struct
  {
    GLuint uniformColour;
    GLuint uniformAmbientIntensity;
    GLuint uniformDifuseIntensity;

    GLuint uniformPosition;
    GLuint uniformConstant;
    GLuint uniformLinear;
    GLuint uniformExponent;
  } uniformPointLight[MAX_POINT_LIGHTS];

  GLuint uniformShopLightCount;

  struct
  {
    GLuint uniformColour;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    GLuint uniformPosition;
    GLuint uniformConstant;
    GLuint uniformLinear;
    GLuint uniformExponent;

    GLuint uniformDirection;
    GLuint uniformEdge;

  } uniformShopLight[MAX_SPOT_LIGHTS];

  struct
  {
    GLuint shadowMap;
    GLuint farPlane;
  } uniformOmniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

  void compileShader(const char *vertexCode, const char *fragmentCode);
  void compileShader(const char *vertexCode, const char *geometryCode, const char *fragmentCode);
  void addShader(GLuint program, const char *shaderCode, GLenum shaderType);

  void compileProgram();
};

#endif