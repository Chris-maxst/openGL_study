//
//  Background.h
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 15..
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

class Background
{
public:
    Background(int screenWidth, int screenHeight);
    ~Background();
    
    void draw(const float * projectionMatrix);
    void setTexture(unsigned char * data, int width, int height, int length);
protected:
    void * vertices = nullptr;
    void * indices = nullptr;
    void * textureCoords = nullptr;
    
    int vertextCount = 0;
    int indexCount = 0;
    int texCoordCount = 0;
    
    int screenWidth = 0;
    int screenHeight = 0;
    
    GLint defaultFBO = 0;
    GLuint offScreenFrameBuffer = 0;
    
    GLuint program = 0;
    GLuint positionHandle = 0;
    GLuint textureCoordHandle = 0;
    GLuint mvpMatrixHandle = 0;
    int textureCount = 1;
    GLuint textureHandle;
    GLuint textureId;
};

