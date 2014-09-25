//
//  ImagePicker.m
//  EBook
//
//  Created by infi on 13-6-27.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#import "ImagePickerIOS.h"
#import <AssetsLibrary/ALAssetsLibrary.h>
/*
@implementation NonRotatingUIImagePickerController
// Disable Landscape mode.
- (BOOL)shouldAutorotate
{
    return NO;
}

- (NSUInteger)supportedInterfaceOrientations{
    return UIInterfaceOrientationMaskPortrait;
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
	return UIInterfaceOrientationPortrait;
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

@end
*/

@implementation ImagePickerIOS

@synthesize imagePickerDelegate = _imagePickerDelegate;


+(ImagePickerIOS*) sharedInstance
{
	if(s_viewController)
		return s_viewController;
	
    s_viewController = [[ImagePickerIOS alloc] init];
    return s_viewController;
}

- (void) pickImageFromAlbum
{
	UIWindow *window = [[UIApplication sharedApplication] keyWindow];
	[window addSubview:self.view];
//	[window.rootViewController addChildViewController:self];
	[self openImagePicker:1];
}

-(void) pickImageFromCamera
{
	UIWindow *window = [[UIApplication sharedApplication] keyWindow];
	[window addSubview:s_viewController.view];
//	[window.rootViewController addChildViewController:self];
	[s_viewController openImagePicker:0];
}

-(id) init
{
    if ((self = [super init]))
    {
        
    }
    return self;
}

- (void) dealloc
{
    [self release];
    [super dealloc];
}

/*弹出选择图片来源的对话框*/
- (void) chooseImageSour
{
    UIActionSheet *chooseImageSheet = [[UIActionSheet alloc] initWithTitle:nil delegate:self cancelButtonTitle:@"Cancel" destructiveButtonTitle:nil otherButtonTitles:@"Camera",@"Photo library", nil];
    [chooseImageSheet showInView:self.view];
}

#pragma mark UIActionSheetDelegate Method
- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    
    switch (buttonIndex)
    {
        case 0://Take picture
            [self openImagePicker:0];
            break;
        case 1://From album
        {
            if ([ALAssetsLibrary authorizationStatus] == ALAuthorizationStatusNotDetermined) {
                
                ALAssetsLibrary *assetsLibrary = [[ALAssetsLibrary alloc] init];
                
                [assetsLibrary enumerateGroupsWithTypes:ALAssetsGroupAll usingBlock:^(ALAssetsGroup *group, BOOL *stop) {
                    
                    if (*stop) {
                        //点击“好”回调方法:
                        NSLog(@"好");
                        return;
                        
                    }
                    *stop = TRUE;
                    
                } failureBlock:^(NSError *error) {
                    
                    //点击“不允许”回调方法:
                    NSLog(@"不允许");
                    [self dismissViewControllerAnimated:YES completion:nil];
                    
                }];
            }
            
            [self openImagePicker:1];
            
            break;
        }
        default:
            break;
    }
    
    [self.view removeFromSuperview];
}

-(void) openImagePicker:(int)action
{
    UIImagePickerController * picker = [[UIImagePickerController alloc] init];
	if(!picker)
		return;
    picker.delegate = self;
    
	s_nAction = action;
    switch (action)
    {
        case 0:
            if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
            {
                picker.sourceType = UIImagePickerControllerSourceTypeCamera;
				picker.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
				picker.allowsEditing = YES;//允许编辑图片
            }
            else
            {
                NSLog(@"模拟器无法打开相机");
            }
            
//			[self.view addSubview:picker.view];//添加到当前窗口
            [self presentViewController:picker animated:YES completion:nil];
            break;
        case 1:
            picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
			picker.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
			picker.allowsEditing = YES;//允许编辑图片
//            if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
//            {
//                _popOver = [[UIPopoverController alloc] initWithContentViewController:picker];
//                [_popOver presentPopoverFromRect:CGRectMake(0, 800, 500, 500) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionUp animated:YES];
//                _popOver.delegate = self;
//            }
//            else
            {
//				[self.view addSubview:picker.view];//添加到当前窗口
                [self presentViewController:picker animated:YES completion:nil];
            }
            break;
    }
}

