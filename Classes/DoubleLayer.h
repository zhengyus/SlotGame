//
//  DoubleLayer.h
//  Slot
//
//  Created by huxf on 14-9-1.
//
//

#ifndef __Slot__DoubleLayer__
#define __Slot__DoubleLayer__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "define.h"
#include "Hdefine.h"
#include "DataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

#define BTN_RED 10
#define BTN_BLACK 11
#define BTN_X2 12
#define BTN_X3 13
#define BTN_X5 14 
#define BTN_GET 15

#define GET_MONEY_TIME 4.0f

class DoubleLayer : public cocos2d::CCLayer
{
    
public:
    
    DoubleLayer();
    ~DoubleLayer();
    
    CREATE_FUNC(DoubleLayer);
    
    virtual bool init();
//    virtual void registerWithTouchDispatcher();
    void touchEvent(CCObject* pSender, TouchEventType type);
    void initBtn();
    void recGameLogicEventFromSever(CCObject * obj);
    void runBiBei(int color);
    void initUI();
    void setcardBGImg(CCNode* pSender);//背景图片改变纹理
    void moveCards();//移动cards
    void addcard();//添加card
    void resetBGcard();//重置BGcard
    void runStarAction(int id);//根据id播放星星动画
    void isCanCloseThisUI();//是否该关闭该界面
    void setWinCanSee();//win图标可见
    void runMoneyFly();//翅膀飞
    void runAwardAction();
    void runUIStar();
    void autoGetScore();
    bool isNoGold(unsigned long long gold);
    void ishaveChange();//是否返回比倍收分消息
    
    void setLabelMoneyFly();
    void setActionFly0();
    void setActionFly1();
    
    void setLabelWinNum();
    
    UILabelAtlas* m_LabelWinNum;
    
    UILayer * m_Widget;
    Layout* m_rootLy;
    
    UIPanel* m_rootPanel;
    
    int m_beiNum;//1, 2, 3, 5
    int m_secondFlag;
    int m_stopSound;
    UIButton* tBtnX2;
    UIButton* tBtnX3;
    UIButton* tBtnX5;
    UIButton* tBtnget;
    UILabelAtlas* m_atl;
    UIImageView* m_card;
    unsigned long long m_needgold;
    unsigned long long m_winGold;//每次赢的钱
    
    bool isBtnCanTouch;//按钮是否可以按
    int m_whichColor;//1红色 2黑色
    int m_starNum;//星星数量
    CCPoint m_p8[8];

    CCArray * m_arrCardImg;//存储牌
    CCClippingNode* m_cliper;
    
    CCSprite* m_spCardBG;

    std::string cardimg[54];
    int m_cardID;
    int m_retWin;
    CCArray *m_arrStar;
    
    int m_page;
    CCSprite * m_spWin;
    CCPoint winPos;
    
    CCArmature *m_armatureAward[3];//中奖特效
    CCArmature *m_armatureGetWin[3];//收分左边
    CCArmature *m_armatureWin;//中奖赢特效
    CCArmature *m_armatureGet;//收分特效
    CCArmature *m_armatureChiBangLeft;//翅膀左边
    CCArmature *m_armatureChiBangRight;//翅膀右边
    

    
    
    CCParticleSystemQuad* m_ptc;//粒子特效
    CCParticleBatchNode* patBatchNode;
    
    CCPoint m_pAtl;
    
};

#endif /* defined(__Slot__DoubleLayer__) */
