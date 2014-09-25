#include "cocos2d.h"

#include "JniPub.h"

#ifdef OS_ANDROID
#include <jni.h>
#include "platform/android/jni/jniHelper.h"
#endif//OS_ANDROID

using namespace cocos2d;
using namespace std;

#ifdef OS_ANDROID
				//************************************************************************************************************************************//
				// Ò»Ð©¸¨Öúº¯Êý
				//************************************************************************************************************************************//
				void JNI_GetStringField(JNIEnv* env, jclass clazz, jobject obj, const char* name, string& strVal)
				{
					jfieldID fid = env->GetFieldID(clazz, name, "Ljava/lang/String;");
					jstring jstrVal = static_cast<jstring>(env->GetObjectField(obj, fid));
					if(jstrVal)
					{
						const char* szVal = env->GetStringUTFChars(jstrVal, NULL);
						if(szVal)
						{
							strVal.assign(szVal);
							CCLog("%s:%s", name, szVal);
				//			GL("%s:%s", name, szVal);
						}
						env->ReleaseStringUTFChars(jstrVal, szVal);
					}
				}
				int JNI_GetIntField(JNIEnv* env, jclass clazz, jobject obj, const char* name)
				{
					jfieldID fid = env->GetFieldID(clazz, name, "I");
					jint jnVal = env->GetIntField(obj, fid);
					CCLog("%s:%d", name, (int)jnVal);
				//	GL("%s:%d", name, (int)jnVal);

					return (int)jnVal;
				}
				long JNI_GetLongField(JNIEnv* env, jclass clazz, jobject obj, const char* name)
				{
					jfieldID fid = env->GetFieldID(clazz, name, "J");
					jlong jlVal = env->GetLongField(obj, fid);
					CCLog("%s:%d", name, (int)jlVal);
				//	GL("%s:%d", name, (int)jlVal);

					return (long)jlVal;
				}

				void JNI_SetStringField(JNIEnv* env, jclass clazz, jobject obj, const char* name, const char* szVal)
				{
					jfieldID jid = env->GetFieldID(clazz, name, "Ljava/lang/String;");
					jstring jstrVal = env->NewStringUTF(szVal);
					env->SetObjectField(obj, jid, jstrVal);
					env->DeleteLocalRef(jstrVal);
				}
				void JNI_SetIntField(JNIEnv* env, jclass clazz, jobject obj, const char* name, jint jnVal)
				{
					jfieldID jid = env->GetFieldID(clazz, name, "I");
					env->SetIntField(obj, jid, jnVal);
				}
				void JNI_SetLongField(JNIEnv* env, jclass clazz, jobject obj, const char* name, jlong jlVal)
				{
					jfieldID jid = env->GetFieldID(clazz, name, "J");
					env->SetLongField(obj, jid, jlVal);
				}
				void JNI_SetFloatField(JNIEnv* env, jclass clazz, jobject obj, const char* name, jfloat jlVal)
				{
					jfieldID jid = env->GetFieldID(clazz, name, "F");
					env->SetFloatField(obj, jid, jlVal);
				}

