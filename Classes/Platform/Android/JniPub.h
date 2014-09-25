#pragma once

#include "cocos2d.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define OS_ANDROID
#endif//

#ifdef OS_ANDROID
#include <jni.h>
#endif//OS_ANDROID

#include <string>

using namespace std;

#ifdef OS_ANDROID

				void JNI_GetStringField(JNIEnv* env, jclass clazz, jobject obj, const char* name, string& strVal);
				int JNI_GetIntField(JNIEnv* env, jclass clazz, jobject obj, const char* name);
				long JNI_GetLongField(JNIEnv* env, jclass clazz, jobject obj, const char* name);
				void JNI_SetStringField(JNIEnv* env, jclass clazz, jobject obj, const char* name, const char* szVal);
				void JNI_SetIntField(JNIEnv* env, jclass clazz, jobject obj, const char* name, jint jnVal);
				void JNI_SetLongField(JNIEnv* env, jclass clazz, jobject obj, const char* name, jlong jlVal);
				void JNI_SetFloatField(JNIEnv* env, jclass clazz, jobject obj, const char* name, jfloat jlVal);

#endif//OS_ANDROID

				bool JNI_CallVoidMethodReturnString(const char* pszClassName, const char* pszMethod, string& strReturn);
				bool JNI_CallVoidMethodReturnVoid(const char* pszClassName, const char* pszMethod);
				bool JNI_CallVoidMethodReturnBoolean(const char* pszClassName, const char* pszMethod, bool& bReturn);
				bool JNI_CallVoidMethodReturnInt(const char* pszClassName, const char* pszMethod, int& nReturn);

				bool JNI_CallStringMethodReturnString(const char* pszClassName, const char* pszMethod, const char* pszParam1, string& strReturn);
				bool JNI_CallStringMethodReturnVoid(const char* pszClassName, const char* pszMethod, const char* pszParam1);
				bool JNI_CallIntMethodReturnVoid(const char* pszClassName, const char* pszMethod, int nVal);
