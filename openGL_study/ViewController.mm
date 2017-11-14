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

typedef enum tagOBJECT_MODE {
    SQUARE = 0,
    CUBE_ORTHO = 1,
    CUBE_PERSPECTIVE = 2,
} OBJECT_MODE;

@interface ViewController ()
{
    Square *square;
    Cube *cube;
    
    float screenSizeWidth;
    float screenSizeHeight;
    OBJECT_MODE currentMode;
}
@property (strong, nonatomic) IBOutlet UILabel *currentModeLabel;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self setupGLcontext];
    
    currentMode = SQUARE;
    
    square = new Square();
    cube = new Cube();
    
    gl_helper::Mat4 modelMatrix = gl_helper::Mat4::Translation(0, 0, -100);
    cube->setTransform(modelMatrix);
}

- (IBAction)buttonPerspective:(id)sender {
    //perspective로 투영했을때
    gl_helper::Mat4 projection = gl_helper::Mat4::Perspective(5, screenSizeWidth, screenSizeHeight, 1, 200).Transpose();
//    gl_helper::Mat4 projection = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(10.0), screenSizeWidth / screenSizeHeight, 1, 200).m; //ios에서 자체제공하는 matrix 함수
    cube->setProjectionMatrix(projection);
    
    currentMode = CUBE_PERSPECTIVE;
    self.currentModeLabel.text = @"Cube Perspective";
}

- (IBAction)buttonOrtho:(id)sender {
    //ortho로 투영했을때
    float aspect = screenSizeWidth / screenSizeHeight; //ortho로 투영할때 화면비율을 곱해줘서 큐브를 정사각형으로 만든다.
    //    gl_helper::Mat4 projection = GLKMatrix4MakeOrtho(-10 * aspect, 10 * aspect, -10, 10, 1, 200).m; //ios에서 자체제공하는 matrix 함수
    gl_helper::Mat4 projection = gl_helper::Mat4::Ortho(-10 * aspect, 10 * aspect, -10, 10, -1, -200).Transpose();
    cube->setProjectionMatrix(projection);
    
    currentMode = CUBE_ORTHO;
    self.currentModeLabel.text = @"Cube Ortho";
}

- (IBAction)buttonSquare:(id)sender {
    currentMode = SQUARE;
    self.currentModeLabel.text = @"Square";
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(currentMode == SQUARE) {
        square->draw();
    } else if(currentMode == CUBE_ORTHO || currentMode == CUBE_PERSPECTIVE) {
        cube->draw();
        cube->setRotation(1, 1, 1, 1);
    }
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
}

@end
