/*!
 * @brief	IOS系统相关的方法
 * @author	Zhou Lingfei
 * @date	2012-9-12
 * @ingroup	Common
 */

#include "System.h"

#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <iostream>
#include <iconv.h>

#import <CommonCrypto/CommonCryptor.h>
#import <CommonCrypto/CommonDigest.h>
#import <AdSupport/AdSupport.h>
#import "GTMBase64.h"
#import "Reachability.h"
#include "SvUDIDTools.h"

#import "cocos2d.h"
USING_NS_CC;

#import <ifaddrs.h>
#import <arpa/inet.h>

// 将NSString类型转化为string类型
std::string NSString2String(NSString* str)
{
    const char* cstr = [str cStringUsingEncoding:NSUTF8StringEncoding];
    return std::string(cstr);
}

// 获取设备中IOS系统的版本
std::string System::getSystemVersion()
{
    NSString* sysVersion = [[UIDevice currentDevice] systemVersion];
    return NSString2String(sysVersion);
}


// 获取设备udid
std::string System::getUDID()
{
    NSString* udid = [SvUDIDTools UDID];
    return NSString2String(udid);
}

// 获取应用程序的版本
std::string System::getAppVersion()
{
//    NSString* appVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
    NSString* appVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    return NSString2String(appVersion);
}

// 获取应用程序的名称
std::string System::getAppName()
{
    NSString* appName = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleDisplayName"];
    return NSString2String(appName);
}

// 获取设备的类型描述: iPad, iPhone, iPod Touch, iPad Simulator
std::string System::getDeviceModel()
{
    NSString* model = [[UIDevice currentDevice] model];
    return NSString2String(model);
}

// 获取设备的类型
DeviceType System::getDeviceType()
{

//	return DEVICE_TYPE_ANDROID;
    if(UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    {
        return DEVICE_TYPE_IPHONE;
    }
    return DEVICE_TYPE_IPAD;
}

std::string System::getPackageName()
{
	return "com.og";
}

std::string System::getIMSI()
{
	return "";
}

std::string System::getMobileNum()
{
	return "";
}

ComfromID System::getComfromID()
{
    return ComfromIPad;
}

// 获取设备的mac地址
std::string System::getMacAddress()
{
    int                 mib[6];
    size_t              len;
    char                *buf;
    unsigned char       *ptr;
    struct if_msghdr    *ifm;
    struct sockaddr_dl  *sdl;
    
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0) 
    {
        return std::string();
    }
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) 
    {
        return std::string();
    }
    
    buf = (char*)malloc(len);
    if (buf == NULL) 
    {
        return std::string();
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) 
    {
        free(buf);
        return std::string();
    }
    
    ifm = (struct if_msghdr *)buf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);
    
    std::string mac;        
    for (int i = 0; i < 6; i++) 
    {
        char hexChar[3] = {0};
        sprintf(hexChar, "%02X", (unsigned int)*(ptr + i));
        mac.append(hexChar);
    }
    free(buf);
    return mac;
}

// 获取设备的唯一标志
std::string System::getUniqueID()
{
	return getUDID();
/*
 NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    NSString *uid = [userDefaults objectForKey:@"unique_identifier"];
    if (uid == nil || [uid length] == 0)
    {
        std::string mac = getMacAddress();
        if (mac == "020000000000")
        {
            mac = std::string([[[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString] UTF8String]);
        }
        uid = [NSString stringWithUTF8String:getDESString(mac).c_str()];
        [userDefaults setObject:uid forKey:@"unique_identifier"];
        [userDefaults synchronize];
    }
    return std::string([uid UTF8String]);
*/
}

// 获取MD5加密的字符串
std::string System::getMD5String(const std::string &src)
{
    const char* cstr = src.c_str();
    unsigned char result[16] = {0};
    CC_MD5(cstr, strlen(cstr), result);
	
    NSMutableString* md5Str = [NSMutableString stringWithCapacity:32];
	for (int i = 0; i < 16; i++)
    {
		[md5Str appendFormat:@"%02x", result[i]];
	}
    return NSString2String(md5Str);
}

