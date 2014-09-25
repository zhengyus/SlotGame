//
//  KCGameLayer.cpp
//  Slot
//
//  Created by zhengyu on 14-8-27.
//
//

#include "KCGameLayer.h"
#include "DataManager.h"
#include "ext/ui/Alert.h"


// on "init" you need to initialize your instance
bool KCGameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;

    }
    
    
    char tmpStr[50];
    srand((unsigned int)time(NULL));
    
    m_arrLineImg = CCArray::create();
    CC_SAFE_RETAIN(m_arrLineImg);
    
    //中奖初始化数据
    for(int i = 0; i < 15; i++)
	{
		m_arrCellsDate[i] = 0;
        m_arrCellsDatetmp[i] = 0;
	}
    
	updateCellsDate();
    
    //初始化中奖id
    int CellID[25][5] =
    {
        {5, 6, 7, 8, 9},//1-
        {0, 1, 2, 3, 4},//2-
        {10, 11, 12, 13, 14},//3-
        {0, 6, 12, 8, 4},//4-
        {10, 6, 2, 8, 14},//5-
        {5, 11, 7, 13, 9},//6-
        {5, 1, 7, 3, 9},//7-
        {10, 6, 12, 8, 14},//8-
        {0, 6, 2, 8, 4},//9-
        {0, 1, 7, 13, 14},//10-
        {10, 11, 7, 3, 4},//11-
        {5, 1, 7, 13, 9},//12-
        {5, 11, 7, 3, 9},//13-
        {0, 6, 7, 8, 14},//14-
        {10, 6, 7, 8, 4},//15-
        {5, 1, 2, 8, 14},//16-
        {5, 11, 12, 8, 4},//17-
        {5, 6, 2, 8, 14},//18-
        {5, 6, 12, 8, 4},//19-
        {0, 1, 7, 13, 9},//20-
        {10, 11, 7, 3, 9},//21-
        {5, 1, 7, 13, 14},//22-
        {5, 11, 7, 3, 4},//23-
        {0, 1, 2, 8, 14},//24-
        {10, 11, 12, 8, 4}//25-
    };
    
    for(int i = 0; i < 25; i++)
    {
    	initPointReward(i, CellID[i]);
    }
    
    
    m_Widget = UILayer::create();
    
    m_rootLy = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("UI4LineHero_1.ExportJson"));
    
    m_Widget->addWidget(m_rootLy);
    
    //这版的cocostudio触屏事件跟Z轴有关系，与是否现实没关系
    m_Panel_BottomZ = static_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_BottomZ"));
    m_Panel_BottomZDS = static_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_BottomZDS"));
    m_Panel_BottomZ->setVisible(true);
    m_Panel_BottomZ->setZOrder(1);
    m_Panel_BottomZDS->setVisible(false);
    m_Panel_BottomZDS->setZOrder(0);
    
    m_Panel_Move0 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_Move0"));
  	m_Panel_Move1 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_Move1"));
  	m_Panel_Move2 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_Move2"));
  	m_Panel_Move3 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_Move3"));
  	m_Panel_Move4 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_Move4"));
    
    m_Panel_MoveCell00 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MoveCell00"));
  	m_Panel_MoveCell01 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MoveCell01"));
  	m_Panel_MoveCell10 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MoveCell10"));
  	m_Panel_MoveCell11 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MoveCell11"));
  	m_Panel_MoveCell20 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MoveCell20"));
  	m_Panel_MoveCell21 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MoveCell21"));
  	m_Panel_MoveCell30 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MoveCell30"));
  	m_Panel_MoveCell31 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MoveCell31"));
  	m_Panel_MoveCell40 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MoveCell40"));
  	m_Panel_MoveCell41 = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MoveCell41"));
    
  	m_Panel_MoveCell00->setVisible(false);
  	m_Panel_MoveCell01->setVisible(false);
  	m_Panel_MoveCell10->setVisible(false);
  	m_Panel_MoveCell11->setVisible(false);
  	m_Panel_MoveCell20->setVisible(false);
  	m_Panel_MoveCell21->setVisible(false);
  	m_Panel_MoveCell30->setVisible(false);
  	m_Panel_MoveCell31->setVisible(false);
  	m_Panel_MoveCell40->setVisible(false);
  	m_Panel_MoveCell41->setVisible(false);
    
  	m_arrPoint0[0] = m_Panel_MoveCell00->getPosition();
  	m_arrPoint0[1] = m_Panel_MoveCell10->getPosition();
  	m_arrPoint0[2] = m_Panel_MoveCell20->getPosition();
  	m_arrPoint0[3] = m_Panel_MoveCell30->getPosition();
  	m_arrPoint0[4] = m_Panel_MoveCell40->getPosition();
    
  	m_arrPoint1[0] = m_Panel_MoveCell01->getPosition();
	m_arrPoint1[1] = m_Panel_MoveCell11->getPosition();
	m_arrPoint1[2] = m_Panel_MoveCell21->getPosition();
	m_arrPoint1[3] = m_Panel_MoveCell31->getPosition();
	m_arrPoint1[4] = m_Panel_MoveCell41->getPosition();
    
    
    m_LabelAtlasLine = dynamic_cast<UILabelAtlas*>(m_Widget->getWidgetByName("AtlasLabel_Xian"));
    m_LabelAtlasBei = dynamic_cast<UILabelAtlas*>(m_Widget->getWidgetByName("AtlasLabel_Bei"));
    m_LabelAtlasCost = dynamic_cast<UILabelAtlas*>(m_Widget->getWidgetByName("AtlasLabel_Cost"));
    m_LabelAtlasFree = dynamic_cast<UILabelAtlas*>(m_Widget->getWidgetByName("AtlasLabel_Free"));
    m_LabelAtlasWin = dynamic_cast<UILabelAtlas*>(m_Widget->getWidgetByName("AtlasLabel_CWin"));
    m_LabelAtlasSY = dynamic_cast<UILabelAtlas*>(m_Widget->getWidgetByName("AtlasLabel_SY"));
    
    //----------------------------老虎机按钮相关-----------------------------
    //开始按钮
  	tBtnGo = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Button_Go"));
  	tBtnGo->addTouchEventListener(this, toucheventselector(KCGameLayer::touchEvent));
    tBtnGo->setTag(BTN_GO);
    //自动按钮 正常
    m_Button_Auto = static_cast<UIButton*>(m_Widget->getWidgetByName("Button_Auto"));
    m_Button_Auto->addTouchEventListener(this, toucheventselector(KCGameLayer::touchEvent));
    m_Button_Auto->setTag(BTN_Auto);
    //线按钮
    UIButton* tBtnLine = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Button_Xian"));
  	tBtnLine->addTouchEventListener(this, toucheventselector(KCGameLayer::touchEvent));
    tBtnLine->setTag(BTN_LINE);
    //倍数按钮
    UIButton* tBtnBei = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Button_Bei"));
  	tBtnBei->addTouchEventListener(this, toucheventselector(KCGameLayer::touchEvent));
    tBtnBei->setTag(BTN_BEI);
    //最大按钮
    UIButton* tBtnMax = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Button_Highest"));
  	tBtnMax->addTouchEventListener(this, toucheventselector(KCGameLayer::touchEvent));
    tBtnMax->setTag(BTN_HMAX);
    //back按钮
    m_BtnBack = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Button_Back"));
  	m_BtnBack->addTouchEventListener(this, toucheventselector(KCGameLayer::touchEvent));
    m_BtnBack->setTag(BTN_BACK);
    m_BtnBack->setOpacity(0);
    //战斗界面自动按钮
    m_BtnZD = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Button_ZD"));
  	m_BtnZD->addTouchEventListener(this, toucheventselector(KCGameLayer::touchEvent));
    m_BtnZD->setTag(BTN_ZD);

    //----------------------------战斗按钮相关-----------------------------
    //逃跑按钮
    UIButton* tBtnTP = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Button_TP"));
  	tBtnTP->addTouchEventListener(this, toucheventselector(KCGameLayer::touchEvent));
    tBtnTP->setTag(BTN_TP);
    //战斗go按钮
    tBtnFGO = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Button_FGo"));
  	tBtnFGO->addTouchEventListener(this, toucheventselector(KCGameLayer::touchEvent));
    tBtnFGO->setTag(BTN_FGO);
    
    //25个钻石
    for(int i = 1; i <= 25; i++)
    {
        sprintf(tmpStr, "Image_ZS1_%d", i);
        m_ZS[i] = dynamic_cast<UIImageView*>(m_Widget->getWidgetByName(tmpStr));
    }
    
    m_modelLineImg = static_cast<UIImageView*>(m_Widget->getWidgetByName("Image_Line"));

    m_PanelMid = static_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_MidZ"));
    
    //初始化25条线img
    for(int i = 0; i < 25; i++)
    {
        
        sprintf(tmpStr, "x-%04d.png", (i/5 + 1)*5);
        UIImageView * timg = UIImageView::create();
        timg->setVisible(false);
        timg->setPosition(m_modelLineImg->getPosition());
        timg->loadTexture(tmpStr, UI_TEX_TYPE_LOCAL);
        m_PanelMid->cocos2d::ui::Widget::addChild(timg);
        m_arrLineImg->addObject((CCObject*)timg);
    }
    
    //初始化战斗动画层
    m_pv = new PetFightLayer();
    m_pv->init();
    m_pv->setPetName(0);
    m_pv->setBackground(CH);
    m_pv->setGameScene(0);
    
    UIPanel* tmptopZ = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_TopZ"));
    tmptopZ->addNode(m_pv);
    tmptopZ->setClippingEnabled(true);
    m_pv->setPosition(ccp(tmptopZ->getContentSize().width/2, tmptopZ->getContentSize().height/2));
    
    //初始化比倍层
    m_double = DoubleLayer::create();
    m_double->retain();
    
    
    //初始化经验标签
    m_LabelPetExp = UILabelAtlas::create();
    m_LabelRoleExp = UILabelAtlas::create();
    m_LabelAtlasPetACK = UILabelAtlas::create();
    m_LabelAtlasBossACK = UILabelAtlas::create();
    m_LabelPetAddBlood = UILabelAtlas::create();
    m_LabelPetExp->setProperty("0", "FontGreen21_27.png", 21, 27, "0");
    m_LabelRoleExp->setProperty("0", "FontGreen21_27.png", 21, 27, "0");
    m_LabelAtlasPetACK->setProperty("0", "FontRed21_27.png", 21, 27, "0");
    m_LabelAtlasBossACK->setProperty("0", "FontRed21_27.png", 21, 27, "0");
    m_LabelPetAddBlood->setProperty("0", "FontGreen21_27.png", 21, 27, "0");
    m_LabelPetExp->setPosition(ccp(600, 320));
    m_LabelRoleExp->setPosition(ccp(200, 480));
    m_LabelAtlasPetACK->setPosition(ccp(300, 480));
    m_LabelAtlasBossACK->setPosition(ccp(700, 480));
    m_LabelPetAddBlood->setPosition(ccp(300, 480));
    m_LabelPetExp->setVisible(false);
    m_LabelRoleExp->setVisible(false);
    m_LabelAtlasPetACK->setVisible(false);
    m_LabelAtlasBossACK->setVisible(false);
    m_LabelPetAddBlood->setVisible(false);

    //开始战斗动画
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("zhandou.ExportJson");
    m_armatureStart = CCArmature::create("zhandou");
    m_armatureStart->setVisible(false);
    m_armatureStart->setPosition(ccp(500, 360));

    winL = CCSprite::create("winL.png");
    LoseL = CCSprite::create("loseL.png");
    GoL = CCSprite::create("goL.png");
    winL->setPosition(ccp(490, 450));
    LoseL->setPosition(ccp(490, 450));
    GoL->setPosition(ccp(490, 450));
    winL->setVisible(false);
    LoseL->setVisible(false);
    GoL->setVisible(false);
    
    this->addChild(m_Widget);
    this->addChild(m_LabelPetExp);
    this->addChild(m_LabelRoleExp);
    this->addChild(m_LabelAtlasPetACK);
    this->addChild(m_LabelAtlasBossACK);
    this->addChild(m_LabelPetAddBlood);
    this->addChild(m_armatureStart);
    this->addChild(winL);
    this->addChild(LoseL);
    this->addChild(GoL);
    this->setTouchEnabled(true);
    
    
    //根据数据重新初始化界面
    initUI();
    
    return true;
}


