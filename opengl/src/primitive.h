#pragma once
#include <GL/glu.h>
#include <vector>
#include <math.h>
#include "cores.h"

struct vec3
{
  float x, y, z;
  vec3() : x(0.0), y(0.0), z(0.0) {}
  vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

// void drawRect(float p1[3], float p2[3], float p3[3], float p4[3], color color)
void drawRect(vec3 p1, vec3 p2, vec3 p3, vec3 p4, color color)
{

  // set color
  glColor3fv(color);

  // draw Quads
  glBegin(GL_QUADS);
  glVertex3fv(&p1.x);
  glVertex3fv(&p2.x);
  glVertex3fv(&p3.x);
  glVertex3fv(&p4.x);
  glEnd();
}

void drawCube(float s)
{
  float d = s / 2.0;

  // vert front
  // float v1[3] = {-d, d, d};
  // float v2[3] = {-d, -d, d};
  // float v3[3] = {d, -d, d};
  // float v4[3] = {d, d, d};
  vec3 v1(-d, d, d);
  vec3 v2(-d, -d, d);
  vec3 v3(d, -d, d);
  vec3 v4(d, d, d);

  // vert back
  // float v5[3] = {d, d, -d};
  // float v6[3] = {d, -d, -d};
  // float v7[3] = {-d, -d, -d};
  // float v8[3] = {-d, d, -d};
  vec3 v5(d, d, -d);
  vec3 v6(d, -d, -d);
  vec3 v7(-d, -d, -d);
  vec3 v8(-d, d, -d);

  // front face
  drawRect(v1, v2, v3, v4, red);

  // right face
  drawRect(v4, v3, v6, v5, blue);

  // back face
  drawRect(v5, v8, v7, v6, yellow);

  // left face
  drawRect(v1, v8, v7, v2, green);

  // top face
  drawRect(v1, v4, v5, v8, orange);

  // botton face
  drawRect(v2, v7, v6, v3, purple);
}

void drawSphere(unsigned int &ID, color color, GLfloat raio, GLuint nStacks, GLuint nSectors)
{

  std::vector<std::vector<GLuint>> indices;

  std::vector<vec3> pontos;
  const GLfloat PI = M_PI;

  GLfloat deltaPhi = PI / nStacks;
  GLfloat deltaTheta = 2 * PI / nSectors;

  for (GLuint i = 0; i <= nStacks; i++)
  {
    GLfloat phi = -PI / 2.0 + i * deltaPhi;
    GLfloat temp = raio * cos(phi);
    GLfloat y = raio * sin(phi);
    std::vector<GLuint> pt;
    for (GLuint j = 0; j < nSectors; j++)
    {
      GLfloat theta = j * deltaTheta;
      GLfloat x = temp * sin(theta);
      GLfloat z = temp * cos(theta);

      pontos.push_back(vec3(x, y, z));

      GLuint index = pontos.size() - 1;

      pt.push_back(index);
    } // next j
    indices.push_back(pt);
  } // next i

  // show pontos
  // glColor3fv(red);
  // glPointSize(2.5);
  // glBegin(GL_POINTS);
  // for(GLuint i=0;i<pontos.size();i++){
  //   glVertex3fv(&pontos[i].x);
  // }
  // glEnd();

  for (GLuint i = 0; i < nStacks; i++)
  {

    glNewList(ID, GL_COMPILE);


  glColor3fv(color);
  glPointSize(1.5);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // if(i%3 == 0){
    //   glColor3fv(white_Snow);
    // }else if (i % 3 ==1){
    //   glColor3fv(grey);
    // }else{
    //   glColor3fv(grey_light);
    // }

    glBegin(GL_TRIANGLE_STRIP);

    for (GLuint j = 0; j < nSectors; j++)
    {
      GLuint index = indices[i][j];
      glVertex3fv(&pontos[index].x);
      index = indices[i + 1][j];
      glVertex3fv(&pontos[index].x);

      if (j == nSectors - 1)
      {
        index = indices[i][0];
        glVertex3fv(&pontos[index].x);
        index = indices[i + 1][0];
        glVertex3fv(&pontos[index].x);
      }
    }
    glEnd();
  }
  // for(GLuint i = 0; i < pontos.size(); i++){
  //   glVertex3fv(&pontos[i].x);
  // }

  glEndList();

  // glEnd();
}

void drawQuad(float size, color color)
{

  float dimension = size / 2;

  glColor3fv(color);

  glBegin(GL_QUADS);
  glVertex3f(-dimension, -dimension, 0.0);
  glVertex3f(dimension, -dimension, 0.0);
  glVertex3f(dimension, dimension, 0.0);
  glVertex3f(-dimension, dimension, 0.0);

  glEnd();
}

void drawTriangle(color color)

{

  glColor3fv(color);
  glBegin(GL_TRIANGLES);
  glVertex3f(0.0, 0.5, 0.0);
  glVertex3f(-0.5, -0.5, 0.0);
  glVertex3f(0.5, -0.5, 0.0);
  glEnd();
}