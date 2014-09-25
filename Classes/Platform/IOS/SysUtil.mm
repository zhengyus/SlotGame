//
//  SysUtil.cpp
//  Texas-iPhone2
//
//  Created by Chaim on 14-5-3.
//
//

#include "SysUtil.h"
#include "MobClick.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "SysUtilIOS.h"
#endif

SysUtil* SysUtil::s_inst = NULL;

SysUtil* SysUtil::sharedInstance()
{
	if(SysUtil::s_inst)
		return SysUtil::s_inst;
	
	SysUtil::s_inst = new SysUtil();
	if(!SysUtil::s_inst->Init())
		return NULL;
	
	return SysUtil::s_inst;
}

bool SysUtil::Init()
{
	return true;
}

//友盟分享
void SysUtil::UMShare(int nChannel, const std::string& strTitle, const std::string& strContent, const std::string& strMedia, const std::string& strImage)
{
#ifdef __Texas_iPhone2__SysUtilIOS__
	[[SysUtilIOS sharedInstance] UMShare:nChannel title:[NSString stringWithUTF8String:strTitle.c_str()] content:[NSString stringWithUTF8String:strContent.c_str()] media:[NSString stringWithUTF8String:strMedia.c_str()] image:[NSString stringWithUTF8String:strImage.c_str()]];
#endif//__Texas_iPhone2__SysUtilIOS__
}

//振动
void SysUtil::Shake()
{
#ifdef __Texas_iPhone2__SysUtilIOS__
	[[SysUtilIOS sharedInstance] Shake];
#endif//__Texas_iPhone2__SysUtilIOS__
}

void SysUtil::JumpUpdateUrl(const std::string& strUrl)
{
	NSString *str = [NSString stringWithUTF8String:strUrl.c_str()];
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
}

void SysUtil::SetDelegate(SysUtilDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
	
	[SysUtilIOS sharedInstance].sysUtilDelegate = pDelegate;
}

//统计接口
bool SysUtil::StatEvent(const char* pszEventID)
{
	[MobClick event:[NSString stringWithUTF8String:pszEventID]];
	
	return true;
}

bool SysUtil::StatEvent(const char* pszEventID, const char* pszEventLabel)
{
	NSDictionary *dict = @{[NSString stringWithUTF8String:pszEventLabel] : @"1"};
	[MobClick event:[NSString stringWithUTF8String:pszEventID] attributes:dict];
	
	return true;
}

bool SysUtil::StatEvent(const char* szEventID, const char* szEventLabel, TCAgentHelper& refAgentHelper)
{
	return false;
}

bool SysUtil::StatEventValue(const char* szEventID, const char* szEventLabel, TCAgentHelper& refAgentHelper, long lVal)
{
	return false;
}

bool SysUtil::GetExternalStorageDirectory(std::string& strDir)
{
	return false;
}

