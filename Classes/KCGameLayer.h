//
//  KCGameLayer.h
//  Slot
//
//  Created by zhengyu on 14-8-27.
//
//

#ifndef __Slot__KCGameLayer__
#define __Slot__KCGameLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "define.h"
#include "Hdefine.h"
#include "PetFightLayer.h"
#include "DoubleLayer.h"
#include "SoundNameDefine.h"
#include "SimpleAudioEngine.h"
#include "AlertText.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace CocosDenshion;

#define BTN 123456
#define BTN_Auto (BTN + 1)
#define BTN_GO (BTN + 2)
#define BTN_LINE (BTN + 3)
#define BTN_BEI (BTN + 4)
#define BTN_HMAX (BTN + 5)
#define BTN_TP (BTN + 6)
#define BTN_FGO (BTN + 7)
#define BTN_BACK (BTN + 8)
#define BTN_ZD (BTN + 9)


#define POS_Y_MOVE 20
#define POS_Y_MOVE_TIME 0.25f

//#define POS_CELL_HEIGHT 310
#define POS_CELL_HEIGHT 647
#define POS_CELL_MOVE_SPEED 70//滚动值

#define TIME_FLAG0 -1
#define TIME_FLAG1 -2
#define TIME_FLAG2 -3
#define TIME_FLAG3 -4
#define TIME_FLAG4 0

#define TIME_SCROLL 30//转动结束标志位

struct Point4ReWard
{
	int m_iCellID[5];
};

struct Date4ReWard
{
	int m_id;//中奖id
	int m_num;//连续数目
};

class KCGameLayer : public CCLayer
{
public:
    
    KCGameLayer()
    {
        m_pv = NULL;
        
        m_isCanGo = true;
        m_isCanTouchAllBtn = true;
        m_isCanDraw = false;
        m_iTimeFlag = 0;
        m_iChangeImgFlag = 0;
        m_id4Reward.clear();
        m_isAllThingsStop = false;
        m_winret = 0;
        m_Ffreenum = 0;
        m_isOnceUpDateImg = true;
        m_roleExp = 0;
        imgid = 3;
        m_ActionFightLogicFlag = 0;
        m_ActionStartLogicFlag = 0;
        m_FlianNum = 0;
        m_FSkill = 0;
        m_FSkillboss = 0;
        m_isGoldEnougth = true;
        
        m_setLineRewNum = 1;
        m_maxLineRewNum = 10;
        m_setLineRewNumTmp = 1;
        m_baseGold = 1;
        m_usrHavaGold = 0;
        m_beiNum = 1;
        m_maxBeiNum = 10;
        m_isCanAuto = false;
        m_roomID = 1;
        m_isCanfreeOn = false;
        m_isCanStopLHJ = false;
        m_winGold = 0;
        m_page = 1;
        m_pageCurr = 1;
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                      callfuncO_selector(KCGameLayer::recGameLogicEventFromSever),
                                                                      EVENT_K_REC_MEG_FROM_SEVER,
                                                                      NULL);
    }
    
    ~KCGameLayer()
    {
        CC_SAFE_RELEASE(m_arrLineImg);
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_K_REC_MEG_FROM_SEVER);
        
        if(m_pv != NULL )
        {
            delete m_pv;
        }
        
        m_double->release();
    }
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual void update(float dt);
    virtual bool init();
