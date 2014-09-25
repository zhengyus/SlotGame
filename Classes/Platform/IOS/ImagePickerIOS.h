//
//  ImagePicker.h
//  EBook
//
//  Created by infi on 13-6-27.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef __IMAGE_PICKER_IOS_H__
#define __IMAGE_PICKER_IOS_H__

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "ImagePickerDelegateIOS.h"
#import "ImagePickerDelegate.h"

@class ImagePickerIOS;

static ImagePickerIOS* s_viewController;
static int s_nAction;

@interface ImagePickerIOS : UIViewController<UIActionSheetDelegate, UIImagePickerControllerDelegate,
UINavigationControllerDelegate, UIPopoverControllerDelegate, ImagePickerDelegateIOS>
{
    ImagePickerDelegate * _imagePickerDelegate;
    UIPopoverController * _popOver;
}

@property(nonatomic,assign) ImagePickerDelegate * imagePickerDelegate;

+(ImagePickerIOS*) sharedInstance;
- (void) chooseImageSour;
- (void) pickImageFromAlbum;
- (void) pickImageFromCamera;

-(void) openImagePicker:(int)action;

@end
/*
@interface NonRotatingUIImagePickerController : UIImagePickerController

@end
*/

@interface  UIImagePickerController (UIImagePickerCategory)
- (BOOL)shouldAutorotate;
@end

#endif