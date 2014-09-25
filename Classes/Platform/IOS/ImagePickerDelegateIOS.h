//
//  ImagePickerDelegate.h
//  EBook
//
//  Created by infi on 13-6-27.
//
//

#ifndef __IMAGE_PICKER_DELEGATE_IOS_H__
#define __IMAGE_PICKER_DELEGATE_IOS_H__

#import <Foundation/Foundation.h>

@protocol ImagePickerDelegateIOS <NSObject>

-(void)onImageData:(UIImage *)image;

@end

#endif
