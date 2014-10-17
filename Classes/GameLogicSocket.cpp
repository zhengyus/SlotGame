//
//  GameLogicSocket.cpp
//  Slot
//
//  Created by zhengyu on 14-8-22.
//
//

#include "GameLogicSocket.h"
#include "NetworkConstants.h"
#include "ThranConn.pb.h"
#include "OGTexasSlotsProtocol.pb.h"

#include <sstream>
#include <System.h>
#include "mobileGame.pb.h"
#include "Utils.h"

//add by zy
#include "GameMsgDefine.h"
#include "OGTexasSlotsProtocol.pb.h"




GameLogicSocket::GameLogicSocket()
{
    m_socket_name = "GameLogicSocket";
    m_ActionType = ACTION_TYPE_UNKNOWN;
    
}

GameLogicSocket::~GameLogicSocket()
{
    CCLog("delete GameLogicSocket!!!!");
}

void GameLogicSocket::onError()
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

//////////////////////////////////////////////////////////////////////////////////
///                                                                           ///
///                              send msg                                     ///
///                                                                           ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
// 心跳(无消息体)
bool GameLogicSocket::sendIdle()
{
    bool result;
    result = send("", OGID_TEXAS_SLOTS_IDLE);
    return result;
    
}

 // 玩家登录验证  (CLIENT+2)
bool GameLogicSocket::sendLogin(string UserId,
                                string Cert,
                                int32  gameId,
                                int32  comfromId,
                                string userName,
                                string nickName,
                                string entrance)
{
    bool result;
    OGReqTRLogin2 login;
    login.set_userid(UserId);
    login.set_cert(Cert);
#if (OPEN_NET_IP == 0)
//    login.set_cert("slotMachine");///--------test
//    login.set_cert("hfHFGOV,dsfjafF;SDAF35h.$&%$&jk45&FDS*&$5FDS/AFO6");///--------test
    
#endif
    login.set_gameid(gameId);
    login.set_comfromid(comfromId);
    login.set_username(userName);
    login.set_entrance(entrance);
    login.set_nickname(nickName);
    result = send(&login,OGID_TEXAS_SLOTS_LOGIN);
    
#if (OPEN_DEBUG == 1)//返回信息给UI
    
//    Ret2UILoginMeg ret2UI;
//    ret2UI.ret = result;
//    
//    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_LOGINMES_FROM_MSG, &ret2UI);
  
#endif
    
    return result;
}

//点击进入房间的操作 	( CLIENT + 3 )
bool GameLogicSocket::sendJoinRoom(int32 roodId)
{
    bool result;
    OGReqJoinRoom joinRoom;
    joinRoom.set_roomid(roodId);
    result = send(&joinRoom, OGID_TEXAS_SLOTS_JOINROOM);
    return result;
}

/////////////////////////////////////////to do//////////////////////////
// 关闭按钮消息 (CLIENT+4)
//    ET_TYPE1            = 1; // 退出房间
//    ET_TYPE2            = 2; // 退出游戏
//    ET_TYPE3            = 3; // 退出比倍
//    ET_TYPE4            = 4; // 退出宠物战斗
bool GameLogicSocket::sendExitGame(int32 msgType)
{
    CCLog("exit!!!!!!!!!!!!!");
    bool result;
    OGReqExitType exitType;
    
    OGReqExitType::E_MsgType typeMsg = (OGReqExitType::E_MsgType)msgType;
    //typeMsg->ET_TYPE1

    exitType.set_msgtype(typeMsg);
    result = send(&exitType, OGID_TEXAS_SLOTS_EXITGAME);
    return result;
}

 //请求宠物消息(CLIENT + 5)
bool GameLogicSocket::sendReqPetList()
{
    bool result;
    
    OGReqPetList petList;
    result = send(&petList, OGID_TEXAS_SLOTS_REQPETLIST);
    return result;
}

//请求宠物洗点（CLIENT + 6）
bool GameLogicSocket::sendReqPetReset(int32 petId)
{
    bool result;
    
    OGReqPetReSet petReset;
    petReset.set_petid(petId);
    result = send(&petReset, OGID_TEXAS_SLOTS_REQPETRESET);
    return result;
}

 //请求宠物参战（CLIENT + 7）
