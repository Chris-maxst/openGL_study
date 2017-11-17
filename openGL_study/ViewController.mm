//
//  ViewController.m
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 10..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#import "ViewController.h"
#import "Square.h"
#import "Cube.h"
#import "TextureCube.h"
#import "UIImage+Converter.h"
#import "Background.h"
#import "CameraController.h"
#import "Types.h"
#import "ProjectionMatrix.h"
#import "CameraCube.h"

typedef enum tagOBJECT_MODE {
    SQUARE = 0,
    CUBE_ORTHO = 1,
    CUBE_PERSPECTIVE = 2,
    TEXTURE_CUBE_BRICK = 3,
    CAMERA = 4,
    CAMERA_CUBE = 5,
} OBJECT_MODE;

@interface ViewController ()
{
    Square *square;
    Cube *cube;
    TextureCube *textureCube;
    Background *background;
    CameraCube *cameraCube;
    
    float screenSizeWidth;
    float screenSizeHeight;
    OBJECT_MODE currentMode;
    
    CameraController *cameraController;
    ProjectionMatrix *projectionMatrix;
}
@property (strong, nonatomic) IBOutlet UILabel *currentModeLabel;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    projectionMatrix = ProjectionMatrix::getInstance();
    [self setupGLcontext];
    
    currentMode = SQUARE;
    
    square = new Square();
    cube = new Cube();
    textureCube = new TextureCube();
    background = new Background(screenSizeWidth, screenSizeHeight);
    cameraCube = new CameraCube();
    cameraController = [CameraController getInstance];
    
    gl_helper::Mat4 modelMatrix = gl_helper::Mat4::Translation(0, 0, -20);
    cube->setTransform(modelMatrix);
    
    float aspect = screenSizeWidth / screenSizeHeight;
    gl_helper::Mat4 projection = gl_helper::Mat4::Ortho(-10 * aspect, 10 * aspect, -10, 10, -1, -200).Transpose();
    textureCube->setProjectionMatrix(projection);
    textureCube->setTransform(modelMatrix);
    
    NSString *brickPath = [[NSBundle mainBundle] pathForResource:@"brick" ofType:@"png"];
    UIImage *brickImage = [UIImage imageWithContentsOfFile:brickPath];
    unsigned char *imageData = [UIImage UIImageToByteArray:brickImage];
    textureCube->setTexture(imageData, brickImage.size.width, brickImage.size.height, brickImage.size.width * brickImage.size.height * 4);
    delete imageData;
    
    cameraCube->setProjectionMatrix(projection);
    cameraCube->setTransform(modelMatrix);
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    
    delete square;
    delete cube;
    delete textureCube;
    delete background;
    delete cameraCube;
}

- (IBAction)buttonPerspective:(id)sender {
    //perspective로 투영했을때
    gl_helper::Mat4 projection = gl_helper::Mat4::Perspective(20, screenSizeWidth, screenSizeHeight, 1, 200).Transpose();
    cube->setProjectionMatrix(projection);
    
    currentMode = CUBE_PERSPECTIVE;
    self.currentModeLabel.text = @"Cube Perspective";
    
    [cameraController stop];
}

- (IBAction)buttonOrtho:(id)sender {
    //ortho로 투영했을때
    float aspect = screenSizeWidth / screenSizeHeight; //ortho로 투영할때 화면비율을 곱해줘서 큐브를 정사각형으로 만든다.
    gl_helper::Mat4 projection = gl_helper::Mat4::Ortho(-10 * aspect, 10 * aspect, -10, 10, -1, -200).Transpose();
    cube->setProjectionMatrix(projection);
    
    currentMode = CUBE_ORTHO;
    self.currentModeLabel.text = @"Cube Orthographic";
    
    [cameraController stop];
}

- (IBAction)buttonSquare:(id)sender {
    currentMode = SQUARE;
    self.currentModeLabel.text = @"Square";
    
    [cameraController stop];
}

