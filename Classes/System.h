/*!
 * @brief	系统相关的方法
 * @author	Zhou Lingfei
 * @date	2012-9-12
 * @ingroup	Common
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <string>

#define SPS(_ccnode_, _x_, _y_)	(_ccnode_->setPosition(ccp(_x_, _y_)))
#define MAR(_ccnode_)		(_ccnode_->setAnchorPoint(ccp(0.5, 0.5)))
#define LAR(_ccnode_)		(_ccnode_->setAnchorPoint(ccp(0, 0.5)))
#define RAR(_ccnode_)		(_ccnode_->setAnchorPoint(ccp(1, 0.5)))
#define TAR(_ccnode_)		(_ccnode_->setAnchorPoint(ccp(0.5, 1)))
#define BAR(_ccnode_)		(_ccnode_->setAnchorPoint(ccp(0.5, 0)))
#define ZAR(_ccnode_)		(_ccnode_->setAnchorPoint(ccp(0, 0)))
#define LTAR(_ccnode_)		(_ccnode_->setAnchorPoint(ccp(0, 1)))
#define RBAR(_ccnode_)		(_ccnode_->setAnchorPoint(ccp(1, 0)))
#define RTAR(_ccnode_)		(_ccnode_->setAnchorPoint(ccp(1, 1)))

typedef enum DeviceType
{
    DEVICE_TYPE_UNKNOWN = 0,
    DEVICE_TYPE_ANDROID = 1,
    DEVICE_TYPE_IPHONE = 2,
    DEVICE_TYPE_IPAD = 3,
    DEVICE_TYPE_PC = 4
}
DeviceType;

typedef enum ComfromID
{
    ComfromUnknown = 0,
    ComfromAndroid,
    ComfromIPhone,
    ComfromIPad,
    ComfromPC,
}
ComfromID;

class System
{
public:
    /*!
     * @brief		获取系统版本。
     */
    static std::string getSystemVersion();
    
    /*!
     * @brief		获取应用程序的版本。
     */
    static std::string getAppVersion();
    
    /*!
     * @brief		获取应用程序的名称。
     */
    static std::string getAppName();
    
    /*!
     * @brief		获取设备的类型描述: iPad, iPhone, iPod Touch, iPad Simulator。
     */
    static std::string getDeviceModel();
    
    /*!
     * @brief		获取设备的类型。
     */
    static DeviceType getDeviceType();
    
    /*!
     * @brief		获取登陆来源id。
     */
    static ComfromID getComfromID();
    
    /*!
     * @brief		获取设备的mac地址。
     */
    static std::string getMacAddress();
    
    /*!
     * @brief		获取设备的唯一标志。
     */
    static std::string getUniqueID();
    
    /*!
     * @brief		获取MD5加密的字符串。
     */
    static std::string getMD5String(const std::string& src);
    
    /*!
     * @brief		获取DES加密的字符串。
     */
    static std::string getDESString(const std::string& src);
    
    /*!
     * @brief		弹出系统对话框。
     */
    static void popSystemDialog(const char* text, const char* buttonLabel);
    
    /*!
     * @brief		获取电池电量，范围0到1.0。－1表示电量未知。
     */
    static float getBatteryLevel();
    
	/*!
     * @brief		获取包名。
     */
	static std::string getPackageName();
    
	/*!
     * @brief		获取手机IMSI
     */
	static std::string getIMSI();
    
	/*!
     * @brief		获取手机号码
     */
	static std::string getMobileNum();
    
    /*!
     * @brief		检测网络是否可用。
     */
    static bool isNetAvailable();
    
    /*!
     * @brief		检测WIFI是否可用。
     */
    static bool isWIFIEnabled();
    
    /*!
     * @brief		检测3G网络是否可用。
     */
    static bool is3GEnabled();
    
    /*!
     * @brief		获取当前时间。
     */
    static std::string getSystemTime();
    
	/*!
	 * @brief		获取系统时间。
	 */
	static long long getCurrTime();
    
	/*!
     * @brief		关闭程序。
     */
	static void shutDown();
    
	/*!
     * @brief		获取系统软键盘的高度。
     */
	static float getSoftKeyboardHeight();
    
    /*!
     * @brief	 转换字符集
     * @param   fromcode:源string使用的字符集，如"UTF-8";，对于汉字每个汉字为3个字节存储
     *                     tocode:目的string使用的字符集，如"GB2312";，对于汉字每个汉字为2个字节存储
     *                     in:需要转码的字符串
     *                     out:转码后的字符串
     * @return  文件全路径
     */
    static int iconv_code(const std::string &fromcode, const std::string &tocode, const std::string &in, std::string &out);
    
    /*!
     * @brief		获取屏幕分辨率ID。
     */
    static std::string getResolutionID();
    
	/*!
     * @brief		获取渠道号。
     */
	static std::string getChannelID();
    
    /*!
     * @brief		获取登录渠道号。
     */
    static std::string getLoginChannelID();
    
    /*!
     * @brief		跳转到页面。
     */
    static void openURL(const std::string& url);
    
	/*!
     * @brief		获取多行文字的高度。
     */
	static int getTextHeight(const std::string &content, const std::string &fontName, int fontSize, int alignment, int width, int height);
    
    /*!
     * @brief		获取UDID。
     */
    static std::string getUDID();

	// Get IP Address
	static std::string getIPAddress();
};

#endif
