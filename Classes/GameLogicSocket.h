//
//  GameLogicSocket.h
//  Slot
//
//  Created by zhengyu on 14-8-22.
//
//

#ifndef __Slot__GameLogicSocket__
#define __Slot__GameLogicSocket__

#include <iostream>
#include "cocos2d.h"
#include "TcpSocket.h"
#include "define.h"
#include "DataManager.h"
#include <string>
#include "OGTexasSlotsProtocol.pb.h"
#include "define.h"
#include "OutTime.h"

using namespace std;
using namespace ::google::protobuf;
using namespace com::ourgame::texasSlots;
USING_NS_CC;

class GameLogicSocket : public TcpSocket
{
public:
    GameLogicSocket();
    ~GameLogicSocket();
    
	
	
    
    //错误处理
    virtual void onError();
/////////////////////////////////////////   send msg handler  ///   ////////////////////////////////////////////
    bool sendIdle();
    
    // 玩家登录验证  (CLIENT+2)
    bool sendLogin( string UserId,
                    string Cert,
                    int32  gameId,
                    int32  comfromId,
                    string userName,
                    string nickName,
                    string entrance);
    
    //点击进入房间的操作 	( CLIENT + 3 ) s+4
    bool sendJoinRoom(int32	roomId);
    
    // 关闭按钮消息 (CLIENT+4)
//    ET_TYPE1            = 1; // 退出房间
//    ET_TYPE2            = 2; // 退出游戏
//    ET_TYPE3            = 3; // 退出比倍
//    ET_TYPE4            = 4; // 退出宠物战斗
    bool sendExitGame(int32 msgType);
    
    //请求宠物消息(CLIENT + 5)-----
    bool sendReqPetList();
    
    //请求宠物洗点（CLIENT + 6）-----
    bool sendReqPetReset(int32 petId);
    
    //请求宠物参战（CLIENT + 7）-----
    bool sendReqPetFight(int32 petId);
    
    //请求宠物属性界面的宠物背包 (CLIENT + 8)---
    bool sendPetPack();
    
    //请求排行榜(CLIENT +9)------
    bool sendRanking(string roleName);//s+13
    
    //邮件请求(CLIENT + 10)-----
    bool sendMails();
    
    //邮件2删除 1读取 3收取(CLIENT + 11)-----
    bool sendMailsState(int32 type,string id);
    
    //请求宠物分解（CLIENT + 12）-------
    bool sendPetResolve(int32 petId);
    
    //完成新手教程（CLIENT + 13）-----
    bool sendGuildOver();
    
    //领取新手奖励（CLIENT + 14）----
    bool sendReqAward();
    
    //请求商品信息（CLIENT + 15）------
    bool sendReqShopItem(int32 itemId);
    
    //购买道具（CLIENT + 16）------
    bool sendReqBuyItem(int32 itemId,int32 num);
    
    //请求道具合成（CLIENT + 17）----------
    bool sendReqPetCompose(int32 debrisItemId,int32 evolveId);
    
    //请求碎片合成方案（CLIENT + 18）-------
    bool sendReqCompse();
    
    //请求分解方案（CLIENT + 19）---------
    bool sendReqPetResolveScheme();
    
    //请求抽奖（CLIENT + 20）--------
    bool sendReqLoginAward(string roleName,int32 lev);
    
    //宠物返奖(CLIENT + 21)------
    bool sendReqPetReturnAward(int32 action);//1喂食 2 抚摸
    
    //打开绑定界面,请求手机是否绑定(CLIENT + 22)------
    bool sendBindPhone();
    
    
    
    //获取验证码(CLIENT + 23)
    bool sendGetCode(string phone);//绑定的手机号------
    
    //输入验证码,获取奖励(CLIENT + 24)
    bool sendGetCodeOK(int32 code);//验证码-------s+38号协议没有
    
    
    // 开始转动游戏 (CLIENT+50)-------s+50
    bool sendReqStartGame(int32 line,int32 times,bool autop);// 押注线数 // 押注倍数// 自动-----
    
    // 开始比倍游戏 (CLIENT+51)------
    bool sendReqDoubleGame(int32 times,int32 colour);// 押注倍数// 押注颜色
    
    // 比倍收分消息 (CLIENT+52)------s+53
    bool sendReqDoubleGetGold();
    
    //接收宠物战斗(CLIENT + 53)-------
    bool sendReqPetFight();
    
    //接收宠物逃跑(CLIENT + 54)------
    bool sendReqPetFlee();
    
    //接收宠物战斗胜利抽奖(CLIENT + 55)------s+56
    bool sendReqFightAward();
   
    //请求jp获奖记录（CLIENT + 56）-----
    bool sendReqJpRecord(string roleName);//s+59
    
    ///////////////////////////////////////////end send handler//////////////////////////
   
///////////////////////////////////////////////////// add by zy////////////////////////////////////////////////////
//////////////////////////////////////////////////receive msg handler//////////////////////////////////////////////

    void onLogin2(const char* message, int size);//返回玩家信息
    void onExitGame(const char* message, int size);//关闭消息返回add
    void onRoomList(const char* message, int size);//接收房间列表
    void onRoomSite(const char* message, int size);//接收房间配置信息
    void onPlayerInfo(const char* message, int size);//更新玩家信息---------
    