bool GameLogicSocket::sendReqPetFight(int32 petId)
{
    bool result;
    
    OGReqPetJoinFight petJoinFight;
    petJoinFight.set_petid(petId);
    result = send(&petJoinFight,OGID_TEXAS_SLOTS_REQPETFIGHT );
    return result;
}

//请求宠物属性界面的宠物背包 (CLIENT + 8)
bool GameLogicSocket::sendPetPack()
{
    bool result;
    
    OGReqPetGrid petGrid;
    result = send(&petGrid, OGID_TEXAS_SLOTS_REQPETPACK);
    return result;
}

 //请求排行榜(CLIENT +9)
bool GameLogicSocket::sendRanking(string roleName)
{
    bool result;
    OGReqRank rank;
    rank.set_rolename(roleName);
    result = send(&rank, OGID_TEXAS_SLOTS_REQRANKING);
    return result;
}

 //邮件请求(CLIENT + 10)
bool GameLogicSocket::sendMails()
{
    bool result;
    
    OGReqMails mails;
   
    result = send(&mails, OGID_TEXAS_SLOTS_REQMAILS);
    return result;
}

 //邮件删除(CLIENT + 11)
bool GameLogicSocket::sendMailsState(int32 type,string id)
{
    bool result;
    OGReqHandleMails mailsState;
    mailsState.set_type(type );
    mailsState.set_id(id);
    result = send(&mailsState, OGID_TEXAS_SLOTS_REQMAILSTATE);
    return result;
}

//请求宠物分解（CLIENT + 12）
bool GameLogicSocket::sendPetResolve(int32 petId)
{
    bool result;
    OGReqPetChange petChange;
    petChange.set_petid(petId);
    result = send(&petChange, OGID_TEXAS_SLOTS_PETRESOLVE);
    return result;
}

//完成新手教程（CLIENT + 13）
bool GameLogicSocket::sendGuildOver()
{
    bool result;
    
    OGReqNewHandOver newHanderOver;
    result = send(&newHanderOver, OGID_TEXAS_SLOTS_GUIDEOVER);
    return result;
}

//领取新手奖励（CLIENT + 14）
bool GameLogicSocket::sendReqAward()
{
    bool result;
    OGReqNewHandAward newHandAward;
    result = send(&newHandAward, OGID_TEXAS_SLOTS_REQAWARD);
    return result;
}

//请求商品信息（CLIENT + 15）
bool GameLogicSocket::sendReqShopItem(int32 itemId)
{
    bool result;
    OGReqShopItem shopItem;
    shopItem.set_itemid(itemId);
    result = send(&shopItem, OGID_TEXAS_SLOTS_REQSHOPITEM);
    return result;
}

//购买道具（CLIENT + 16）
bool GameLogicSocket::sendReqBuyItem(int32 itemId,int32 num)
{
    bool result;
    OGReqBuyItem buyItem;
    buyItem.set_itemid(itemId);
    buyItem.set_num(num);
    result = send(&buyItem, OGID_TEXAS_SLOTS_REQBUYITEM);
    return result;
}

//请求道具合成（CLIENT + 17）
bool GameLogicSocket::sendReqPetCompose(int32 debrisItemId,int32 evolveId)
{
    bool result;
    OGReqItemChange itemChange;
    itemChange.set_debrisitemid(debrisItemId);
    itemChange.set_evolveid(evolveId);
    result = send(&itemChange, OGID_TEXAS_SLOTS_REQPETCOMPOSE);
    return result;
}

//请求碎片合成方案（CLIENT + 18）
bool GameLogicSocket::sendReqCompse()
{
    bool result;
    OGReqItemChangeData itemChangeData;
    result = send(&itemChangeData,OGID_TEXAS_SLOTS_REQCOMPOSE);
    return result;
}

//请求分解方案（CLIENT + 19）
bool GameLogicSocket::sendReqPetResolveScheme()
{
    bool result;
    OGReqPetChangeData petChangeData;
    result = send(&petChangeData, OGID_TEXAS_SLOTS_REQPETRESOLVESCHEME);
    return result;
}

