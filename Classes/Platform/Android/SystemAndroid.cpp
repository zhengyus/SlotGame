#include "Common/System.h"
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <dlfcn.h>

#include "md5/md5.h"
#include "cipher/des.h"
#include "cipher/base64.h"

#include "jni/JniHelper.h"
#include "cocos2d.h"

#include "JniPub.h"

USING_NS_CC;

#define CLASS_NAME	"com/ourgame/common/SysUtil"

std::string System::getSystemVersion()
{
	std::string strRet = "";
	JNI_CallVoidMethodReturnString(CLASS_NAME, "getSystemVersion", strRet);
	return strRet;
}

std::string System::getAppVersion()
{
	std::string strRet = "";
	JNI_CallVoidMethodReturnString(CLASS_NAME, "getVersionName", strRet);
	return strRet;
}

std::string System::getAppName()
{
    return std::string("TexasPoker");
}

std::string System::getPackageName()
{
	std::string strRet = "com.ourgame";
	JNI_CallVoidMethodReturnString(CLASS_NAME, "getPackageName", strRet);
	return strRet;
}

std::string System::getIMSI()
{
	std::string strRet;
	JNI_CallVoidMethodReturnString(CLASS_NAME, "getSubscriberId", strRet);
	return strRet;
}

std::string System::getMobileNum()
{
	std::string strRet;
	JNI_CallVoidMethodReturnString(CLASS_NAME, "getPhoneNumber", strRet);
	return strRet;
}

std::string System::getDeviceModel()
{
    return std::string("Android");
}

DeviceType System::getDeviceType()
{
    return DEVICE_TYPE_ANDROID;
}

ComfromID System::getComfromID()
{
    return ComfromAndroid;
}

