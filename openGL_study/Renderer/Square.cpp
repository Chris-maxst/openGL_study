//
//  Square.cpp
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 13..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#include "Square.h"

static float VERTEX_BUF[] =
{
    0.5f, -0.5f, 0.0f,
    0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
    -0.5f,  -0.5f, 0.0f,
};

static unsigned char INDEX_BUF[] =
{
    0, 1, 2,
    2, 3, 0
};

static float COLOR_BUF[] =
{
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
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

Square::Square()
{
    vertices = &VERTEX_BUF[0];
    indices = &INDEX_BUF[0];
    colors = &COLOR_BUF[0];
    
    vertextCount = sizeof(VERTEX_BUF) / sizeof(float);
    indexCount = sizeof(INDEX_BUF) / sizeof(unsigned char);
    
    localMVPMatrix = gl_helper::Mat4::Identity();
}

Square::~Square()
{
}

void Square::draw()
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
    
    glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, localMVPMatrix.Ptr());
    
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, indices);
    
    glDisableVertexAttribArray(positionHandle);
    glDisableVertexAttribArray(colorHandle);
    glUseProgram(0);
}