// 获取DES加密的字符串
std::string System::getDESString(const std::string& str)
{       
    char key[] = { 52, 43, -36, 87, 90, -89, 99, 72 };       
    char iv[] = { 59, 67, -32, 79, 36, -99, 89, 27 };
    char encrypedBuffer[1024] = {0};
    
    size_t bufferNumBytes;
    CCCryptorStatus cryptStatus = CCCrypt(kCCEncrypt, 
                                          
                                          kCCAlgorithmDES, 
                                          
                                          kCCOptionPKCS7Padding,
                                          
                                          key, 
                                          
                                          kCCKeySizeDES,
                                          
                                          iv, 
                                          
                                          str.c_str(), 
                                          
                                          str.length(),
                                          
                                          encrypedBuffer, 
                                          
                                          1024,
                                          
                                          &bufferNumBytes);
    
    std::string output;
    if(cryptStatus == kCCSuccess)
    {
        NSData *data = [NSData dataWithBytes:encrypedBuffer length:(NSUInteger)bufferNumBytes];
        
        //转换到base64 
        data = [GTMBase64 encodeData:data]; 
        NSString * ciphertext = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]autorelease]; 
        
        output = [ciphertext UTF8String];
    }
    return output; 
}

void System::popSystemDialog(const char* text, const char* buttonLabel)
{
    NSString* dlgString = [NSString stringWithUTF8String:text];
    NSString* buttonString = [NSString stringWithUTF8String:buttonLabel];
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"" 
                                                    message:dlgString 
                                                   delegate:nil 
                                          cancelButtonTitle:buttonString 
                                          otherButtonTitles:nil,nil];
    [alert show];
    [alert release];
}

// 获取电池电量，范围0到1.0。－1表示电量未知。
float System::getBatteryLevel()
{
    [[UIDevice currentDevice] setBatteryMonitoringEnabled:YES];
    return [[UIDevice currentDevice] batteryLevel];
}

void System::shutDown()
{
    exit(0);
}

// 检测WIFI是否可用
bool System::isWIFIEnabled()
{
    return ([[Reachability reachabilityForLocalWiFi] currentReachabilityStatus] != NotReachable);
}

// 检测3G网络是否可用
bool System::is3GEnabled()
{
    return [[Reachability reachabilityForInternetConnection] isReachableViaWWAN];
}

//网络是否可用
bool System::isNetAvailable()
{
    return   (isWIFIEnabled() ||  is3GEnabled());
}

// 获取系统软键盘的高度
float System::getSoftKeyboardHeight()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    return size.height / 2;
}

//std::string System::getSystemTime()
//{
//    std::string s;
//    NSString* date;
//    NSDateFormatter* formatter = [[[NSDateFormatter alloc] init] autorelease];
//    [formatter setDateFormat:@"HH:mm"];
//    date = [formatter stringFromDate:[NSDate date]];    
//    s = [date UTF8String];
//    return s;
//}

std::string System::getSystemTime()
{
	time_t t = time(NULL);
	struct tm* tt = localtime(&t);
	char timeString[16] = {0};
	sprintf(timeString, "%02d:%02d", tt->tm_hour, tt->tm_min);
	return std::string(timeString);
}

std::string System::getResolutionID()
{
    std::string s = std::string("1024x768");
    return s;
}

void System::openURL(const std::string &url)
{
    NSString* str = [NSString stringWithCString:url.c_str() encoding:NSUTF8StringEncoding];
    NSURL* URL = [NSURL URLWithString:str];
    [[UIApplication sharedApplication] openURL:URL];
}

long long System::getCurrTime()
{
    return time(NULL);
}

int System::iconv_code(const std::string& fromcode, const std::string& tocode, const std::string& in, std::string& out)
{
    char *sin, *sout;
    size_t lenin, lenout;
    int ret;
    iconv_t c_pt;
    
    if ((c_pt = iconv_open(tocode.c_str(), fromcode.c_str())) == (iconv_t)(-1)){
        out=in;
        return -1;
    }
    
    lenin = in.size()+1;//字符串要有个结束符位
    char* oldPasserName = new char[lenin];//这里oldPasserName为工作中用的名字，没什么特殊含义
    strcpy(oldPasserName, in.c_str());
    
    lenout = lenin*10;
    char* newPasserName = new char[lenout];
    
    sin = oldPasserName;
    sout = newPasserName;
    ret = iconv(c_pt, &sin, &lenin, &sout, &lenout);
    if(ret == -1){
        out=in;
        iconv_close(c_pt);
        return -1;
    }
    iconv_close(c_pt);
    
    out = newPasserName;
    delete []oldPasserName;//别忘记释放掉
    delete []newPasserName;
    return 0;
}

std::string System::getChannelID()
{
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
	return "thran";
}

std::string System::getLoginChannelID()
{
    return "thran";
}

int System::getTextHeight(const std::string &content, const std::string &fontName, int fontSize, int alignment, int width, int height)
{
	return 0;
}

// Get IP Address
std::string System::getIPAddress()
{
    NSString *address = @"";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                if([[NSString stringWithUTF8String:temp_addr->ifa_name] isEqualToString:@"en0"]) {
                    // Get NSString from C String
                    address = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr)];
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
    return NSString2String(address);
}

