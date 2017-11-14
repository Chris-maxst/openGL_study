//
//  Cube.cpp
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 13..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#include "Cube.h"

static float VERTEX_BUF[] =
{
    -1, 1, 1, //left up front
    -1, 1, -1, //left up back
    1, 1, -1, //right up back
    1, 1, 1, //right up front
    -1, -1, 1, //left down front
    -1, -1, -1, //left down back
    1, -1, -1, //right down back
    1, -1, 1 //right down front
};

static unsigned char INDEX_BUF[] =
{
    0, 1, 2,
    2, 3, 0, //up
    
    4, 5, 6,
    6, 7, 4, //down
    
    0, 3, 7,
    7, 4, 0, //front
    
    1, 2, 6,
    6, 5, 1, //back
    
    0, 1, 5,
    5, 4, 0, //left
    
    3, 2, 6,
    6, 7, 3 //right
};

static float COLOR_BUF[] =
{
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

static const char * VERTEX_SHADER_SRC =
"attribute vec4 a_position;\n"                                \
"attribute vec4 a_color;\n"                                    \
"uniform mat4 u_mvpMatrix;\n"                                \
"varying vec4 v_color;\n"                    \
"void main()\n"                                                    \
"{\n"                                                            \
"    gl_Position = u_mvpMatrix  * a_position;\n"                    \
"    v_color = a_color;\n"                                        \
"}\n";

static const char * FRAGMENT_SHADER_SRC =
"precision mediump float;\n"
"varying vec4 v_color;\n"                    \
"void main()\n"                                    \
"{\n"                                            \
"    gl_FragColor = v_color;\n"                    \
"}\n";

Cube::Cube()
{
    vertices = &VERTEX_BUF[0];
    indices = &INDEX_BUF[0];
    colors = &COLOR_BUF[0];
    
    vertextCount = sizeof(VERTEX_BUF) / sizeof(float);
    indexCount = sizeof(INDEX_BUF) / sizeof(unsigned char);
    
    localMVPMatrix = gl_helper::Mat4::Identity();
    modelMatrix = gl_helper::Mat4::Identity();
    projectionMatrix = gl_helper::Mat4::Identity();
}

Cube::~Cube()
{
}

void Cube::setProjectionMatrix(gl_helper::Mat4 & projection)
{
    projectionMatrix = projection;
}

void Cube::setTransform(gl_helper::Mat4 & transform)
{
    modelMatrix = transform;
}

void Cube::setPosition(float x, float y, float z)
{
    modelMatrix.PostTranslate(x, y, z);
}

void Cube::setRotation(float angle, float x, float y, float z)
{
    modelMatrix.PostRotate(angle, x, y, z);
}

void Cube::setScale(float x, float y, float z)
{
    modelMatrix.PostScale(x, y, z);
}

void Cube::draw()
{
    if(program == 0)
    {
        program = ShaderUtil::createProgram(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
        positionHandle = glGetAttribLocation(program, "a_position");
        colorHandle = glGetAttribLocation(program, "a_color");
        mvpMatrixHandle = glGetUniformLocation(program, "u_mvpMatrix");
    }
    
    glUseProgram(program);
    ShaderUtil::checkGlError("glUseProgram");
    
    glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(positionHandle);
    
    glVertexAttribPointer(colorHandle, 4, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(colorHandle);
    
    localMVPMatrix = projectionMatrix * modelMatrix;
    glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, localMVPMatrix.Ptr());
    
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, indices);
    
    glDisableVertexAttribArray(positionHandle);
    glDisableVertexAttribArray(colorHandle);
    glUseProgram(0);
}
