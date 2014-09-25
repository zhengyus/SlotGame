//
//  LinkGameSocket.h
//  Slot
//
//  Created by zhengyu on 14-8-22.
//
//

#ifndef __Slot__LinkGameSocket__
#define __Slot__LinkGameSocket__

#include <iostream>
#include "cocos2d.h"
#include "TcpSocket.h"
#include "define.h"

USING_NS_CC;

class LinkGameSocket : public TcpSocket
{
public:
    LinkGameSocket();
    ~LinkGameSocket();
    
	//接收登入后的IP PROT消息
	void onLogin1(const char* message, int size);
    
    //错误处理
    virtual void onError();
    
    //接收信息
    virtual void onReceiveData(int messageID, const char* message, int size);
    
       
private:
    ThranActionType m_ActionType;
    
};

#endif /* defined(__Slot__LinkGameSocket__) */
