//
//  UIControl.mm
//  Texas-iPhone2
//
//  Created by Chaim on 14-7-1.
//
//

#include "UISystemControl.h"
#include "RootViewController.h"

#import "EAGLView.h"
#import "UISystemControlTextView.h"

static std::map<int, UISystemControlTextView*> g_mapObj;
static int g_nIndexObj;

UISystemControl* UISystemControl::s_inst = NULL;

UISystemControl* UISystemControl::sharedInstance()
{
	if(UISystemControl::s_inst)
		return UISystemControl::s_inst;
	
	UISystemControl::s_inst = new UISystemControl();
	
	return UISystemControl::s_inst;
}

int UISystemControl::attachEdit(CCRect r, float fScaleX, float fScaleY)
{
    cocos2d::CCSize size = r.size;
    CCPoint point = r.origin;
    cocos2d::CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	CGRect bounds = [[UIScreen mainScreen] bounds];
	float fRateX = winsize.width / bounds.size.height;
	float fRateY = winsize.height / bounds.size.width;
//	float fX = (point.x-(size.width*fScaleX)/2)/fRateX;
//	float fY = (winsize.height-(point.y+(size.height*fScaleY)/2))/fRateY;
	float fX = (point.x*fScaleX)/fRateX;
	float fY = (winsize.height-(point.y+size.height)*fScaleY)/fRateY;
	float fWidth = (size.width*fScaleX/fRateX);
	float fHeight = (size.height*fScaleY/fRateY);

	UISystemControlTextView* textField = [[UISystemControlTextView alloc] init];
    textField.frame = CGRectMake(fX, fY, fWidth, fHeight);
    //设置边框样式
    /*
     UITextBorderStyleNone, - 无边框＋透明
     UITextBorderStyleLine, － 黑色边框 ＋ 透明
     UITextBorderStyleBezel, － 灰色边框 ＋ 透明
     UITextBorderStyleRoundedRect － 圆角边框 ＋ 白色
     */
//    textField.borderStyle = UITextBorderStyleNone;
	textField.editable = true;
	textField.font = [UIFont fontWithName:@"Arial" size:22.0];
	textField.returnKeyType = UIReturnKeyDefault;
    textField.backgroundColor = [UIColor clearColor];
	textField.textColor = [UIColor whiteColor];
//    textField.placeholder = @"请在此输入广播文字，文字最大限制100字";
    textField.secureTextEntry = NO;
    textField.keyboardType = UIKeyboardTypeEmailAddress;
	
	[textField setAutocorrectionType:UITextAutocorrectionTypeNo];
	[textField setAutocapitalizationType:UITextAutocapitalizationTypeNone];
	
    //设置弹出键盘风格
    textField.keyboardAppearance = UIKeyboardAppearanceDefault;
    textField.hidden = YES;
	
//	[textField addObserver];
	
	//添加到map中
	g_mapObj[++g_nIndexObj] = textField;
	
    [[EAGLView sharedEGLView] addSubview:textField];
//    [textField release];
	
	return g_nIndexObj;
}

void UISystemControl::detachEdit(int nObjID)
{
	std::map<int, UISystemControlTextView*>::iterator it = g_mapObj.find(nObjID);
	if(it != g_mapObj.end())
	{
		[it->second removeFromSuperview];
		[it->second release];
	}
}

bool UISystemControl::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
//	std::map<int, UISystemControlTextView*>::iterator it = g_mapObj.find(g_nIndexObj);
//	if(it != g_mapObj.end())
//	{
//		[it->second resignFirstResponder];
//	}

	return true;
}

void UISystemControl::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void UISystemControl::setVisible(int nObjID, bool bVisible)
{
	std::map<int, UISystemControlTextView*>::iterator it = g_mapObj.find(nObjID);
	if(it != g_mapObj.end())
	{
		if(bVisible)
			it->second.hidden = NO;
		else
			it->second.hidden = YES;
	}
}

void UISystemControl::openIME(int nObjID)
{
	std::map<int, UISystemControlTextView*>::iterator it = g_mapObj.find(g_nIndexObj);
	if(it != g_mapObj.end())
	{
		[it->second becomeFirstResponder];
	}
}

void UISystemControl::closeIME(int nObjID)
{
	std::map<int, UISystemControlTextView*>::iterator it = g_mapObj.find(g_nIndexObj);
	if(it != g_mapObj.end())
	{
		[it->second resignFirstResponder];
	}
}

void UISystemControl::setMaxLimit(int nObjID, int nMaxLimit)
{
	std::map<int, UISystemControlTextView*>::iterator it = g_mapObj.find(g_nIndexObj);
	if(it != g_mapObj.end())
	{
		it->second.nMaxLimit = nMaxLimit;
	}
}

void UISystemControl::clear(int nObjID)
{
	std::map<int, UISystemControlTextView*>::iterator it = g_mapObj.find(g_nIndexObj);
	if(it != g_mapObj.end())
	{
		it->second.text = nil;
	}
}

std::string UISystemControl::getText(int nObjID)
{
	static std::string s_strEmpty;
	
	std::map<int, UISystemControlTextView*>::iterator it = g_mapObj.find(g_nIndexObj);
	if(it != g_mapObj.end())
	{
		const char* cstr = [it->second.text cStringUsingEncoding:NSUTF8StringEncoding];
		return std::string(cstr);
	}
	
	return s_strEmpty;
}

void UISystemControl::setHeight(int nObjID, float fHeight)
{
	std::map<int, UISystemControlTextView*>::iterator it = g_mapObj.find(g_nIndexObj);
	if(it != g_mapObj.end())
	{
		cocos2d::CCSize winsize = CCDirector::sharedDirector()->getWinSize();
		CGRect bounds = [[UIScreen mainScreen] bounds];
//		float fRateX = winsize.width / bounds.size.height;
		float fRateY = winsize.height / bounds.size.width;

		CGRect r = it->second.frame;
		r.size.height = fHeight/fRateY;
		it->second.frame = r;
	}
}



