//
//  ProjectionMatrix.cpp
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 17..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#include "ProjectionMatrix.h"

static ProjectionMatrix instance;

ProjectionMatrix * ProjectionMatrix::getInstance()
{
    return &instance;
}

ProjectionMatrix::ProjectionMatrix()
{
    projectionMatrix = gl_helper::Mat4::Identity();
}

ProjectionMatrix::~ProjectionMatrix()
{
}

gl_helper::Mat4 & ProjectionMatrix::getProjectionMatrix()
{
    if (needToCalcProjectionMatrix)
    {
        recalcProjectionMatrix();
        needToCalcProjectionMatrix = false;
    }
    
    return projectionMatrix;
}

void ProjectionMatrix::setCameraSize(int width, int height)
{
    if (cameraWidth != width || cameraHeight != height)
    {
        cameraWidth = width;
        cameraHeight = height;
        needToCalcProjectionMatrix = true;
    }
}

void ProjectionMatrix::setScreenOrientation(ScreenOrientation orientation)
{
    if (screenOrientation != orientation)
    {
        screenOrientation = orientation;
        needToCalcProjectionMatrix = true;
    }
}

ScreenOrientation ProjectionMatrix::getScreenOrientation()
{
    return screenOrientation;
}

void ProjectionMatrix::setClippingPlane(float nPlane, float fPlane)
{
    if (nearClipPlane != nPlane || farClipPlane != fPlane)
    {
        nearClipPlane = nPlane;
        farClipPlane = fPlane;
        needToCalcProjectionMatrix = true;
    }
}

float ProjectionMatrix::getNearClipPlane()
{
    return nearClipPlane;
}

float ProjectionMatrix::getFarClipPlane()
{
    return farClipPlane;
}

void ProjectionMatrix::setScreenSize(int width, int height)
{
    if (screenWidth != width || screenHeight != height)
    {
        screenWidth = width;
        screenHeight = height;
        needToCalcProjectionMatrix = true;
    }
}

int ProjectionMatrix::getScreenWidth()
{
    return screenWidth;
}

int ProjectionMatrix::getScreenHeight()
{
    return screenHeight;
}

int ProjectionMatrix::getCameraWidth()
{
    return cameraWidth;
}

int ProjectionMatrix::getCameraHeight()
{
    return cameraHeight;
}

bool ProjectionMatrix::flipHorizontal(bool toggle)
{
    needToCalcBackgroundPlaneMatrix = true;
    isFlipHorizontal = toggle;
    return true;
}

bool ProjectionMatrix::flipVertical(bool toggle)
{
    needToCalcBackgroundPlaneMatrix = true;
    isFlipVertical = toggle;
    return true;
}

bool ProjectionMatrix::getIsFlipHorizontal()
{
    return isFlipHorizontal;
}

bool ProjectionMatrix::getIsFlipVertical()
{
    return isFlipVertical;
}

void ProjectionMatrix::recalcProjectionMatrix()
{
    if (needToCalcProjectionMatrix && cameraWidth != 0 && cameraHeight != 0 && screenWidth != 0 && screenHeight != 0)
    {
        calcOrientedProjectionMatrix(cameraWidth, cameraHeight, screenWidth, screenHeight, nearClipPlane, farClipPlane, screenOrientation);
        needToCalcProjectionMatrix = false;
        needToCalcBackgroundPlaneMatrix = true;
    }
}

