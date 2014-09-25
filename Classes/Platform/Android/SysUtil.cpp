//
//  SysUtil.cpp
//  Texas-iPhone2
//
//  Created by Chaim on 14-5-3.
//
//

#include "SysUtil.h"
#include "JniPub.h"
#include <jni.h>
#include "platform/android/jni/jniHelper.h"
#include "AppGlobal.h"

#define CLASS_NAME	"com/ourgame/common/SysUtil"

extern "C"
{
	void Java_com_ourgame_common_SysUtil_OnUMShare(JNIEnv *env, jobject thiz, jint jnResult, jint jnUMCode)
	{
		int nResult = (int)jnResult;
		int nUMCode = (int)jnUMCode;
		CCLog("Java_com_ourgame_common_SysUtil_OnUMShare:%d,%d", nResult, nUMCode);
		SysUtilDelegate* pDelegate = SysUtil::sharedInstance()->GetDelegate();
		if(pDelegate)
		{
			pDelegate->OnUMShare(nResult, nUMCode);
		}
	}
}

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
void SysUtil::UMShare(int nPlatformID, const std::string& strTitle, const std::string& strContent, const std::string& strMedia, const std::string& strIcon)
{
	CCLog("UMeng_Share");
	
	std::string strImage = strIcon;
	if(strIcon.empty() || strIcon.length() == 0)
	{//如果没有指定分享图片则截图
		std::string strFile = CCFileUtils::sharedFileUtils()->getWritablePath() + "texas_screenshoot.jpg";
		if(AppGlobal::sharedInstance()->ScreenShoot(strFile))
			strImage = strFile;
	}
	
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		if(JniHelper::getMethodInfo(t, CLASS_NAME, "UMeng_Share", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
		{
			jint jnPlatformID = nPlatformID;
			jstring jstrTitle = t.env->NewStringUTF(strTitle.c_str());
			jstring jstrContent = t.env->NewStringUTF(strContent.c_str());
			jstring jstrMedia = t.env->NewStringUTF(strMedia.c_str());
			jstring jstrImage = t.env->NewStringUTF(strImage.c_str());
			t.env->CallVoidMethod(jobj, t.methodID, jnPlatformID, jstrTitle, jstrContent, jstrMedia, jstrImage);
			t.env->DeleteLocalRef(jstrMedia);
			t.env->DeleteLocalRef(jstrContent);
			t.env->DeleteLocalRef(jstrTitle);
			t.env->DeleteLocalRef(jstrImage);
			return;
		}
	}
	return;
}

//振动
void SysUtil::Shake()
{
	JNI_CallIntMethodReturnVoid(CLASS_NAME, "Vibrate", 1000);
}

void SysUtil::JumpUpdateUrl(const std::string& strUrl)
{
	JNI_CallStringMethodReturnVoid(CLASS_NAME, "lanuchBrowser", strUrl.c_str());
}

//统计接口
bool SysUtil::StatEvent(const char* szEventID)
{
	CCLog("TCAgentEvent:%s", szEventID);
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		if(JniHelper::getMethodInfo(t, CLASS_NAME, "TCAgentEvent", "(Ljava/lang/String;)V"))
		{
			jstring jstrEventID = t.env->NewStringUTF(szEventID);
			t.env->CallVoidMethod(jobj, t.methodID, jstrEventID);
			t.env->DeleteLocalRef(jstrEventID);
			return true;
		}
	}
	return false;
}

bool SysUtil::StatEvent(const char* szEventID, const char* szEventLabel)
{
	CCLog("TCAgentEvent:%s,%s", szEventID, szEventLabel);
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		if(JniHelper::getMethodInfo(t, CLASS_NAME, "TCAgentEvent", "(Ljava/lang/String;Ljava/lang/String;)V"))
		{
			jstring jstrEventID = t.env->NewStringUTF(szEventID);
			jstring jstrEventLabel = t.env->NewStringUTF(szEventLabel);
			t.env->CallVoidMethod(jobj, t.methodID, jstrEventID, jstrEventLabel);
			t.env->DeleteLocalRef(jstrEventID);
			t.env->DeleteLocalRef(jstrEventLabel);
			return true;
		}
	}
	return false;
}