//请求抽奖（CLIENT + 20）
bool GameLogicSocket::sendReqLoginAward(string roleName,int32 lev)
{
    bool result;
    OGReqLoginGetAward loginGetAward;
    loginGetAward.set_rolename(roleName);
    loginGetAward.set_lev(lev);
    result = send(&loginGetAward, OGID_TEXAS_SLOTS_REQLOGINAWARD);
    return result;
}

//宠物返奖(CLIENT + 21)
bool GameLogicSocket::sendReqPetReturnAward(int32 action)//1喂食 2 抚摸
{
    bool result;
    OGReqPetReturnAward petReturnAward;
    petReturnAward.set_action(action);
    result = send(&petReturnAward, OGID_TEXAS_SLOTS_REQPETRETURNAWARD);
    return result;
}

//打开绑定界面,请求手机是否绑定(CLIENT + 22)
bool GameLogicSocket::sendBindPhone()
{
    bool result;
    OGReqBindPhone bindPhone;
    result = send(&bindPhone, OGID_TEXAS_SLOTS_BINDPHONE);
    return result;
}



//获取验证码(CLIENT + 23)
bool GameLogicSocket::sendGetCode(string phone)//绑定的手机号
{
    bool result;
    OGReqGetPhoneCode getPhoneCode;
    getPhoneCode.set_phone(phone);
    result = send(&getPhoneCode, OGID_TEXAS_SLOTS_GETCODE);
    return result;
}

//输入验证码,获取奖励(CLIENT + 24)
bool GameLogicSocket::sendGetCodeOK(int32 code)//验证码
{
    bool result;
    OGReqGetPhoneAward getPhoneAward;
    getPhoneAward.set_code(code);
    result = send(&getPhoneAward, OGID_TEXAS_SLOTS_GETCODEOK);
    return result;
}

// 开始转动游戏 (CLIENT+50)
bool GameLogicSocket::sendReqStartGame(int32 line,int32 times,bool autop)// 押注线数 // 押注倍数// 自动
{
    bool result;
    OGReqStartGame startGame;
    startGame.set_line(line);
    startGame.set_times(times);
    startGame.set_auto_(autop);
    result = send(&startGame, OGID_TEXAS_SLOTS_REQGAMESTRT);
    return result;
}

// 开始比倍游戏 (CLIENT+51)
bool GameLogicSocket::sendReqDoubleGame(int32 times,int32 colour)// 押注倍数// 押注颜色
{
    bool result;
    OGReqDoubleGame doubleGame;
    doubleGame.set_times(times);
    doubleGame.set_colour(colour);
    result = send(&doubleGame, OGID_TEXAS_SLOTS_REQDOUBLEGAME);
    return result;
}
// 比倍收分消息 (CLIENT+52)
bool GameLogicSocket::sendReqDoubleGetGold()
{
    bool result;
    OGReqDoubleGetGold doubleGetGold;
    result = send(&doubleGetGold, OGID_TEXAS_SLOTS_REQDOUBLEGETGOLD);
    return result;
}

//接收宠物战斗(CLIENT + 53)
bool GameLogicSocket::sendReqPetFight()
{
    bool result;
    OGReqPetFight petFight;
    result = send(&petFight, OGID_TEXAS_SLOTS_REQPETFIGHT2);
    return result;
}

//接收宠物逃跑(CLIENT + 54)
bool GameLogicSocket::sendReqPetFlee()
{
    bool result;
    OGReqPetFlee petFlee;
    result = send(&petFlee,OGID_TEXAS_SLOTS_REQPETFLEE );
    return result;
}

//接收宠物战斗胜利抽奖(CLIENT + 55)
bool GameLogicSocket::sendReqFightAward()
{
    bool result;
    OGReqPetGetAward petGetAward;
    result = send(&petGetAward, OGID_TEXAS_SLOTS_REQFIGHTAWARD);
    return result;
}

