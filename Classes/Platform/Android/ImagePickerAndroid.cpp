//
//  ImagePickerAndroid.cpp
//  Texas-iPhone2
//
//  Created by Chaim on 14-5-3.
//
//

#include "platform/android/jni/jniHelper.h"
#include "JniPub.h"
#include "ImagePickerAndroid.h"
#include "Utility/ActionEx.h"
#include "Utility/SysUtil.h"

#define CLASS_NAME		"com/ourgame/common/SysUtil"

#define	RESULT_OK				(0)
#define	RESULT_CANCELED			(-1)
#define	RESULT_ERROR			(-2)

using namespace std;
using namespace cocos2d;

extern "C"
{
	void Java_com_ourgame_common_SysUtil_OnPickImageFromCamera(JNIEnv *env, jobject thiz, jint jnResult, jstring jstrFile)
	{
		if(jstrFile)
		{
			const char* pText = env->GetStringUTFChars(jstrFile, NULL);
			CCLog("OnPickImageFromCamera:%d,%s", jnResult, pText);
			ImagePickerAndroid::sharedInstance()->OnPickImageFromCamera(jnResult, pText);
			if(jnResult == RESULT_OK)
			{
				ImagePickerAndroid::sharedInstance()->CropImage(pText);
			}
			env->ReleaseStringUTFChars(jstrFile, pText);
		}
		else
		{
			CCLog("OnPickImageFromCamera:%d,jstrFile is null", jnResult);
			ImagePickerAndroid::sharedInstance()->OnPickImageFromCamera(jnResult, NULL);
		}
	}
	
	void Java_com_ourgame_common_SysUtil_OnPickPhotoFromGallery(JNIEnv *env, jobject thiz, jint jnResult, jstring jstrFile)
	{
		if(jstrFile)
		{
			const char* pText = env->GetStringUTFChars(jstrFile, NULL);
			CCLog("OnPickPhotoFromGallery:%d,%s", jnResult, pText);
			ImagePickerAndroid::sharedInstance()->OnPickPhotoFromGallery(jnResult, pText);
//			if(jnResult == RESULT_OK)
//			{
//				SHOWIMAGE(pText);
//			}
			env->ReleaseStringUTFChars(jstrFile, pText);
		}
		else
		{
			CCLog("OnPickPhotoFromGallery:%d,jstrFile is null", jnResult);
			ImagePickerAndroid::sharedInstance()->OnPickPhotoFromGallery(jnResult, NULL);
		}
	}
	
	void Java_com_ourgame_common_SysUtil_OnCropImage(JNIEnv *env, jobject thiz, jint jnResult, jstring jstrFile)
	{
		if(jstrFile)
		{
			const char* pText = env->GetStringUTFChars(jstrFile, NULL);
			CCLog("OnCropImage:%d,%s", jnResult, pText);
			ImagePickerAndroid::sharedInstance()->OnCropImage(jnResult, pText);
//			if(jnResult == RESULT_OK)
//			{
//				SHOWIMAGE(pText);
//			}
			env->ReleaseStringUTFChars(jstrFile, pText);
		}
		else
		{
			CCLog("OnCropImage:%d,jstrFile is null", jnResult);
			ImagePickerAndroid::sharedInstance()->OnCropImage(jnResult, NULL);
		}
	}

	
	
}


ImagePickerAndroid* ImagePickerAndroid::s_inst = NULL;

ImagePickerAndroid* ImagePickerAndroid::sharedInstance()
{
	if(ImagePickerAndroid::s_inst)
		return ImagePickerAndroid::s_inst;
	
	ImagePickerAndroid::s_inst = new ImagePickerAndroid();
	if(!ImagePickerAndroid::s_inst->Init())
		return NULL;
	
	return ImagePickerAndroid::s_inst;
}

bool ImagePickerAndroid::Init()
{
	return true;
}

bool ImagePickerAndroid::PickPhotoFromGallery(const char* szFile)
{
	CCLog("PickPhotoFromGallery, file:%s", szFile);
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		if(JniHelper::getMethodInfo(t, CLASS_NAME, "pickPhotoFromGallery", "(Ljava/lang/String;)Z"))
		{
			jboolean jbRet;
			jstring jstrFile = t.env->NewStringUTF(szFile);
			jbRet = t.env->CallBooleanMethod(jobj, t.methodID, jstrFile);
			t.env->DeleteLocalRef(jstrFile);
			return jbRet == JNI_TRUE ? true : false;
		}
	}
	return false;
}