bool SysUtil::StatEvent(const char* szEventID, const char* szEventLabel, TCAgentHelper& refAgentHelper)
{
	CCLog("TCAgentEvent:%s,%s", szEventID, szEventLabel);
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		if(JniHelper::getMethodInfo(t, CLASS_NAME, "TCAgentEvent", "(Ljava/lang/String;Ljava/lang/String;[Lcom/ourgame/common/JniTCAgentVal;)V"))
		{
			jclass objClass = t.env->FindClass("com/ourgame/common/JniTCAgentVal");
			jmethodID jmcons = t.env->GetMethodID(objClass, "<init>", "()V");
			
			std::vector<TCAgentHelper::TCAgentVal>& vtVal = refAgentHelper.m_vtVal;
			int nNum = vtVal.size();
			CCLog("Num:%d", nNum);
			jobjectArray jayVal;
			jayVal = t.env->NewObjectArray(nNum, objClass, 0);
			int i = 0;
			vector<TCAgentHelper::TCAgentVal>::iterator it;
			for(it = vtVal.begin(); it != vtVal.end(); it++, i++)
			{
				TCAgentHelper::TCAgentVal& v = *it;
				jobject jobjVal = t.env->NewObject(objClass, jmcons);
				
				JNI_SetStringField(t.env, objClass, jobjVal, "strKey", v.szKey);
				JNI_SetIntField(t.env, objClass, jobjVal, "nType", (jint)v.nType);
				switch(v.nType)
				{
					case TCAgentHelper::TCAVT_STRING:
						JNI_SetStringField(t.env, objClass, jobjVal, "strVal", v.szVal);
						break;
					case TCAgentHelper::TCAVT_INT:
						JNI_SetIntField(t.env, objClass, jobjVal, "nVal", (jint)v.nVal);
						break;
					case TCAgentHelper::TCAVT_FLOAT:
						JNI_SetFloatField(t.env, objClass, jobjVal, "fVal", (jfloat)v.fVal);
						break;
				}
				
				t.env->SetObjectArrayElement(jayVal, i, jobjVal);
			}
			jstring jstrEventID = t.env->NewStringUTF(szEventID);
			jstring jstrEventLabel = t.env->NewStringUTF(szEventLabel);
			t.env->CallVoidMethod(jobj, t.methodID, jstrEventID, jstrEventLabel, jayVal);
			t.env->DeleteLocalRef(jstrEventID);
			t.env->DeleteLocalRef(jstrEventLabel);
			t.env->DeleteLocalRef(jayVal);
			
			return true;
		}
	}

	return false;
}

bool SysUtil::StatEventValue(const char* szEventID, const char* szEventLabel, TCAgentHelper& refAgentHelper, long lVal)
{
	CCLog("TCAgentEvent:%s,%s,%d", szEventID, szEventLabel, lVal);
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		if(JniHelper::getMethodInfo(t, CLASS_NAME, "TCAgentEvent", "(Ljava/lang/String;Ljava/lang/String;[Lcom/ourgame/common/JniTCAgentVal;J)V"))
		{
			jclass objClass = t.env->FindClass("com/ourgame/common/JniTCAgentVal");
			jmethodID jmcons = t.env->GetMethodID(objClass, "<init>", "()V");
			
			std::vector<TCAgentHelper::TCAgentVal>& vtVal = refAgentHelper.m_vtVal;
			int nNum = vtVal.size();
			CCLog("Num:%d", nNum);
			jobjectArray jayVal;
			jayVal = t.env->NewObjectArray(nNum, objClass, 0);
			int i = 0;
			vector<TCAgentHelper::TCAgentVal>::iterator it;
			for(it = vtVal.begin(); it != vtVal.end(); it++, i++)
			{
				TCAgentHelper::TCAgentVal& v = *it;
				jobject jobjVal = t.env->NewObject(objClass, jmcons);
				
				JNI_SetStringField(t.env, objClass, jobjVal, "strKey", v.szKey);
				JNI_SetIntField(t.env, objClass, jobjVal, "nType", (jint)v.nType);
				switch(v.nType)
				{
					case TCAgentHelper::TCAVT_STRING:
						JNI_SetStringField(t.env, objClass, jobjVal, "strVal", v.szVal);
						break;
					case TCAgentHelper::TCAVT_INT:
						JNI_SetIntField(t.env, objClass, jobjVal, "nVal", (jint)v.nVal);
						break;
					case TCAgentHelper::TCAVT_FLOAT:
						JNI_SetFloatField(t.env, objClass, jobjVal, "fVal", (jfloat)v.fVal);
						break;
				}
				
				t.env->SetObjectArrayElement(jayVal, i, jobjVal);
			}
			jstring jstrEventID = t.env->NewStringUTF(szEventID);
			jstring jstrEventLabel = t.env->NewStringUTF(szEventLabel);
			t.env->CallVoidMethod(jobj, t.methodID, jstrEventID, jstrEventLabel, jayVal, (jlong)lVal);
			t.env->DeleteLocalRef(jstrEventID);
			t.env->DeleteLocalRef(jstrEventLabel);
			t.env->DeleteLocalRef(jayVal);
			
			return true;
		}
	}
	
	return false;
	
}

void SysUtil::SetDelegate(SysUtilDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

bool SysUtil::GetExternalStorageDirectory(std::string& strDir)
{
	return JNI_CallVoidMethodReturnString(CLASS_NAME, "getExternalStorageDirectory", strDir);
}