//请求jp获奖记录（CLIENT + 56）
bool GameLogicSocket::sendReqJpRecord(string roleName)
{
    bool result;
    OGReqJpRecord jpRecord;
    jpRecord.set_rolename(roleName);
    result = send(&jpRecord, AISLE_TEXAS_SLOTS_JACKPOTLIST);
    return result;
}



//////////////////////////////////////////////////////////////////////////////////
///                                                                           ///
///                              recieve msg                                  ///
///                                                                           ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
void GameLogicSocket::onRoomSite(const char* message, int size)
{
    OGAckRoom meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk result = %d", meg.result());
    CCLog("kkk reason = %d", meg.reason());
    CCLog("kkk roomid = %d", meg.roomid());
    CCLog("kkk roomname = %s", meg.roomname().c_str());
    CCLog("kkk roommaxline = %d", meg.roommaxline());
    CCLog("kkk roomPic = %d", meg.roompic());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ROOMSITE;
    tmpMeg.ackOGAckRoom = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
    
#if (OPEN_DEBUG == 1)//连线转动
    
//    GameLogicMeg2Sever tmpMeg1;
//    tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQGAMESTRT;
//    tmpMeg1.line = 1;
//    tmpMeg1.times = 1;
//    tmpMeg1.autop = false;
//    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
    
#endif
}

