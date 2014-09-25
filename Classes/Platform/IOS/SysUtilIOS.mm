//
//  SysUtilIOS.mm
//  Texas-iPhone2
//
//  Created by Chaim on 14-5-3.
//
//

#import "SysUtilIOS.h"
#import "EAGLView.h"
#import "UMSocialScreenShoter.h"
#import <AudioToolbox/AudioToolbox.h>

@implementation SysUtilIOS

@synthesize sysUtilDelegate = _sysUtilDelegate;

+(SysUtilIOS*) sharedInstance
{
	if(s_utilView)
		return s_utilView;
	
    s_utilView = [[SysUtilIOS alloc] init];
    return s_utilView;
}

-(id) init
{
    if ((self = [super init]))
    {
        
    }

	[UMSocialData openLog:YES];
	[UMSocialConfig setSupportedInterfaceOrientations:UIInterfaceOrientationMaskLandscape];

    return self;
}

- (void) dealloc
{
    [self release];
    [super dealloc];
}


- (void) Shake
{
	AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

- (void) UMShare:(int)nChannel title:(NSString*)strTitle content:(NSString*)strContent media:(NSString*)strMedia image:(NSString*)strImage
{
//	UIWindow *window = [[UIApplication sharedApplication] keyWindow];
//	[window addSubview:self.view];
//	[[EAGLView sharedEGLView] addSubview:self.view];
	
//	[UMSocialSnsService presentSnsIconSheetView:self
//										 appKey:@"5364a46056240bd42b07fbfd"
//									  shareText:@"你要分享的文字"
//									 shareImage:[UIImage imageNamed:@"icon.png"]
//								shareToSnsNames:[NSArray arrayWithObjects:UMShareToSina,UMShareToTencent,UMShareToRenren,nil]
//									   delegate:self];

//	[[UMSocialUIService defaultDataService]
	 
//	[UMSocialSnsService presentSnsIconSheetView:self
//										 appKey:@"5364a46056240bd42b07fbfd"
//									  shareText:@"友盟社会化分享让您快速实现分享等社会化功能，www.umeng.com/social"
//									 shareImage:[UIImage imageNamed:@"icon.png"]
//								shareToSnsNames:@[UMShareToWechatSession,UMShareToWechatTimeline,UMShareToWechatFavorite]
//									   delegate:self];
	
//	[[UMSocialDataService defaultDataService]  postSNSWithTypes:@[UMShareToSina] content:@"分享内嵌文字" image:nil location:nil urlResource:nil presentedController:self completion:^(UMSocialResponseEntity *response){
//		if (response.responseCode == UMSResponseCodeSuccess) {
//			NSLog(@"分享成功！");
//		}
//		[self.view removeFromSuperview];
//	}];

	//在分享代码前设置微信分享应用类型，用户点击消息将跳转到应用，或者到下载页面
	//UMSocialWXMessageTypeImage 为纯图片类型
//    [UMSocialData defaultData].extConfig.wxMessageType = UMSocialWXMessageTypeApp;
    [UMSocialData defaultData].extConfig.wxMessageType = UMSocialWXMessageTypeWeb;
	//分享图文样式到微信朋友圈显示字数比较少，只显示分享标题
    [UMSocialData defaultData].extConfig.title = strTitle;
	//设置微信好友或者朋友圈的分享url,下面是微信好友，微信朋友圈对应wechatTimelineData
    [UMSocialData defaultData].extConfig.wechatSessionData.url = strMedia;
	//设置微信好友或者朋友圈的分享url,下面是微信好友，微信朋友圈对应wechatTimelineData
    [UMSocialData defaultData].extConfig.wechatTimelineData.url = strMedia;
	
	//使用UMShareToWechatSession,UMShareToWechatTimeline,UMShareToWechatFavorite分别代表微信好友、微信朋友圈、微信收藏
	NSArray* platformType = @[UMShareToWechatSession];
	if(nChannel == 1)
		platformType = @[UMShareToWechatSession];
	else if(nChannel == 2)
		platformType = @[UMShareToWechatTimeline];
	else if(nChannel == 3)
		platformType = @[UMShareToSina];
	UIImage* img = nil;
	if(strImage.length > 3)
	{
		img = [UIImage imageNamed:strImage];
	}
	else
	{
		img = [[UMSocialScreenShoterCocos2d screenShoterFromEaglView:[EAGLView sharedEGLView]] getScreenShot];
	}
	[[UMSocialDataService defaultDataService]  postSNSWithTypes:platformType content:strContent image:img location:nil urlResource:nil presentedController:[UIApplication sharedApplication].keyWindow.rootViewController completion:^(UMSocialResponseEntity *response){
		if(_sysUtilDelegate)
			_sysUtilDelegate->OnUMShare(response.responseCode == UMSResponseCodeSuccess ? 0 : -1, response.responseCode);
		if (response.responseCode == UMSResponseCodeSuccess) {
			NSLog(@"分享成功！");
		}
	}];

}

-(void)didFinishGetUMSocialDataInViewController:(UMSocialResponseEntity *)response
{
    //根据`responseCode`得到发送结果,如果分享成功
    if(response.responseCode == UMSResponseCodeSuccess)
    {
        //得到分享到的微博平台名
        NSLog(@"share to sns name is %@",[[response.data allKeys] objectAtIndex:0]);
    }

//    [self.view removeFromSuperview];
}

-(void)didCloseUIViewController:(UMSViewControllerType)fromViewControllerType
{
//    [self.view removeFromSuperview];
}

@end