#pragma 拍照选择照片协议方法
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    NSString *mediaType = [info objectForKey:UIImagePickerControllerMediaType];
    
    NSData *data;
    
    if ([mediaType isEqualToString:@"public.image"]){
        
        //切忌不可直接使用originImage，因为这是没有经过格式化的图片数据，可能会导致选择的图片颠倒或是失真等现象的发生，从UIImagePickerControllerOriginalImage中的Origin可以看出，很原始，哈哈
        UIImage *originImage = [info objectForKey:UIImagePickerControllerEditedImage];
		float fX = 200 / originImage.size.width;
		float fY = 200 / originImage.size.height;
		float fScale = fmin(fX, fY);
        
        //图片压缩，因为原图都是很大的，不必要传原图
        UIImage *scaleImage = [self scaleImage:originImage toScale:fScale];
//        UIImage *scaleImage = [self scaleImage:originImage width:200 height:200];
        
        //以下这两步都是比较耗时的操作，最好开一个HUD提示用户，这样体验会好些，不至于阻塞界面
        if (UIImagePNGRepresentation(scaleImage) == nil) {
            //将图片转换为JPG格式的二进制数据
            data = UIImageJPEGRepresentation(scaleImage, 0.8);
        } else {
            //将图片转换为PNG格式的二进制数据
            data = UIImagePNGRepresentation(scaleImage);
        }
        
        //将二进制数据生成UIImage
    //    UIImage *image = [UIImage imageWithData:data];
        
		if(_imagePickerDelegate)
			 _imagePickerDelegate->onImageData( (char*)[data bytes], [data length]);
    }
	
    if(!_popOver)
	{
//		[picker.view removeFromSuperview];//退出照相机
//		[picker release];
        [picker dismissViewControllerAnimated:NO completion:nil];
		[picker release];
	}
    else
	{
        [_popOver dismissPopoverAnimated:YES];
	}

    [self.view removeFromSuperview];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    if(!_popOver)
	{
//		[picker.view removeFromSuperview];//退出照相机
//		[picker release];
        [self dismissViewControllerAnimated:NO completion:nil];
		[picker release];
	}
    else
	{
        [_popOver dismissPopoverAnimated:YES];
	}
    
    [self.view removeFromSuperview];
}

/*回传过来的图片数据*/
-(void)onImageData:(UIImage *)image
{
   
}

#pragma mark- 缩放图片
-(UIImage *)scaleImage:(UIImage *)image toScale:(float)scaleSize
{
    UIGraphicsBeginImageContext(CGSizeMake(image.size.width*scaleSize,image.size.height*scaleSize));
    [image drawInRect:CGRectMake(0, 0, image.size.width * scaleSize, image.size.height *scaleSize)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaledImage;
}

-(UIImage*)scaleImage:(UIImage*)image width:(int)nWidth height:(int)nHeight
{
    UIGraphicsBeginImageContext(CGSizeMake(nWidth, nHeight));
    [image drawInRect:CGRectMake(0, 0, nWidth, nHeight)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaledImage;
}

-(BOOL)shouldAutorotate
{
	if(s_nAction == 0)
		return NO;
	else
		return YES;
}

-(NSUInteger)supportedInterfaceOrientations
{
	if(s_nAction == 0)
		return UIInterfaceOrientationMaskPortrait;
	else
		return UIInterfaceOrientationMaskAllButUpsideDown;//UIInterfaceOrientationMaskLandscape
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight );
	
} 

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
	if(s_nAction == 0)
		return UIInterfaceOrientationPortrait;
	else
		return UIInterfaceOrientationLandscapeLeft;
}

@end


@implementation UIImagePickerController (UIImagePickerCategory)

-(BOOL)shouldAutorotate
{
	if(s_nAction == 0)
		return NO;
	else
		return YES;
}

-(NSUInteger)supportedInterfaceOrientations
{
	if(s_nAction == 0)
		return UIInterfaceOrientationMaskPortrait;
	else
		return UIInterfaceOrientationMaskAllButUpsideDown;//UIInterfaceOrientationMaskLandscape;//
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight );
	
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
	if(s_nAction == 0)
		return UIInterfaceOrientationPortrait;
	else
		return [[UIApplication sharedApplication] statusBarOrientation];//UIInterfaceOrientationLandscapeLeft;
}

@end