void KCGameLayer::touchEvent(CCObject* pSender, TouchEventType type)
{
    if(m_isAllThingsStop)
    {
        return;
    }
    
    if(!this->isVisible())
    {
        return;
    }
    
    if(!m_Widget->isVisible())
    {
        return;
    }
    
    switch (type)
    {
        case cocos2d::ui::TOUCH_EVENT_BEGAN:
        {
            playSound(BUTTON_CLICK);
            break;
        }
        case cocos2d::ui::TOUCH_EVENT_ENDED:
        {
            int tTag = ((UILayer*)pSender)->getTag();
            
            switch(tTag)
            {
                case BTN_GO:
                {
                    
                    if(!m_isCanTouchAllBtn)
                    {
                        return;
                    }
                    
                    char tmStr[50];
                    
                    if(DataManager::sharedDataManager()->currFreeNum > 0)
                    {
                        DataManager::sharedDataManager()->currFreeNum--;
                    }

                    
                    sprintf(tmStr, "%d", DataManager::sharedDataManager()->currFreeNum);
                    m_LabelAtlasFree->setStringValue(tmStr);
                    
                    UIPanel *pnf = static_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_Free"));
                    UIPanel *pnc = static_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_CostM"));
                    
                    if(DataManager::sharedDataManager()->currFreeNum > 0)
                    {
                        pnc->setVisible(false);
                        pnf->setVisible(true);
                    }
                    else
                    {
                        pnf->setVisible(false);
                        pnc->setVisible(true);
                    }

                    sprintf(tmStr, "ZD%d.png", imgid);
                    m_Panel_MoveCell00->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell01->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell10->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell11->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell20->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell21->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell30->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell31->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell40->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell41->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    
                    
                    
                    m_roleExp = 0;
                    stopRewardAction();
                    
                    unsigned long long tmpLNum = m_setLineRewNum*m_beiNum*m_baseGold;
                    
                    //金币足够的情况下发送消息
                    if(tmpLNum <= m_usrHavaGold)
                    {
                        
                        if(m_roomID == 2)
                        {
                            playSound(ROLL_EFFECT_EG);
                        }
                        else if(m_roomID == 3)
                        {
                            playSound(ROLL_EFFECT_US);
                        }
                        else if(m_roomID == 1)
                        {
                            playSound(ROLL_EFFECT_CH);
                        }

                        
                        runActionLHJ();
                        
                        GameLogicMeg2Sever tmpMeg1;
                        tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQGAMESTRT;
                        tmpMeg1.line = m_setLineRewNum;
                        tmpMeg1.times = m_beiNum;
                        tmpMeg1.autop = m_isCanAuto;
                        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
                        
                        m_isGoldEnougth = true;
                        
                    }
                    else
                    {
                        Alert::create(ALERTTEXT_GOLD)->show();
                        m_isGoldEnougth = false;
                    }
                    
                    break;
                }
                case BTN_FGO:
                {
                    if(!m_isCanTouchAllBtn)
                    {
                        return;
                    }
                    
                    playSound(ROLL_EFFECT_FIGHT);
                    
                    char tmStr[50];
                    sprintf(tmStr, "ZD%d.png", 0);
                    m_Panel_MoveCell00->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell01->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell10->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell11->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell20->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell21->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell30->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell31->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell40->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    m_Panel_MoveCell41->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
                    
                    //更新免费转动数字
                    m_Ffreenum -= 1;
                    
                    if(m_Ffreenum < 0)
                    {
                        m_Ffreenum = 0;
                    }
                    
                    sprintf(tmStr, "%d", m_Ffreenum);
                    m_LabelAtlasSY->setStringValue(tmStr);
                    
                    //转动动画
                    runActionLHJ();
                    
                    //发送战斗消息
                    GameLogicMeg2Sever tmpMeg1;
                    tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQPETFIGHT2;
                    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
                    
                    break;
                }
                case BTN_LINE:
                {
                    if(!m_isCanTouchAllBtn)
                    {
                        return;
                    }
                    
                    stopRewardAction();
                    
                    if(m_setLineRewNum == 1)
                    {
                        m_setLineRewNum = 3;
                    }
                    else if(m_setLineRewNum == 3)
                    {
                        m_setLineRewNum = 5;
                    }
                    else if(m_setLineRewNum == 5)
                    {
                        m_setLineRewNum = 10;
                    }
                    else if(m_setLineRewNum == 10)
                    {
                        m_setLineRewNum = 15;
                    }
                    else if(m_setLineRewNum == 15)
                    {
                        m_setLineRewNum = 25;
                    }
                    else if(m_setLineRewNum == 25)
                    {
                        m_setLineRewNum = 1;
                    }
                    
                    
                    if(m_setLineRewNum > m_maxLineRewNum)
                    {
                        m_setLineRewNum = 1;
                    }
                    
                    //压入金额大于玩家拥有金额返回
                    if(m_setLineRewNum*m_baseGold*m_beiNum > m_usrHavaGold)
                    {
                        m_setLineRewNum = 1;
                    }
                    
                    char tmStr[50];
                    //设置数字
                    sprintf(tmStr, "%d", m_setLineRewNum);
                    m_LabelAtlasLine->setStringValue(tmStr);
                    
                    unsigned long long tmpLNum = m_setLineRewNum*m_beiNum*m_baseGold;
                    
                    sprintf(tmStr, "%lld", tmpLNum);
                    m_LabelAtlasCost->setStringValue(tmStr);
                    
                    //开始画线----------------------------------------------
                    
//                    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB5A1);
                    
                    for (int i = 0; i < m_arrLineImg->count(); i++)
                    {
                        ((UIImageView*)m_arrLineImg->objectAtIndex(i))->setVisible(false);
                    }
                    
                    for(int i = 0; i < m_setLineRewNum; i++)
                    {
                        
                        int tnum = i*5 + 1;
                        
                        sprintf(tmStr, "x-%04d.png", tnum);
                        
                        CCLog("tmStr = %s", tmStr);
                        
                        ((UIImageView*)m_arrLineImg->objectAtIndex(i))->loadTexture(tmStr, UI_TEX_TYPE_LOCAL);
                        ((UIImageView*)m_arrLineImg->objectAtIndex(i))->setVisible(true);
                        
                    }
                    //画线完毕--------------------------------------------
                    
                    updateZS();
                    
                    break;
                }
                case BTN_BEI:
                {
                    if(!m_isCanTouchAllBtn)
                    {
                        return;
                    }
                    
                    m_beiNum++;
                    
                    if(m_beiNum > m_maxBeiNum)
                    {
                        m_beiNum = 1;
                    }
                    
                    //压入金额大于玩家拥有金额返回
                    if(m_setLineRewNum*m_baseGold*m_beiNum > m_usrHavaGold)
                    {
                        m_beiNum = 1;
                    }
                    
                    char tmStr[50];
                    //设置数字
                    sprintf(tmStr, "%d", m_beiNum);
                    m_LabelAtlasBei->setStringValue(tmStr);
                    
                    unsigned long long tmpLNum = m_setLineRewNum*m_beiNum*m_baseGold;
                    
                    sprintf(tmStr, "%lld", tmpLNum);
                    m_LabelAtlasCost->setStringValue(tmStr);
                    
                    break;
                }
                case BTN_Auto:
                {
                    stopRewardAction();
                    
                    if(m_isCanAuto)
                    {
                        m_isCanAuto = false;
                        m_Button_Auto->loadTextureNormal("BtnZD.png", UI_TEX_TYPE_PLIST);
                    }
                    else
                    {
                        m_isCanAuto = true;
                        m_Button_Auto->loadTextureNormal("BtnZDOn.png", UI_TEX_TYPE_PLIST);
                    }
                    
                    break;
                }
                case BTN_ZD:
                {
                    
                    if(m_isCanAuto)
                    {
                        m_isCanAuto = false;
                        m_BtnZD->loadTextureNormal("BtnZD.png", UI_TEX_TYPE_PLIST);
                    }
                    else
                    {
                        m_isCanAuto = true;
                        m_BtnZD->loadTextureNormal("BtnZDOn.png", UI_TEX_TYPE_PLIST);
                    }
                    
                    break;
                }
                case BTN_HMAX:
                {
                    if(!m_isCanTouchAllBtn)
                    {
                        return;
                    }
                    
                    stopRewardAction();
                    
                    if(m_baseGold == 0)
                    {
                        m_baseGold = 999999999;
                    }
                    
                    int tmpLineNum = m_usrHavaGold/m_baseGold;
                    
                    if(tmpLineNum == 0)
                    {
                        tmpLineNum = 1;
                    }
                    
                    if(tmpLineNum > m_maxLineRewNum)
                    {
                        tmpLineNum = m_maxLineRewNum;
                    }
                    
                    m_setLineRewNum = tmpLineNum;
                    
                    if(m_setLineRewNum == 0 || m_baseGold == 0)
                    {
                        m_setLineRewNum = 999999999;
                        m_baseGold = 999999999;
                    }
                    
                    int tmpBeiNum = m_usrHavaGold/(m_setLineRewNum*m_baseGold);
                    
                    if(tmpBeiNum == 0)
                    {
                        tmpBeiNum = 1;
                    }
                    
                    if(tmpBeiNum > m_maxBeiNum)
                    {
                        tmpBeiNum = m_maxBeiNum;
                    }
                    
                    m_beiNum = tmpBeiNum;
                    
                    char tmStr[50];
                    //设置数字
                    sprintf(tmStr, "%d", m_setLineRewNum);
                    m_LabelAtlasLine->setStringValue(tmStr);
                    
                    sprintf(tmStr, "%d", m_beiNum);
                    m_LabelAtlasBei->setStringValue(tmStr);
                    
                    unsigned long long tmpLNum = m_setLineRewNum*m_beiNum*m_baseGold;
                    
                    sprintf(tmStr, "%lld", tmpLNum);
                    m_LabelAtlasCost->setStringValue(tmStr);
                    
                    if(tmpLNum <= m_usrHavaGold)
                    {
                        
                        if(m_roomID == 2)
                        {
                            playSound(ROLL_EFFECT_EG);
                        }
                        else if(m_roomID == 3)
                        {
                            playSound(ROLL_EFFECT_US);
                        }
                        else if(m_roomID == 1)
                        {
                            playSound(ROLL_EFFECT_CH);
                        }
                        
                        runActionLHJ();
                        
                        GameLogicMeg2Sever tmpMeg1;
                        tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQGAMESTRT;
                        tmpMeg1.line = m_setLineRewNum;
                        tmpMeg1.times = m_beiNum;
                        tmpMeg1.autop = m_isCanAuto;
                        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
                        
                        m_isGoldEnougth = true;
                    }
                    else
                    {
                        Alert::create(ALERTTEXT_GOLD)->show();
                        m_isGoldEnougth = false;
                    }
                    
                    updateZS();
                    
                    break;
                }
                case BTN_TP:
                {
                    if(!m_isCanTouchAllBtn)
                    {
                        return;
                    }
                    
                    GameLogicMeg2Sever tmpMeg1;
                    tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQPETFLEE;
                    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
                    m_page = 1;//回到老虎机界面
                    break;
                }
                case BTN_BACK:
                {
                    if(!m_isCanTouchAllBtn)
                    {
                        return;
                    }
                    
                    if(!m_BtnBack->isVisible())
                    {
                        return;
                    }
                    
                    stopRewardAction();
                    
                    GameLogicMeg2Sever tmpMeg1;
                    tmpMeg1.m_id = OGID_TEXAS_SLOTS_EXITGAME;
                    tmpMeg1.msgtyoe = 1;
                    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
                    
                    
                    Meg2UIDate sendmeg;
                    sendmeg.m_id = GAME_LAYER_BACK;
                    
                    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
                    
                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        default:
            break;
    }
    
}

void KCGameLayer::set2PanelVisitTrue0()
{
	m_Panel_MoveCell00->setVisible(true);
	m_Panel_MoveCell01->setVisible(true);
	m_Panel_Move0->setPosition(ccp(0, -POS_Y_MOVE));
    
	this->schedule(schedule_selector(KCGameLayer::updatePanelCell0), 0.0f);
}

void KCGameLayer::set2PanelVisitTrue1()
{
	m_Panel_MoveCell10->setVisible(true);
	m_Panel_MoveCell11->setVisible(true);
	m_Panel_Move1->setPosition(ccp(0, -POS_Y_MOVE));
    
	this->schedule(schedule_selector(KCGameLayer::updatePanelCell1), 0.0f);
}

void KCGameLayer::set2PanelVisitTrue2()
{
	m_Panel_MoveCell20->setVisible(true);
	m_Panel_MoveCell21->setVisible(true);
	m_Panel_Move2->setPosition(ccp(0, -POS_Y_MOVE));
    
	this->schedule(schedule_selector(KCGameLayer::updatePanelCell2), 0.0f);
}

void KCGameLayer::set2PanelVisitTrue3()
{
	m_Panel_MoveCell30->setVisible(true);
	m_Panel_MoveCell31->setVisible(true);
	m_Panel_Move3->setPosition(ccp(0, -POS_Y_MOVE));
    
	this->schedule(schedule_selector(KCGameLayer::updatePanelCell3), 0.0f);
}

void KCGameLayer::set2PanelVisitTrue4()
{
	m_Panel_MoveCell40->setVisible(true);
	m_Panel_MoveCell41->setVisible(true);
	m_Panel_Move4->setPosition(ccp(0, -POS_Y_MOVE));
    
	this->schedule(schedule_selector(KCGameLayer::updatePanelCell4), 0.0f);
}

void KCGameLayer::updatePanelCell0()
{
	CCPoint tP0 = m_Panel_MoveCell00->getPosition();
	CCPoint tP1 = m_Panel_MoveCell01->getPosition();
    
	tP0.y = tP0.y - POS_CELL_MOVE_SPEED;
	tP1.y = tP1.y - POS_CELL_MOVE_SPEED;
    
	if(tP0.y <= -POS_CELL_HEIGHT)
	{
		tP0.y = tP1.y + m_Panel_MoveCell00->getContentSize().height;
	}
    
	if(tP1.y <= -POS_CELL_HEIGHT)
	{
		tP1.y = tP0.y + m_Panel_MoveCell01->getContentSize().height;
	}
    
	m_Panel_MoveCell00->setPosition(tP0);
	m_Panel_MoveCell01->setPosition(tP1);
    
    //如果收到消息，那么可以停止转动了，标志位＋＋
    if(m_isCanStopLHJ)
    {
        m_iTimeFlag++;
    }
    
    //更新图片
    if(m_isCanStopLHJ && m_isOnceUpDateImg)
    {
        m_isOnceUpDateImg = false;
        updateCellImg();
    }
    
	if(TIME_SCROLL == m_iTimeFlag)
	{
		this->unschedule(schedule_selector(KCGameLayer::updatePanelCell0));
		m_Panel_MoveCell00->setVisible(false);
		m_Panel_MoveCell01->setVisible(false);
		m_Panel_MoveCell00->setPosition(m_arrPoint0[0]);
		m_Panel_MoveCell01->setPosition(m_arrPoint1[0]);
        
		m_Panel_Move0->setVisible(true);
		m_Panel_Move0->runAction(CCSequence::create(CCMoveTo::create(POS_Y_MOVE_TIME, ccp(0, 0)),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::setFlag0)),
                                                    NULL));
	}
}

void KCGameLayer::updatePanelCell1()
{
	CCPoint tP0 = m_Panel_MoveCell10->getPosition();
	CCPoint tP1 = m_Panel_MoveCell11->getPosition();
    
	tP0.y = tP0.y - POS_CELL_MOVE_SPEED;
	tP1.y = tP1.y - POS_CELL_MOVE_SPEED;
    
	if(tP0.y <= -POS_CELL_HEIGHT)
	{
		tP0.y = tP1.y + m_Panel_MoveCell10->getContentSize().height;
	}
    
	if(tP1.y <= -POS_CELL_HEIGHT)
	{
		tP1.y = tP0.y + m_Panel_MoveCell11->getContentSize().height;
	}
    
	m_Panel_MoveCell10->setPosition(tP0);
	m_Panel_MoveCell11->setPosition(tP1);
    
	if(TIME_FLAG0 == m_iTimeFlag)
	{
		this->unschedule(schedule_selector(KCGameLayer::updatePanelCell1));
		m_Panel_MoveCell10->setVisible(false);
		m_Panel_MoveCell11->setVisible(false);
		m_Panel_MoveCell10->setPosition(m_arrPoint0[1]);
		m_Panel_MoveCell11->setPosition(m_arrPoint1[1]);
        
		m_Panel_Move1->setVisible(true);
		m_Panel_Move1->runAction(CCSequence::create(CCMoveTo::create(POS_Y_MOVE_TIME, ccp(0, 0)),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::setFlag1)),
                                                    NULL));
	}
}

