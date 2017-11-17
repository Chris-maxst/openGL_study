//
//  ProjectionMatrix.h
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 17..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#pragma once

#include "vecmath.h"
#include "Types.h"

class ProjectionMatrix
{
public:
    static ProjectionMatrix * getInstance();
    ProjectionMatrix();
    ~ProjectionMatrix();
    
    void setScreenOrientation(ScreenOrientation orientation);
    void setScreenSize(int width, int height);
    void setCameraSize(int width, int height);
    void setClippingPlane(float nPlane, float fPlane);
    gl_helper::Mat4 & getProjectionMatrix();
    gl_helper::Mat4 & getBackgroundPlaneProjectionMatrix();
    int getScreenWidth();
    int getScreenHeight();
    int getCameraWidth();
    int getCameraHeight();
    float getNearClipPlane();
    float getFarClipPlane();
    ScreenOrientation getScreenOrientation();
    bool flipHorizontal(bool toggle);
    bool flipVertical(bool toggle);
    bool getIsFlipHorizontal();
    bool getIsFlipVertical();
private:
    void recalcProjectionMatrix();
    void calcOrientedProjectionMatrix(int cameraWidth, int cameraHeight, int screenWidth, int screenHeight, float nearClipPlane, float farClipPlane, ScreenOrientation orientation);
    
    gl_helper::Mat4 projectionMatrix;
    gl_helper::Mat4 backgroundPlaneMatrix;
    
    int screenWidth = 0;
    int screenHeight = 0;
    int cameraWidth = 0;
    int cameraHeight = 0;
    float nearClipPlane = 0.03f;
    float farClipPlane = 10000.0f;
    bool needToCalcProjectionMatrix = false;
    bool needToCalcBackgroundPlaneMatrix = false;
    bool isFlipHorizontal = true;
    bool isFlipVertical = false;
    ScreenOrientation screenOrientation = ScreenOrientation::PORTRAIT_UP;
};