#endif//OS_ANDROID


				bool JNI_CallVoidMethodReturnString(const char* pszClassName, const char* pszMethod, string& strReturn)
				{
#ifdef OS_ANDROID
					JniMethodInfo t;
					if(JniHelper::getStaticMethodInfo(t, pszClassName, "rtnObject", "()Ljava/lang/Object;"))
					{
						jobject jobj;
						jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

						if(JniHelper::getMethodInfo(t, pszClassName, pszMethod, "()Ljava/lang/String;"))
						{
							jstring jInfo = static_cast<jstring>(t.env->CallObjectMethod(jobj, t.methodID));
							if(jInfo)
							{
								const char* pszChars = t.env->GetStringUTFChars(jInfo, NULL);
								if(strlen(pszChars) > 0)
								{
									strReturn.assign(pszChars);
									t.env->ReleaseStringUTFChars(jInfo, pszChars);
									return true;
								}				
								t.env->ReleaseStringUTFChars(jInfo, pszChars);
							}
						}
					}
#endif//OS_ANDROID
					
					return false;
				}

				bool JNI_CallStringMethodReturnString(const char* pszClassName, const char* pszMethod, const char* pszParam1, string& strReturn)
				{
#ifdef OS_ANDROID
					JniMethodInfo t;
					if(JniHelper::getStaticMethodInfo(t, pszClassName, "rtnObject", "()Ljava/lang/Object;"))
					{
						jobject jobj;
						jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

						if(JniHelper::getMethodInfo(t, pszClassName, pszMethod, "(Ljava/lang/String;)Ljava/lang/String;"))
						{
							jstring jstrParam1 = t.env->NewStringUTF(pszParam1);
							jstring jInfo = static_cast<jstring>(t.env->CallObjectMethod(jobj, t.methodID, jstrParam1));
							t.env->DeleteLocalRef(jstrParam1);
							if(jInfo)
							{
								const char* pszChars = t.env->GetStringUTFChars(jInfo, NULL);
								if(strlen(pszChars) > 0)
								{
									strReturn.assign(pszChars);
									t.env->ReleaseStringUTFChars(jInfo, pszChars);
									return true;
								}				
								t.env->ReleaseStringUTFChars(jInfo, pszChars);
							}
						}
					}
#endif//OS_ANDROID
					
					return false;
				}

				bool JNI_CallStringMethodReturnVoid(const char* pszClassName, const char* pszMethod, const char* pszParam1)
				{
					
#ifdef OS_ANDROID
					JniMethodInfo t;
					if(JniHelper::getStaticMethodInfo(t, pszClassName, "rtnObject", "()Ljava/lang/Object;"))
					{
						jobject jobj;
						jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
						
						if(JniHelper::getMethodInfo(t, pszClassName, pszMethod, "(Ljava/lang/String;)V"))
						{
							jstring jstrParam1 = t.env->NewStringUTF(pszParam1);
							t.env->CallVoidMethod(jobj, t.methodID, jstrParam1);
							t.env->DeleteLocalRef(jstrParam1);
							return true;
						}
					}
#endif//OS_ANDROID
					
					return false;
				}

				bool JNI_CallVoidMethodReturnVoid(const char* pszClassName, const char* pszMethod)
				{
#ifdef OS_ANDROID
					JniMethodInfo t;
					if(JniHelper::getStaticMethodInfo(t, pszClassName, "rtnObject", "()Ljava/lang/Object;"))
					{
						jobject jobj;
						jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

						if(JniHelper::getMethodInfo(t, pszClassName, pszMethod, "()V"))
						{
							t.env->CallVoidMethod(jobj, t.methodID);
							return true;
						}
					}
#endif//OS_ANDROID
					
					return false;
				}

				bool JNI_CallVoidMethodReturnBoolean(const char* pszClassName, const char* pszMethod, bool& bReturn)
				{
#ifdef OS_ANDROID
					JniMethodInfo t;
					if(JniHelper::getStaticMethodInfo(t, pszClassName, "rtnObject", "()Ljava/lang/Object;"))
					{
						jobject jobj;
						jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

						if(JniHelper::getMethodInfo(t, pszClassName, pszMethod, "()Z"))
						{
							jboolean jbRet = t.env->CallBooleanMethod(jobj, t.methodID);
							bReturn = jbRet == JNI_TRUE ? true : false;
							return true;
						}
					}
#endif//OS_ANDROID
					
					return false;
				}

				bool JNI_CallVoidMethodReturnInt(const char* pszClassName, const char* pszMethod, int& nReturn)
				{
#ifdef OS_ANDROID
					JniMethodInfo t;
					if(JniHelper::getStaticMethodInfo(t, pszClassName, "rtnObject", "()Ljava/lang/Object;"))
					{
						jobject jobj;
						jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

						if(JniHelper::getMethodInfo(t, pszClassName, pszMethod, "()I"))
						{
							jint jnRet = t.env->CallIntMethod(jobj, t.methodID);
							nReturn = (int)jnRet;
							return true;
						}
					}
#endif//OS_ANDROID
					
					return false;
				}

				bool JNI_CallIntMethodReturnVoid(const char* pszClassName, const char* pszMethod, int nVal)
				{
					
#ifdef OS_ANDROID
					JniMethodInfo t;
					if(JniHelper::getStaticMethodInfo(t, pszClassName, "rtnObject", "()Ljava/lang/Object;"))
					{
						jobject jobj;
						jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
						
						if(JniHelper::getMethodInfo(t, pszClassName, pszMethod, "(I)V"))
						{
							jint jnVal = nVal;
							t.env->CallVoidMethod(jobj, t.methodID, jnVal);
							return true;
						}
					}
#endif//OS_ANDROID
					
					return false;
				}
				