void KCGameLayer::updatePanelCell2()
{
	CCPoint tP0 = m_Panel_MoveCell20->getPosition();
	CCPoint tP1 = m_Panel_MoveCell21->getPosition();
    
	tP0.y = tP0.y - POS_CELL_MOVE_SPEED;
	tP1.y = tP1.y - POS_CELL_MOVE_SPEED;
    
	if(tP0.y <= -POS_CELL_HEIGHT)
	{
		tP0.y = tP1.y + m_Panel_MoveCell20->getContentSize().height;
	}
    
	if(tP1.y <= -POS_CELL_HEIGHT)
	{
		tP1.y = tP0.y + m_Panel_MoveCell21->getContentSize().height;
	}
    
	m_Panel_MoveCell20->setPosition(tP0);
	m_Panel_MoveCell21->setPosition(tP1);
    
	if(TIME_FLAG1 == m_iTimeFlag)
	{
		this->unschedule(schedule_selector(KCGameLayer::updatePanelCell2));
		m_Panel_MoveCell20->setVisible(false);
		m_Panel_MoveCell21->setVisible(false);
		m_Panel_MoveCell20->setPosition(m_arrPoint0[2]);
		m_Panel_MoveCell21->setPosition(m_arrPoint1[2]);
        
		m_Panel_Move2->setVisible(true);
		m_Panel_Move2->runAction(CCSequence::create(CCMoveTo::create(POS_Y_MOVE_TIME, ccp(0, 0)),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::setFlag2)),
                                                    NULL));
	}
}

