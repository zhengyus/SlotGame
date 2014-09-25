//
//  LinkGameSocket.cpp
//  Slot
//
//  Created by zhengyu on 14-8-22.
//
//

#include "LinkGameSocket.h"
#include "NetworkConstants.h"
#include "ThranConn.pb.h"
#include "OGTexasSlotsProtocol.pb.h"

#include <sstream>
#include <System.h>
#include "mobileGame.pb.h"
#include "Utils.h"


LinkGameSocket::LinkGameSocket()
{
    m_socket_name = "LinkGameSocket";
    m_ActionType = ACTION_TYPE_UNKNOWN;
    
}

LinkGameSocket::~LinkGameSocket()
{
    CCLog("kkk delete LinkSocket!!!!");
}

void LinkGameSocket::onError()
{
    if(m_ActionType == ACTION_TYPE_REG)
    {
        if( System::isNetAvailable())
        {
            CCLog("login err0-0");
        }
        else
        {
            CCLog("login err0-1");
        }
    }
    else if(m_ActionType == ACTION_TYPE_LOGIN)
    {
        if( System::isNetAvailable())
        {
            CCLog("login err1-0");
        }
        else
        {
            CCLog("login err1-1");
        }
    }
}

void LinkGameSocket::onReceiveData(int messageID, const char* message, int size)
{
	//不能断开，断开后再连会需要时间，这段时间send不出去
//	disconnect();
    
    CCLog("in LinkGameSocket Ret");

    
	switch (messageID)
    {
        case RET_MEG_ID4LOGIN1:
        {
            onLogin1(message, size);
            break;
		}
        default:
        {
            CCLog("in LinkGameSocket Ret err !!!");
			break;
        }
	}
}

void LinkGameSocket::onLogin1(const char* message, int size)
{
    com::ourgame::texasSlots::OGAckLinkServer ackLinkSever;
    ackLinkSever.ParseFromArray(message, size);
    
    CCLog("kkk linkIP = %s", ackLinkSever.linkip().c_str());
    CCLog("kkk linkport = %d", ackLinkSever.linkport());
    
    
    ConectGameSocket tmpGameSocket;
    tmpGameSocket.ackLinkSever = ackLinkSever;
    //如果接收到服务器传来的IP和端口，发送连接事件
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_CONECT_GAME, &tmpGameSocket);
    
}