- (IBAction)buttonBrick:(id)sender {
    currentMode = TEXTURE_CUBE_BRICK;
    self.currentModeLabel.text = @"Brick Cube";
    
    [cameraController stop];
}

- (IBAction)buttonCamera:(id)sender {
    currentMode = CAMERA;
    self.currentModeLabel.text = @"Camera";
    
    [cameraController startCamera];
}

- (IBAction)buttonCameraCube:(id)sender {
    currentMode = CAMERA_CUBE;
    self.currentModeLabel.text = @"Camera Cube";
    
    [cameraController startCamera];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    switch (currentMode) {
        case SQUARE:
            square->draw();
            break;
        case CUBE_ORTHO:
        case CUBE_PERSPECTIVE:
            cube->draw();
            cube->setRotation(1, 1, 1, 1);
            break;
        case TEXTURE_CUBE_BRICK:
            textureCube->draw();
            textureCube->setRotation(1, 1, 1, 1);
            break;
        case CAMERA: {
            unsigned char *imageData = [cameraController getData];
            CGSize size = [cameraController getSize];
            background->setTexture(imageData, size.width, size.height, size.width * size.height * 3);
            background->draw(projectionMatrix->getBackgroundPlaneProjectionMatrix().Ptr());
            break;
        }
        case CAMERA_CUBE: {
            unsigned char *imageData = [cameraController getData];
            CGSize size = [cameraController getSize];
            cameraCube->setTexture(imageData, size.width, size.height, size.width * size.height * 3);
            cameraCube->draw();
            cameraCube->setRotation(1, 1, 1, 1);
            break;
        }
    }
    glDisable(GL_DEPTH_TEST);
}

-(void) setupGLcontext
{
    GLKView *glkview = (GLKView *)self.view;
    glkview.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    glkview.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    glkview.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [EAGLContext setCurrentContext:glkview.context];
    
    glClearColor(0.0, 0, 0, 1.0);
    screenSizeWidth = [[UIScreen mainScreen] nativeBounds].size.width;
    screenSizeHeight = [[UIScreen mainScreen] nativeBounds].size.height;
    
    projectionMatrix->setScreenSize(screenSizeWidth, screenSizeHeight);
    projectionMatrix->setCameraSize(640, 480);
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    [self setOrientaionChange];
}

- (void)setOrientaionChange
{
    if(UIDevice.currentDevice.orientation == UIDeviceOrientationPortrait)
    {
        screenSizeWidth = [[UIScreen mainScreen] nativeBounds].size.width;
        screenSizeHeight = [[UIScreen mainScreen] nativeBounds].size.height;
        projectionMatrix->setScreenOrientation(ScreenOrientation::PORTRAIT_UP);
    }
    else if(UIDevice.currentDevice.orientation == UIDeviceOrientationPortraitUpsideDown)
    {
        screenSizeWidth = [[UIScreen mainScreen] nativeBounds].size.width;
        screenSizeHeight = [[UIScreen mainScreen] nativeBounds].size.height;
        projectionMatrix->setScreenOrientation(ScreenOrientation::PORTRAIT_DOWN);
    }
    else if(UIDevice.currentDevice.orientation == UIDeviceOrientationLandscapeLeft)
    {
        screenSizeWidth = [[UIScreen mainScreen] nativeBounds].size.height;
        screenSizeHeight = [[UIScreen mainScreen] nativeBounds].size.width;
        projectionMatrix->setScreenOrientation(ScreenOrientation::LANDSCAPE_LEFT);
    }
    else if(UIDevice.currentDevice.orientation == UIDeviceOrientationLandscapeRight)
    {
        screenSizeWidth = [[UIScreen mainScreen] nativeBounds].size.height;
        screenSizeHeight = [[UIScreen mainScreen] nativeBounds].size.width;
        projectionMatrix->setScreenOrientation(ScreenOrientation::LANDSCAPE_RIGHT);
    }
    projectionMatrix->setScreenSize(screenSizeWidth, screenSizeHeight);
}

@end
