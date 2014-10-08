//
//  ControllerMeg.cpp
//  Slot
//
//  Created by zhengyu on 14-8-22.
//
//

#include "ControllerMeg.h"

ControllerMeg::ControllerMeg()
{

    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                 callfuncO_selector(ControllerMeg::recLoginEventFromUI),
                                                                  EVENT_LOGIN2MEG,
                                                                  NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(ControllerMeg::recRegEventFromUI),
                                                                  EVENT_REG2MEG,
                                                                  NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(ControllerMeg::sendOGReqTRLogin1),
                                                                  EVENT_LINKGAME2MSG,
                                                                  NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(ControllerMeg::conectGameSocket),
                                                                  EVENT_CONECT_GAME,
                                                                  NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(ControllerMeg::sendLinkGame),
                                                                  EVENT_SENDLINK_GAME,
                                                                  NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(ControllerMeg::sendMeg2Sever),
                                                                  EVENT_SEND_MEG2SEVER,
                                                                  NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(ControllerMeg::onMegFromSever),
                                                                  EVENT_REC_MEG_FROM_SEVER,
                                                                  NULL);
    
    
    m_loginSocket = new LoginSocket();
    
#if (OPEN_SOCKET_OR_HTTP == 0)
    m_loginSocket->connect(LOGIN_IP, LOGIN_PORT);
#endif
    
    m_linkGameSocket = new LinkGameSocket();
    m_linkGameSocket->connect(LOGIN_GAME_IP, LOGIN_GAME_PORT);
    
    m_gameLogicSocket = new GameLogicSocket();

}

ControllerMeg::~ControllerMeg()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ControllerMeg::sendIdle),
                                                                     this);
    
    if(m_loginSocket != NULL)
        delete m_loginSocket;
    
    if(m_linkGameSocket != NULL)
        delete m_linkGameSocket;
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_LOGIN2MEG);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_REG2MEG);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_LINKGAME2MSG);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_CONECT_GAME);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_SENDLINK_GAME);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_SEND_MEG2SEVER);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_REC_MEG_FROM_SEVER);
    
}

void ControllerMeg::recLoginEventFromUI(CCObject * obj)
{
    
    std::string strUsrName = ((LoginMeg*)obj)->name;
    std::string strPsw = ((LoginMeg*)obj)->pwd;
    
    m_loginSocket->sendLogin(strUsrName.c_str(),
                             strPsw.c_str(),
                             m_loginSocket->m_tlogconfig.logtype,
                             m_loginSocket->m_tlogconfig.logthirdName,
                             m_loginSocket->m_tlogconfig.logthirdKey,
                             m_loginSocket->m_tlogconfig.logthirdAppID);
}

void ControllerMeg::recRegEventFromUI(CCObject * obj)
{
    
    std::string strUsrName = ((RegMeg*)obj)->name;
    std::string strPsw = ((RegMeg*)obj)->pwd;
    m_loginSocket->sendReg(strUsrName.c_str(), strPsw.c_str());
}

bool ControllerMeg::sendOGReqTRLogin1(CCObject * obj)
{
    com::ourgame::texasSlots::OGReqTRLogin1 login;
    login = ((LoginMeg2*)obj)->login1;
    CCLog("kkk Event usrID = %s", login.userid().c_str());
    
    return m_linkGameSocket->send(&login, OGID_TEXAS_SLOTS_SERVER);
}

void ControllerMeg::conectGameSocket(CCObject * obj)
{
    std::string ip = ((ConectGameSocket*)obj)->ackLinkSever.linkip();
    int port = ((ConectGameSocket*)obj)->ackLinkSever.linkport();
    
    m_gameLogicSocket->connect(ip.c_str(), port);
    CCLog("kkk Event Link IP Port");
    
    //向服务器发票阵信息
    sleep(1);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SENDLINK_GAME, NULL);

}