    void onPetList(const char* message, int size);//请求宠物列表消息+7
    void onPetReset(const char* message, int size);//接收宠物属性重置
    void onOGAckJP(const char* message, int size);//返回jp奖池信息 (1001/SERVER+51)
    void onOGAckMessage(const char* message, int size);//广播  SERVER +8
    void onOGAckPetGridOnList(const char* message, int size);//返回宠物开启条件（SERVER +12)
    void onOGAckUnreadMail(const char* message, int size);//有未读邮件发送（SERVER +19）
    void onOGAckItems(const char* message, int size);//道具列表（SERVER +18）
    void onOGAckDoublePlus(const char* message, int size);//发送比倍星级百分比（SERVER + 31）
    void onOGAckLoginAwardList(const char* message, int size);//返回抽奖列表（SERVER +29）
    void onOGAckJpStart(const char* message, int size);//返回jp明星数据(SERVER +35)1022
    void onOGAckRank(const char* message, int size);//返回收分榜(SERVER +13)
    void onOGAckMails(const char* message, int size);//邮件消息(SERVER + 16)
    void onOGAckShopItemInfo(const char* message, int size);//发送商品信息（SERVER +22）
    void onOGAckLoginGetAward(const char* message, int size);//返回抽中奖品信息（SERVER +28)
    void onOGAckBindPhone(const char* message, int size);//返回绑定结果(SERVER +36)
    void onOGAckGetPhoneCode(const char* message, int size);//返回获取验证码(SERVER +37)
    void onOGAckGameResult(const char* message, int size);//返回游戏结果（SERVER＋50）
    void onOGAckJpRecord(const char* message, int size);//返回jp获奖记录（SERVER +59）
    void onOGAckBoss(const char* message, int size);//返回boss距离相关（SERVER + 11）
    void onOGAckJpHall(const char* message, int size);//大厅中最近中jp信息（SERVER + 39）
    void onOGAckDoubleGetGold(const char* message, int size);//更新收分消息（SERVER + 53）
    void onOGAckPetUpdatedMsg(const char* message, int size);//更新宠物消息（SERVER + 14）
    void onOGAckPetFightResult(const char* message, int size);//更新宠物战斗结果（SERVER + 14）
    void onOGAckDoubleResult(const char* message, int size);//服务器返回比倍结果（SERVER + 52）
    void onOGAckPetGetAward(const char* message, int size);//返回宠物战斗胜利后抽奖结果（SERVER + 56）
    void onOGAckDoubleJpResult(const char* message, int size);//返回宠物战斗胜利后抽奖结果（SERVER + 60）
    void onOGAckStopGame(const char* message, int size);//止损（SERVER + 57）
    
    void onOGAckCutGame(const char* message, int size);//断线
    
    /*
    void onBoradCast(const char* message, int size);//接收广播信息
    void onGiftPet(const char* message, int size);//接收系统赠送宠物信息
    
    void onBossInfo(const char* message, int size);//接收boss配置信息
    void onPetPack(const char* message, int size);//接收宠物背包消息
    void onRandkingData(const char* message, int size);//接收排行榜数据
    void onPetUpdateMsg(const char* message, int size);//接收宠物更新消息
    void onCloseModule(const char* message, int size);//接收关闭Module消息
    
    void onAckMails(const char* message, int size);//接收邮件消息
    void onAckMailChange(const char* message, int size);//接收邮件附件消息
    void onAckItems(const char* message, int size);//接收道具消息
    void onAckNewMail(const char* message, int size);//接收新邮件消息
    
    void onAckGuideAward(const char* message, int size);//接收新手奖励消息
    void onAckPropsConsumption(const char* message, int size);//接收道具消耗消息
    void onShopList(const char* message, int size);//接收商城商品消息
    void onPetResolve(const char* message, int size);//宠物分解返回结果
    void onPetCompose(const char* message, int size);//接收宠物合成结果
    void onScheme(const char* message, int size);//接收宠物合成方案
    void onResolveScheme(const char* message, int size);//接收宠物分解方案
    
    void onJpmessage(const char* message, int size);//玩家登录接收JP中奖消息
    void onLoginAward(const char* message, int size);//返回登陆中奖信息
    void onLoginReward(const char* message, int size);//接收抽奖消息
    void onOgReqNotice(const char* message, int size);//系统公告
    void onDoublePlus(const char* message, int size);//比倍星级加成
    void onPetReturnAward(const char* message, int size);//宠物互动结果
    void onJpStar(const char* message, int size);//JP明星

    void onBindPhone(const char* message, int size);//返回是否绑定
    void onGetCode(const char* message, int size);//返回验证码
    void onGetCodeOK(const char* message, int size);//输入验证码
    
    void onAckJackPotList(const char* message, int size);//接收jackpot数据
    void onDoubleJp(const char* message, int size);//接收比倍jp消息

    //玩家超过止损消息 SERVER +57
    void onAckStopGame(const char* message, int size);

    //消息提示（SERVER +58）
    void onAckErrorMsg(const char* message, int size);
     */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //接收信息
    virtual void onReceiveData(int messageID, const char* message, int size);
    
    
private:
    ThranActionType m_ActionType;
    
};

#endif /* defined(__Slot__GameLogicSocket__) */
