//
//  LoginSocket.h
//  Slot
//
//  Created by zhengyu on 14-8-21.
//
//

#ifndef __Slot__LoginSocket__
#define __Slot__LoginSocket__

#include <iostream>
#include "cocos2d.h"
#include "TcpSocket.h"
#include "define.h"
#include "cocos-ext.h"
#include "mobileGame.pb.h"
#include "Json.h"

USING_NS_CC;
USING_NS_CC_EXT;

#if (OPEN_SOCKET_OR_HTTP == 0)
class LoginSocket : public TcpSocket
#else
class LoginSocket : public CCLayer
#endif
{
public:
    LoginSocket();
    ~LoginSocket();
    
    //发送登入消息
    bool sendLogin(const char* name,//用户名
                           const char* password,//密码
                           int loginType,//
                           const ::std::string& thirdDigitalName= std::string(""),//
                           const ::std::string& thirdKey= std::string(""),//
                           const ::std::string& thirdAppID= std::string(""));//
#if (OPEN_SOCKET_OR_HTTP == 0)
    //接收登入后的消息
	void onLogin(const char* message, int size);
    //接收注册信息
    void onReg(const char* message, int size);
#else
    void onLogin(AckGameLogin ackLogin);
    void onReg(AckGameRegister ackLogin);
#endif
	
    
    //发送注册信息
    bool sendReg(const char* name, const char* password);
    
    //发送创建角色信息
    bool sendCreateRole(const char* userName, const char* roleName, int maleid);
    
    //错误处理
    virtual void onError();
    
    //接收信息
    virtual void onReceiveData(int messageID, const char* message, int size);
    
    //http接受信息
    void onHttpRequestCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    Logconfig m_tlogconfig;
    
    std::string zhuanYi(const std::string mystr);//字符转移
    std::string zhuanYiFan(const std::string mystr);//反相转义
    
    const char *hexStringToByte(std::string hex);
    const char *fromHex(const char * sc, int len);
    const char *fromHexString(std::string hex);
    
private:
    ThranActionType m_ActionType;
   

};


#endif /* defined(__Slot__LoginSocket__) */