void KCGameLayer::updatePanelCell3()
{
	CCPoint tP0 = m_Panel_MoveCell30->getPosition();
	CCPoint tP1 = m_Panel_MoveCell31->getPosition();
    
	tP0.y = tP0.y - POS_CELL_MOVE_SPEED;
	tP1.y = tP1.y - POS_CELL_MOVE_SPEED;
    
	if(tP0.y <= -POS_CELL_HEIGHT)
	{
		tP0.y = tP1.y + m_Panel_MoveCell30->getContentSize().height;
	}
    
	if(tP1.y <= -POS_CELL_HEIGHT)
	{
		tP1.y = tP0.y + m_Panel_MoveCell31->getContentSize().height;
	}
    
	m_Panel_MoveCell30->setPosition(tP0);
	m_Panel_MoveCell31->setPosition(tP1);
    
	if(TIME_FLAG2 == m_iTimeFlag)
	{
		this->unschedule(schedule_selector(KCGameLayer::updatePanelCell3));
		m_Panel_MoveCell30->setVisible(false);
		m_Panel_MoveCell31->setVisible(false);
		m_Panel_MoveCell30->setPosition(m_arrPoint0[3]);
		m_Panel_MoveCell31->setPosition(m_arrPoint1[3]);
        
		m_Panel_Move3->setVisible(true);
		m_Panel_Move3->runAction(CCSequence::create(CCMoveTo::create(POS_Y_MOVE_TIME, ccp(0, 0)),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::setFlag3)),
                                                    NULL));
	}
}

void KCGameLayer::updatePanelCell4()
{
	CCPoint tP0 = m_Panel_MoveCell40->getPosition();
	CCPoint tP1 = m_Panel_MoveCell41->getPosition();
    
	tP0.y = tP0.y - POS_CELL_MOVE_SPEED;
	tP1.y = tP1.y - POS_CELL_MOVE_SPEED;
    
	if(tP0.y <= -POS_CELL_HEIGHT)
	{
		tP0.y = tP1.y + m_Panel_MoveCell40->getContentSize().height;
	}
    
	if(tP1.y <= -POS_CELL_HEIGHT)
	{
		tP1.y = tP0.y + m_Panel_MoveCell41->getContentSize().height;
	}
    
	m_Panel_MoveCell40->setPosition(tP0);
	m_Panel_MoveCell41->setPosition(tP1);
    
	if(TIME_FLAG3 == m_iTimeFlag)
	{
		this->unschedule(schedule_selector(KCGameLayer::updatePanelCell4));
		m_Panel_MoveCell40->setVisible(false);
		m_Panel_MoveCell41->setVisible(false);
		m_Panel_MoveCell40->setPosition(m_arrPoint0[4]);
		m_Panel_MoveCell41->setPosition(m_arrPoint1[4]);
        
		m_Panel_Move4->setVisible(true);
		m_Panel_Move4->runAction(CCSequence::create(CCMoveTo::create(POS_Y_MOVE_TIME, ccp(0, 0)),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::setFlag4)),
                                                    NULL));
	}
}

void KCGameLayer::setFlag0()
{
	m_iTimeFlag = TIME_FLAG0;
}

void KCGameLayer::setFlag1()
{
	m_iTimeFlag = TIME_FLAG1;
}

void KCGameLayer::setFlag2()
{
	m_iTimeFlag = TIME_FLAG2;
}

void KCGameLayer::setFlag3()
{
	m_iTimeFlag = TIME_FLAG3;
}

void KCGameLayer::setFlag4()//转动停止后的动作在此调用
{
    m_isCanStopLHJ = false;
    m_isOnceUpDateImg = true;
	m_iTimeFlag = TIME_FLAG4;
    
//	m_isCanGo = true;
//	m_isCanTouchAllBtn = true;
    
    if(m_pageCurr == 1)
    {
        this->runAction(CCSequence::create(CCDelayTime::create(1.0f),
                                           CCCallFunc::create(this, callfunc_selector(KCGameLayer::setAllFlagTrue)),
                                           NULL));
        
        m_pv->stop();
        
        if(m_roleExp > 0)
        {
            char tmpStr[50];
            sprintf(tmpStr, ":%lld", m_roleExp);
            m_LabelRoleExp->setVisible(true);
            m_LabelRoleExp->setStringValue(tmpStr);
            
            float posY = m_LabelRoleExp->getPositionY();
            
            m_LabelRoleExp->runAction(CCMoveTo::create(0.5f, ccp(m_LabelRoleExp->getPositionX(), posY + 50)));
            
            m_LabelRoleExp->runAction(CCSequence::create(CCFadeOut::create(0.5f),
                                                        CCCallFunc::create(this, callfunc_selector(KCGameLayer::setLabelRoleExp)),
                                                        NULL));
        }
    }
    else if(m_pageCurr == 4)
    {
        this->runAction(CCSequence::create(CCDelayTime::create(2.6f),
                                           CCCallFunc::create(this, callfunc_selector(KCGameLayer::setAllFlagTrue)),
                                           NULL));
        
        m_ActionFightLogicFlag = 0;
        this->schedule(schedule_selector(KCGameLayer::runFightAction), 0.0f);
    }
    else
    {
        this->runAction(CCSequence::create(CCDelayTime::create(1.0f),
                                           CCCallFunc::create(this, callfunc_selector(KCGameLayer::setAllFlagTrue)),
                                           NULL));
    }
    
    char strNum[20];
    sprintf(strNum, "%lld", m_winGold);
    m_LabelAtlasWin->setStringValue(strNum);
	
    if(m_id4Reward.size() > 0)
	{
		m_isCanDraw = true;
        showLine(m_isCanDraw);
	}
}

void KCGameLayer::setVisitFalse(CCObject * pSender)
{
	((UIWidget*)pSender)->setVisible(false);
	m_iChangeImgFlag++;
    
//    CCLog("in update img~~~~~~~=%d", m_iChangeImgFlag);
    
	if(5 == m_iChangeImgFlag)
	{
		m_iChangeImgFlag = 0;
	}
}

void KCGameLayer::updateCellImg()
{
	char strTmp[50];
    
//    CCLog("img~~~~~~~~~~~~~~~~~~~~~~~~");
//    CCLog("%d, %d, %d ,%d, %d",
//          m_arrCellsDate[0],
//          m_arrCellsDate[1],
//          m_arrCellsDate[2],
//          m_arrCellsDate[3],
//          m_arrCellsDate[4]);
//    
//    CCLog("%d, %d, %d ,%d, %d",
//          m_arrCellsDate[5],
//          m_arrCellsDate[6],
//          m_arrCellsDate[7],
//          m_arrCellsDate[8],
//          m_arrCellsDate[9]);
//    
//    CCLog("%d, %d, %d ,%d, %d",
//          m_arrCellsDate[10],
//          m_arrCellsDate[11],
//          m_arrCellsDate[12],
//          m_arrCellsDate[13],
//          m_arrCellsDate[14]);

    
	for(int i = 0; i < 15; i++)
	{
		sprintf(strTmp, "Image_Img4Cell%d", i);
		UIImageView* tCellImg = dynamic_cast<UIImageView*>(m_Widget->getWidgetByName(strTmp));
        
        int img = m_roomID;
        
        if(m_roomID == 2)
        {
            img = 2;
        }
        else if(m_roomID == 3)
        {
            img = 1;
        }
        else if(m_roomID == 1)
        {
            img = 3;
        }
        
        if(m_pageCurr == 4)
        {
            img = 0;
        }
        
		sprintf(strTmp, "c%d%d.png", img, m_arrCellsDate[i]);
        
        tCellImg->loadTexture(strTmp, UI_TEX_TYPE_PLIST);//setTexture(strTmp, UI_TEX_TYPE_PLIST);
        
	}
    
}

