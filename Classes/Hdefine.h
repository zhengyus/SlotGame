//
//  Hdefine.h
//  Slot
//
//  Created by zhengyu on 14-8-25.
//
//

#ifndef __Slot__Hdefine__
#define __Slot__Hdefine__


#include "cocos2d.h"
#include "OGTexasSlotsProtocol.pb.h"

USING_NS_CC;

#define EVENT_REC_FROM_MSG_ALL "7000"
#define EVENT_REC_FROM_MSG_UILOGIC "7001"

class GameLogicMegFromSever :public CCObject
{
    
public:
    GameLogicMegFromSever()
    {
        
    }
    
    ~GameLogicMegFromSever()
    {
        
    }
    
    int m_id;
    
    com::ourgame::texasSlots::OGAckRoleLoginMsg ackLoginMeg;
    com::ourgame::texasSlots::OGAckExit ackOGAckExit;
    com::ourgame::texasSlots::OGAckRoomList ackOGAckRoomList;
    com::ourgame::texasSlots::OGAckRoom ackOGAckRoom;
    com::ourgame::texasSlots::OGAckRoleMsg ackOGAckRoleMsg;
    com::ourgame::texasSlots::OGAckPetList ackOGAckPetList;
    com::ourgame::texasSlots::OGAckPetReSet ackOGAckPetReSet;
    com::ourgame::texasSlots::OGAckJP ackOGAckJP;
    com::ourgame::texasSlots::OGAckMessage ackOGAckMessage;
    com::ourgame::texasSlots::OGAckPetGridOnList ackOGAckPetGridOnList;
    com::ourgame::texasSlots::OGAckUnreadMail ackOGAckUnreadMail;
    com::ourgame::texasSlots::OGAckItems ackOGAckItems;
    com::ourgame::texasSlots::OGAckDoublePlus ackOGAckDoublePlus;
    com::ourgame::texasSlots::OGAckLoginAwardList ackOGAckLoginAwardList;
    com::ourgame::texasSlots::OGAckJpStart ackOGAckJpStart;
    com::ourgame::texasSlots::OGAckRank ackOGAckRank;
    com::ourgame::texasSlots::OGAckMails ackOGAckMails;
    com::ourgame::texasSlots::OGAckShopItemInfo ackOGAckShopItemInfo;
    com::ourgame::texasSlots::OGAckLoginGetAward ackOGAckLoginGetAward;
    com::ourgame::texasSlots::OGAckBindPhone ackOGAckBindPhone;
    com::ourgame::texasSlots::OGAckGetPhoneCode ackOGAckGetPhoneCode;
    com::ourgame::texasSlots::OGAckGameResult ackOGAckGameResult;
    com::ourgame::texasSlots::OGAckJpRecord ackOGAckJpRecord;
    com::ourgame::texasSlots::OGAckBoss ackOGAckBoss;
    com::ourgame::texasSlots::OGAckJpHall ackOGAckJpHall;
    com::ourgame::texasSlots::OGAckDoubleGetGold ackOGAckDoubleGetGold;
    com::ourgame::texasSlots::OGAckPetUpdatedMsg ackOGAckPetUpdatedMsg;
    com::ourgame::texasSlots::OGAckPetFightResult ackOGAckPetFightResult;
    com::ourgame::texasSlots::OGAckDoubleResult ackOGAckDoubleResult;
    com::ourgame::texasSlots::OGAckPetGetAward ackOGAckPetGetAward;
};

struct MyRoomList
{
    int roomid;
    std::string roomName;
    int roomLevelLimt;
    unsigned long long roomGoldLimt;
    int roomOn;
};

struct MyRankList
{
    int rankid;
    std::string rankName;
    unsigned long long rankGold;
};

struct MyJPRankList
{
    std::string JPrankName;
    unsigned long long JPrankwinGold;
    unsigned long long JPrankcurrGold;
    std::string JPrankTime;
};

struct MyMailMeg
{
    std::string mailid;//id
    std::string mailContent;//mail内容
    std::string mailTime;//创建时间
    std::string mailTitle;//mail标题
    int mailNum;//数量
    int mailType;//邮件类型
    int isRead;//已读
    int isOK;//收取标记
    std::string mailPic;//图片路径
    
};

struct MyPetList
{
    std::string petName;//名称
    int petpinzhi;//品质
    int petblood;//基础血
    int petbloodMin;//最小值血
    int petbloodMax;//最大值血
    int petbloodPlus;//血加成
    
    int petattack;//基础攻击
    int petattackMin;//最小值攻击
    int petattackMax;//最大值攻击
    int petattackPlus;//攻击加成
    
