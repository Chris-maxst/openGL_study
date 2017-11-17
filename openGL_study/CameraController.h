//
//  CameraController.h
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 15..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>
#import <Accelerate/Accelerate.h>
#import <GLKit/GLKit.h>

@interface CameraController : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>

+ (CameraController *) getInstance;
- (int) startCamera;
- (void) stop;
- (unsigned char*) getData;
- (CGSize) getSize;

@end