std::string System::getMacAddress()
{
	struct ifreq *ifr;
	struct ifconf ifc;
	int s, i;
	int numif;
	
	// find number of interfaces.
	memset(&ifc, 0, sizeof(ifc));
	ifc.ifc_ifcu.ifcu_req = NULL;
	ifc.ifc_len = 0;
	
	if ((s = ::socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Could not obtain socket!\n");
		return "";
	}
	
	if (ioctl(s, SIOCGIFCONF, &ifc) < 0)
	{
		printf("ioctl SIOCGIFCONF error!\n");
		return "";
	}
	
	if ((ifr = (ifreq*) malloc(ifc.ifc_len)) == NULL)
	{
		printf("Could not malloc ifreq!\n");
		return "";
	}
	
	ifc.ifc_ifcu.ifcu_req = ifr;
	
	if (ioctl(s, SIOCGIFCONF, &ifc) < 0)
	{
		printf("ioctl SIOCGIFCONF error!");
		return "";
	}
	
	numif = ifc.ifc_len / sizeof(struct ifreq);
	
	char macaddr[18] = {0};
	for (i = 0; i < numif; i++)
	{
		struct ifreq *r = &ifr[i];
		struct sockaddr_in *sin = (struct sockaddr_in *)&r->ifr_addr;
		
		// skip loopback interface
		if (!strcmp(r->ifr_name, "lo"))
		{
			continue;
		}
		
		// get MAC address
		if(ioctl(s, SIOCGIFHWADDR, r) < 0)
		{
			printf("ioctl(SIOCGIFHWADDR) error!\n");
			continue;
		}
		
		sprintf(macaddr, " %02X:%02X:%02X:%02X:%02X:%02X",
				(unsigned char)r->ifr_hwaddr.sa_data[1],
				(unsigned char)r->ifr_hwaddr.sa_data[0],
				(unsigned char)r->ifr_hwaddr.sa_data[2],
				(unsigned char)r->ifr_hwaddr.sa_data[3],
				(unsigned char)r->ifr_hwaddr.sa_data[4],
				(unsigned char)r->ifr_hwaddr.sa_data[5]);
		break;
	}
	close(s);
	
	free(ifr);
	
	return std::string(macaddr);
}

std::string System::getUniqueID()
{
    return getMacAddress();
}

std::string System::getMD5String(const std::string &src)
{
	MD5_CTX context;
	char result[33] = {0};
	unsigned char digest[16] = {0};
	
	MD5Init (&context);
	MD5Update (&context, (unsigned char*)src.c_str(), src.size());
	MD5Final (digest, &context);
	
	for (int i=0; i<16; i++)
	{
		sprintf(result+2*i, "%02x", digest[i]);
	}
	return std::string(result);
}

std::string System::getDESString(const std::string& src)
{
	if(src.empty()){
		return std::string("");
	}
	
	int srcLen = src.size();
	int srcMod = srcLen % 8;
	int srcPadding = 8 - srcMod;
	const int  len = srcLen + srcPadding;
	
	unsigned char* desInputBuffer = new unsigned char[len*4];
	memset(desInputBuffer, 0, len*4);
	unsigned char* desOutputBuffer = new unsigned char[len*4];
	memset(desOutputBuffer, 0, len*4);
	
	const  char* srcdata = src.data();
    int i,j;
	for(i=0;i<srcLen;i++){
		desInputBuffer[i] = srcdata[i];
	}
	for(j=0;j<srcPadding;j++){
		desInputBuffer[srcLen+j] =  srcPadding;
	}
	des_context ctx;
	
	//byte[] iv = new byte[] { 59, 67, -32, 79, 36, -99, 89, 27 };
	unsigned char des3_test_iv[8] =
	{
		0x3B, 0x43, 0xE0, 0x4F, 0x24, 0x9D, 0x59, 0x1B,
	};
	
	//byte[] key= new byte[] { 52, 43, -36, 87, 90, -89, 99, 72 };
	unsigned char des3_test_keys[8] =
	{
		0x34, 0x2B, 0xDC, 0x57, 0x5A, 0xA7, 0x63, 0x48,
	};
	
	des_setkey_enc( &ctx, (unsigned char *) des3_test_keys );
	int desInt = des_crypt_cbc( &ctx, DES_ENCRYPT, 24, des3_test_iv, desInputBuffer, desOutputBuffer);
	size_t resultlen = len*4;
	unsigned char resultBuffer[256] = {0};
	int resInt = base64_encode( resultBuffer, &resultlen, desOutputBuffer, len );
	
	delete[] desInputBuffer;
	delete[] desOutputBuffer;
    std::string resultStr = std::string((char*)resultBuffer, resultlen);
	
	return resultStr;
}

void System::popSystemDialog(const char* text, const char* buttonLabel)
{
}

float System::getBatteryLevel()
{
    return 1.0f;
}

bool System::isWIFIEnabled()
{
	bool bRet = false;
	JNI_CallVoidMethodReturnBoolean(CLASS_NAME, "isWifiConnected", bRet);
	return bRet;
}

bool System::is3GEnabled()
{
	bool bRet = false;
	JNI_CallVoidMethodReturnBoolean(CLASS_NAME, "isMobileConnected", bRet);
	return bRet;
}

bool System::isNetAvailable()
{
//    return   (isWIFIEnabled() ||  is3GEnabled());
	bool bRet = false;
	JNI_CallVoidMethodReturnBoolean(CLASS_NAME, "isNetworkConnected", bRet);
	return bRet;
}

std::string System::getSystemTime()
{
	time_t t = time(NULL);
	struct tm* tt = localtime(&t);
	char timeString[16] = {0};
	sprintf(timeString, "%02d:%02d", tt->tm_hour, tt->tm_min);
	return std::string(timeString);
}

long long System::getCurrTime()
{
	return time(NULL);
}

void System::shutDown()
{
	return;
	JniMethodInfo method;
	if (JniHelper::getStaticMethodInfo(method, CLASS_NAME, "shutDown", "()V"))
	{
		JNIEnv* env = method.env;
		env->CallStaticObjectMethod(method.classID, method.methodID);
		env->DeleteLocalRef(method.classID);
	}
}

std::string System::getResolutionID()
{
    return std::string("800480");
}

float System::getSoftKeyboardHeight()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if (size.height > 320)
	{
		return size.height * 0.72;
	}
	else
	{
		return size.height * 0.85;
	}
}

void System::openURL(const std::string& url)
{
}

std::string GBKToUTF8(const std::string& gbk)
{
	JniMethodInfo method;
	if (JniHelper::getStaticMethodInfo(method, CLASS_NAME, "convertGBKToUTF8", "([B)[B"))
	{
		JNIEnv* env = method.env;
		jbyteArray array = env->NewByteArray(gbk.size());
		env->SetByteArrayRegion(array, 0, gbk.size(), (const jbyte*)gbk.data());
		jbyteArray result = (jbyteArray)env->CallStaticObjectMethod(method.classID, method.methodID, array);
		env->DeleteLocalRef(array);
		env->DeleteLocalRef(method.classID);
		
		int length = env->GetArrayLength(result);
		char* cstr = (char*)env->GetByteArrayElements(result, 0);
		
		std::string utf8(cstr, length);
		env->ReleaseByteArrayElements(result, (jbyte*)cstr, 0);
		return utf8;
	}
	else
		return "";
}

