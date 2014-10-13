//
//  ControllerMeg.h
//  Slot
//
//  Created by zhengyu on 14-8-22.
//
//

#ifndef __Slot__ControllerMeg__
#define __Slot__ControllerMeg__

#include <iostream>
#include "cocos2d.h"
#include "define.h"
#include "LoginSocket.h"
#include "LinkGameSocket.h"
#include "GameLogicSocket.h"
#include "Base64.h"
#include "Utils.h"


class ControllerMeg : public cocos2d::CCObject
{
public:
    ControllerMeg();
    ~ControllerMeg();
    
    //接受事件从UI传来的
    void recLoginEventFromUI(CCObject * obj);
    //接受事件从UI传来的
    void recRegEventFromUI(CCObject * obj);
    //发送登入消息
    bool sendOGReqTRLogin1(CCObject * obj);
    
    //收到负载ip 端口后触发连接
    void conectGameSocket(CCObject * obj);
    
    //想收到的负载均衡游戏服务器发票阵信息
    void sendLinkGame(CCObject * obj);
    
    //根据ID向服务器发送对应的消息
    void sendMeg2Sever(CCObject * obj);
    
    //根据ID接收服务器传来的消息，发送给UI，对其中的特殊数据需要处理后发送给UI
    void onMegFromSever(CCObject * obj);
    
    //发心跳
    void sendIdle();
    
    LoginSocket * m_loginSocket;
    LinkGameSocket * m_linkGameSocket;
    GameLogicSocket * m_gameLogicSocket;
    
private:

    int m_idleNum;
    
};


#endif /* defined(__Slot__ControllerMeg__) */