void KCGameLayer::updateCellsDate()
{
    
//    CCLog("%d, %d, %d ,%d, %d",
//          m_arrCellsDate[0],
//          m_arrCellsDate[1],
//          m_arrCellsDate[2],
//          m_arrCellsDate[3],
//          m_arrCellsDate[4]);
//    
//    CCLog("%d, %d, %d ,%d, %d",
//          m_arrCellsDate[5],
//          m_arrCellsDate[6],
//          m_arrCellsDate[7],
//          m_arrCellsDate[8],
//          m_arrCellsDate[9]);
//    
//    CCLog("%d, %d, %d ,%d, %d",
//          m_arrCellsDate[10],
//          m_arrCellsDate[11],
//          m_arrCellsDate[12],
//          m_arrCellsDate[13],
//          m_arrCellsDate[14]);
//    
//    CCLog("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

    //对0的图片进行防止图片重复处理
    std::vector<int> noRepeat;//存储重复容器
    std::vector<int> vid;//存储可用id
    
    for(int i = 0; i < 15; i++)
    {
        noRepeat.clear();
        
        if(0 == m_arrCellsDate[i])
        {
            //右边不能超出矩阵
            if((i + 1) < (i/5 + 1)*5 && (i + 1) < 15
               && (i == 0 || i == 5 || i == 10))
            {
                //右边不为0将右边的id存入不重复数组
                if(m_arrCellsDate[i + 1] != 0)
                {
                    noRepeat.push_back(m_arrCellsDate[i + 1]);
                }
            }
//            //上边不能超出矩阵
//            if((i - 5) >= 0)
//            {
//                if(m_arrCellsDate[i - 5] != 0)
//                {
//                    noRepeat.push_back(m_arrCellsDate[i - 5]);
//                }
//            }
            //左边不能超出矩阵
            if((i - 1) >= (i/5)*5 && (i - 1) >= 0)
            {
                if(m_arrCellsDate[i - 1] != 0)
                {
                    noRepeat.push_back(m_arrCellsDate[i - 1]);
                }
            }
//            //下边不能超出矩阵
//            if((i + 5) < 15)
//            {
//                if(m_arrCellsDate[i + 5] != 0)
//                {
//                    noRepeat.push_back(m_arrCellsDate[i + 5]);
//                }
//            }
            
            //-----------------
            //右上边不能超出矩阵 右边+上边判断组合
            if((i + 1) < (i/5 + 1)*5 && (i + 1) < 15 && (i - 5) >= 0
               && (i == 0 || i == 5 || i == 10))
            {
                //不为0将右边的id存入不重复数组
                if(m_arrCellsDate[i + 1 - 5] != 0)
                {
                    noRepeat.push_back(m_arrCellsDate[i + 1 - 5]);
                }
            }
            //右下边不能超出矩阵 右边+下边边判断组合
            if((i + 1) < (i/5 + 1)*5 && (i + 1) < 15 && (i + 5) < 15
               && (i == 0 || i == 5 || i == 10))
            {
                //不为0将右边的id存入不重复数组
                if(m_arrCellsDate[i + 1 + 5] != 0)
                {
                    noRepeat.push_back(m_arrCellsDate[i + 1 + 5]);
                }
            }
            //----------------
            //左上边不能超出矩阵 左边+上边判断组合
            if((i - 1) >= (i/5)*5 && (i - 1) >= 0 && (i - 5) >= 0)
            {
                //不为0将右边的id存入不重复数组
                if(m_arrCellsDate[i - 1 - 5] != 0)
                {
                    noRepeat.push_back(m_arrCellsDate[i - 1 - 5]);
                }
            }
            //左下边不能超出矩阵 左边+下边判断组合
            if((i - 1) >= (i/5)*5 && (i - 1) >= 0 && (i + 5) < 15)
            {
                //不为0将右边的id存入不重复数组
                if(m_arrCellsDate[i - 1 + 5] != 0)
                {
                    noRepeat.push_back(m_arrCellsDate[i - 1 + 5]);
                }
            }
            
            //id 1-8中遍历,取出与noRepeat中不相等的数作为当前数组id
            bool iscanSet = true;
            vid.clear();
            
            int maxCell = 8;//
            //宠物战斗界面随机5个cell
            if(m_pageCurr == 1)
            {
                maxCell = 8;
            }
            else if(m_pageCurr == 4)
            {
                maxCell = 5;
            }
            
            for(int j = 1; j <= maxCell; j++)
            {
                iscanSet = true;
                
                for(int norep = 0; norep < noRepeat.size(); norep ++)
                {
                    if(j == noRepeat[norep])
                    {
                        iscanSet = false;
                        break;
                    }
                }
                
                if(iscanSet)
                {
                    vid.push_back(j);
                }
                
            }
            
            //随机从可使用id容器里取一个
            int max = vid.size() - 1;
            int min = 0;
            int rd = rand() % (max - min + 1) + min;
            m_arrCellsDate[i] = vid[rd];
            
        }
    }
    
//    CCLog("%d, %d, %d ,%d, %d",
//          m_arrCellsDate[0],
//          m_arrCellsDate[1],
//          m_arrCellsDate[2],
//          m_arrCellsDate[3],
//          m_arrCellsDate[4]);
//    
//    CCLog("%d, %d, %d ,%d, %d",
//          m_arrCellsDate[5],
//          m_arrCellsDate[6],
//          m_arrCellsDate[7],
//          m_arrCellsDate[8],
//          m_arrCellsDate[9]);
//    
//    CCLog("%d, %d, %d ,%d, %d",
//          m_arrCellsDate[10],
//          m_arrCellsDate[11],
//          m_arrCellsDate[12],
//          m_arrCellsDate[13],
//          m_arrCellsDate[14]);
//    
//    CCLog("----------------------------------");
    
    /*test new------
	int max = 8;
	int min = 1;
	int rd = 0;
    
	for(int i = 0; i < 15; i++)
	{
		rd = rand() % (max - min + 1) + min;
		m_arrCellsDate[i] = rd;
	}
    */
    
}

void KCGameLayer::mangerCellsDate()
{
	int tnum = 0;//连续cell节点数目
	m_id4Reward.clear();
    
	for(int i = 0; i < m_setLineRewNum; i++)
	{
        
		tnum = 0;
        
		for(int j = 0; j < 5; j++)
		{
            
			if(m_arrCellsDate[m_p4r[i].m_iCellID[0]] != m_arrCellsDate[m_p4r[i].m_iCellID[j]])
			{
				break;
			}
            
			tnum++;
		}
        
		//如果中奖了把中奖id存在容器中
		if(tnum > 1)
		{
			Date4ReWard tmpDate4ReWard;
			tmpDate4ReWard.m_id = i;
			tmpDate4ReWard.m_num = tnum;
			m_id4Reward.push_back(tmpDate4ReWard);
            
		}
	}
}

void KCGameLayer::initPointReward(int id, int CellID[5])
{
	for(int i = 0; i < 5; i++)
	{
		m_p4r[id].m_iCellID[i] = CellID[i];
	}
}

void KCGameLayer::showLine(bool iscandraw)
{
    if(iscandraw)
	{
//        CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_A8);
        
        char tmStr[50];
        
        for (int i = 0; i < m_arrLineImg->count(); i++)
        {
            ((UIImageView*)m_arrLineImg->objectAtIndex(i))->setVisible(false);
        }
		
		for(int i = 0; i < m_id4Reward.size(); i++)
		{
            
            int tnum = m_id4Reward[i].m_id*5 + m_id4Reward[i].m_num;
            
            sprintf(tmStr, "x-%04d.png", tnum);
            
//            CCLog("tmStr = %s", tmStr);
            
            ((UIImageView*)m_arrLineImg->objectAtIndex(m_id4Reward[i].m_id))->loadTexture(tmStr, UI_TEX_TYPE_LOCAL);
            ((UIImageView*)m_arrLineImg->objectAtIndex(m_id4Reward[i].m_id))->setVisible(true);
            
		}
        
	}
    else
    {
        for (int i = 0; i < m_arrLineImg->count(); i++)
        {
            ((UIImageView*)m_arrLineImg->objectAtIndex(i))->setVisible(false);
        }
    }
    
}

void KCGameLayer::runActionLHJ()
{
    if(m_isCanGo)
    {
        
        if(m_pageCurr == 1)
        {
            m_pv->petPlay(WALK);
            
            DataManager::sharedDataManager()->petCurrGold += m_setLineRewNum*m_beiNum*m_baseGold;
            CCLog("c=%lld", DataManager::sharedDataManager()->petCurrGold);
            CCLog("n=%lld", DataManager::sharedDataManager()->bossNeedGold);
            
            float p = float(DataManager::sharedDataManager()->petCurrGold)/float(DataManager::sharedDataManager()->bossNeedGold);
            if(p > 1)
            {
                p = 1;
            }
            
            int dis = 700*p;
            
            m_pv->movePet(dis);
        }
        else if(m_pageCurr == 4)
        {
            
        }
        
        m_isCanDraw = false;
        showLine(m_isCanDraw);
        m_isCanTouchAllBtn = false;
        m_isCanGo = false;
        
        m_Panel_Move0->runAction(CCSequence::create(CCMoveTo::create(POS_Y_MOVE_TIME, ccp(0, POS_Y_MOVE)),
                                                    CCCallFuncO::create(this, callfuncO_selector(KCGameLayer::setVisitFalse), m_Panel_Move0),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::set2PanelVisitTrue0)),
                                                    NULL));
        
        m_Panel_Move1->runAction(CCSequence::create(CCMoveTo::create(POS_Y_MOVE_TIME, ccp(0, POS_Y_MOVE)),
                                                    CCCallFuncO::create(this, callfuncO_selector(KCGameLayer::setVisitFalse), m_Panel_Move1),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::set2PanelVisitTrue1)),
                                                    NULL));
        
        m_Panel_Move2->runAction(CCSequence::create(CCMoveTo::create(POS_Y_MOVE_TIME, ccp(0, POS_Y_MOVE)),
                                                    CCCallFuncO::create(this, callfuncO_selector(KCGameLayer::setVisitFalse), m_Panel_Move2),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::set2PanelVisitTrue2)),
                                                    NULL));
        
        m_Panel_Move3->runAction(CCSequence::create(CCMoveTo::create(POS_Y_MOVE_TIME, ccp(0, POS_Y_MOVE)),
                                                    CCCallFuncO::create(this, callfuncO_selector(KCGameLayer::setVisitFalse), m_Panel_Move3),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::set2PanelVisitTrue3)),
                                                    NULL));
        
        m_Panel_Move4->runAction(CCSequence::create(CCMoveTo::create(POS_Y_MOVE_TIME, ccp(0, POS_Y_MOVE)),
                                                    CCCallFuncO::create(this, callfuncO_selector(KCGameLayer::setVisitFalse), m_Panel_Move4),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::set2PanelVisitTrue4)),
                                                    NULL));
        
    }
    
}

