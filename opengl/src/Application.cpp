
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <iostream>

// #include "cores.h"
#include "primitive.h"

static float angulo = 0.0;

// Display List
unsigned int sphereID[3];

void redirect(int w, int h)
{

  glViewport(0, 0, w, h);
  float aspect = (float)w / float(h);

  // for cube
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // if (w >= h)
  // {
  //   glOrtho(-10.0 * aspect, 10.0 * aspect, -10.0, 10.0, 1.0, -1.0);
  // }
  // else
  // {
  //   glOrtho(-10.0, 10.0, -10.0 / aspect, 10.0 / aspect, 1.0, -1.0);
  // }

  // camera
  gluPerspective(60.0, aspect, 0.1, 1000);

  glMatrixMode(GL_MODELVIEW);
}

void draw()
{

  glLoadIdentity();

  glPushMatrix();
  glTranslatef(-25.0, 0.0, -50.0);
  glRotatef(angulo, 1.0, 1.0, 0.0);
  // esfera[0] = glGenLists(3);
  glCallList(sphereID[0]);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0, 0.0, -50.0);
  glRotatef(angulo, 1.0, 1.0, 0.0);
  glCallList(sphereID[1]);
  glPopMatrix();


glPushMatrix();
  glTranslatef(20.0,0.0,-50.0);
  glRotatef(-angulo,1.0,1.0,0.0);
  glCallList(sphereID[2]);
glPopMatrix();


      angulo += 0.05;

  // glLoadIdentity();

  // glPushMatrix();
  // // move the object
  // glTranslatef(-10.0, 0.0, -50.0);
  // glRotatef(angulo, 1.0, 1.0, 0.0);

  // // rotate the object
  // // glRotatef(30.0,1.0,1.0,0.0);

  // // scale the object
  // // glScalef(2.0,1.0,2.0);
  // // drawCube(8.0);
  //   drawEsfera(10.0,20.0,20.0);
  // glPopMatrix();

  // // glLoadIdentity();
  // glPushMatrix();
  // glTranslatef(20.0, 0.0, -50.0);
  // glRotatef(-angulo, 1.0, 1.0, 0.0);
  // drawCube(8.0);
  // glPopMatrix();

  // angulo += 0.1;
}

void init()
{
  // Background color
  // glClearColor(0.0, 0.15, 0.25, 1.0);
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // espia el o q
  glEnable(GL_DEPTH_TEST);

  // create identificador para esfera
  sphereID[0] = glGenLists(3);
  sphereID[1] = sphereID[0] + 1;
  sphereID[2] = sphereID[0] + 2;

  drawSphere(sphereID[0], green, 10.0, 20.0, 20.0);

  drawSphere(sphereID[1], red, 5.0, 20.0, 20.0);

  drawSphere(sphereID[2], blue, 8.0, 20.0, 20.0);
}

int main(void)
{

  const int WindowWidth = 800;
  const int WindowHeight = 600;
  const char *WindowTitle = "App";

  // Initialize the library
  if (!glfwInit())
  {
    return -1;
  }

  // Create a windowed mode window and its OpenGL Context
  GLFWwindow *window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);

  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Pc info
  std::cout << "Company:" << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Model:" << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OPENGL:" << glGetString(GL_VERSION) << std::endl;

  // # start
  init();

  // change the Triangle
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glLineWidth(4.f);

  // Loop until the user close the windows
  while (!glfwWindowShouldClose(window))
  {

    // Poll for and process events
    glfwPollEvents();

    // event use s to close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Render here
    // clear background color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // redireciona
    redirect(width, height);

    draw();

    // drawTriangle();
    // drawSquare();

    // Swap front and back buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}