/*!
 * @brief	数据管理类，负责管理游戏中的所有数据。
 * @author	bug kang
 * @date	2014-8-21
 * @ingroup	Data
 */

#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "cocos2d.h"
#include "mobileGame.pb.h"
#include "OGTexasSlotsProtocol.pb.h"

USING_NS_CC;

struct SRoomBet
{
    unsigned long long min;
    unsigned long long max;
    int bet;
};


class DataManager : public CCObject
{
public:
    DataManager();
    ~DataManager();
    
    static DataManager* sharedDataManager();
    
    //登入游戏逻辑时需要的参数----------------------------
    std::string userid;//也是rolename
    std::string cert;
    std::string cert16;//16进制
    int gameid;
    int comfromid;
    std::string entrance;
    std::string nickname;
    std::string username;
    int sex;
    std::string errmeg;//错误信息
    
    com::ourgame::texasSlots::OGAckRoleLoginMsg AckRoleLoginMsg;
    
    //大厅相关数据-------------------------------------
    int currLevel;//当前人物等级
    unsigned long long currGold;//当前人物拥有的金币（万能豆）
    int currExp;
    int needExp;
    int currFreeNum;//免费转动次数 老虎机
    
    //游戏内部相关数据------------------------------------
    //房间内部配置数据
    int ret;//返回登入房间成功失败0-成功， 1失败
    int err;//失败原因 0未知错误 1等级不足 2金币不足 3已经有房间了 4未找到房间
    int roomID;//房间id
    std::string roomName;//房间名称
    int maxLine;//最大线数
    int maxBei;//最大倍数
    unsigned long long roomBaseGold;//底金
    int roomPic;//房间图片
    int roomDoubleOn;//比倍开关
    int freeOn;//免费转开关
    int fourTimesOn;//四倍开关
    std::vector<SRoomBet> roomBetList;
    int grid[15];
    
    int petID;//宠物id
    
    //转动游戏
    int gird[15];//格子id
    unsigned long long winGold;//中奖金额
    int lineRet;//中奖结果0-未中 1-普通 2jp大奖
    int jpLineNun;//ret = 2时jp几连
    int winExp;//获得经验
    
    
    
    //战斗画面数据
    int boosID;
    unsigned long long bossNeedGold;//boss距离
    int roomPicInRoom;//房间显示图片类型
    unsigned long long bossPic;//boss图片类型
    unsigned long long petCurrGold;//宠物话费金币(位置)
    int freeNum;//免费次数 战斗界面
    unsigned long long needGold;//每次转动花费金币
    int boosBlood;//boss当前血量
    int bossBloodMax;//boss满值血
    
    int petattack;//参战宠物攻击力
    int petblood;//参战宠物血量
    
    
 
};

#endif
