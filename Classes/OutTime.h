//
//  OutTime.h
//  Slot
//
//  Created by kc on 14-9-18.
//
//

#ifndef __Slot__OutTime__
#define __Slot__OutTime__

#include "cocos2d.h"
#include "define.h"


#define RUN_SCHE_FUNC(__TYPE__, NUM) \
void manager##__TYPE__() \
{ \
if(NUM == m_map_SendMegID_timeNum[__TYPE__]) \
{ \
CCLog("~~~~~~~与服务器断开链接"); \
release(__TYPE__); \
Meg2UIDate sendmeg; \
sendmeg.m_id = ERR_RET; \
sendmeg.errmeg = "与服务器断开链接,请重新登入游戏"; \
sendmeg.errType = __TYPE__; \
CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg); \
} \
m_map_SendMegID_timeNum[__TYPE__]++; \
}

class OutTime : public cocos2d::CCObject
{
public:
    RUN_SCHE_FUNC(OGID_TEXAS_SLOTS_REQGAMESTRT, 10);//（函数名称，超时时间）
    RUN_SCHE_FUNC(OGID_TEXAS_SLOTS_REQPETFIGHT2, 10);
    RUN_SCHE_FUNC(OGID_TEXAS_SLOTS_LOGIN, 15);
    
    OutTime();
    ~OutTime();
    
    static OutTime* sharedDataManager();
    
    void create(int sendMegID);
    void release(int retMegID);
    
    

private:
    
    std::map<int, int> m_map_SendMegID_timeNum;
};

#endif /* defined(__Slot__OutTime__) */