void ControllerMeg::sendLinkGame(CCObject * obj)
{
    
    CCLog("kkk Event send Link Game");
    std::string userid = DataManager::sharedDataManager()->userid;
    std::string cert = DataManager::sharedDataManager()->cert;
    int gameid = DataManager::sharedDataManager()->gameid;
    int comfromid = DataManager::sharedDataManager()->comfromid;
    std::string entrance = DataManager::sharedDataManager()->entrance;
    std::string nickname = DataManager::sharedDataManager()->nickname;
    std::string username = DataManager::sharedDataManager()->username;
    
    
//    cert = "CAEQyOrUoAUaATAi0AIG0wlwXFoK9jzZMNA1hY4MpeI2MtqZkhjtexb2AtFBlRqehWI0fyb1CxqoyXLwuLUtX6KdhaIVRpJWnOvjaT/9RZ5Yoy9d1bu3hTKBUrhxR8Ys4dyfQmYeC3XiKcCvFS84N5LcyjLSvoc4EzTtgDFS0JWKuCIIR/yaD25%2BLiYyUBLwPZcrFbZL8KdOr0EnmppKZDadgoRTVs55lvTI/xEMXts9Si2bS7p0djC5JbpLeA/x8GhkOabvZrICxzbSbE4NT3s4Qi/pe0bGvHJe4HObyr4ysUqunIoVgjTSpZUjj54UcTxeRTRLLIsBVnJDawAY6Ik/vXsiom2MSOQqX2CBYUXQFgfGNSY0FKB939PtnLzfl1J4gTiaznmW9Mj/EQyMztFmXTsz/Itg%2BPcNG91vznmW9Mj/EQyFZkyRlasDGUes9nOrU0NOMyZSZ1WcSuc=";//test
    
    //加密
    std::string cerstring;
//    ZBase64 zBase;
    //网络库
//    cerstring = zBase.Encode(cert.c_str(), cert.length());
//    CCLog("EncodeBase64(0)=%s", cerstring.c_str());
    
    //德州库
//    unsigned long uInlen =cert.length();
//    const char *p = cert.c_str();
//    unsigned char * pIn = (unsigned char*)p;
//    zBase.Encode(pIn, uInlen, cerstring);
   
    //不加密
    cerstring = cert;
    
    CCLog("EncodeBase64(1)=%s", cerstring.c_str());
    
//    com::ourgame::texasSlots::OGReqTRLogin2 login2;
//    login2.set_cert(cerstring);
//    login2.set_gameid(gameid);
//    login2.set_comfromid(comfromid);
//    login2.set_entrance(entrance);
//    login2.set_nickname(nickname);
//    login2.set_username(username);
//
//    m_gameLogicSocket->send(&login2, SEND_MEG_ID4LOGIN2);
    
    m_gameLogicSocket->sendLogin(userid,
                                 cerstring,
                                 gameid,
                                 comfromid,
                                 username,
                                 nickname,
                                 entrance);
    
    OutTime::sharedDataManager()->create(OGID_TEXAS_SLOTS_LOGIN);
}

void ControllerMeg::sendIdle()
{
    m_gameLogicSocket->sendIdle();
    CCLog("idle~~~");
}

