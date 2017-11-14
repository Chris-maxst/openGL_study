//
//  Cube.h
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 13..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#pragma once

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <GLKit/GLKMatrix4.h>

#include <memory.h>
#include <string.h>

#include "ShaderUtil.h"
#include "vecmath.h"

class Cube
{
public:
    Cube();
    ~Cube();
    
    void draw();
    void setProjectionMatrix(gl_helper::Mat4 & projection);
    void setTransform(gl_helper::Mat4 & transform);
    void setPosition(float x, float y, float z);
    void setRotation(float angle, float x, float y, float z);
    void setScale(float x, float y, float z);
protected:
    void * vertices;
    void * indices;
    void * colors;
    
    int vertextCount = 0;
    int indexCount = 0;
    
    GLuint program = 0;
    GLuint positionHandle = 0;
    GLuint colorHandle = 0;
    GLuint mvpMatrixHandle = 0;
    GLuint projectionMatrixHandle = 0;
    GLuint modelMatrixHandle = 0;
    
    gl_helper::Mat4 modelMatrix;
    gl_helper::Mat4 localMVPMatrix;
    gl_helper::Mat4 projectionMatrix;
};