void GameLogicSocket::onRoomList(const char* message, int size)
{
    OGAckRoomList meg;
    meg.ParseFromArray(message, size);
    
    if(meg.roomlist_size() == 0)
    {
        CCLog("~~~~~~~~in OGAckRoomList ListSize is 0~~~~~~~~~~");
        
    }
    else
    {
        for (int i = 0; i < meg.roomlist_size(); i++)
        {
            CCLog("kkk roomidhhhhhhh = %d", meg.roomlist(i).roomid());
            CCLog("kkk roomname = %s", meg.roomlist(i).roomname().c_str());
            CCLog("kkk roomlevel = %d", meg.roomlist(i).roomlevel());
            CCLog("kkk roomgold = %lld", meg.roomlist(i).roomgold());
            CCLog("kkk roomtip = %s", meg.roomlist(i).roomtip().c_str());
            CCLog("kkk roomon = %d", meg.roomlist(i).roomon());
        }
    }
    
    
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ROOMLIST;
    tmpMeg.ackOGAckRoomList = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onPlayerInfo(const char* message, int size)
{
    OGAckRoleMsg meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk level = %d", meg.level());
    CCLog("kkk exp = %d", meg.exp());
    CCLog("kkk gold = %lld", meg.gold());
    CCLog("kkk reqexp = %d", meg.reqexp());
    CCLog("kkk gridnum = %d", meg.gridnum());
    CCLog("kkk freenum = %d", meg.freenum());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_PLAYERINFO;
    tmpMeg.ackOGAckRoleMsg = meg;
    
    
    DataManager::sharedDataManager()->currLevel = meg.level();
    DataManager::sharedDataManager()->currGold = meg.gold();
    
    DataManager::sharedDataManager()->currExp = meg.exp();
    DataManager::sharedDataManager()->needExp = meg.reqexp();
    
    if(DataManager::sharedDataManager()->isCanMangerGoldJP)
    {
        DataManager::sharedDataManager()->isCanMangerGoldJP = false;
        DataManager::sharedDataManager()->currGold -= DataManager::sharedDataManager()->needGoldJP;
    }
    
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onPetList(const char* message, int size)
{
    OGAckPetList meg;
    meg.ParseFromArray(message, size);
    
    if(meg.petlist_size() == 0)
    {
        CCLog("~~~~~~~~in OGAckPetList ListSize is 0~~~~~~~~~~");
        
    }
    else
    {
        CCLog("kkk petname = %s", meg.petlist(0).petname().c_str());
        CCLog("kkk pinzhi = %d", meg.petlist(0).pinzhi());
        CCLog("kkk blood = %d", meg.petlist(0).blood());
    }
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_PETLIST;
    tmpMeg.ackOGAckPetList = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onExitGame(const char* message, int size)
{
    OGAckExit meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk msgtype = %d", meg.msgtype());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_CLOSEMODULE;
    tmpMeg.ackOGAckExit = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onPetReset(const char* message, int size)
{
    OGAckPetReSet meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk petid = %d", meg.petid());
    CCLog("kkk blood = %d", meg.blood());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_PETRESET;
    tmpMeg.ackOGAckPetReSet = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckJP(const char* message, int size)
{
    OGAckJP meg;
    meg.ParseFromArray(message, size);
//    CCLog("kkk jp = %lld", meg.jp());
    
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_JACKPOT;
    tmpMeg.ackOGAckJP = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckMessage(const char* message, int size)
{
    OGAckMessage meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk userid = %s", meg.userid().c_str());
    CCLog("kkk rolename = %s", meg.rolename().c_str());
    CCLog("kkk content = %s", meg.content().c_str());
    
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_BORADCAST;
    tmpMeg.ackOGAckMessage = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
    
}

void GameLogicSocket::onOGAckPetGridOnList(const char* message, int size)
{
    OGAckPetGridOnList meg;
    meg.ParseFromArray(message, size);
    
    if(meg.petgridlist_size() == 0)
    {
        CCLog("~~~~~~~~in OGAckPetGridOnList ListSize is 0~~~~~~~~~~");
    }
    else
    {
        CCLog("kkk id = %d", meg.petgridlist(0).id());
        CCLog("kkk grid = %d", meg.petgridlist(0).grid());
        CCLog("kkk tips = %s", meg.petgridlist(0).tips().c_str());
    }
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_PETPACK;
    tmpMeg.ackOGAckPetGridOnList = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckUnreadMail(const char* message, int size)
{
    OGAckUnreadMail meg;
    meg.ParseFromArray(message, size);
    CCLog("有未读邮件！！！");
    
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKNEWMAIL;
    tmpMeg.ackOGAckUnreadMail = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckItems(const char* message, int size)
{
    OGAckItems meg;
    meg.ParseFromArray(message, size);
    if(meg.itemlist_size() == 0)
    {
        CCLog("no Item!!!!!");
    }
    else
    {
        CCLog("kkk id = %d", meg.itemlist(0).id());
        CCLog("kkk name = %s", meg.itemlist(0).name().c_str());
        CCLog("kkk tip = %s", meg.itemlist(0).tip().c_str());
        CCLog("kkk type = %d", meg.itemlist(0).type());
    }
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKITEMS;
    tmpMeg.ackOGAckItems = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckDoublePlus(const char* message, int size)
{
    OGAckDoublePlus meg;
    meg.ParseFromArray(message, size);
    
    if(meg.doublepluslist_size() == 0)
    {
        CCLog("~~~~~~~~in OGAckDoublePlus ListSize is 0~~~~~~~~~~");
    }
    else
    {
        CCLog("kkk startlev = %d", meg.doublepluslist(0).startlev());
        CCLog("kkk plus = %d", meg.doublepluslist(0).plus());
    }
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKDOUBLEPLUS;
    tmpMeg.ackOGAckDoublePlus = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckLoginAwardList(const char* message, int size)
{
    OGAckLoginAwardList meg;
    meg.ParseFromArray(message, size);
    
    
    if(meg.awardlist_size() == 0)
    {
        CCLog("~~~~~~~~in OGAckLoginAwardList ListSize is 0~~~~~~~~~~");
    }
    else
    {
        CCLog("kkk content = %s", meg.awardlist(0).content().c_str());
        CCLog("kkk num = %d", meg.awardlist(0).num());
        CCLog("kkk type = %d", meg.awardlist(0).type());
        CCLog("kkk rolename = %s", meg.rolename().c_str());
    }
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKLOGINREWARD;
    tmpMeg.ackOGAckLoginAwardList = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckJpStart(const char* message, int size)
{
    OGAckJpStart meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk rolename = %s", meg.rolename().c_str());
    CCLog("kkk jpstarname = %s", meg.jpstarname().c_str());
    CCLog("kkk jpstartemnum = %lld", meg.jpstartemnum());
    
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKJPSTAR;
    tmpMeg.ackOGAckJpStart = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckRank(const char* message, int size)
{
    OGAckRank meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk crankname = %s", meg.crankname().c_str());
    CCLog("kkk rolename = %s", meg.rolename().c_str());
    CCLog("kkk bgold = %lld", meg.bgold());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_RANKINGDATA;
    tmpMeg.ackOGAckRank = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckShopItemInfo(const char* message, int size)
{
    OGAckShopItemInfo meg;
    meg.ParseFromArray(message, size);
    
    if(meg.shoplist_size() == 0)
    {
        CCLog("~~~~~~~~in OGAckShopItemInfo ListSize is 0~~~~~~~~~~");
    }
    else
    {
        CCLog("kkk itemname = %s", meg.shoplist(0).itemname().c_str());
        CCLog("kkk itemid = %d", meg.shoplist(0).itemid());
        CCLog("kkk shopdesc = %s", meg.shoplist(0).shopdesc().c_str());
    }
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKSHOPLIST;
    tmpMeg.ackOGAckShopItemInfo = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckLoginGetAward(const char* message, int size)
{
    OGAckLoginGetAward meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk pos = %d", meg.pos());
    CCLog("kkk pic = %s", meg.pic().c_str());
    CCLog("kkk content = %s", meg.content().c_str());
    
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKLOGINAWARD;
    tmpMeg.ackOGAckLoginGetAward = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckBindPhone(const char* message, int size)
{
    OGAckBindPhone meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk result = %d", meg.result());
    CCLog("kkk phone = %s", meg.phone().c_str());
    
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKBINDPHONE;
    tmpMeg.ackOGAckBindPhone = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckGetPhoneCode(const char* message, int size)
{
    OGAckGetPhoneCode meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk msg = %s", meg.msg().c_str());
    
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKGETCODE;
    tmpMeg.ackOGAckGetPhoneCode = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}


void GameLogicSocket::onOGAckMails(const char* message, int size)
{
    OGAckMails meg;
    meg.ParseFromArray(message, size);
    
    if(meg.maillist_size() == 0)
    {
        CCLog("~~~~~~~~in OGAckMails ListSize is 0~~~~~~~~~~");
    }
    else
    {
        CCLog("kkk itemname = %s", meg.maillist(0).itemname().c_str());
        CCLog("kkk sendrolename = %s", meg.maillist(0).sendrolename().c_str());
        CCLog("kkk recvrolename = %s", meg.maillist(0).recvrolename().c_str());
    }
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKMAILS;
    tmpMeg.ackOGAckMails = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckGameResult(const char* message, int size)
{
    OGAckGameResult meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk grid1 = %d", meg.grid1());
    CCLog("kkk grid2 = %d", meg.grid2());
    CCLog("kkk grid3 = %d", meg.grid3());
    CCLog("kkk result = %d", meg.result());
    CCLog("kkk wingold = %lld", meg.wingold());
    
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_GAMERESULT;
    tmpMeg.ackOGAckGameResult = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);

   
#if (OPEN_DEBUG == 1)//比倍
    
    this->sendReqDoubleGetGold();
    
#endif
    
}

void GameLogicSocket::onOGAckJpRecord(const char* message, int size)
{
    OGAckJpRecord meg;
    meg.ParseFromArray(message, size);
    
    if(meg.jprecordlist_size() == 0)
    {
        CCLog("~~~~~~~~in OGAckJpRecord ListSize is 0~~~~~~~~~~");
    }
    else
    {
        CCLog("kkk id = %s", meg.jprecordlist(0).userid().c_str());
        CCLog("kkk username = %s", meg.jprecordlist(0).rolename().c_str());
        CCLog("kkk gold = %lld", meg.jprecordlist(0).wingold());
        CCLog("kkk type = %d", meg.jprecordlist(0).showtype());
    }

    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ACKJACKPOTLIST;
    tmpMeg.ackOGAckJpRecord = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckBoss(const char* message, int size)
{
    OGAckBoss meg;
    meg.ParseFromArray(message, size);
    
    CCLog("kkk bossid = %d", meg.bossid());
    CCLog("kkk bossblood = %d", meg.bossblood());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_BOSSINFO;
    tmpMeg.ackOGAckBoss = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckJpHall(const char* message, int size)
{
    OGAckJpHall meg;
    meg.ParseFromArray(message, size);
    
    if(meg.contentlist_size() == 0)
    {
        CCLog("~~~~~~~~in onOGAckJpHall ListSize is 0~~~~~~~~~~");
    }
    else
    {
        CCLog("kkk id = %s", meg.contentlist(0).name().c_str());
    }
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = ACK_SEVER_JPHALL;
    tmpMeg.ackOGAckJpHall = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckDoubleGetGold(const char* message, int size)
{
    OGAckDoubleGetGold meg;
    meg.ParseFromArray(message, size);
    
    CCLog("kkk name = %s", meg.rolename().c_str());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_COLLECTION;
    tmpMeg.ackOGAckDoubleGetGold = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckPetUpdatedMsg(const char* message, int size)
{
    OGAckPetUpdatedMsg meg;
    meg.ParseFromArray(message, size);
    
    CCLog("kkk name = %d", meg.petid());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_PETUPDATEDMSG;
    tmpMeg.ackOGAckPetUpdatedMsg = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckPetFightResult(const char* message, int size)
{
    OGAckPetFightResult meg;
    meg.ParseFromArray(message, size);
    
    CCLog("kkk page = %d", meg.page());
    CCLog("kkk petjoinnum = %d", meg.petjoinnum());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_FIGHTRESULT;
    tmpMeg.ackOGAckPetFightResult = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckDoubleResult(const char* message, int size)
{
    OGAckDoubleResult meg;
    meg.ParseFromArray(message, size);
    
    CCLog("kkk ret = %d", meg.result());
    CCLog("kkk basegold = %lld", meg.wingold());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_DOUBLEGAME;
    tmpMeg.ackOGAckDoubleResult = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckPetGetAward(const char* message, int size)
{
    OGAckPetGetAward meg;
    meg.ParseFromArray(message, size);
    
    if(meg.awardlist_size() == 0)
    {
        CCLog("~~~~~~~~in onOGAckPetGetAward ListSize is 0~~~~~~~~~~");
    }
    else
    {
        CCLog("kkk id = %d", meg.awardlist(0).type());
    }
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_FIGHTREWARD;
    tmpMeg.ackOGAckPetGetAward = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckDoubleJpResult(const char* message, int size)
{
    OGAckDoubleJpResult meg;
    meg.ParseFromArray(message, size);
    
    CCLog("kkk id = %lld", meg.jp());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_DOUBLEJP;
    tmpMeg.ackOGAckDoubleJpResult = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckStopGame(const char* message, int size)
{
    OGAckStopGame meg;
    meg.ParseFromArray(message, size);
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_STOPGAME;
    tmpMeg.ackOGAckStopGame = meg;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onOGAckCutGame(const char* message, int size)
{
    OGAckLeaveGame meg;
    meg.ParseFromArray(message, size);
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_CUT_LINE;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
}

void GameLogicSocket::onLogin2(const char* message, int size)
{
    OGAckRoleLoginMsg meg;
    meg.ParseFromArray(message, size);
    CCLog("kkk result = %d", meg.result());
    CCLog("kkk reason = %d", meg.reason());
    CCLog("kkk newgold = %lld", meg.newgold());
    CCLog("kkk gold = %lld", meg.gold());
    
    GameLogicMegFromSever tmpMeg;
    tmpMeg.m_id = OGID_TEXAS_SLOTS_ROLEMSG;
    tmpMeg.ackLoginMeg = meg;
    
    //共享登入返回信息，S+2数据，防止切换界面时数据丢失
    DataManager::sharedDataManager()->AckRoleLoginMsg = meg;
    DataManager::sharedDataManager()->currLevel = meg.level();
    DataManager::sharedDataManager()->currGold = meg.gold();
    DataManager::sharedDataManager()->currFreeNum = meg.freenum();
    
    //自动发送rank请求
    GameLogicMeg2Sever tmpMeg0;
    tmpMeg0.m_id = OGID_TEXAS_SLOTS_REQRANKING;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg0);
    //自动发送jp请求
    GameLogicMeg2Sever tmpMeg1;
    tmpMeg1.m_id = AISLE_TEXAS_SLOTS_JACKPOTLIST;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
    //自动发送邮件请求
    GameLogicMeg2Sever tmpMeg3;
    tmpMeg3.m_id = OGID_TEXAS_SLOTS_REQMAILS;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg3);
    
    //启动心跳
    GameLogicMeg2Sever tmpMeg4;
    tmpMeg4.m_id = OGID_TEXAS_SLOTS_IDLE;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg4);
    
    //向UI发送结果
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_MEG_FROM_SEVER, &tmpMeg);
    
    
#if (OPEN_DEBUG == 1)
    
//    GameLogicMeg2Sever tmpMeg122;
//    tmpMeg122.m_id = OGID_TEXAS_SLOTS_REQRANKING;
//    
//    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg122);
    
#endif
}

void GameLogicSocket::onReceiveData(int messageID, const char* message, int size)
{
	//不能断开，断开后再连会需要时间，这段时间send不出去
//    disconnect();
    CCLog("-------------------------------------------");
    CCLog("this megID is=%d", messageID);
    
	switch (messageID)
    {
        case OGID_TEXAS_SLOTS_ROLEMSG://
        {
            onLogin2(message, size);
            break;
		}
        case OGID_TEXAS_SLOTS_CLOSEMODULE://
        {
            onExitGame(message, size);
            break;
		}
        case OGID_TEXAS_SLOTS_ROOMLIST://
        {
            onRoomList(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ROOMSITE://
        {
            onRoomSite(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_PLAYERINFO://
        {
            onPlayerInfo(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_PETLIST://
        {
            onPetList(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_PETRESET://
        {
            onPetReset(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_JACKPOT://
        {
            onOGAckJP(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_BORADCAST://
        {
            onOGAckMessage(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_PETPACK://
        {
            onOGAckPetGridOnList(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKNEWMAIL://
        {
            onOGAckUnreadMail(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKITEMS://
        {
            onOGAckItems(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKDOUBLEPLUS://
        {
            onOGAckDoublePlus(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKLOGINREWARD://
        {
            onOGAckLoginAwardList(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKJPSTAR://
        {
            onOGAckJpStart(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_RANKINGDATA://
        {
            onOGAckRank(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKMAILS://
        {
            onOGAckMails(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKSHOPLIST://
        {
            onOGAckShopItemInfo(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKLOGINAWARD://
        {
            onOGAckLoginGetAward(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKBINDPHONE://
        {
            onOGAckBindPhone(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKGETCODE://
        {
            onOGAckGetPhoneCode(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_GAMERESULT://
        {
            onOGAckGameResult(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_ACKJACKPOTLIST://
        {
            onOGAckJpRecord(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_BOSSINFO:
        {
            onOGAckBoss(message, size);
            break;
        }
        case ACK_SEVER_JPHALL:
        {
            onOGAckJpHall(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_COLLECTION:
        {
            onOGAckDoubleGetGold(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_PETUPDATEDMSG:
        {
            onOGAckPetUpdatedMsg(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_FIGHTRESULT:
        {
            onOGAckPetFightResult(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_DOUBLEGAME:
        {
            onOGAckDoubleResult(message, size);
            CCLog("比倍界面 返回消息～～～");
            break;
        }
        case OGID_TEXAS_SLOTS_FIGHTREWARD:
        {
            onOGAckPetGetAward(message, size);
            break;
        }
        case OGID_TEXAS_SLOTS_DOUBLEJP:
        {
            onOGAckDoubleJpResult(message, size);
            CCLog("比倍界面 JP巨奖消息～～～");
            break;
        }
        case OGID_TEXAS_SLOTS_STOPGAME:
        {
            onOGAckStopGame(message, size);
            
            CCLog("止损～～～～～～～");
            break;
        }
        case OGID_TEXAS_SLOTS_CUT_LINE:
        {
            CCLog("断网");
        }
        default:
        {
            CCLog("~~~~~GameLogicSocket Undefine Message %d",messageID);
			break;
        }
	}
    
}
