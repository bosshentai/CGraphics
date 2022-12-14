//
// Created by bosshentai on 11/6/22.
//

#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H

#include <stdio.h>
#include <GL/glew.h>

class Mesh
{
public:
    Mesh();

    void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void renderMesh();
    void clearMesh();

    ~Mesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};


#endif //OPENGL_MESH_H
