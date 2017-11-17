//
//  CameraCube.h
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 17..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#pragma once

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <memory.h>
#include <string.h>

#include "ShaderUtil.h"
#include "vecmath.h"

#define MAX_TEXTURE_COUNT 8

class CameraCube
{
public:
    CameraCube();
    ~CameraCube();
    
    void setProjectionMatrix(gl_helper::Mat4 & projection);
    void setTransform(gl_helper::Mat4 & transform);
    void setPosition(float x, float y, float z);
    void setRotation(float angle, float x, float y, float z);
    void setScale(float x, float y, float z);
    void draw();
    void setTexture(unsigned char * data, int width, int height, int length);
protected:
    void * vertices = nullptr;
    void * textureCoords = nullptr;
    
    int vertextCount = 0;
    int texCoordCount = 0;
    
    GLuint program = 0;
    GLuint positionHandle = 0;
    GLuint textureCoordHandle = 0;
    GLuint mvpMatrixHandle = 0;
    int textureCount = 2;
    GLuint textureId;
    GLuint textureHandle;
    
    gl_helper::Mat4 modelMatrix;
    gl_helper::Mat4 localMVPMatrix;
    gl_helper::Mat4 projectionMatrix;
};


