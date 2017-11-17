//
//  CameraCube.cpp
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 17..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#include "CameraCube.h"

static float VERTEX_BUF[] =
{    //Vertices according to faces
    // 1. Up face
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    
    // 2. Bottom face
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    
    // 3. Front face
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    
    // 4. Right face
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f,  1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    
    // 5. Back face
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    
    // 6. Left face
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
};

static float TEX_COORD_BUF[] =
{
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
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

CameraCube::CameraCube()
{
    vertices = &VERTEX_BUF[0];
    textureCoords = &TEX_COORD_BUF[0];
    
    vertextCount = sizeof(VERTEX_BUF) / sizeof(float);
    texCoordCount = sizeof(TEX_COORD_BUF) / sizeof(float);
    
    localMVPMatrix = gl_helper::Mat4::Identity();
    modelMatrix = gl_helper::Mat4::Identity();
    projectionMatrix = gl_helper::Mat4::Identity();
    
    glGenTextures(textureCount, &textureId);
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

CameraCube::~CameraCube()
{
}

void CameraCube::setProjectionMatrix(gl_helper::Mat4 & projection)
{
    projectionMatrix = projection;
}

void CameraCube::setTransform(gl_helper::Mat4 & transform)
{
    modelMatrix = transform;
}

void CameraCube::setPosition(float x, float y, float z)
{
    modelMatrix.PostTranslate(x, y, z);
}

void CameraCube::setRotation(float angle, float x, float y, float z)
{
    modelMatrix.PostRotate(angle, x, y, z);
}

void CameraCube::setScale(float x, float y, float z)
{
    modelMatrix.PostScale(x, y, z);
}

void CameraCube::setTexture(unsigned char * data, int width, int height, int length)
{
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void CameraCube::draw()
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
    
    localMVPMatrix = projectionMatrix * modelMatrix;
    glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, localMVPMatrix.Ptr());
    
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(textureHandle, 2);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    
    glDisableVertexAttribArray(positionHandle);
    glDisableVertexAttribArray(textureCoordHandle);
    glUseProgram(0);
}