void KCGameLayer::recGameLogicEventFromSever(CCObject * obj)
{
    Meg2UIDate * tmeg = ((Meg2UIDate*)obj);
    int tid = tmeg->m_id;
    
    switch (tid)
    {
        case OGID_TEXAS_SLOTS_GAMERESULT://
        {
            CCLog("in game OGID_TEXAS_SLOTS_GAMERESULT");
            
            for(int i = 0; i < 15; i++)
            {
                m_arrCellsDate[i] = tmeg->grid[i];
            }
            
            CCLog("中奖结果~~~~~~~~~~~=%d", tmeg->Rret);
            CCLog("page~~~~~~~~=%d", tmeg->page);
            CCLog("宠物经验~~~~~~~~=%lld", tmeg->getPetExp);
            CCLog("任务经验~~~~~~~~=%lld", tmeg->getRoleExp);
            CCLog("curr page~~~~~~~~=%d", m_pageCurr);
        
            m_page = tmeg->page;
            m_winGold = tmeg->wingold;
            m_roleExp = tmeg->getRoleExp;
            updateCellsDate();
            mangerCellsDate();
            m_isCanStopLHJ = true;
            
            if(tmeg->getPetExp != 0)
            {
                char tmpStr[50];
                sprintf(tmpStr, ":%lld", tmeg->getPetExp);
                m_LabelPetExp->setVisible(true);
                m_LabelPetExp->setStringValue(tmpStr);
                
                float posY = m_LabelPetExp->getPositionY();
                
                m_LabelPetExp->runAction(CCMoveTo::create(0.5f, ccp(m_LabelPetExp->getPositionX(), posY + 50)));
                
                m_LabelPetExp->runAction(CCSequence::create(CCFadeOut::create(0.5f),
                                                            CCCallFunc::create(this, callfunc_selector(KCGameLayer::setLabelPetExp)),
                                                            NULL));
                
            }
            
            break;
		}
        case OGID_TEXAS_SLOTS_FIGHTRESULT:
        {
            CCLog("in game OGID_TEXAS_SLOTS_FIGHTRESULT");
            
            CCLog("战斗结果~~~~~~~~~~~=%d", tmeg->Fwinret);
            CCLog("中线~~~~~~~~~~~=%d", tmeg->Fret);
            CCLog("几连~~~~~~~~~~~=%d", tmeg->FpetJoinNum);
            CCLog("技能~~~~~~~~~~~=%d", tmeg->FpetSkill);
            CCLog("page~~~~~~~~=%d", tmeg->Fpage);
            
            m_FlianNum = tmeg->FpetJoinNum;
            m_FSkill = tmeg->FpetSkill;
            
            //每次战斗结束都要更新boss最大血量
            //虽然不是实时更新，但是有更新肯定会改变
            bossMaxBlood = DataManager::sharedDataManager()->bossBloodMax;
            petMaxBlood = DataManager::sharedDataManager()->petblood;
            
            for(int i = 0; i < 15; i++)
            {
                m_arrCellsDate[i] = 0;
            }

            if(tmeg->Fwinret == 0)//zd
            {
                
                petAttack = tmeg->FpetACK;
                bossAttack = tmeg->FbossACK;

                
                if(tmeg->Fret == 1)//中线
                {
                    int max = 2;
                    int min = 0;
                    int rd = rand() % (max - min + 1) + min;
                    
                    int tmpNum = 0;
                    
                    for(int i = rd*5; i < rd*5 + 5; i++)
                    {
                        if(tmpNum == tmeg->FpetJoinNum)
                        {
                            break;
                        }
                        
                        m_arrCellsDate[i] = tmeg->FpetSkill;
                        tmpNum++;
                    }
                    
                }
                else if(tmeg->Fret == -1)//金币不足
                {
                    Alert::create(ALERTTEXT_GOLD)->show();
                    m_isGoldEnougth = false;
                }
                else if (tmeg->Fret == 0)//未中线
                {
                    for(int i = 0; i < 15; i++)
                    {
                        m_arrCellsDate[i] = 0;
                    }
                }
            }
            else if(tmeg->Fwinret == 1)//pet win
            {
                m_winret = tmeg->Fwinret;
                m_isAllThingsStop = true;
                
            }
            else if(tmeg->Fwinret == 2)//boss win
            {
                m_winret = tmeg->Fwinret;
                m_isAllThingsStop = true;
                
            }
            
            m_page = tmeg->Fpage;////////
            
            updateCellsDate();
            mangerCellsDate();
            m_isCanStopLHJ = true;
            
            break;
        }
        case OGID_TEXAS_SLOTS_COLLECTION:
        {
            CCLog("in game OGID_TEXAS_SLOTS_COLLECTION");
            m_page = tmeg->Bpage;
        
            DataManager::sharedDataManager()->currGold = tmeg->Brolegold;
            Meg2UIDate sendmeg;
            sendmeg.m_id = OGID_TEXAS_SLOTS_PLAYERINFO;
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            
            break;
        }
        case SET_ALL_THINGS_RUN:
        {
            m_isAllThingsStop = false;
            break;
        }
        default:
        {
            CCLog("No this Message ID in GAME!!!!!");
            break;
        };
    }
}

void KCGameLayer::initUI()
{
    m_winret = 0;
    m_isGoldEnougth = true;
    m_setLineRewNum = 1;
    m_pv->setPetName(DataManager::sharedDataManager()->petID);
    
    m_FlianNum = 0;
    m_FSkill = 0;
    m_armatureStart->setVisible(false);
    m_armatureStart->stopAllActions();
    
    setLabelPetExp();
    setLabelRoleExp();
    
    this->scheduleUpdate();
    showLine(false);
    
    bossCurrBlood = DataManager::sharedDataManager()->boosBlood;
    bossMaxBlood = DataManager::sharedDataManager()->bossBloodMax;
    petCurrBlood = DataManager::sharedDataManager()->petblood;
    petMaxBlood = DataManager::sharedDataManager()->petblood;
    petAttack = 1;
    bossAttack = 1;

    
    m_roomID = DataManager::sharedDataManager()->roomID;
    m_maxLineRewNum = DataManager::sharedDataManager()->maxLine;
    m_maxBeiNum = DataManager::sharedDataManager()->maxBei;
    m_baseGold = DataManager::sharedDataManager()->roomBaseGold;
    m_usrHavaGold = DataManager::sharedDataManager()->currGold;
    m_isCanfreeOn = DataManager::sharedDataManager()->freeOn;
    
    m_pageCurr = 1;
    m_page = 1;
    m_isCanGo = true;
    m_isCanTouchAllBtn = true;
    m_isCanDraw = false;
    m_iTimeFlag = 0;
    m_iChangeImgFlag = 0;
    m_id4Reward.clear();
    m_isAllThingsStop = false;
    m_isOnceUpDateImg = true;
    m_roleExp = 0;
    m_ActionFightLogicFlag = 0;
    m_ActionStartLogicFlag = 0;
    
    imgid = 3;
    
    if(m_roomID == 1)
    {
        m_pv->setBackground(CH);
        imgid = 3;
    }
    else if(m_roomID == 2)
    {
        m_pv->setBackground(EG);
        imgid = 2;
    }
    else if(m_roomID == 3)
    {
        m_pv->setBackground(US);
        imgid = 1;
    }
    
    for(int i = 0; i < DataManager::sharedDataManager()->roomBetList.size(); i++)
    {
        if(m_roomID == 1)
        {
            if(m_usrHavaGold >= DataManager::sharedDataManager()->roomBetList[i].min
               && m_usrHavaGold < DataManager::sharedDataManager()->roomBetList[i].max)
            {
                m_beiNum = DataManager::sharedDataManager()->roomBetList[i].bet;
            }
        }
        else if(m_roomID == 2)
        {
            if(m_usrHavaGold >= DataManager::sharedDataManager()->roomBetList[i].min
               && m_usrHavaGold < DataManager::sharedDataManager()->roomBetList[i].max)
            {
                m_beiNum = DataManager::sharedDataManager()->roomBetList[i].bet;
            }
            
        }
        else if(m_roomID == 3)
        {
            if(m_usrHavaGold >= DataManager::sharedDataManager()->roomBetList[i].min
               && m_usrHavaGold < DataManager::sharedDataManager()->roomBetList[i].max)
            {
                m_beiNum = DataManager::sharedDataManager()->roomBetList[i].bet;
            }

        }
        
    }
    
    char tmStr[50];
    //设置数字
    sprintf(tmStr, "%d", m_setLineRewNum);
    m_LabelAtlasLine->setStringValue(tmStr);
    
    sprintf(tmStr, "%d", m_beiNum);
    m_LabelAtlasBei->setStringValue(tmStr);
    
    unsigned long long tmpLNum = m_setLineRewNum*m_beiNum*m_baseGold;
    sprintf(tmStr, "%lld", tmpLNum);
    m_LabelAtlasCost->setStringValue(tmStr);
    
    
    sprintf(tmStr, "%d", DataManager::sharedDataManager()->currFreeNum);
    m_LabelAtlasFree->setStringValue(tmStr);
    
    
    m_Ffreenum = DataManager::sharedDataManager()->freeNum;
    sprintf(tmStr, "%d", m_Ffreenum);
    m_LabelAtlasSY->setStringValue(tmStr);
    
    UIPanel *pnf = static_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_Free"));
    UIPanel *pnc = static_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_CostM"));
    
    pnf->setVisible(false);
    pnc->setVisible(false);
    
    if(DataManager::sharedDataManager()->currFreeNum > 0)
    {
        pnf->setVisible(true);
    }
    else
    {
        pnc->setVisible(true);
    }
    
    
    for(int i = 0; i < 15; i++)
    {
        m_arrCellsDate[i] = 0;
    }
    
    updateCellsDate();
    mangerCellsDate();
    updateCellImg();
    
    sprintf(tmStr, "ZD%d.png", imgid);
    m_Panel_MoveCell00->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
    m_Panel_MoveCell01->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
    m_Panel_MoveCell10->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
    m_Panel_MoveCell11->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
    m_Panel_MoveCell20->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
    m_Panel_MoveCell21->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
    m_Panel_MoveCell30->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
    m_Panel_MoveCell31->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
    m_Panel_MoveCell40->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
    m_Panel_MoveCell41->setBackGroundImage(tmStr, UI_TEX_TYPE_PLIST);
    
    updateZS();
}

