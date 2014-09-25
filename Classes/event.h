//
//  event.h
//  Slot
//
//  Created by zhengyu on 14-8-21.
//
//

#ifndef __Slot__event__
#define __Slot__event__

#include <iostream>

#define EVENT_LOGIN2MEG "0"
#define EVENT_REG2MEG "1"
#define EVENT_LINKGAME2MSG "2"

#define EVENT_LOGINMES_FROM_MSG "1000"
#define EVENT_REGMES_FROM_MSG "1001"


#define EVENT_CONECT_GAME "5000"
#define EVENT_SENDLINK_GAME "5001"

#define EVENT_SEND_MEG2SEVER "6000"//消息层向服务器发messageID
#define EVENT_REC_MEG_FROM_SEVER "6001"//把服务器接收的数据传到UI

#define EVENT_K_REC_MEG_FROM_SEVER "10001" //游戏内部事件
#define EVENT_K2_REC_MEG_FROM_SEVER "10002"

#endif /* defined(__Slot__event__) */
