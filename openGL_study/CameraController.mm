//
//  CameraController.m
//  openGL_study
//
//  Created by kwanghee on 2017. 11. 15..
//  Copyright © 2017년 kwanghee. All rights reserved.
//

#import "CameraController.h"

static CameraController *instance = nil;

using namespace std;

typedef enum
{
    Success = 0,
    
    CameraPermissionIsNotResolved = 100,
    CameraDevicedRestriced = 101,
    CameraPermissionIsNotGranted = 102,
    CameraAlreadyOpened = 103,
    
    TrackerAlreadyStarted = 200,
    
    UnknownError = 1000,
} ResultCode;

@interface CameraController()
{
   AVCaptureSession * captureSession;
   
   unsigned char* rgbData;
   int bufferWidth;
   int bufferHeight;
}

@end

@implementation CameraController

+ (CameraController *)getInstance
{
    static dispatch_once_t dis_one;
    dispatch_once(&dis_one, ^{
        instance = [[CameraController alloc] init];
    });
    
    return instance;
}

- (int)startCamera
{
    AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    
    switch(status)
    {
        case AVAuthorizationStatusNotDetermined:
            [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
                if(granted){
                    NSLog(@"granted");
                } else {
                    NSLog(@"not granted");
                }
            }];
            return CameraPermissionIsNotResolved;
        case AVAuthorizationStatusDenied:
            return CameraPermissionIsNotGranted;
        case AVAuthorizationStatusRestricted:
            return CameraDevicedRestriced;
        case AVAuthorizationStatusAuthorized:
            break;
    }
    
    captureSession = [[AVCaptureSession alloc]init];
    
    [captureSession beginConfiguration];
    
    BOOL result;
    result = [self addVideoInput];
    if (!result)
    {
        NSLog(@"%@", @"AVCaptureDeviceInput Error");
        return UnknownError;
    }
    result = [self addVideoDataOutput];
    if (!result)
    {
        NSLog(@"%@", @"AVCaptureVideoDataOutput Error");
        return UnknownError;
    }

    NSString* tsSessionPreset = AVCaptureSessionPreset640x480;

    [captureSession setSessionPreset:tsSessionPreset];
    
    [captureSession commitConfiguration];
    
    [captureSession startRunning];
    
    return Success;
}

- (void)stop
{
    if(captureSession != nullptr)
    {
        [captureSession stopRunning];
    }
   
   free(rgbData);
   rgbData = NULL;
}

- (unsigned char *)getData
{
   return rgbData;
}

- (CGSize)getSize
{
   CGSize size = CGSizeMake(bufferWidth, bufferHeight);
   
   return size;
}

- (void)captureOutput:(AVCaptureOutput *)output didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    
    CVPixelBufferLockBaseAddress( pixelBuffer, 0 );
    
    bufferHeight = (int)CVPixelBufferGetHeight(pixelBuffer);
    bufferWidth = (int)CVPixelBufferGetWidth(pixelBuffer);
   
   int pixelSize = (int)CVPixelBufferGetBytesPerRow(pixelBuffer);
   
   if (rgbData == NULL)
      rgbData = (unsigned char*)malloc(bufferWidth * bufferHeight * 3);

   unsigned char* linebase = (unsigned char *)CVPixelBufferGetBaseAddress( pixelBuffer );
   
   vImage_Buffer src;
   src.width = bufferWidth;
   src.height = bufferHeight;
   src.data = linebase;
   src.rowBytes = pixelSize;
   
   vImage_Buffer destImage;
   destImage.width = bufferWidth;
   destImage.height = bufferHeight;
   destImage.rowBytes = bufferWidth * 3;
   destImage.data = rgbData;
   
   vImage_Error err;
   
   err = vImageConvert_BGRA8888toRGB888(&src, &destImage, kvImageNoFlags);
   if(err != kvImageNoError){
      NSLog(@"Error in Pixel Copy vImage_error %ld", err);
   }
    
    CVPixelBufferUnlockBaseAddress( pixelBuffer, 0 );
}

- (bool)addVideoInput
{
    AVCaptureDevice* videoDevice = nil;
    
    NSArray* tNSArrayCamera = [AVCaptureDevice devices];
    for(AVCaptureDevice* tAVCaptureDevice in tNSArrayCamera)
    {
        if([tAVCaptureDevice hasMediaType:AVMediaTypeVideo])
        {
            videoDevice = tAVCaptureDevice;
            break;
        }
    }
    
    if(videoDevice == nil)
    {
        return NO;
    }
    
    NSError* tError;
    BOOL tbResult;
    tbResult = [videoDevice lockForConfiguration:&tError];
    if (!tbResult) {
        return NO;
    }
    
    for(AVCaptureDeviceFormat *vFormat in [videoDevice formats] )
    {
        CMFormatDescriptionRef description= vFormat.formatDescription;
        float maxrate = ((AVFrameRateRange*) [vFormat.videoSupportedFrameRateRanges objectAtIndex:0]).maxFrameRate;
        
        if(maxrate > 59 && CMFormatDescriptionGetMediaSubType(description) == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange)
        {
            videoDevice.activeFormat = vFormat;
            [videoDevice setActiveVideoMinFrameDuration:CMTimeMake(10,600)];
            [videoDevice setActiveVideoMaxFrameDuration:CMTimeMake(10,600)];
            NSLog(@"formats  %@ %@ %@", vFormat.mediaType, vFormat.formatDescription, vFormat.videoSupportedFrameRateRanges);
        }
    }
    
    if ([videoDevice isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]) {
        NSLog(@"AVCaptureFocusModeContinuousAutoFocus support");
        [videoDevice setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
    } else if ([videoDevice isFocusModeSupported:AVCaptureFocusModeAutoFocus]) {
        NSLog(@"AVCaptureFocusModeAutoFocus support");
        [videoDevice setFocusMode:AVCaptureFocusModeAutoFocus];
    } else {
        NSLog(@"AVCaptureFocusModeContinuousAutoFocus And AVCaptureFocusModeAutoFocus not support");
    }
    
    [videoDevice unlockForConfiguration];
    
    AVCaptureDeviceInput* tCaptureDeviceInput = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error:&tError];
    if (tError) {
        return NO;
    }
    tbResult = [captureSession canAddInput:tCaptureDeviceInput];
    if (!tbResult) {
        return NO;
    }
    [captureSession addInput:tCaptureDeviceInput];
    
    return YES;
}

- (bool)addVideoDataOutput
{
    AVCaptureVideoDataOutput* avCaptureVideoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
    dispatch_queue_t tDispatchQueue = dispatch_queue_create("CameraImgQueue", DISPATCH_QUEUE_SERIAL);
    
    [avCaptureVideoDataOutput setAlwaysDiscardsLateVideoFrames:YES];
    NSDictionary* tDVideoSetting = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey];
    [avCaptureVideoDataOutput setVideoSettings:tDVideoSetting];
    [avCaptureVideoDataOutput setSampleBufferDelegate:self queue:tDispatchQueue];
    
    BOOL tbResult;
    tbResult = [captureSession canAddOutput:avCaptureVideoDataOutput];
    if (!tbResult) {
        return NO;
    }
    [captureSession addOutput:avCaptureVideoDataOutput];
    
    return YES;
}

@end