//    virtual void registerWithTouchDispatcher();//防止击穿
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    // implement the "static node()" method manually
    CREATE_FUNC(KCGameLayer);
    
    static int playSound(const char * name);//播放音效
    
    void initUI();
    void runActionLHJ();//老虎机转动
    void setGoLVisible();
    
    void touchEvent(CCObject* pSender, TouchEventType type);
    
    void setVisitFalse(CCObject * pSender);
    
    void set2PanelVisitTrue0();
	void set2PanelVisitTrue1();
	void set2PanelVisitTrue2();
	void set2PanelVisitTrue3();
	void set2PanelVisitTrue4();
    
	void updatePanelCell0();
	void updatePanelCell1();
	void updatePanelCell2();
	void updatePanelCell3();
	void updatePanelCell4();
    
	void setFlag0();
	void setFlag1();
	void setFlag2();
	void setFlag3();
	void setFlag4();
    
	void initPointReward(int id, int CellID[5]);//初始化cell数据
	void initRect2Point();//初始化draw Point
	void updateCellsDate();//更新cell数据
	void mangerCellsDate();//处理cell数据 是否有中奖情况
	void updateCellImg();//更新cell图片
    void recGameLogicEventFromSever(CCObject * obj);//接受事件
    
    void showLine(bool iscandraw);//判断是否该隐藏或者显示line true-显示
    void setAllFlagTrue();//切换界面时候稍微停顿1秒再重置影响转动和按钮点击到标识
    void runRewardAction();//播发中奖cell动画
    void stopRewardAction();//停止播发中奖cell动画
    void setLabelPetExp();//重置数字标签petExp位置
    void setLabelRoleExp();//重置数字标签roleExp位置
    void setLabelBossACK();
    void setLabelPetACK();
    void setLabelAddBlood();
    void runFightAction();//战斗动画
    void runStartAction();//开始动画
    void runFightWin0();
    void runFightWin1();
    void runFightWin2();
    void setGoL();
    void setwinL();
    void setLoseL();
    void runFightLose();
    void updateZS();//更新钻石
    UIImageView* m_ZS[26];//
    
    UILayer * m_Widget;
    Layout* m_rootLy;
    
    UIPanel *m_Panel_BottomZ;
    UIPanel *m_Panel_BottomZDS;
    
    UIButton *m_Button_Auto;
    UIButton *m_BtnBack;
    UIButton *m_BtnZD;
    UIButton* tBtnGo;
    UIButton* tBtnFGO;
    
    
    CCPoint m_arrPoint0[5];//记录初始位置
	CCPoint m_arrPoint1[5];//记录初始位置
    
    UIPanel* m_Panel_Move0;
	UIPanel* m_Panel_Move1;
	UIPanel* m_Panel_Move2;
	UIPanel* m_Panel_Move3;
	UIPanel* m_Panel_Move4;
    
    UIPanel* m_Panel_MoveCell00;
	UIPanel* m_Panel_MoveCell01;
	UIPanel* m_Panel_MoveCell10;
	UIPanel* m_Panel_MoveCell11;
	UIPanel* m_Panel_MoveCell20;
	UIPanel* m_Panel_MoveCell21;
	UIPanel* m_Panel_MoveCell30;
	UIPanel* m_Panel_MoveCell31;
	UIPanel* m_Panel_MoveCell40;
	UIPanel* m_Panel_MoveCell41;
    
    UIImageView* m_modelLineImg;//线模板
    UIPanel* m_PanelMid;//中间部分panel
    UIImageView* m_PanelCostZD;//
    
    UILabelAtlas* m_LabelAtlasLine;
    UILabelAtlas* m_LabelAtlasBei;
    UILabelAtlas* m_LabelAtlasCost;
    UILabelAtlas* m_LabelAtlasFree;
    UILabelAtlas* m_LabelAtlasWin;
    UILabelAtlas* m_LabelAtlasSY;
    
    UILabelAtlas* m_LabelAtlasPetACK;
    UILabelAtlas* m_LabelAtlasBossACK;
    UILabelAtlas* m_LabelPetExp;
    UILabelAtlas* m_LabelRoleExp;
    UILabelAtlas* m_LabelPetAddBlood;
    UILabelAtlas* m_LabelCostZD;
    
    CCArmature* m_armatureStart;
    
    bool m_isCanGo;//是否可以点击开始
	bool m_isCanTouchAllBtn;//是否可以点击所有按钮
    bool m_isCanDraw;//可以画线
    int m_iTimeFlag;//时间标志位，老虎机第一条停止时间
    int m_iChangeImgFlag;//防止不同步
    int m_arrCellsDate[15];//15个cellimg id数据
    int m_arrCellsDatetmp[15];//临时存储从战斗返回用
    
    int m_setLineRewNum;//压入线中奖数目
    int m_setLineRewNumTmp;//临时存放连线数目
    int m_maxLineRewNum;//最大线中奖数目
    int m_beiNum;//倍数
    int m_maxBeiNum;//最大倍数
    int m_roomID;//房间id
    int m_page;//界面
    int m_pageCurr;//当前界面
    int imgid;
    int m_FlianNum;//战斗界面连线数量
    int m_FSkill;//战斗界面技能结果
    int m_FSkillboss;
    unsigned long long m_baseGold;//底金
    unsigned long long m_usrHavaGold;//玩家金
    unsigned long long m_winGold;//赢取钱数
    unsigned long long m_roleExp;//人物获得的经验
    bool m_isCanAuto;//是否可以自动 true-可以
    bool m_isCanfreeOn; //免费转动
    bool m_isCanStopLHJ;//是否可以停止转动
    bool m_isAllThingsStop;//阻塞所有事情
    
    int bossCurrBlood;// = DataManager::sharedDataManager()->boosBlood;
    int bossMaxBlood;// = DataManager::sharedDataManager()->bossBloodMax;
    int bossAttack;
    int petCurrBlood;// = DataManager::sharedDataManager()->petCurrGold;
    int petMaxBlood;// = DataManager::sharedDataManager()->petCurrGold;
    int petAttack;//
    int m_winret;//胜利状态1-pet win 2-boss win
    int m_Ffreenum;//战斗界面免费次数
    
    std::vector <Date4ReWard> m_id4Reward;//中奖情况容器
    Point4ReWard m_p4r[25];//存储中奖情况关键点
    CCArray * m_arrLineImg;//存储25条线
    
    PetFightLayer* m_pv;
    DoubleLayer* m_double;
    
    int m_ActionNum;//闪烁动作控制计数
    int m_ActionFightLogicFlag;//战斗动画逻辑标识
    int m_ActionStartLogicFlag;//开始动画逻辑标识
    
    bool m_isOnceUpDateImg;
    CCSprite *winL;
    CCSprite *LoseL;
    CCSprite *GoL;
    
    CCSprite *m_spPetExp;
    CCSprite *m_spRoleExp;
    bool m_isGoldEnougth;//金币是否充足
    
};


#endif /* defined(__Slot__KCGameLayer__) */