gl_helper::Mat4 & ProjectionMatrix::getBackgroundPlaneProjectionMatrix()
{
    //if (needToCalcBackgroundPlaneMatrix)
    {
        backgroundPlaneMatrix = gl_helper::Mat4::Identity();
        
        float aspect = (float)screenWidth / (float)screenHeight;
        float widthRatio = (float)screenWidth / (float)cameraWidth;
        float heightRatio = (float)screenHeight / (float)cameraHeight;
        float tfarClipPlane = farClipPlane * 0.90f;
        float tanFovWidth = (1.0f / (float)screenWidth) * (float)screenHeight;
        float frustumWidth = tanFovWidth * tfarClipPlane * aspect;
        float viewWidth = (float)frustumWidth / (float)screenWidth;
        float viewHeight = viewWidth * (widthRatio / heightRatio);
        
        backgroundPlaneMatrix.PostTranslate(0.0f, 0.0f, tfarClipPlane);
        if (widthRatio > heightRatio)
        {
            backgroundPlaneMatrix.PostScale(widthRatio * cameraWidth * viewWidth,
                                            widthRatio * cameraHeight * viewWidth, 0.1f);
        }
        else
        {
            backgroundPlaneMatrix.PostScale(heightRatio * cameraWidth * viewHeight,
                                            heightRatio * cameraHeight * viewHeight, 0.1f);
        }
        
        gl_helper::Mat4 flipMat = gl_helper::Mat4::Identity();
        
        if (isFlipHorizontal)
        {
            flipMat.PostScale(1, -1, 1);
        }
        
        if (isFlipVertical)
        {
            flipMat.PostScale(-1, 1, 1);
        }
        
        backgroundPlaneMatrix = getProjectionMatrix() * backgroundPlaneMatrix * flipMat;
    }
    
    needToCalcBackgroundPlaneMatrix = false;
    
    return backgroundPlaneMatrix;
}

void ProjectionMatrix::calcOrientedProjectionMatrix(int cameraWidth, int cameraHeight, int screenWidth, int screenHeight, float nearClipPlane, float farClipPlane, ScreenOrientation orientation)
{
    float w = 0.0f;
    float h = 0.0f;
    float sr = 0.0f;
    float cr = 0.0f;
    
    if (orientation == ScreenOrientation::PORTRAIT_UP || orientation == ScreenOrientation::PORTRAIT_DOWN)
    {
        w = (float)cameraWidth;
        h = (float)(cameraWidth * screenWidth) / screenHeight;
        sr = (float)screenHeight / screenWidth;
        cr = (float)cameraWidth / cameraHeight;
    }
    else if (orientation == ScreenOrientation::LANDSCAPE_LEFT || orientation == ScreenOrientation::LANDSCAPE_RIGHT)
    {
        w = (float)cameraWidth;
        h = (float)cameraWidth * screenHeight / screenWidth;
        sr = (float)screenWidth / screenHeight;
        cr = (float)cameraWidth / cameraHeight;
    }
    
    float x = 0.0f;
    float y = 0.0f;
    
    if (sr < cr)
    {
        x = h / w * cr;
        y = cr;
    }
    else
    {
        x = 1;
        y = w / h;
    }
    
    projectionMatrix = gl_helper::Mat4::Zero();
    
    switch (orientation)
    {
        case PORTRAIT_UP:
            projectionMatrix.Ptr()[1] = 2.0f * -x;
            projectionMatrix.Ptr()[4] = 2.0f * -y;
            break;
            
        case PORTRAIT_DOWN:
            projectionMatrix.Ptr()[1] = 2.0f * x;
            projectionMatrix.Ptr()[4] = 2.0f * y;
            break;
            
        case LANDSCAPE_LEFT:
            projectionMatrix.Ptr()[0] = 2.0f * x;
            projectionMatrix.Ptr()[5] = 2.0f * -y;
            break;
            
        case LANDSCAPE_RIGHT:
            projectionMatrix.Ptr()[0] = 2.0f * -x;
            projectionMatrix.Ptr()[5] = 2.0f * y;
            break;
            
        case UNKNOWN:
        default:
            projectionMatrix.Ptr()[0] = 2.0f * x;
            projectionMatrix.Ptr()[5] = 2.0f * -y;
            break;
    }
    
    projectionMatrix.Ptr()[10] = (farClipPlane + nearClipPlane) / (farClipPlane - nearClipPlane);
    projectionMatrix.Ptr()[11] = 1.0f;
    projectionMatrix.Ptr()[14] = -(2.0f * farClipPlane * nearClipPlane) / (farClipPlane - nearClipPlane);
}
