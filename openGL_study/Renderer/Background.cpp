//
//  Background.cpp
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 15..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#include "Background.h"

static float VERTEX_BUF[] =
{
    -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f,  -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
};

static unsigned char INDEX_BUF[] =
{
    0, 1, 2,
    2, 3, 0
};

static float TEX_COORD_BUF[] =
{
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};

static const char * VERTEX_SHADER_SRC =
"attribute vec4 a_position;\n"                                \
"uniform mat4 u_mvpMatrix;\n"                                \
"attribute vec2 a_vertexTexCoord;\n"                    \
"varying vec2 v_texCoord;\n"                        \
"void main()\n"                                                    \
"{\n"                                                            \
"    gl_Position = u_mvpMatrix  * a_position;\n"                    \
"    v_texCoord = a_vertexTexCoord;             \n"        \
"}\n";

static const char * FRAGMENT_SHADER_SRC =
"precision mediump float;\n"
"uniform sampler2D u_texture_1;\n"                            \
"varying vec2 v_texCoord;\n"                                \
"void main()\n"                                    \
"{\n"                                            \
"    gl_FragColor = texture2D(u_texture_1, v_texCoord.xy);\n"    \
"}\n";

Background::Background(int width, int height)
{
    vertices = &VERTEX_BUF[0];
    textureCoords = &TEX_COORD_BUF[0];
    indices = &INDEX_BUF[0];
    
    vertextCount = sizeof(VERTEX_BUF) / sizeof(float);
    texCoordCount = sizeof(TEX_COORD_BUF) / sizeof(float);
    indexCount = sizeof(INDEX_BUF) / sizeof(unsigned char);
    
    screenWidth = width;
    screenHeight = height;
    
    glGenTextures(textureCount, &textureId);
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Background::~Background()
{
}

void Background::setTexture(unsigned char * data, int width, int height, int length)
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void Background::draw(const float * projectionMatrix)
{
    if(program == 0)
    {
        program = ShaderUtil::createProgram(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
        positionHandle = glGetAttribLocation(program, "a_position");
        mvpMatrixHandle = glGetUniformLocation(program, "u_mvpMatrix");
        textureCoordHandle = glGetAttribLocation(program, "a_vertexTexCoord");
        textureHandle = glGetUniformLocation(program, "u_texture_1");
    }
    
    glUseProgram(program);
    ShaderUtil::checkGlError("glUseProgram");
    
    glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(positionHandle);
    
    glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, textureCoords);
    glEnableVertexAttribArray(textureCoordHandle);
    
    glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, projectionMatrix);
    
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(textureHandle, 1);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, indices);
    
    glDisableVertexAttribArray(positionHandle);
    glDisableVertexAttribArray(textureCoordHandle);
    glUseProgram(0);
}

