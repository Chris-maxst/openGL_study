//
//  Square.h
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 10..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#pragma once

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <GLKit/GLKMatrix4.h>

#include <memory.h>
#include <string.h>

class Square
{
public:
    Square();
    ~Square();
    
    void draw();
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
    
    GLKMatrix4 modelMatrix;
    GLKMatrix4 localMVPMatrix;
    GLKMatrix4 projectionMatrix;
};