    int petaward;//基础奖励
    int petawardMin;//最小值奖励
    int petawardMax;//最大值奖励
    int petawardPlus;//
    
    int petlevel;//等级
    int petexp;//经验
    int petreqexp;//升级所需经验
    int petID;//id
    int petstatus;//参战状态 1参战 0未参战
    std::string petPic;//宠物图片
    int petMaxLev;//等级上限
};

struct MyPetFAward
{
    int Atype;//类型 1金币 2角色经验 3宠物经验 4宠物 5威客
    unsigned long long Anum;//数量
    int Aok;//是否中奖 0未中 1中奖
    int AitemID;//id
};

class Meg2UIDate :public CCObject
{
public:
    int m_id;
    std::string errmeg;//错误内容
    int errType;//错误类型
    //----------------------大厅--------------------------
    //大厅宠物人物信息  OGID_TEXAS_SLOTS_PETLIST
    std::string roleName;
    int roleLv;
    int petLv;
    std::string petPic;
    unsigned long long gold;
    int currExp;//当前经验
    int needExp;//升级所需经验
    
    //jp奖池  OGID_TEXAS_SLOTS_JACKPOT
    unsigned long long jp;
    
    //jckstar中奖人物信息 OGID_TEXAS_SLOTS_ACKJACKPOTLIST
    std::string jpStarName;
    unsigned long long jpStarNum;
    std::string jpStarTime;
    
    //周一排行榜 OGID_TEXAS_SLOTS_RANKINGDATA
    std::string rankName;
    unsigned long long rankGold;
    
    //广播消息  OGID_TEXAS_SLOTS_BORADCAST
    std::string content;
    
    //房间列表  OGID_TEXAS_SLOTS_ROOMLIST
    std::vector<MyRoomList> mroomList;
    
    //排行榜列表  OGID_TEXAS_SLOTS_RANKINGDATA
    std::vector<MyRankList> mrankListArr[4];//0-周一 1-周四 2-历史周一 3-历史周四
    
    //jp排行榜 OGID_TEXAS_SLOTS_ACKJACKPOTLIST
    std::vector<MyJPRankList> mJPRankList[2];//0-jp明星奖 1-jp巨奖
    
    //邮件 OGID_TEXAS_SLOTS_ACKMAILS
    std::vector<MyMailMeg> mMailvector;
    
    //宠物列表 OGID_TEXAS_SLOTS_PETLIST
    std::vector<MyPetList> mPetList;
    
    //战斗胜利抽奖    OGID_TEXAS_SLOTS_FIGHTREWARD
    std::vector<MyPetFAward> mPetAWDList;
    //-----------------------------------------------------
    
    //-----------------------------游戏内部-----------------------------
    //OGID_TEXAS_SLOTS_GAMERESULT
    int grid[15];
    unsigned long long wingold;//中奖金额
    int Rret;//中奖结果0-未中奖 1普通中奖 2jp大奖
    int jplineNum;//中jp大奖时 的几连
    unsigned long long getRoleExp;//获得人物经验
    unsigned long long getPetExp;//获得宠物经验
    int page;//界面1老虎机 2大厅 3比倍 4宠物战斗
    unsigned long long petWingold;//宠物获得额外大奖
    std::string wrongmeg;//错误消息
    int freeZ;//免费转动标记
    unsigned long long freeZgetGold;//免费事件获得金币
    //OGID_TEXAS_SLOTS_FIGHTRESULT
    int Fwinret;//0战斗中 1宠物胜利 2boss胜利
    int Fret;//0未中 1中线 -1金币不足
    int FpetJoinNum;//宠物几连
    int FpetSkill;//宠物技能0-无 1-铁 2-铜 3-金 4闪电 5-加血
    int FbossJoinNum;//boss几连
    int FbossSkill;//boss技能
    int Fpage;//界面
    int FpetACK;//界面
    int FbossACK;//界面
    //OGID_TEXAS_SLOTS_COLLECTION
    unsigned long long Bwingold;
    unsigned long long Brolegold;
    int Bpage;
    //OGID_TEXAS_SLOTS_DOUBLEGAME
    int Bret;//比倍结果 1成功 0失败
    int BstartNum;//当前星级
    int BcardNumber;//花色 牌值
    unsigned long long Bneedgold;//押注值
    int BgoldPlus;//额外反奖比例
    unsigned long long BgoldPlusValue;//额外反奖金额
    
    //-----------------------------------------------------
    
};

typedef struct
{
    std::string id;
    std::string account;
    std::string password;
    
}UserAccount;


class UiLogicMsg : public CCObject //UI逻辑消息
{
    
public:
    int id;//1:收分动画
    
} ;


#endif /* defined(__Slot__Hdefine__) */
