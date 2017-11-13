//
//  ViewController.m
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 10..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#import "ViewController.h"
#import "Square.h"

@interface ViewController ()
{
    Square *square;
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self setupGLcontext];
    
    square = new Square();
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    square->draw();
}

-(void) setupGLcontext
{
    GLKView *glkview = (GLKView *)self.view;
    glkview.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    glkview.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    glkview.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [EAGLContext setCurrentContext:glkview.context];
    
    glClearColor(0.0, 0, 0, 1.0);
}

@end
