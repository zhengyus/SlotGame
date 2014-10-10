//
//  define.h
//  Slot
//
//  Created by zhengyu on 14-8-21.
//
//

#ifndef __Slot__define__
#define __Slot__define__

#include "event.h"
#include "cocos2d.h"
#include "OGTexasSlotsProtocol.pb.h"
#include "GameMsgDefine.h"
#include "Hdefine.h"

USING_NS_CC;

#define RET_FROM_SERVE  134217728

#define RET_MEG_ID4LOGIN1  (RET_FROM_SERVE + 1)
#define RET_MEG_ID4LOGIN2  (RET_FROM_SERVE + 2)

#define OPEN_SOCKET_OR_HTTP 1//开启登入开关1-http 0其他scoket

#define OPEN_DEBUG 0//调试开关 1-开启调试 0其他关闭调试

#define OPEN_NET_IP 0//内外网切换开关1-连外网 0其他-连内网



//#define LOGIN_IP    "172.28.14.68"
//#define LOGIN_PORT 22446

#if (OPEN_NET_IP == 1)//---------------------------
//平台服务－－－－－－－
#define URLIP_PORT    "202.108.0.83:22447"
//游戏服务－－－－－－－
#define LOGIN_GAME_IP   "115.182.20.184"
#define LOGIN_GAME_PORT 22319
#else//---------------------------------------------
//平台服务－－－－－－－
#define URLIP_PORT    "172.28.14.86:22447"
//游戏服务－－－－－－－
//QA
//#define LOGIN_GAME_IP   "172.28.14.215"
//#define LOGIN_GAME_PORT 5001
//张明明
//#define LOGIN_GAME_IP   "172.28.26.181"
//#define LOGIN_GAME_PORT 5001
//胡小峰
//#define LOGIN_GAME_IP   "172.28.26.154"
//#define LOGIN_GAME_PORT 5001
#endif

#define LOGIN_TYPE 1
#define LOGIN_THIRD_D_NAME  "25022"
#define LOGIN_THIRD_KEY ""
#define LOGIN_THIRD_APPID   ""

#define SAVE_H_ACCOUNT  "1000"
#define SAVE_H_PASSWORD "1001"

#define SAVE_H_REGACCOUNT  "1002"
#define SAVE_H_REGPASSWORD "1003"

enum RetFromServe
{
    RET_SUCCESS = 1
};

struct Logconfig
{
    int logtype;
    std::string logthirdName;
    std::string logthirdKey;
    std::string logthirdAppID;
};


typedef enum
{
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_LOGIN,
    ACTION_TYPE_REG,
}
ThranActionType;


class LoginMeg :public CCObject
{
    
public:
    
    std::string name;
    std::string pwd;
    
};

class Ret2UILoginMeg :public CCObject
{
    
public:
    
    int ret;//-1-失去链接 0-失败 1-成功
    std::string errmeg;
    
};

class RegMeg :public CCObject
{
    
public:
    
    std::string name;
    std::string pwd;
    
};

class Ret2UIRegMeg :public CCObject
{
    
public:
    
    int ret;//0-失败 1-成功 2-用户名重复 3-用户格式非法
    std::string errmessage;//错误信息
    
};

class LoginMeg2 :public CCObject
{
    
public:
    com::ourgame::texasSlots::OGReqTRLogin1 login1;
    
};

class ConectGameSocket :public CCObject
{
    
public:
    com::ourgame::texasSlots::OGAckLinkServer ackLinkSever;
    
};

class GameLogicMeg2Sever :public CCObject
{
    
public:
    GameLogicMeg2Sever()
    {
        roomid = 2;
        msgtyoe = 2;
        petid = 1928644258;
        mailtype = 0;//奖励类型1宠物2道具3角色经验4宠物经验
        mailid = "";
        itemId = 0;
        num = 1;
        
        debrisItemId = 0;
        evolveId = 0;
        lev = 1;
        
        action = 1;
        roleName = "";
        phone = "";
        
        code = 0;
        
        // 押注线数 // 押注倍数// 自动
        line = 1;
        times = 1;
        autop = false;
        
        colour = 1;
    }
    
    ~GameLogicMeg2Sever()
    {
        
    }
    
    int m_id;//消息id
    
    //CREATE_ROLE_EVENT
    std::string CuserName;
    std::string CroleName;
    int Cmaleid;
    
    //OGID_TEXAS_SLOTS_JOINROOM-----
    int roomid;//玩家登入房间ID
    
    //OGID_TEXAS_SLOTS_EXITGAME-----
    int msgtyoe;//退出房间参数
    
    //OGID_TEXAS_SLOTS_REQPETRESET-----
    //OGID_TEXAS_SLOTS_REQPETFIGHT---
    //OGID_TEXAS_SLOTS_PETRESOLVE----
    int petid;//宠物id
    
    //OGID_TEXAS_SLOTS_REQMAILSTATE:---
    int mailtype;
    std::string mailid;
    
    //OGID_TEXAS_SLOTS_REQSHOPITEM-----
    //OGID_MSGBASE_TEXAS_SLOTS---
    int itemId;
    //OGID_MSGBASE_TEXAS_SLOTS---
    int num;
    //OGID_TEXAS_SLOTS_REQPETCOMPOSE---
    int debrisItemId;
    int evolveId;

    //OGID_TEXAS_SLOTS_REQLOGINAWARD---
    int lev;
    
    //OGID_TEXAS_SLOTS_REQPETRETURNAWARD----
    int action;
    
    //OGID_TEXAS_SLOTS_GETCODE-----
    std::string phone;
    
    //OGID_TEXAS_SLOTS_GETCODEOK---
    int code;
    
    //OGID_TEXAS_SLOTS_REQGAMESTRT----
    int line;// 押注线数
    bool autop;//自动
    
    //OGID_TEXAS_SLOTS_REQDOUBLEGAME---
    //OGID_TEXAS_SLOTS_REQGAMESTRT---
    int times;// 押注倍数

    //OGID_TEXAS_SLOTS_REQDOUBLEGAME---
    int colour;// 押注颜色
    
    //OGID_TEXAS_SLOTS_REQRANKING---
    //OGID_TEXAS_SLOTS_REQLOGINAWARD---
    std::string roleName;
};


#endif /* defined(__Slot__define__) */