std::string UTF8ToGBK(const std::string& utf8)
{
	JniMethodInfo method;
	if (JniHelper::getStaticMethodInfo(method, CLASS_NAME, "convertUTF8ToGBK", "([B)[B"))
	{
		JNIEnv* env = method.env;
		jbyteArray array = env->NewByteArray(utf8.size());
		env->SetByteArrayRegion(array, 0, utf8.size(), (const jbyte*)utf8.data());
		
		jbyteArray result = (jbyteArray)env->CallStaticObjectMethod(method.classID, method.methodID, array);
		env->DeleteLocalRef(array);
		env->DeleteLocalRef(method.classID);
		
		int length = env->GetArrayLength(result);
		char* cstr = (char*)env->GetByteArrayElements(result, 0);
		
		std::string gbk(cstr, length);
		env->ReleaseByteArrayElements(result, (jbyte*)cstr, 0);
		return gbk;
	}
	else
		return "";
}

int System::getTextHeight(const std::string &content, const std::string &fontName, int fontSize, int alignment, int width, int height)
{
	JniMethodInfo method;
	if (JniHelper::getStaticMethodInfo(method, "org/cocos2dx/lib/Cocos2dxBitmap", "getTextHeight", "([B[BIIII)I"))
	{
		JNIEnv* env = method.env;
		jbyteArray contentArray = env->NewByteArray(content.size());
		env->SetByteArrayRegion(contentArray, 0, content.size(), (const jbyte*)content.data());
		
		jbyteArray fontNameArray = env->NewByteArray(fontName.size());
		env->SetByteArrayRegion(fontNameArray, 0, fontName.size(), (const jbyte*)fontName.data());
		jint _fontSize = fontSize;
		jint _alignment = alignment;
		jint _width = width;
		jint _height = height;
		jint _int = (jint)env->CallStaticObjectMethod(method.classID, method.methodID, contentArray, fontNameArray, _fontSize, _alignment, _width, _height);
		
		env->DeleteLocalRef(contentArray);
		env->DeleteLocalRef(fontNameArray);
		env->DeleteLocalRef(method.classID);
		
		int i = _int;
		return i;
	}
	else
		return 0;
}

int System::iconv_code(const std::string &fromcode, const std::string &tocode, const std::string &in, std::string &out)
{
	if (fromcode.compare("GBK") == 0 && tocode.compare("UTF-8") == 0)
	{
		out = GBKToUTF8(in);
	}
	else if (fromcode.compare("UTF-8") == 0 && tocode.compare("GBK") == 0)
	{
		out = UTF8ToGBK(in);
	}
	return 0;
}

std::string System::getChannelID()
{
	std::string strChannel = "official";
	JNI_CallStringMethodReturnString(CLASS_NAME, "getApplicationMetaData", "UMENG_CHANNEL", strChannel);
	CCLog("getApplicationMetaData channel:%s", strChannel.c_str());
	return strChannel;
	
//	JniMethodInfo method;
//	if (JniHelper::getStaticMethodInfo(method, "com/og/common/OGMainActivity", "getChannelID", "()Ljava/lang/String;"))
//	{
//		JNIEnv* env = method.env;
//		jstring channel = (jstring)env->CallStaticObjectMethod(method.classID, method.methodID);
//		std::string channelStr = JniHelper::jstring2string(channel);
//		env->DeleteLocalRef(method.classID);
//		
//		return channelStr;
//	}
//	return "thran";
}

std::string System::getLoginChannelID()
{
	std::string strChannel = "official";
	JNI_CallStringMethodReturnString(CLASS_NAME, "getApplicationMetaData", "LOGIN_CHANNEL",strChannel);
	CCLog("getApplicationMetaData channel:%s", strChannel.c_str());
	return strChannel;
}

// 获取设备udid
std::string System::getUDID()
{
    return getUniqueID();
}

// Get IP Address
std::string System::getIPAddress()
{
	static std::string s_strEmpty;
	return s_strEmpty;
}