void KCGameLayer::update(float dt)
{
    
    if(m_isAllThingsStop)
    {
        return;
    }
    
    if(m_pageCurr != m_page && m_isCanGo && m_page != 0)
    {
        
        
        switch (m_page)
        {
            case 1://老虎机
            {
                //更新自动按钮状态
                if(m_isCanAuto)
                {
                    m_Button_Auto->loadTextureNormal("BtnZDOn.png", UI_TEX_TYPE_PLIST);
                }
                else
                {
                    m_Button_Auto->loadTextureNormal("BtnZD.png", UI_TEX_TYPE_PLIST);
                }
                
                //如果从战斗界面切换的在做处理
                if(m_pageCurr == 4)
                {
                    DataManager::sharedDataManager()->petCurrGold = 0;
                    m_BtnBack->setVisible(true);
                    m_pageCurr = m_page;
                    m_pv->setGameScene(0);
                    m_setLineRewNum = m_setLineRewNumTmp;
                    m_Panel_BottomZ->setVisible(true);
                    m_Panel_BottomZDS->setVisible(false);
                    m_Panel_BottomZ->setZOrder(1);
                    m_Panel_BottomZDS->setZOrder(0);
                    
                    for(int i = 0; i< 15; i++)
                    {
                        m_arrCellsDate[i] = m_arrCellsDatetmp[i];
                    }
                    
                    
                    updateCellsDate();
                    mangerCellsDate();
                    updateCellImg();
                    showLine(true);

                }
                
                //如果从比倍界面切换的在做处理
                if(m_pageCurr == 3)
                {
                    m_pageCurr = m_page;
                    this->removeChild(m_double);
                    m_BtnBack->setVisible(true);
                    m_Widget->setVisible(true);
                }
                
                //播放中奖动画
                if(m_id4Reward.size() > 0)
                {
                    m_ActionNum = 0;
                    this->schedule(schedule_selector(KCGameLayer::runRewardAction), 1.0f);//runRewardAction();
                }
                
                break;
            }
            case 2://大厅
            {
                break;
            }
            case 3://比倍
            {
                m_BtnBack->setVisible(false);
                m_Widget->setVisible(false);
                this->addChild(m_double);
                m_double->initUI();
                break;
            }
            case 4://宠物战斗
            {
                
                
                //播发开始动画
                m_isCanTouchAllBtn = false;//禁用所有按钮
                m_ActionStartLogicFlag = 0;
                this->schedule(schedule_selector(KCGameLayer::runStartAction), 0.0f);
                
                //更新自动按钮状态
                if(m_isCanAuto)
                {
                    m_BtnZD->loadTextureNormal("BtnZDOn.png", UI_TEX_TYPE_PLIST);
                }
                else
                {
                    m_BtnZD->loadTextureNormal("BtnZD.png", UI_TEX_TYPE_PLIST);
                }

                
                //如果从 老虎机界面 切换的再做处理
                if(m_pageCurr == 1)
                {
                    
                    m_BtnBack->setVisible(false);
                    m_pageCurr = m_page;
                    m_pv->setGameScene(1);
                    m_setLineRewNumTmp = m_setLineRewNum;
                    m_setLineRewNum = 3;
                    m_Panel_BottomZ->setVisible(false);
                    m_Panel_BottomZDS->setVisible(true);
                    m_Panel_BottomZ->setZOrder(0);
                    m_Panel_BottomZDS->setZOrder(1);
                    
                    for(int i = 0; i< 15; i++)
                    {
                        m_arrCellsDatetmp[i] = m_arrCellsDate[i];
                        m_arrCellsDate[i] = 0;
                    }
                    
                    showLine(false);
                    updateCellsDate();
                    mangerCellsDate();
                    updateCellImg();
                    
                }
                //如果从 比倍界面 切换的再做处理
                if(m_pageCurr == 3)
                {
                    this->removeChild(m_double);
                    m_BtnBack->setVisible(true);
                    m_Widget->setVisible(true);
                    
                    m_BtnBack->setVisible(false);
                    m_pageCurr = m_page;
                    m_pv->setGameScene(1);
                    m_setLineRewNumTmp = m_setLineRewNum;
                    m_setLineRewNum = 3;
                    m_Panel_BottomZ->setVisible(false);
                    m_Panel_BottomZDS->setVisible(true);
                    m_Panel_BottomZ->setZOrder(0);
                    m_Panel_BottomZDS->setZOrder(1);
                    
                    for(int i = 0; i< 15; i++)
                    {
                        m_arrCellsDatetmp[i] = m_arrCellsDate[i];
                        m_arrCellsDate[i] = 0;
                    }
                    
                    showLine(false);
                    updateCellsDate();
                    mangerCellsDate();
                    updateCellImg();
                }
                
                float p = (float)bossCurrBlood/(float)bossMaxBlood;
                int pa = p*100;
                m_pv->setBossBloodValue(pa);
                
                petCurrBlood = petMaxBlood;
                p = (float)petCurrBlood/(float)petMaxBlood;
                pa = p*100;
                m_pv->setPetBloodValue(pa);
                
                break;
            }
            default:
            {
                break;
            }
        }
        
        m_pageCurr = m_page;
        updateZS();
    }
    
    //如果点击自动按钮，根据自动标识位进行自动处转动
    if(m_isCanAuto)
    {
        if(!this->isVisible())
        {
            return;
        }
        
        if(!m_Widget->isVisible())
        {
            return;
        }
        
        if(!m_isCanTouchAllBtn)
        {
            return;
        }
        
        if(!m_isCanGo)
        {
            return;
        }
        
        if(!m_isGoldEnougth)
        {
            return;
        }
        
        //老虎机
        if(m_pageCurr == 1)
        {
            touchEvent(tBtnGo, cocos2d::ui::TOUCH_EVENT_ENDED);
        }
        //宠物战斗
        else if(m_pageCurr == 4)
        {
            touchEvent(tBtnFGO, cocos2d::ui::TOUCH_EVENT_ENDED);
        }
    }
    
}

void KCGameLayer::setAllFlagTrue()
{
    m_isCanGo = true;
    m_isCanTouchAllBtn = true;
}

void KCGameLayer::runRewardAction()
{
    char strTmp[50];
    
    //停止所有cell动作
    for(int i = 0; i < 15; i++)
	{
		sprintf(strTmp, "Image_Img4Cell%d", i);
		UIImageView* tCellImg = dynamic_cast<UIImageView*>(m_Widget->getWidgetByName(strTmp));
        tCellImg->stopAllActions();
        
	}
    
    //隐藏所有线
    showLine(false);
    
    //闪烁中奖cells
    if(m_ActionNum < m_id4Reward.size())
    {
        
        int tnum = m_id4Reward[m_ActionNum].m_id*5 + m_id4Reward[m_ActionNum].m_num;
        
        sprintf(strTmp, "x-%04d.png", tnum);
        ((UIImageView*)m_arrLineImg->objectAtIndex(m_id4Reward[m_ActionNum].m_id))->loadTexture(strTmp, UI_TEX_TYPE_LOCAL);
        ((UIImageView*)m_arrLineImg->objectAtIndex(m_id4Reward[m_ActionNum].m_id))->setVisible(true);
        
        
        for(int i = 0; i < m_id4Reward[m_ActionNum].m_num; i++)
        {
            int cellid = m_p4r[m_id4Reward[m_ActionNum].m_id].m_iCellID[i];
            
            sprintf(strTmp, "Image_Img4Cell%d", cellid);
            UIImageView* tCellImg = dynamic_cast<UIImageView*>(m_Widget->getWidgetByName(strTmp));
            tCellImg->runAction(CCBlink::create(1.0f, 3));
        }
        
        m_ActionNum++;
        
        if(m_ActionNum >= m_id4Reward.size())
        {
            m_ActionNum = 0;
        }
    }
}

void KCGameLayer::stopRewardAction()
{
    //先停掉中奖动作
    this->unschedule(schedule_selector(KCGameLayer::runRewardAction));
    m_ActionNum = 0;
    
    char strTmp[50];
    //停止所有cell动作
    for(int i = 0; i < 15; i++)
    {
        sprintf(strTmp, "Image_Img4Cell%d", i);
        UIImageView* tCellImg = dynamic_cast<UIImageView*>(m_Widget->getWidgetByName(strTmp));
        tCellImg->stopAllActions();
        tCellImg->setVisible(true);
        
    }

}

void KCGameLayer::setLabelPetExp()
{
    m_LabelPetExp->setPosition(ccp(600, 320));
    m_LabelPetExp->setVisible(false);
}

void KCGameLayer::setLabelRoleExp()
{
    m_LabelRoleExp->setPosition(ccp(200, 480));
    m_LabelRoleExp->setVisible(false);
}

void KCGameLayer::setLabelBossACK()
{
    m_LabelAtlasBossACK->setPosition(ccp(700, 480));
    m_LabelAtlasBossACK->setVisible(false);
}

void KCGameLayer::setLabelPetACK()
{
    m_LabelAtlasPetACK->setPosition(ccp(300, 480));
    m_LabelAtlasPetACK->setVisible(false);
}

