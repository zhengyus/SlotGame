//
//  SysUtilIOS.h
//  Texas-iPhone2
//
//  Created by Chaim on 14-5-3.
//
//

#ifndef __Texas_iPhone2__SysUtilIOS__
#define __Texas_iPhone2__SysUtilIOS__

//#import <UIKit/UIKit.h>
//#import <Foundation/Foundation.h>
#import "UMSocial.h"
#import "Utility/SysUtil.h"

@class SysUtilIOS;
static SysUtilIOS* s_utilView;

//@interface SysUtilIOS : UIViewController<UMSocialUIDelegate>
@interface SysUtilIOS : NSObject
{
    SysUtilDelegate* _sysUtilDelegate;
}

+(SysUtilIOS*) sharedInstance;
- (void) Shake;
- (void) UMShare:(int)nChannel title:(NSString*)strTitle content:(NSString*)strContent media:(NSString*)strMedia image:(NSString*)strImage;

@property(nonatomic,assign) SysUtilDelegate* sysUtilDelegate;

@end

#endif /* defined(__Texas_iPhone2__SysUtilIOS__) */