void ControllerMeg::sendMeg2Sever(CCObject * obj)
{
    GameLogicMeg2Sever * tmeg = ((GameLogicMeg2Sever*)obj);
    int tid = tmeg->m_id;
    
    CCLog("===========================================");
    CCLog("EVENT in sendMeg2Sever id=%d", tid);
    
    tmeg->roleName = DataManager::sharedDataManager()->userid;
    
    OutTime::sharedDataManager()->create(tmeg->m_id);
    
    switch (tid)
    {
        case CREATE_ROLE_EVENT:
        {
            m_loginSocket->sendCreateRole(tmeg->CuserName.c_str(), tmeg->CroleName.c_str(), tmeg->Cmaleid);
            break;
        }
        case OGID_TEXAS_SLOTS_IDLE:
        {
//            m_gameLogicSocket->sendIdle();
            
//            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ControllerMeg::sendIdle),
//                                                                           this,
//                                                                           2.0f,
//                                                                           false);
            break;
        }
        case OGID_TEXAS_SLOTS_JOINROOM:
        {
            int roomid = tmeg->roomid;
            m_gameLogicSocket->sendJoinRoom(roomid);
            break;
        }
        case OGID_TEXAS_SLOTS_EXITGAME:
        {
            int roomid = tmeg->msgtyoe;
            m_gameLogicSocket->sendExitGame(roomid);
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETLIST:
        {
            m_gameLogicSocket->sendReqPetList();
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETRESET:
        {
            int petid = tmeg->petid;
            m_gameLogicSocket->sendReqPetReset(petid);
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETFIGHT:
        {
            int petid = tmeg->petid;
            m_gameLogicSocket->sendReqPetFight(petid);
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETPACK:
        {
            m_gameLogicSocket->sendPetPack();
            break;
        }
        case OGID_TEXAS_SLOTS_REQRANKING:
        {
            std::string rolename = tmeg->roleName;
            m_gameLogicSocket->sendRanking(rolename);
            break;
        }
        case OGID_TEXAS_SLOTS_REQMAILS:
        {
            m_gameLogicSocket->sendMails();
            break;
        }
        case OGID_TEXAS_SLOTS_REQMAILSTATE:
        {
            int type = tmeg->mailtype;
            std::string id = tmeg->mailid;
            m_gameLogicSocket->sendMailsState(type, id);
            break;
        }
        case OGID_TEXAS_SLOTS_PETRESOLVE:
        {
            int petid = tmeg->petid;
            m_gameLogicSocket->sendPetResolve(petid);
            break;
        }
        case OGID_TEXAS_SLOTS_GUIDEOVER:
        {
            m_gameLogicSocket->sendGuildOver();
            break;
        }
        case OGID_TEXAS_SLOTS_REQAWARD:
        {
            m_gameLogicSocket->sendReqAward();
            break;
        }
        case OGID_TEXAS_SLOTS_REQSHOPITEM:
        {
            int itemId = tmeg->itemId;
            m_gameLogicSocket->sendReqShopItem(itemId);
            break;
        }
        case OGID_TEXAS_SLOTS_REQBUYITEM:
        {
            int itemId = tmeg->itemId;
            int num = tmeg->num;
            m_gameLogicSocket->sendReqBuyItem(itemId, num);
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETCOMPOSE:
        {
            int debrisItemId = tmeg->debrisItemId;
            int evolveId = tmeg->evolveId;
            m_gameLogicSocket->sendReqPetCompose(debrisItemId, evolveId);
            break;
        }
        case OGID_TEXAS_SLOTS_REQCOMPOSE:
        {
            m_gameLogicSocket->sendReqCompse();
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETRESOLVESCHEME:
        {
            m_gameLogicSocket->sendReqPetResolveScheme();
            break;
        }
        case OGID_TEXAS_SLOTS_REQLOGINAWARD:
        {
            std::string roleName = tmeg->roleName;
            int lev = tmeg->lev;
            m_gameLogicSocket->sendReqLoginAward(roleName, lev);
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETRETURNAWARD:
        {
            int action = tmeg->action;
            m_gameLogicSocket->sendReqPetReturnAward(action);//1喂食 2 抚摸
            break;
        }
        case OGID_TEXAS_SLOTS_BINDPHONE:
        {
            m_gameLogicSocket->sendBindPhone();
            break;
        }
        case OGID_TEXAS_SLOTS_GETCODE:
        {
            std::string phone = tmeg->phone;
            m_gameLogicSocket->sendGetCode(phone);
            break;
        }
        case OGID_TEXAS_SLOTS_GETCODEOK:
        {
            int code = tmeg->code;
            m_gameLogicSocket->sendGetCodeOK(code);
            break;
        }
        case OGID_TEXAS_SLOTS_REQGAMESTRT:
        {
            // 押注线数 // 押注倍数// 自动
            int line = tmeg->line;
            int times = tmeg->times;
            bool autop = tmeg->autop;
            m_gameLogicSocket->sendReqStartGame(line, times, autop);
            
            break;
        }
        case OGID_TEXAS_SLOTS_REQDOUBLEGAME:
        {
            int times = tmeg->times;// 押注倍数
            int colour = tmeg->colour;//押注颜色
            m_gameLogicSocket->sendReqDoubleGame(times, colour);
            break;
        }
        case OGID_TEXAS_SLOTS_REQDOUBLEGETGOLD:
        {
            m_gameLogicSocket->sendReqDoubleGetGold();
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETFIGHT2:
        {
            m_gameLogicSocket->sendReqPetFight();
            break;
        }
        case OGID_TEXAS_SLOTS_REQPETFLEE:
        {
            m_gameLogicSocket->sendReqPetFlee();
            break;
        }
        case OGID_TEXAS_SLOTS_REQFIGHTAWARD:
        {
            m_gameLogicSocket->sendReqFightAward();
            break;
        }
        case AISLE_TEXAS_SLOTS_JACKPOTLIST:
        {
            string roleName  = tmeg->roleName;
            m_gameLogicSocket->sendReqJpRecord(roleName);
            break;
        }
        case 0:
        {
            //m_gameLogicSocket->sendReqFightAward();
            break;
        }
        default:
        {
            CCLog("No this Message ID!!!!!");
            break;
        };
    }
    
    CCLog("===========================================");
    
}

void ControllerMeg::onMegFromSever(CCObject * obj)
{
    GameLogicMegFromSever * tmeg = ((GameLogicMegFromSever*)obj);
    int tid = tmeg->m_id;
    Meg2UIDate sendmeg;
    sendmeg.m_id = tid;
    
    CCLog("++++++++++++++++++++++++++++++++++");
    CCLog("EVENT in recMegFromSever id=%d", tid);
    
    switch (tid)
    {
        case OGID_TEXAS_SLOTS_ROLEMSG://
        {
            CCLog("OGID_TEXAS_SLOTS_ROLEMSG");
            break;
		}
        case OGID_TEXAS_SLOTS_CLOSEMODULE://
        {
            CCLog("OGID_TEXAS_SLOTS_CLOSEMODULE");
            break;
		}
        case OGID_TEXAS_SLOTS_ROOMLIST:
        {
            CCLog("OGID_TEXAS_SLOTS_ROOMLIST");
            
            OutTime::sharedDataManager()->release(OGID_TEXAS_SLOTS_LOGIN);
            
            sendmeg.mroomList.clear();
            MyRoomList tmplist;
            
            for(int i = 0; i < tmeg->ackOGAckRoomList.roomlist_size(); i++)
            {
                tmplist.roomid = tmeg->ackOGAckRoomList.roomlist(i).roomid();
                tmplist.roomName = tmeg->ackOGAckRoomList.roomlist(i).roomname();
                tmplist.roomLevelLimt = tmeg->ackOGAckRoomList.roomlist(i).roomlevel();
                tmplist.roomGoldLimt = tmeg->ackOGAckRoomList.roomlist(i).roomgold();
                tmplist.roomOn = tmeg->ackOGAckRoomList.roomlist(i).roomon();
                
                sendmeg.mroomList.push_back(tmplist);
                
            }
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            break;
        }
        case OGID_TEXAS_SLOTS_ROOMSITE:
        {
            CCLog("OGID_TEXAS_SLOTS_ROOMSITE");
            
            DataManager::sharedDataManager()->ret = tmeg->ackOGAckRoom.result();
            DataManager::sharedDataManager()->err = tmeg->ackOGAckRoom.reason();
            DataManager::sharedDataManager()->roomID = tmeg->ackOGAckRoom.roomid();
            DataManager::sharedDataManager()->roomName = tmeg->ackOGAckRoom.roomname();
            DataManager::sharedDataManager()->maxLine = tmeg->ackOGAckRoom.roommaxline();
            DataManager::sharedDataManager()->maxBei = tmeg->ackOGAckRoom.roommaxtimes();
            DataManager::sharedDataManager()->roomBaseGold = tmeg->ackOGAckRoom.roombasegold();
            DataManager::sharedDataManager()->roomPic = tmeg->ackOGAckRoom.roompic();
            DataManager::sharedDataManager()->roomDoubleOn = tmeg->ackOGAckRoom.roomdoubleon();
            DataManager::sharedDataManager()->freeOn = tmeg->ackOGAckRoom.freeon();
            DataManager::sharedDataManager()->fourTimesOn = tmeg->ackOGAckRoom.fourtimeson();
            
            SRoomBet troomBet;
            DataManager::sharedDataManager()->roomBetList.clear();
            
            for(int i = 0; i < tmeg->ackOGAckRoom.betlist_size(); i++)
            {
                troomBet.min = tmeg->ackOGAckRoom.betlist(i).min();
                troomBet.max = tmeg->ackOGAckRoom.betlist(i).max();
                troomBet.bet = tmeg->ackOGAckRoom.betlist(i).bet();
                DataManager::sharedDataManager()->roomBetList.push_back(troomBet);
                CCLog("--------min%d=%lld", i, troomBet.min);
                CCLog("--------max%d=%lld", i, troomBet.max);
                CCLog("--------bet%d=%d", i, troomBet.bet);
            }
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            
            break;
        }
        case OGID_TEXAS_SLOTS_PLAYERINFO:
        {
            CCLog("OGID_TEXAS_SLOTS_PLAYERINFO");
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            break;
        }
        case OGID_TEXAS_SLOTS_PETLIST:
        {
            CCLog("OGID_TEXAS_SLOTS_PETLIST");
            
            sendmeg.mPetList.clear();
            MyPetList tmplist;
            
            for(int i = 0; i < tmeg->ackOGAckPetList.petlist_size(); i++)
            {
                //如果为参战状态1
                if(1 == tmeg->ackOGAckPetList.petlist(i).status())
                {
                    //参战宠物攻击
                    if(tmeg->ackOGAckPetList.petlist(i).status() == 1)
                    {
                        DataManager::sharedDataManager()->petattack = tmeg->ackOGAckPetList.petlist(i).attack() + tmeg->ackOGAckPetList.petlist(i).attackplus();
                        
                        DataManager::sharedDataManager()->petblood = tmeg->ackOGAckPetList.petlist(i).blood() + tmeg->ackOGAckPetList.petlist(i).bloodplus();
                    }
                    
                    sendmeg.petLv = tmeg->ackOGAckPetList.petlist(i).level();
                    sendmeg.petPic = tmeg->ackOGAckPetList.petlist(i).petpic();
                    
                    tmplist.petName = tmeg->ackOGAckPetList.petlist(i).petname();
                    tmplist.petpinzhi = tmeg->ackOGAckPetList.petlist(i).pinzhi();
                    tmplist.petblood = tmeg->ackOGAckPetList.petlist(i).blood();
                    tmplist.petbloodMin = tmeg->ackOGAckPetList.petlist(i).bloodmin();
                    tmplist.petbloodMax = tmeg->ackOGAckPetList.petlist(i).bloodmax();
                    tmplist.petbloodPlus = tmeg->ackOGAckPetList.petlist(i).bloodplus();
                    tmplist.petattack = tmeg->ackOGAckPetList.petlist(i).attack();
                    tmplist.petattackMin = tmeg->ackOGAckPetList.petlist(i).attackmin();
                    tmplist.petattackMax = tmeg->ackOGAckPetList.petlist(i).attackmax();
                    tmplist.petattackPlus = tmeg->ackOGAckPetList.petlist(i).attackplus();
                    tmplist.petaward = tmeg->ackOGAckPetList.petlist(i).award();
                    tmplist.petawardMin = tmeg->ackOGAckPetList.petlist(i).awardmin();
                    tmplist.petawardMax = tmeg->ackOGAckPetList.petlist(i).awardmax();
                    tmplist.petawardPlus = tmeg->ackOGAckPetList.petlist(i).awardplus();
                    tmplist.petlevel = tmeg->ackOGAckPetList.petlist(i).level();
                    tmplist.petexp = tmeg->ackOGAckPetList.petlist(i).exp();
                    tmplist.petreqexp = tmeg->ackOGAckPetList.petlist(i).reqexp();
                    tmplist.petID = tmeg->ackOGAckPetList.petlist(i).petid();
                    tmplist.petstatus = tmeg->ackOGAckPetList.petlist(i).status();
                    tmplist.petPic = tmeg->ackOGAckPetList.petlist(i).petpic();
                    tmplist.petMaxLev = tmeg->ackOGAckPetList.petlist(i).maxlev();
                    
                    sendmeg.mPetList.push_back(tmplist);
                    
                    DataManager::sharedDataManager()->petID = tmeg->ackOGAckPetList.petlist(i).petid();
                    
                }
            }
            
            sendmeg.roleLv = DataManager::sharedDataManager()->AckRoleLoginMsg.level();
            sendmeg.roleName = DataManager::sharedDataManager()->userid;
            sendmeg.gold = DataManager::sharedDataManager()->AckRoleLoginMsg.gold();
            sendmeg.currExp = DataManager::sharedDataManager()->AckRoleLoginMsg.exp();
            sendmeg.needExp = DataManager::sharedDataManager()->AckRoleLoginMsg.reqexp();
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            
            break;
        }
        case OGID_TEXAS_SLOTS_PETRESET:
        {
            CCLog("OGID_TEXAS_SLOTS_PETRESET");
            break;
        }
        case OGID_TEXAS_SLOTS_JACKPOT:
        {
//            CCLog("OGID_TEXAS_SLOTS_JACKPOT");
            
            sendmeg.jp = tmeg->ackOGAckJP.jp();

            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            break;
        }
        case OGID_TEXAS_SLOTS_BORADCAST:
        {
            CCLog("OGID_TEXAS_SLOTS_BORADCAST");
            
            sendmeg.content = tmeg->ackOGAckMessage.content();
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            break;
        }
        case OGID_TEXAS_SLOTS_PETPACK:
        {
            CCLog("OGID_TEXAS_SLOTS_PETPACK");
            break;
        }
        case OGID_TEXAS_SLOTS_ACKNEWMAIL:
        {
            CCLog("OGID_TEXAS_SLOTS_ACKNEWMAIL");
            break;
        }
        case OGID_TEXAS_SLOTS_ACKITEMS:
        {
            CCLog("OGID_TEXAS_SLOTS_ACKITEMS");
            break;
        }
        case OGID_TEXAS_SLOTS_ACKDOUBLEPLUS:
        {
            CCLog("OGID_TEXAS_SLOTS_ACKDOUBLEPLUS");
            break;
        }
        case OGID_TEXAS_SLOTS_ACKLOGINREWARD:
        {
            CCLog("OGID_TEXAS_SLOTS_ACKLOGINREWARD");
            break;
        }
        case OGID_TEXAS_SLOTS_ACKJPSTAR:
        {
            CCLog("OGID_TEXAS_SLOTS_ACKJPSTAR");
            break;
        }
        case OGID_TEXAS_SLOTS_RANKINGDATA:
        {
            CCLog("OGID_TEXAS_SLOTS_RANKINGDATA");
            
            if(tmeg->ackOGAckRank.lastweeka_size() > 0)
            {
            
                sendmeg.rankName = tmeg->ackOGAckRank.lastweeka(0).rolename();
                sendmeg.rankGold = tmeg->ackOGAckRank.lastweeka(0).getgold();
                
                MyRankList tmyronkList;
                sendmeg.mrankListArr[0].clear();
                sendmeg.mrankListArr[1].clear();
                sendmeg.mrankListArr[2].clear();
                sendmeg.mrankListArr[3].clear();
                //初始化周一 排行数据
                for(int i = 0; i < tmeg->ackOGAckRank.ranklista_size(); i++)
                {
                    tmyronkList.rankGold = tmeg->ackOGAckRank.ranklista(i).rankid();
                    tmyronkList.rankName = tmeg->ackOGAckRank.ranklista(i).rolename();
                    tmyronkList.rankGold = tmeg->ackOGAckRank.ranklista(i).getgold();
                    
                    sendmeg.mrankListArr[0].push_back(tmyronkList);
                }
                
                //初始化周四 排行数据
                for(int i = 0; i < tmeg->ackOGAckRank.ranklistb_size(); i++)
                {
                    tmyronkList.rankGold = tmeg->ackOGAckRank.ranklistb(i).rankid();
                    tmyronkList.rankName = tmeg->ackOGAckRank.ranklistb(i).rolename();
                    tmyronkList.rankGold = tmeg->ackOGAckRank.ranklistb(i).getgold();
                    
                    sendmeg.mrankListArr[1].push_back(tmyronkList);
                }
                
                //初始化上周一 排行数据
                for(int i = 0; i < tmeg->ackOGAckRank.lastweeka_size(); i++)
                {
                    tmyronkList.rankGold = tmeg->ackOGAckRank.lastweeka(i).rankid();
                    tmyronkList.rankName = tmeg->ackOGAckRank.lastweeka(i).rolename();
                    tmyronkList.rankGold = tmeg->ackOGAckRank.lastweeka(i).getgold();
                    
                    sendmeg.mrankListArr[2].push_back(tmyronkList);
                }
                
                //初始化上周四 排行数据
                for(int i = 0; i < tmeg->ackOGAckRank.lastweekb_size(); i++)
                {
                    tmyronkList.rankGold = tmeg->ackOGAckRank.lastweekb(i).rankid();
                    tmyronkList.rankName = tmeg->ackOGAckRank.lastweekb(i).rolename();
                    tmyronkList.rankGold = tmeg->ackOGAckRank.lastweekb(i).getgold();
                    
                    sendmeg.mrankListArr[3].push_back(tmyronkList);
                }
                
                CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            
            }
            
            break;
        }
        case OGID_TEXAS_SLOTS_ACKMAILS:
        {
            CCLog("OGID_TEXAS_SLOTS_ACKMAILS");
            
            MyMailMeg tmail;
            sendmeg.mMailvector.clear();
            
            for(int i = 0; i < tmeg->ackOGAckMails.maillist_size(); i++)
            {
                tmail.mailContent = tmeg->ackOGAckMails.maillist(i).content();//mail内容
                tmail.mailTime = tmeg->ackOGAckMails.maillist(i).createtime();//创建时间
                tmail.mailTitle = tmeg->ackOGAckMails.maillist(i).mailtitle();//mail标题
                tmail.mailNum = tmeg->ackOGAckMails.maillist(i).itemnum();//数量
                tmail.mailType = tmeg->ackOGAckMails.maillist(i).awardtype();//奖励类型
                tmail.isRead = tmeg->ackOGAckMails.maillist(i).isread();//已读
                tmail.isOK = tmeg->ackOGAckMails.maillist(i).isok();//收取标记
                tmail.mailPic = tmeg->ackOGAckMails.maillist(i).pic();//图片路径
                char strID[30];
                sprintf(strID, "%d", tmeg->ackOGAckMails.maillist(i).id());
                tmail.mailid = strID;
                CCLog("maillll~~~~~~~%s", tmail.mailid.c_str());
                sendmeg.mMailvector.push_back(tmail);
            }
            
            
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            
            break;
        }
        case OGID_TEXAS_SLOTS_ACKSHOPLIST:
        {
            CCLog("OGID_TEXAS_SLOTS_ACKSHOPLIST");
            break;
        }
        case OGID_TEXAS_SLOTS_ACKLOGINAWARD:
        {
            CCLog("OGID_TEXAS_SLOTS_ACKLOGINAWARD");
            break;
        }
        case OGID_TEXAS_SLOTS_ACKBINDPHONE:
        {
            CCLog("OGID_TEXAS_SLOTS_ACKBINDPHONE");
            break;
        }
        case OGID_TEXAS_SLOTS_ACKGETCODE:
        {
            CCLog("OGID_TEXAS_SLOTS_ACKGETCODE");
            break;
        }
        case OGID_TEXAS_SLOTS_GAMERESULT:
        {
            CCLog("OGID_TEXAS_SLOTS_GAMERESULT");
            sendmeg.wingold = tmeg->ackOGAckGameResult.wingold();//中奖金额
            sendmeg.Rret = tmeg->ackOGAckGameResult.result();//中奖结果0-未中奖 1普通中奖 2jp大奖
            sendmeg.jplineNum = tmeg->ackOGAckGameResult.jplinenum();//中jp大奖时 的几连
            sendmeg.getRoleExp = tmeg->ackOGAckGameResult.winexp();//获得人物经验
            sendmeg.getPetExp = tmeg->ackOGAckGameResult.petwinexp();//获得宠物经验
            sendmeg.page = tmeg->ackOGAckGameResult.page();//界面1老虎机 2大厅 3比倍 4宠物战斗
            sendmeg.petWingold = tmeg->ackOGAckGameResult.petwinexp();//宠物获得额外大奖
            sendmeg.wrongmeg = tmeg->ackOGAckGameResult.wrongmsg();//错误消息
            sendmeg.freeZ = tmeg->ackOGAckGameResult.freeflg();//免费转动标记
            sendmeg.freeZgetGold = tmeg->ackOGAckGameResult.goldplus();//免费事件获得金币
            
            sendmeg.grid[0] = tmeg->ackOGAckGameResult.grid1();
            sendmeg.grid[1] = tmeg->ackOGAckGameResult.grid2();
            sendmeg.grid[2] = tmeg->ackOGAckGameResult.grid3();
            sendmeg.grid[3] = tmeg->ackOGAckGameResult.grid4();
            sendmeg.grid[4] = tmeg->ackOGAckGameResult.grid5();
            sendmeg.grid[5] = tmeg->ackOGAckGameResult.grid6();
            sendmeg.grid[6] = tmeg->ackOGAckGameResult.grid7();
            sendmeg.grid[7] = tmeg->ackOGAckGameResult.grid8();
            sendmeg.grid[8] = tmeg->ackOGAckGameResult.grid9();
            sendmeg.grid[9] = tmeg->ackOGAckGameResult.grid10();
            sendmeg.grid[10] = tmeg->ackOGAckGameResult.grid11();
            sendmeg.grid[11] = tmeg->ackOGAckGameResult.grid12();
            sendmeg.grid[12] = tmeg->ackOGAckGameResult.grid13();
            sendmeg.grid[13] = tmeg->ackOGAckGameResult.grid14();
            sendmeg.grid[14] = tmeg->ackOGAckGameResult.grid15();
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_K_REC_MEG_FROM_SEVER, &sendmeg);
            
            OutTime::sharedDataManager()->release(OGID_TEXAS_SLOTS_REQGAMESTRT);
            
            break;
        }
        case OGID_TEXAS_SLOTS_ACKJACKPOTLIST:
        {
            if(tmeg->ackOGAckJpRecord.jprecordlist_size() > 0)
            {
                CCLog("OGID_TEXAS_SLOTS_ACKJACKPOTLIST");
                sendmeg.jpStarName = tmeg->ackOGAckJpRecord.jprecordlist(0).gamename();
                sendmeg.jpStarNum = tmeg->ackOGAckJpRecord.jprecordlist(0).wingold();
                sendmeg.jpStarTime = tmeg->ackOGAckJpRecord.jprecordlist(0).createtime();
                
                MyJPRankList tmpJPRankList;
                sendmeg.mJPRankList[0].clear();
                sendmeg.mJPRankList[1].clear();
                
                for(int i = 0; i < tmeg->ackOGAckJpRecord.jprecordlist_size(); i++)
                {
//                CCLog("id=%s; rame=%s; wgold=%lld; currjp=%lld; showtp=%d; ganme=%s; createtime=%s;",
//                      tmeg->ackOGAckJpRecord.jprecordlist(i).userid().c_str(),
//                      tmeg->ackOGAckJpRecord.jprecordlist(i).rolename().c_str(),
//                      tmeg->ackOGAckJpRecord.jprecordlist(i).wingold(),
//                      tmeg->ackOGAckJpRecord.jprecordlist(i).currjp(),
//                      tmeg->ackOGAckJpRecord.jprecordlist(i).showtype(),
//                      tmeg->ackOGAckJpRecord.jprecordlist(i).gamename().c_str(),
//                      tmeg->ackOGAckJpRecord.jprecordlist(i).createtime().c_str());
                    
                    //jp明星巨奖
                    if(tmeg->ackOGAckJpRecord.jprecordlist(i).showtype() == 1)
                    {
                        tmpJPRankList.JPrankName = tmeg->ackOGAckJpRecord.jprecordlist(i).userid();
                        tmpJPRankList.JPrankwinGold = tmeg->ackOGAckJpRecord.jprecordlist(i).wingold();
                        tmpJPRankList.JPrankcurrGold = tmeg->ackOGAckJpRecord.jprecordlist(i).currjp();
                        tmpJPRankList.JPrankTime = tmeg->ackOGAckJpRecord.jprecordlist(i).createtime();
                        sendmeg.mJPRankList[0].push_back(tmpJPRankList);
                    }
                    //jp巨奖
                    else
                    {
                        tmpJPRankList.JPrankName = tmeg->ackOGAckJpRecord.jprecordlist(i).userid();
                        tmpJPRankList.JPrankwinGold = tmeg->ackOGAckJpRecord.jprecordlist(i).wingold();
                        tmpJPRankList.JPrankcurrGold = tmeg->ackOGAckJpRecord.jprecordlist(i).currjp();
                        tmpJPRankList.JPrankTime = tmeg->ackOGAckJpRecord.jprecordlist(i).createtime();
                        sendmeg.mJPRankList[1].push_back(tmpJPRankList);
                    }
                }
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);

            }
            
            break;
        }
        case OGID_TEXAS_SLOTS_BOSSINFO:
        {
            CCLog("OGID_TEXAS_SLOTS_BOSSINFO");
            DataManager::sharedDataManager()->boosID = tmeg->ackOGAckBoss.bossid();
            DataManager::sharedDataManager()->bossNeedGold = tmeg->ackOGAckBoss.bossneedgold();
            DataManager::sharedDataManager()->roomPic = tmeg->ackOGAckBoss.roompic();
            DataManager::sharedDataManager()->bossPic = tmeg->ackOGAckBoss.bosspic();
            DataManager::sharedDataManager()->petCurrGold = tmeg->ackOGAckBoss.petcurrgold();
            DataManager::sharedDataManager()->freeNum = tmeg->ackOGAckBoss.freenum();
            DataManager::sharedDataManager()->needGold = tmeg->ackOGAckBoss.needgold();
            DataManager::sharedDataManager()->boosBlood = tmeg->ackOGAckBoss.bossblood();
            DataManager::sharedDataManager()->bossBloodMax = tmeg->ackOGAckBoss.bossbloodmax();
            break;
        }
        case ACK_SEVER_JPHALL:
        {
            CCLog("ACK_SEVER_JPHALL");
            break;
        }
        case OGID_TEXAS_SLOTS_COLLECTION:
        {
            CCLog("OGID_TEXAS_SLOTS_COLLECTION");
            sendmeg.Bpage = tmeg->ackOGAckDoubleGetGold.page();
            sendmeg.Bwingold = tmeg->ackOGAckDoubleGetGold.wingold();
            sendmeg.Brolegold = tmeg->ackOGAckDoubleGetGold.gold();
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_K_REC_MEG_FROM_SEVER, &sendmeg);
            
            break;
        }
        case OGID_TEXAS_SLOTS_PETUPDATEDMSG:
        {
            CCLog("OGID_TEXAS_SLOTS_PETUPDATEDMSG");
            break;
        }
        case OGID_TEXAS_SLOTS_FIGHTRESULT:
        {
            
            OutTime::sharedDataManager()->release(OGID_TEXAS_SLOTS_REQPETFIGHT2);
            CCLog("OGID_TEXAS_SLOTS_FIGHTRESULT");
            
            sendmeg.Fwinret = tmeg->ackOGAckPetFightResult.winresult();
            sendmeg.Fret = tmeg->ackOGAckPetFightResult.result();
            sendmeg.FpetJoinNum = tmeg->ackOGAckPetFightResult.petjoinnum();
            sendmeg.FpetSkill = tmeg->ackOGAckPetFightResult.petskill();
            sendmeg.FbossJoinNum = tmeg->ackOGAckPetFightResult.bossjoinnum();
            sendmeg.FbossSkill = tmeg->ackOGAckPetFightResult.bossskill();
            sendmeg.Fpage = tmeg->ackOGAckPetFightResult.page();
            sendmeg.FpetACK = tmeg->ackOGAckPetFightResult.petdata();
            sendmeg.FbossACK = tmeg->ackOGAckPetFightResult.bossdata();
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_K_REC_MEG_FROM_SEVER, &sendmeg);
            break;
        }
        case OGID_TEXAS_SLOTS_DOUBLEGAME:
        {
            CCLog("OGID_TEXAS_SLOTS_DOUBLEGAME");
            
            sendmeg.Bret = tmeg->ackOGAckDoubleResult.result();
            sendmeg.Bwingold = tmeg->ackOGAckDoubleResult.wingold();
            sendmeg.BstartNum = tmeg->ackOGAckDoubleResult.start();
            sendmeg.Bpage = tmeg->ackOGAckDoubleResult.page();
            sendmeg.Brolegold = tmeg->ackOGAckDoubleResult.gold();
            sendmeg.Bneedgold = tmeg->ackOGAckDoubleResult.needgold();
            sendmeg.BgoldPlus = tmeg->ackOGAckDoubleResult.goldplus();
            sendmeg.BgoldPlusValue = tmeg->ackOGAckDoubleResult.goldplusvalue();
            sendmeg.BcardNumber = tmeg->ackOGAckDoubleResult.cardnumber();
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_K2_REC_MEG_FROM_SEVER, &sendmeg);
            break;
        }
        case OGID_TEXAS_SLOTS_FIGHTREWARD:
        {
            CCLog("OGID_TEXAS_SLOTS_FIGHTREWARD");
            
            
            MyPetFAward tsruck;
            sendmeg.mPetAWDList.clear();
            
            for(int i = 0; i < tmeg->ackOGAckPetGetAward.awardlist_size(); i++)
            {
                tsruck.Atype = tmeg->ackOGAckPetGetAward.awardlist(i).type();//mail内容
                tsruck.Anum = tmeg->ackOGAckPetGetAward.awardlist(i).num();//创建时间
                tsruck.Aok = tmeg->ackOGAckPetGetAward.awardlist(i).ok();//mail标题
                tsruck.AitemID = tmeg->ackOGAckPetGetAward.awardlist(i).itemid();//数量
                sendmeg.mPetAWDList.push_back(tsruck);
            }
            
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            break;
        }
        case OGID_TEXAS_SLOTS_DOUBLEJP://比倍界面返回jp巨奖
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_K2_REC_MEG_FROM_SEVER, &sendmeg);
            
            break;
        }
        default:
        {
            CCLog("No this Message ID~~~!!!!!%d", tid);
            break;
        };
    }
    
    CCLog("++++++++++++++++++++++++++++++++++");
    
}