void KCGameLayer::setLabelAddBlood()
{
    m_LabelPetAddBlood->setPosition(ccp(300, 480));
    m_LabelPetAddBlood->setVisible(false);
}

void KCGameLayer::runFightAction()
{
    this->unschedule(schedule_selector(KCGameLayer::runFightAction));
    
    //宠物攻击
    if(0 == m_ActionFightLogicFlag)
    {
        
        m_pv->petPlay(ATTACK);
        this->schedule(schedule_selector(KCGameLayer::runFightAction), 0.3f);
        
        //加血
        if(5 == m_FSkill)
        {
            int tmpNum = 0;
            
            if(3 == m_FlianNum)//10%
            {
                tmpNum = (petMaxBlood*10)/100;
                petCurrBlood += (petMaxBlood*10)/100;
            }
            else if(4 == m_FlianNum)//15%
            {
                tmpNum = (petMaxBlood*15)/100;
                petCurrBlood += (petMaxBlood*15)/100;
            }
            else if(5 == m_FlianNum)//20%
            {
                tmpNum = (petMaxBlood*20)/100;
                petCurrBlood += (petMaxBlood*20)/100;
            }
            
            
            //播发标签动作
            char tmpStr[50];
            sprintf(tmpStr, ":%d", tmpNum);
            m_LabelPetAddBlood->setVisible(true);
            m_LabelPetAddBlood->setStringValue(tmpStr);
            
            float posY = m_LabelPetAddBlood->getPositionY();
            
            m_LabelPetAddBlood->runAction(CCMoveTo::create(0.5f, ccp(m_LabelPetAddBlood->getPositionX(), posY + 50)));
            
            m_LabelPetAddBlood->runAction(CCSequence::create(CCFadeOut::create(0.5f),
                                                             CCCallFunc::create(this, callfunc_selector(KCGameLayer::setLabelAddBlood)),
                                                             NULL));
            
            
            float p;
            int pa;
            
            if(petCurrBlood < 0)
            {
                petCurrBlood = 0;
            }
            
            p = (float)petCurrBlood/(float)petMaxBlood;
            pa = p*100;
            m_pv->setPetBloodValue(pa);
        }
    }
    
    //boss被伤害
    if(1 == m_ActionFightLogicFlag)
    {
        //播发标签动作
        char tmpStr[50];
        sprintf(tmpStr, ";%d", petAttack);
        m_LabelAtlasBossACK->setVisible(true);
        m_LabelAtlasBossACK->setStringValue(tmpStr);
        
        float posY = m_LabelAtlasBossACK->getPositionY();
        
        m_LabelAtlasBossACK->runAction(CCMoveTo::create(0.5f, ccp(m_LabelAtlasBossACK->getPositionX(), posY + 50)));
        
        m_LabelAtlasBossACK->runAction(CCSequence::create(CCFadeOut::create(0.5f),
                                                    CCCallFunc::create(this, callfunc_selector(KCGameLayer::setLabelBossACK)),
                                                    NULL));
        
        //播放boss被打动作
        if(4 == m_FSkill)
        {
            m_pv->boosPlay(BOSS_LIGHTED);
        }
        else
        {
            m_pv->boosPlay(BOSS_ONATTACKED);
        }
        
        bossCurrBlood -= petAttack;
        
        if(bossCurrBlood < 0)
        {
            bossCurrBlood = 0;
        }
        
        
        float p = (float)bossCurrBlood/(float)bossMaxBlood;
        int pa = p*100;
        m_pv->setBossBloodValue(pa);
        
        if(m_winret == 1)//宠物赢了
        {
            bossCurrBlood = bossMaxBlood;
            petCurrBlood = petMaxBlood;
            this->scheduleOnce(schedule_selector(KCGameLayer::runFightWin0), 0.8f);
        }
        else
        {
           this->schedule(schedule_selector(KCGameLayer::runFightAction), 0.9f);
        }

    }
    
    //boss攻击
    if(2 == m_ActionFightLogicFlag)
    {
        m_pv->boosPlay(BOSS_ATTACT);
        this->schedule(schedule_selector(KCGameLayer::runFightAction), 0.3f);
    }
    
    //宠物被打
    if(3 == m_ActionFightLogicFlag)
    {
        
        //播发标签动作
        char tmpStr[50];
        sprintf(tmpStr, ";%d", bossAttack);
        m_LabelAtlasPetACK->setVisible(true);
        m_LabelAtlasPetACK->setStringValue(tmpStr);
        
        float posY = m_LabelAtlasPetACK->getPositionY();
        
        m_LabelAtlasPetACK->runAction(CCMoveTo::create(0.5f, ccp(m_LabelAtlasPetACK->getPositionX(), posY + 50)));
        
        m_LabelAtlasPetACK->runAction(CCSequence::create(CCFadeOut::create(0.5f),
                                                          CCCallFunc::create(this, callfunc_selector(KCGameLayer::setLabelPetACK)),
                                                          NULL));
        
        //播发人物动作
        m_pv->petPlay(ONATTACKED);
        float p;
        int pa;
        petCurrBlood -= bossAttack;
        CCLog("~~~~~~~~pet Blood = %d", petCurrBlood);

        if(petCurrBlood < 0)
        {
            petCurrBlood = 0;
        }

        p = (float)petCurrBlood/(float)petMaxBlood;
        pa = p*100;
        m_pv->setPetBloodValue(pa);
        
        //宠物输了
        if(m_winret == 2)
        {
            float posX = LoseL->getPositionX();
            float posY = LoseL->getPositionY();
            
            LoseL->setVisible(true);
            LoseL->runAction(CCSequence::create(CCMoveTo::create(1.62, ccp(posX, posY + 100)),
                                                NULL));
            
            LoseL->runAction(CCSequence::create(CCFadeOut::create(1.62f),
                                                CCCallFunc::create(this, callfunc_selector(KCGameLayer::setLoseL)),
                                                NULL));
            
            
            this->scheduleOnce(schedule_selector(KCGameLayer::runFightLose), 2.0f);
        }
    }
    
    m_ActionFightLogicFlag++;
    
}

void KCGameLayer::runStartAction()
{
    this->unschedule(schedule_selector(KCGameLayer::runStartAction));
    
    if(0 == m_ActionStartLogicFlag)
    {
        m_armatureStart->stopAllActions();
        m_armatureStart->setVisible(true);
        m_armatureStart->getAnimation()->playWithIndex(0);
        this->schedule(schedule_selector(KCGameLayer::runStartAction), 2.7f);
        
        float posX = GoL->getPositionX();
        float posY = GoL->getPositionY();
        
        GoL->runAction(CCSequence::create(CCDelayTime::create(1.5),
                                          CCCallFunc::create(this, callfunc_selector(KCGameLayer::setGoLVisible)),
                                          CCMoveTo::create(1.0, ccp(posX, posY + 100)),
                                          NULL));
        
        GoL->runAction(CCSequence::create(CCDelayTime::create(1.5),
                                          CCCallFunc::create(this, callfunc_selector(KCGameLayer::setGoLVisible)),
                                          CCFadeOut::create(1.0f),
                                          CCCallFunc::create(this, callfunc_selector(KCGameLayer::setGoL)),
                                          NULL));
        
    }
    
    if(1 == m_ActionStartLogicFlag)
    {
        m_isCanTouchAllBtn = true;
    }
    
    m_ActionStartLogicFlag++;
}
            
void KCGameLayer::runFightWin0()
{
    m_pv->boosPlay(BOSS_LOST);
    this->scheduleOnce(schedule_selector(KCGameLayer::runFightWin1), 1.0f);
}

void KCGameLayer::runFightWin1()
{
    float posX = winL->getPositionX();
    float posY = winL->getPositionY();
    
    winL->setVisible(true);
    winL->runAction(CCSequence::create(CCMoveTo::create(1.0, ccp(posX, posY + 100)),
                                       NULL));
    
    winL->runAction(CCSequence::create(CCFadeOut::create(1.0f),
                                       CCCallFunc::create(this, callfunc_selector(KCGameLayer::setwinL)),
                                      NULL));
    
    this->scheduleOnce(schedule_selector(KCGameLayer::runFightWin2), 1.0f);

}

void KCGameLayer::runFightWin2()
{
    //发送战斗胜利抽奖请求
    GameLogicMeg2Sever tmpMeg1;
    tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQFIGHTAWARD;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
    
    m_winret = 0;
}

void KCGameLayer::setGoL()
{
    GoL->stopAllActions();
    GoL->setVisible(false);
    GoL->setPosition(ccp(490, 450));
}

void KCGameLayer::setwinL()
{
    winL->stopAllActions();
    winL->setVisible(false);
    winL->setPosition(ccp(490, 450));
}

void KCGameLayer::setLoseL()
{
    LoseL->stopAllActions();
    LoseL->setVisible(false);
    LoseL->setPosition(ccp(490, 450));
    
}

void KCGameLayer::playSound(const char * name)
{
    SimpleAudioEngine::sharedEngine()->playEffect(name);
}

void KCGameLayer::setGoLVisible()
{
    GoL->setVisible(true);
}

void KCGameLayer::runFightLose()
{
    m_isAllThingsStop = false;
    m_winret = 0;
}

void KCGameLayer::updateZS()
{
    char strTmp[50];
    
    for(int i = 1; i <= 25; i++)
    {
        m_ZS[i]->loadTexture("zs0.png", UI_TEX_TYPE_PLIST);
    }
    
    for(int i = 1; i <= m_setLineRewNum; i++)
    {
        sprintf(strTmp, "zs%d.png", i);
        m_ZS[i]->loadTexture(strTmp, UI_TEX_TYPE_PLIST);
    }
}
//void KCGameLayer::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
//}
//
//bool KCGameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
////    this->setVisible(<#bool visible#>)
//        return this->isVisible();
//}