bool ImagePickerAndroid::PickPhotoFromGallery()
{
	string strFile = CCFileUtils::sharedFileUtils()->getWritablePath();
	strFile.append("gallery.jpg");
	return PickPhotoFromGallery(strFile.c_str());
}

bool ImagePickerAndroid::CropImage(const char* szSrcFile, const char* szDstFile)
{
	CCLog("CropImage, srcfile:%s,dstfile:%s", szSrcFile, szDstFile);
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		if(JniHelper::getMethodInfo(t, CLASS_NAME, "cropImage", "(Ljava/lang/String;Ljava/lang/String;)Z"))
		{
			jboolean jbRet;
			jstring jstrSrcFile = t.env->NewStringUTF(szSrcFile);
			jstring jstrDstFile = t.env->NewStringUTF(szDstFile);
			jbRet = t.env->CallBooleanMethod(jobj, t.methodID, jstrSrcFile, jstrDstFile);
			t.env->DeleteLocalRef(jstrSrcFile);
			t.env->DeleteLocalRef(jstrDstFile);
			return jbRet == JNI_TRUE ? true : false;
		}
	}
	return false;
}

bool ImagePickerAndroid::CropImage(const char* szSrcFile)
{
	string strFile = CCFileUtils::sharedFileUtils()->getWritablePath();
	strFile.append("camera.jpg");
	
	return CropImage(szSrcFile, strFile.c_str());
}

bool ImagePickerAndroid::PickImageFromCamera(const char* szFile)
{
	CCLog("PickImageFromCamera file:%s", szFile);
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "rtnObject", "()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		
		if(JniHelper::getMethodInfo(t, CLASS_NAME, "pickImageFromCamera", "(Ljava/lang/String;)V"))
		{
			jstring jstrFile = t.env->NewStringUTF(szFile);
			t.env->CallVoidMethod(jobj, t.methodID, jstrFile);
			t.env->DeleteLocalRef(jstrFile);
			return true;
		}
	}
	return false;
}

std::string ImagePickerAndroid::s_strCameraFile = "";
bool ImagePickerAndroid::PickImageFromCamera()
{
	std::string strFile;
	if(!SysUtil::sharedInstance()->GetExternalStorageDirectory(strFile))
		return false;
	
	std::ostringstream osFile;
	osFile << strFile << "vcamera_" << time(NULL) << ".jpg";
	ImagePickerAndroid::s_strCameraFile = osFile.str().c_str();
	return PickImageFromCamera(ImagePickerAndroid::s_strCameraFile.c_str());
}

void ImagePickerAndroid::OnPickImageFromCamera(int nResult, const char* pszFile)
{
	
}

void ImagePickerAndroid::OnPickPhotoFromGallery(int nResult, const char* pszFile)
{
	if(RESULT_OK == nResult)
	{
		std::string strFile = pszFile;
		CCAction* pActions = CCSequence::create(
												CallFunctionImmediately::create([this, strFile]() {
			OnImageFile(strFile.c_str());
		}),
												nullptr);
		CCDirector::sharedDirector()->getRunningScene()->runAction(pActions);
	}
}

void ImagePickerAndroid::OnCropImage(int nResult, const char* pszFile)
{
	if(ImagePickerAndroid::s_strCameraFile.length() > 0)
	{
		CCLog("delete temp camera file:%s", ImagePickerAndroid::s_strCameraFile.c_str());
		remove(ImagePickerAndroid::s_strCameraFile.c_str());
		ImagePickerAndroid::s_strCameraFile.clear();
	}

	if(RESULT_OK == nResult)
	{
		std::string strFile = pszFile;
		CCAction* pActions = CCSequence::create(
												CallFunctionImmediately::create([this, strFile]() {
			OnImageFile(strFile.c_str());
		}),
												nullptr);
		CCDirector::sharedDirector()->getRunningScene()->runAction(pActions);
	}
}

void ImagePickerAndroid::OnImageFile(const char* pszFile)
{
	if(!pszFile)
		return;
	
    FILE* file = fopen(pszFile, "r");
    if (file)
	{
        char* buf = NULL;
        int len;
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        rewind(file);
        CCLOG("count the file content len = %d",len);
        //分配buf空间
        buf = (char*)malloc(sizeof(char) * len + 1);
        if (!buf)
		{
            CCLOG("malloc space is not enough.");
            return;
        }
        
        //读取文件
        //读取进的buf，单位大小，长度，文件指针
        int rLen = fread(buf, sizeof(char), len, file);
        buf[rLen] = '\0';
        CCLOG("has read Length = %d",rLen);

        if (m_pDelegate)
		{
			m_pDelegate->onImageData(buf, len);
		}
		
        fclose(file);
        free(buf);
    }
}






