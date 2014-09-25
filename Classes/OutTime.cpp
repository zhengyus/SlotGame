//
//  OutTime.cpp
//  Slot
//
//  Created by kc on 14-9-18.
//
//

#include "OutTime.h"

static OutTime* s_dataManager = NULL;

OutTime::OutTime()
{
    m_map_SendMegID_timeNum.clear();
    m_map_SendMegID_timeNum.insert(std::pair<int, int>(OGID_TEXAS_SLOTS_REQGAMESTRT, 0));//老虎机转动
    m_map_SendMegID_timeNum.insert(std::pair<int, int>(OGID_TEXAS_SLOTS_REQPETFIGHT2, 0));//战斗转动
    m_map_SendMegID_timeNum.insert(std::pair<int, int>(OGID_TEXAS_SLOTS_LOGIN, 0));//登入
    
}

OutTime::~OutTime()
{
    
}

OutTime* OutTime::sharedDataManager()
{
    if (s_dataManager == NULL)
    {
        s_dataManager = new OutTime;
    }
    
    return s_dataManager;
}

void OutTime::create(int sendMegID)
{
    switch (sendMegID)
    {
        case OGID_TEXAS_SLOTS_REQGAMESTRT:
        {
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                                                                           schedule_selector(OutTime::managerOGID_TEXAS_SLOTS_REQGAMESTRT),
                                                                           this,
                                                                           1.0f,
                                                                           false);
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETFIGHT2:
        {
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                                                                           schedule_selector(OutTime::managerOGID_TEXAS_SLOTS_REQPETFIGHT2),
                                                                           this,
                                                                           1.0f,
                                                                           false);
            break;
        }
        case OGID_TEXAS_SLOTS_LOGIN:
        {
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                                                                           schedule_selector(OutTime::managerOGID_TEXAS_SLOTS_LOGIN),
                                                                           this,
                                                                           1.0f,
                                                                           false);
            
            break;
        }
        default:
        {
            break;
        }
    }
    
    
}

void OutTime::release(int retMegID)
{
    switch (retMegID)
    {
        case OGID_TEXAS_SLOTS_REQGAMESTRT:
        {
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
                                                                             schedule_selector(OutTime::managerOGID_TEXAS_SLOTS_REQGAMESTRT),
                                                                             this);
            
            m_map_SendMegID_timeNum[OGID_TEXAS_SLOTS_REQGAMESTRT] = 0;
            
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETFIGHT2:
        {
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
                                                                             schedule_selector(OutTime::managerOGID_TEXAS_SLOTS_REQPETFIGHT2),
                                                                             this);
            
            m_map_SendMegID_timeNum[OGID_TEXAS_SLOTS_REQPETFIGHT2] = 0;
            
            break;
        }
        case OGID_TEXAS_SLOTS_LOGIN:
        {
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
                                                                             schedule_selector(OutTime::managerOGID_TEXAS_SLOTS_LOGIN),
                                                                             this);
            
            m_map_SendMegID_timeNum[OGID_TEXAS_SLOTS_LOGIN] = 0;
            
            break;
        }
        default:
        {
            break;
        }
    }
    
}

//void OutTime::managerOGID_TEXAS_SLOTS_REQGAMESTRT()
//{
//    
//    if(10 == m_map_SendMegID_timeNum[OGID_TEXAS_SLOTS_REQGAMESTRT])
//    {
//        CCLog("~~~~~~~与服务器断开链接");
//        release(OGID_TEXAS_SLOTS_REQGAMESTRT);

//        Meg2UIDate sendmeg;
//        sendmeg.m_id = ERR_RET;
//        sendmeg.errmeg = "与服务器断开链接";
//        sendmeg.errType = OGID_TEXAS_SLOTS_REQGAMESTRT;
//        
//        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
//    }
//
//    m_map_SendMegID_timeNum[OGID_TEXAS_SLOTS_REQGAMESTRT]++;
//    
//}

//void OutTime::managerOGID_TEXAS_SLOTS_REQPETFIGHT2()
//{
//    
//    if(10 == m_map_SendMegID_timeNum[OGID_TEXAS_SLOTS_REQPETFIGHT2])
//    {
//        CCLog("~~~~~~~与服务器断开链接");
//        release(OGID_TEXAS_SLOTS_REQPETFIGHT2);
//    }
//    
//    m_map_SendMegID_timeNum[OGID_TEXAS_SLOTS_REQPETFIGHT2]++;
//    
//}
