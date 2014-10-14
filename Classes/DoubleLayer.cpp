//
//  DoubleLayer.cpp
//  Slot
//
//  Created by huxf on 14-9-1.
//
//

#include "DoubleLayer.h"
#include "KCGameLayer.h"
#include "HeadView.h"

DoubleLayer::DoubleLayer()
{
    m_secondFlag = 0;
    m_beiNum = 1;
    m_whichColor = 1;
    isBtnCanTouch = true;
    m_cardID = 1;
    m_page = 3;
    m_winGold = 0;
    m_retWin = 1;
    winPos = ccp(160, 520);
    m_yzGold = 0;
    
    m_p8[0] = ccp(310, 480);
    m_p8[1] = ccp(360, 480);
    m_p8[2] = ccp(410, 480);
    m_p8[3] = ccp(460, 480);
    m_p8[4] = ccp(510, 480);
    m_p8[5] = ccp(555, 480);
    m_p8[6] = ccp(600, 480);
    m_p8[7] = ccp(650, 480);
    
    char tmpStr[5];
    
    for(int i = 1; i <= 53; i++)
    {
        sprintf(tmpStr, "%d.png", i);
        cardimg[i] = tmpStr;
        
    }
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(DoubleLayer::recGameLogicEventFromSever),
                                                                  EVENT_K2_REC_MEG_FROM_SEVER,
                                                                  NULL);
}

DoubleLayer::~DoubleLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_K2_REC_MEG_FROM_SEVER);

    CC_SAFE_RELEASE(m_arrCardImg);
    CC_SAFE_RELEASE(m_arrStar);
}

bool DoubleLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    m_arrCardImg = CCArray::create();
    m_arrStar = CCArray::create();
    CC_SAFE_RETAIN(m_arrCardImg);
    CC_SAFE_RETAIN(m_arrStar);
    
    m_Widget = UILayer::create();
    m_rootLy = static_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("doubelLayer.ExportJson"));
    
    m_Widget->addWidget(m_rootLy);
    
    
    //red按钮
  	UIButton* tBtnCRed = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("BetBtn_R"));
  	tBtnCRed->addTouchEventListener(this, toucheventselector(DoubleLayer::touchEvent));
    tBtnCRed->setTag(BTN_RED);
    //black正常
    UIButton* tBtnCBlack = static_cast<UIButton*>(m_Widget->getWidgetByName("BetBtn_B"));
    tBtnCBlack->addTouchEventListener(this, toucheventselector(DoubleLayer::touchEvent));
    tBtnCBlack->setTag(BTN_BLACK);
    //x2按钮
    tBtnX2 = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Btn2x"));
  	tBtnX2->addTouchEventListener(this, toucheventselector(DoubleLayer::touchEvent));
    tBtnX2->setTag(BTN_X2);
    //x3按钮
    tBtnX3 = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Btn3x"));
  	tBtnX3->addTouchEventListener(this, toucheventselector(DoubleLayer::touchEvent));
    tBtnX3->setTag(BTN_X3);
    //x5按钮
    tBtnX5 = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("Btn5x"));
  	tBtnX5->addTouchEventListener(this, toucheventselector(DoubleLayer::touchEvent));
    tBtnX5->setTag(BTN_X5);
    //收分按钮
    tBtnget = dynamic_cast<UIButton*>(m_Widget->getWidgetByName("GetMoneyBtn"));
  	tBtnget->addTouchEventListener(this, toucheventselector(DoubleLayer::touchEvent));
    tBtnget->setTag(BTN_GET);
    
    m_atl = dynamic_cast<UILabelAtlas*>(m_Widget->getWidgetByName("AtlasLabel_winMoney"));
    m_atl->setStringValue("0");
    m_pAtl = m_atl->getPosition();
    
    m_card = dynamic_cast<UIImageView*>(m_Widget->getWidgetByName("cardBg"));
    m_rootPanel = dynamic_cast<UIPanel*>(m_Widget->getWidgetByName("Panel_20"));
    
    //裁切层
    CCDrawNode* shape = CCDrawNode::create();
    CCPoint point[4] = {ccp(5, 0), ccp(435, 0), ccp(435, 168), ccp(5, 168)};
    shape->drawPolygon(point, 4, ccc4f(255, 255, 255, 255), 2, ccc4f(255, 255, 255, 255));
    
    m_cliper = CCClippingNode::create();
    m_cliper->setStencil(shape);
    m_cliper->setPosition(330, 230);
    
    m_spCardBG = CCSprite::create(cardimg[53].c_str());
    m_spCardBG->setPosition(ccp(253, 320));
    
    
    //贴星星
    char strTmp[50];
    
    for(int i = 1; i <= 8; i++)
    {
        sprintf(strTmp, "Image_28_%d", i);
        UIImageView *tmpImg = dynamic_cast<UIImageView*>(m_Widget->getWidgetByName(strTmp));;
        m_arrStar->addObject(tmpImg);
        tmpImg->setVisible(false);
        tmpImg->setScale(0);
    }
    
    //赢取图片
    m_spWin = CCSprite::create("win.png");
    m_spWin->setVisible(false);
    m_spWin->setPosition(winPos);
    m_spWin->setScale(3.0f);
    m_spWin->setOpacity(97);
    
    
    //骨骼资源
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Awards.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Great.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("dajiang03.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Gold.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("chibang.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Gold01.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Gold02.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Gold03.ExportJson");
//    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Win.ExportJson");

    m_armatureAward[0] = CCArmature::create("Awards");
    m_armatureAward[1] = CCArmature::create("Great");
    m_armatureAward[2] = CCArmature::create("dajiang03");
    
    for (int i = 0; i < 3; i++)
    {
        m_armatureAward[i]->setVisible(false);
        m_armatureAward[i]->setPosition(500, 620/2);
    }
    
    m_LabelWinNum = UILabelAtlas::create();
    m_LabelWinNum->setProperty("0", "GETNUM37_46.png", 48, 62, "0");
    m_LabelWinNum->setVisible(false);
    m_LabelWinNum->setPosition(ccp(500, 320));

    
    m_armatureGet = CCArmature::create("Gold");
    m_armatureGet->setPosition(ccp(350, 320));
    m_armatureGet->setVisible(false);
    
    //翅膀
    m_armatureChiBangLeft = CCArmature::create("chibang");
    m_armatureChiBangLeft->setPosition(ccp(380, 550));
    m_armatureChiBangLeft->setVisible(false);
    m_armatureChiBangRight = CCArmature::create("chibang");
    m_armatureChiBangRight->setPosition(ccp(600, 550));
    m_armatureChiBangRight->setScaleX(-1);
    m_armatureChiBangRight->setVisible(false);
    
//    m_armatureWin = CCArmature::create("Win");
//    m_armatureWin->setPosition(ccp(500, 320));
//    m_armatureWin->getAnimation()->playWithIndex(0);
    m_armatureGetWin[0] = CCArmature::create("Gold01");
    m_armatureGetWin[0]->setPosition(ccp(450, 180));
    m_armatureGetWin[0]->setVisible(false);
    m_armatureGetWin[1] = CCArmature::create("Gold02");
    m_armatureGetWin[1]->setPosition(ccp(450, 180));
    m_armatureGetWin[1]->setVisible(false);
    m_armatureGetWin[2] = CCArmature::create("Gold03");
    m_armatureGetWin[2]->setPosition(ccp(450, 180));
    m_armatureGetWin[2]->setVisible(false);
    
    //粒子特效
    patBatchNode = CCParticleBatchNode::createWithTexture(CCParticleSystemQuad::create("par.plist")->getTexture());
    patBatchNode->setAnchorPoint(ccp(0, 0));
    
    this->addChild(m_Widget);
    this->addChild(m_cliper);
    this->addChild(m_spCardBG);
    this->addChild(m_spWin);
    this->addChild(m_armatureGet);
//    this->addChild(m_armatureWin);
    this->addChild(m_armatureChiBangLeft);
    this->addChild(m_armatureChiBangRight);
    this->addChild(m_armatureGetWin[0]);
    this->addChild(m_armatureGetWin[1]);
    this->addChild(m_armatureGetWin[2]);
    this->addChild(m_LabelWinNum);
    
    this->addChild(patBatchNode);
    
    for(int i = 0; i < 3; i++)
    {
        this->addChild(m_armatureAward[i]);
        
    }
    
    //增加自动收分
    this->schedule(schedule_selector(DoubleLayer::autoGetScore), 1.0f);
    
    return true;
}

void DoubleLayer::touchEvent(CCObject* pSender, TouchEventType type)
{
//    m_armatureWin->getAnimation()->playWithIndex(0);
    
    //粒子特效
//    m_ptc = CCParticleSystemQuad::create("par.plist");
//    m_ptc->setPosition(m_p8[6]);
//    m_ptc->setPositionType(kCCPositionTypeFree);
//    m_ptc->setAutoRemoveOnFinish(true);
//    patBatchNode->addChild(m_ptc);
    
    if(-1 != m_stopSound)
    {
        SimpleAudioEngine::sharedEngine()->stopEffect(m_stopSound);
    }
    
    m_secondFlag = 0;
    
    if(!this->isVisible())
    {
        return;
    }
    
    if(!m_rootPanel->isVisible())
    {
        return;
    }
    
    if(!isBtnCanTouch)
    {
        return;
    }
    
    switch (type)
    {
        case cocos2d::ui::TOUCH_EVENT_BEGAN:
        {
            KCGameLayer::playSound(BUTTON_CLICK);
            break;
        }
        case cocos2d::ui::TOUCH_EVENT_ENDED:
        {
            
            int tTag = ((UILayer*)pSender)->getTag();
            
            for(int i = 0; i < 3; i++)
            {
                m_armatureAward[i]->getAnimation()->stop();
                m_armatureAward[i]->setVisible(false);
            }

            
            switch(tTag)
            {
                case BTN_RED:
                {
                    //大于2亿不再比倍
                    if(m_winGold >= 200000000)
                    {
                        return;
                    }
                    
                    runBiBei(1);
                    
                    break;
                }
                case BTN_BLACK:
                {
                    //大于2亿不再比倍
                    if(m_winGold >= 200000000)
                    {
                        return;
                    }
                    
                    runBiBei(2);
                    
                    break;
                }
                case BTN_X2:
                {
                    //大于2亿不再比倍
                    if(m_winGold >= 200000000)
                    {
                        return;
                    }
//                    HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(m_beiNum - 1));
                    
                    if(m_beiNum != 2)
                    {
                        //如果底金不足
                        if(isNoGold(m_needgold*(2 - 1)))
                        {
                            return;
                        }
                        
                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(m_beiNum - 1));
                        
                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() - m_needgold*(2 - 1));
                        
                        
                        char strTmp[50];
                        unsigned long long tmpNum = m_needgold;
                        tmpNum = tmpNum*2;
                        sprintf(strTmp, "%lld", tmpNum);
                        m_atl->setStringValue(strTmp);
                        
                        initBtn();
                        m_beiNum = 2;
                        sprintf(strTmp, "x%d_d.png", m_beiNum);
                        tBtnX2->loadTextureNormal(strTmp, UI_TEX_TYPE_PLIST);
                    }
                    else
                    {
                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(m_beiNum - 1));
//                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(2 - 1));
                        
                        m_beiNum = 1;
                        char strTmp[50];
                        unsigned long long tmpNum = m_needgold;
                        sprintf(strTmp, "%lld", tmpNum);
                        m_atl->setStringValue(strTmp);
                        
                        initBtn();
                    }

                    break;
                }
                case BTN_X3:
                {
                    //大于2亿不再比倍
                    if(m_winGold >= 200000000)
                    {
                        return;
                    }
//                    HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(m_beiNum - 1));
                    
                    if(m_beiNum != 3)
                    {
                        
                        //如果底金不足
                        if(isNoGold(m_needgold*(3 - 1)))
                        {
                            return;
                        }
                        
                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(m_beiNum - 1));
                        
                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() - m_needgold*(3 - 1));
                        
                        char strTmp[50];
                        unsigned long long tmpNum = m_needgold;
                        tmpNum = tmpNum*3;
                        sprintf(strTmp, "%lld", tmpNum);
                        m_atl->setStringValue(strTmp);
                        
                        initBtn();
                        m_beiNum = 3;
                        sprintf(strTmp, "x%d_d.png", m_beiNum);
                        tBtnX3->loadTextureNormal(strTmp, UI_TEX_TYPE_PLIST);
                    }
                    else
                    {
                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(m_beiNum - 1));
//                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(3 - 1));
                        
                        m_beiNum = 1;
                        char strTmp[50];
                        unsigned long long tmpNum = m_needgold;
                        sprintf(strTmp, "%lld", tmpNum);
                        m_atl->setStringValue(strTmp);
                        
                        initBtn();
                    }
            
                    break;
                }
                case BTN_X5:
                {
                    //大于2亿不再比倍
                    if(m_winGold >= 200000000)
                    {
                        return;
                    }
                    
                    if(m_beiNum != 5)
                    {
                        //如果底金不足
                        if(isNoGold(m_needgold*(5 - 1)))
                        {
                            return;
                        }
                        
                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(m_beiNum - 1));
                        
                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() - m_needgold*(5 - 1));
                        
                        char strTmp[50];
                        unsigned long long tmpNum = m_needgold;
                        tmpNum = tmpNum*5;
                        sprintf(strTmp, "%lld", tmpNum);
                        m_atl->setStringValue(strTmp);
                        
                        initBtn();
                        m_beiNum = 5;
                        sprintf(strTmp, "x%d_d.png", m_beiNum);
                        tBtnX5->loadTextureNormal(strTmp, UI_TEX_TYPE_PLIST);
                    }
                    else
                    {
                       HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(m_beiNum - 1));
//                        HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_needgold*(5 - 1));
                        
                        m_beiNum = 1;
                        char strTmp[50];
                        unsigned long long tmpNum = m_needgold;
                        sprintf(strTmp, "%lld", tmpNum);
                        m_atl->setStringValue(strTmp);
                        
                        initBtn();
                    }
      
                    break;
                }
                case BTN_GET://收分
                {
                    if(isBtnCanTouch)
                    {
                        isBtnCanTouch = false;
                    }
                    
                    this->unschedule(schedule_selector(DoubleLayer::autoGetScore));
                    
                    if(m_retWin != 0)
                    {
                        KCGameLayer::playSound(BIG_AWARD1);
                        
                        char tmpStr[50];
                        sprintf(tmpStr, ":%lld", m_winGold);
                        m_LabelWinNum->setVisible(true);
                        m_LabelWinNum->setStringValue(tmpStr);
                        
                        float posY = m_LabelWinNum->getPositionY();
                        
                        m_LabelWinNum->runAction(CCMoveTo::create(1.5f, ccp(m_LabelWinNum->getPositionX(), posY + 50)));

//to do fix
                        m_LabelWinNum->runAction(CCSequence::create(
//                                                                    CCFadeTo::create(1.5f, 0),
//                                                                    CCFadeOut::create(1.5f),
                                                                                                                                        CCDelayTime::create(2.0f),
                                                                    CCSpawn::create(
                                        
                                                                                    CCMoveTo::create(0.5f, ccp(m_LabelWinNum->getPositionX() - 150, posY + 300)),
                                                                                    CCScaleTo::create(1.0f, 0.0f),
                                                                                    NULL
                                                                                    ),
                                                                    CCDelayTime::create(1.0f),
                                                                    CCCallFunc::create(this, callfunc_selector(DoubleLayer::setLabelWinNum)),
                                                                    NULL));
                        
                        this->schedule(schedule_selector(DoubleLayer::runUIStar), 3.5);
                        
                        int wi = 0;
                        
                        if(m_winGold < 1000000)
                        {
                            wi = 0;
                        }
                        else if(m_winGold >= 1000000 && m_winGold < 8000000)
                        {
                            wi = 1;
                        }
                        else
                        {
                            wi = 2;
                        }
                        
                        KCGameLayer::playSound(SHOW_GOLD);
                        
                        m_armatureGetWin[wi]->setPosition(ccp(450, 195));
                        m_armatureGetWin[wi]->setVisible(true);
                        m_armatureGetWin[wi]->getAnimation()->playWithIndex(0);

                    }
                    else
                    {
                        GameLogicMeg2Sever tmpMeg1;
                        tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQDOUBLEGETGOLD;
                        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
                        
                        this->schedule(schedule_selector(DoubleLayer::ishaveChange), 5.0f);
                        
                        for (int i = 0; i < m_arrCardImg->count(); i++)
                        {
                            m_cliper->removeChild((CCNode*)m_arrCardImg->objectAtIndex(i));
                            
                        }
                        
                        m_arrCardImg->removeAllObjects();
                        
                        //赢的金币大于2亿 直接返回大厅界面
                        if(m_winGold >= 200000000)
                        {
//                            GameLogicMeg2Sever tmpMeg1;
//                            tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQDOUBLEGETGOLD;
//                            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
                            
                            for (int i = 0; i < m_arrCardImg->count(); i++)
                            {
                                m_cliper->removeChild((CCNode*)m_arrCardImg->objectAtIndex(i));
                                
                            }
                            
                            m_arrCardImg->removeAllObjects();
                            this->unschedule(schedule_selector(DoubleLayer::autoGetScore));
                            m_starNum = 0;
                            
                            ((KCGameLayer*)this->getParent())->m_page = m_page;
//                            ((KCGameLayer*)this->getParent())->m_page = 2;//返回大厅界面
                            
                        }

                    }

                    
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

void DoubleLayer::initBtn()
{
    tBtnX2->loadTextureNormal("x2.png", UI_TEX_TYPE_PLIST);
    tBtnX3->loadTextureNormal("x3.png", UI_TEX_TYPE_PLIST);
    tBtnX5->loadTextureNormal("x5.png", UI_TEX_TYPE_PLIST);
}

void DoubleLayer::recGameLogicEventFromSever(CCObject * obj)
{
    Meg2UIDate * tmeg = ((Meg2UIDate*)obj);
    int tid = tmeg->m_id;
    
    switch (tid)
    {
        case OGID_TEXAS_SLOTS_DOUBLEGAME://
        {
            CCLog("in KCGameLayer ");
            CCLog("~~~~~~~~~~比倍结果=%d", tmeg->Bret);
            CCLog("~~~~~~~~~~牌值=%d", tmeg->BcardNumber);
            CCLog("~~~~~~~~~~星星数量=%d", tmeg->BstartNum);
            CCLog("~~~~~~~~~~压注额=%lld", tmeg->Bneedgold);
            CCLog("~~~~~~~~~~赢的底金额=%lld", tmeg->Bwingold);
            CCLog("~~~~~~~~~~玩家钱=%lld", tmeg->Brolegold);
            CCLog("~~~~~~~~~~界面page=%d", tmeg->Bpage);
            
            m_yzGold = tmeg->Bneedgold;
            m_winGold = tmeg->Bwingold;
            m_cardID = tmeg->BcardNumber;
            m_retWin = tmeg->Bret;
            m_starNum = tmeg->BstartNum;
            
            DataManager::sharedDataManager()->currGold = tmeg->Brolegold;
            Meg2UIDate sendmeg;
            sendmeg.m_id = OGID_TEXAS_SLOTS_PLAYERINFO;
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_ALL, &sendmeg);
            
            char strTmp[50];
            
            
            m_page = tmeg->Bpage;
            
            //card动画
            int posnum = m_arrCardImg->count();
            if(posnum > 8)
            {
                posnum = 8;
            }
            
            m_spCardBG->setVisible(true);
            m_spCardBG->runAction(CCSequence::create(CCMoveTo::create(0.1f, ccp(390 + 40*posnum, 314)),
                                                     CCScaleTo::create(0.1, 0.0f, 1.0f),
                                                     CCCallFuncN::create(this, callfuncN_selector(DoubleLayer::setcardBGImg)),
                                                     CCScaleTo::create(0.1, 1.0f, 1.0f),
                                                     CCCallFunc::create(this, callfunc_selector(DoubleLayer::moveCards)),
                                                     CCDelayTime::create(0.4f),
                                                     CCCallFunc::create(this, callfunc_selector(DoubleLayer::addcard)),
                                                     CCCallFunc::create(this, callfunc_selector(DoubleLayer::resetBGcard)),
                                                     CCCallFunc::create(this, callfunc_selector(DoubleLayer::isCanCloseThisUI)),
                                                     NULL));
            
            
            //比倍成功
            //赢取动画
            if(tmeg->Bret == 1)
            {
                
                //重置金币显示数量
                HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() - m_winGold);
                
                KCGameLayer::playSound(DOUBLE_WIN);
                
                m_needgold = tmeg->Bwingold;
                initBtn();
                m_beiNum = 1;
                
                sprintf(strTmp, "%lld", m_needgold);
                m_atl->setStringValue(strTmp);
                
                //播放星星动画
                runStarAction(tmeg->BstartNum);
                
                //播放赢动画
                m_spWin->runAction(CCSequence::create(CCDelayTime::create(0.5f),
                                                      CCCallFunc::create(this, callfunc_selector(DoubleLayer::setWinCanSee)),
                                                      CCMoveTo::create(0.1f, ccp(420 + 40*posnum, 380)),
                                                      NULL));
                m_spWin->runAction(CCSequence::create(CCDelayTime::create(0.5f),
                                                      CCScaleTo::create(0.1f, 1.0f),
                                                      NULL));
                m_spWin->runAction(CCSequence::create(CCDelayTime::create(0.5f),
                                                      CCFadeTo::create(0.08f, 255),
                                                      NULL));
                
//                if(tmeg->Bwingold >= 5000 && tmeg->Bwingold)
//                {
//                    KCGameLayer::playSound(BIG_AWARD4);
//                    m_armatureAward[2]->setVisible(true);
//                    m_armatureAward[2]->getAnimation()->playWithIndex(0);
//                }
//                else
//                {
                
                this->scheduleOnce(schedule_selector(DoubleLayer::runAwardAction), 0.5f);
                
//                    if(tmeg->Bwingold >= 1 && tmeg->Bwingold < 2000)
//                    {
//                        KCGameLayer::playSound(BIG_AWARD2);
//                        m_armatureAward[0]->setVisible(true);
//                        m_armatureAward[0]->getAnimation()->playWithIndex(0);
//                    }
//                    else if(tmeg->Bwingold >= 2000)
//                    {
//                        KCGameLayer::playSound(BIG_AWARD3);
//                        m_armatureAward[1]->setVisible(true);
//                        m_armatureAward[1]->getAnimation()->playWithIndex(0);
//                    }
//                }
            
            }
            //比倍失败播放钱飞走动画
            else
            {
                KCGameLayer::playSound(DOUBLE_LOST);
                runMoneyFly();
                
                unsigned long long needBaseGold;
                needBaseGold = atoi(m_atl->getStringValue());
                
                m_tmpGold = HeadView::getInstance()->getBean();
                
                if(HeadView::getInstance()->getBean() >= needBaseGold)
                {
                    //重置金币显示数量
                    HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() - needBaseGold);

                }
            }
            
            break;
		}
        case OGID_TEXAS_SLOTS_DOUBLEJP:
        {
            m_stopSound = KCGameLayer::playSound(BIG_AWARD4);
            m_armatureAward[2]->setVisible(true);
            m_armatureAward[2]->getAnimation()->playWithIndex(0);
            break;
        }
        default:
        {
            CCLog("No this Message ID in DoubleLayer!!!!!");
            break;
        };
    }
}

void DoubleLayer::runBiBei(int color)
{
    
    m_spWin->stopAllActions();
    m_spWin->setVisible(false);
    m_spWin->setPosition(winPos);
    m_spWin->setScale(3.0f);
    m_spWin->setOpacity(97);
    
    
    m_whichColor = color;
    
    if(isBtnCanTouch)
    {
        isBtnCanTouch = false;
    }
    
    GameLogicMeg2Sever tmpMeg1;
    tmpMeg1.times = m_beiNum;
    tmpMeg1.colour = color;
    tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQDOUBLEGAME;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
}

void DoubleLayer::initUI()
{
    m_yzGold = 0;
    m_stopSound = -1;
    //增加自动收分
    this->unschedule(schedule_selector(DoubleLayer::autoGetScore));
    this->schedule(schedule_selector(DoubleLayer::autoGetScore), 1.0f);
    
    m_secondFlag = 0;
    m_starNum = 0;
    initBtn();
    m_beiNum = 1;
    m_whichColor = 1;
    isBtnCanTouch = true;
    m_cardID = 1;
    m_page = 3;
    m_retWin = 1;
   
    char strTmp[50];
    
    m_needgold = ((KCGameLayer*)this->getParent())->m_winGold;
    m_winGold = m_needgold;
    sprintf(strTmp, "%lld", m_needgold);
    m_atl->setStringValue(strTmp);
    
    m_LabelWinNum->setPosition(ccp(500, 320));
    m_LabelWinNum->setVisible(false);
    
    m_armatureGet->setPosition(ccp(350, 320));
    m_armatureGet->setVisible(false);
    
    //初始化星星
    for(int i = 1; i <= 8; i++)
    {
        sprintf(strTmp, "Image_28_%d", i);
        UIImageView *tmpImg = dynamic_cast<UIImageView*>(m_Widget->getWidgetByName(strTmp));;
        tmpImg->setVisible(false);
        tmpImg->setScale(0);
    }
    
    //初始化赢图标
    m_spWin->setVisible(false);
    m_spWin->setPosition(winPos);
    m_spWin->setScale(3.0f);
    m_spWin->setOpacity(97);
    
    //初始化奖励动画 收分动画
    for (int i = 0; i < 3; i++)
    {
        m_armatureAward[i]->setVisible(false);
        m_armatureAward[i]->getAnimation()->stop();
        
        m_armatureGetWin[i]->setVisible(false);
        m_armatureGetWin[i]->getAnimation()->stop();
    }
    
    //删除卡牌
    for (int i = 0; i < m_arrCardImg->count(); i++)
    {
        m_cliper->removeChild((CCNode*)m_arrCardImg->objectAtIndex(i));
        
    }
    
    m_arrCardImg->removeAllObjects();
    
    //背景图片
    resetBGcard();
}

void DoubleLayer::setcardBGImg(CCNode* pSender)
{
    KCGameLayer::playSound(DOUBLE_TURN_OVER);
    ((CCSprite*)pSender)->setTexture(CCSprite::create(cardimg[m_cardID].c_str())->getTexture());
}

void DoubleLayer::moveCards()
{
    int posnum = m_arrCardImg->count();
    
    if(posnum > 8)
    {
        for(int i = 0; i < m_arrCardImg->count(); i++)
        {
            CCPoint tmpP;
            tmpP.x = ((CCSprite*)m_arrCardImg->objectAtIndex(i))->getPositionX() - 40;
            tmpP.y = ((CCSprite*)m_arrCardImg->objectAtIndex(i))->getPositionY();
            ((CCSprite*)m_arrCardImg->objectAtIndex(i))->runAction(CCMoveTo::create(0.18f, tmpP));
        }
    }
}

void DoubleLayer::addcard()
{
    int posnum = m_arrCardImg->count();
    
    if(posnum > 8)
    {
        posnum = 8;
    }
    
    CCSprite *tCardimg = CCSprite::create(cardimg[m_cardID].c_str());
    tCardimg->setPosition(ccp(60 + 40*posnum, 84));
    m_cliper->addChild(tCardimg);
    m_arrCardImg->addObject(tCardimg);
}

void DoubleLayer::resetBGcard()
{
    m_spCardBG->setVisible(false);
    m_spCardBG->setTexture(CCSprite::create(cardimg[53].c_str())->getTexture());
    m_spCardBG->setPosition(ccp(253, 320));
    isBtnCanTouch = true;
    
    
}

void DoubleLayer::isCanCloseThisUI()
{
    this->unschedule(schedule_selector(DoubleLayer::isCanCloseThisUI));
    
    //退回老虎机界面
    if(m_starNum >= 9 && m_page != 3)
    {
        isBtnCanTouch = false;
        m_starNum = -9;
        this->schedule(schedule_selector(DoubleLayer::isCanCloseThisUI), 2.0f);
    }
    //退回对应界面
    else if(m_starNum!= -19 && m_page != 3)
    {
        isBtnCanTouch = false;
        m_starNum = -19;
        this->schedule(schedule_selector(DoubleLayer::isCanCloseThisUI), 2.0f);
    }
    else if(-9 == m_starNum)
    {
        m_starNum = -19;
        
        KCGameLayer::playSound(BIG_AWARD1);
        char tmpStr[50];
        sprintf(tmpStr, ":%lld", m_winGold);
        m_LabelWinNum->setVisible(true);
        m_LabelWinNum->setStringValue(tmpStr);
        
        float posY = m_LabelWinNum->getPositionY();
        
        m_LabelWinNum->runAction(CCMoveTo::create(1.5f, ccp(m_LabelWinNum->getPositionX(), posY + 50)));
        
        m_LabelWinNum->runAction(CCSequence::create(
                                                    //                                                                    CCFadeTo::create(1.5f, 0),
                                                    //                                                                    CCFadeOut::create(1.5f),
                                                    CCDelayTime::create(2.0f),
                                                    CCSpawn::create(
                                                                    
                                                                    CCMoveTo::create(0.5f, ccp(m_LabelWinNum->getPositionX() - 150, posY + 300)),
                                                                    CCScaleTo::create(1.0f, 0.0f),
                                                                    NULL
                                                                    ),
                                                    CCDelayTime::create(1.0f),
//                                                    CCCallFunc::create(this, callfunc_selector(DoubleLayer::setLabelWinNum)),
                                                    NULL));
        
        this->schedule(schedule_selector(DoubleLayer::runUIStar), 3.5);
        
        int wi = 0;
        
        if(m_winGold < 1000000)
        {
            wi = 0;
        }
        else if(m_winGold >= 1000000 && m_winGold < 8000000)
        {
            wi = 1;
        }
        else
        {
            wi = 2;
        }
        
        KCGameLayer::playSound(SHOW_GOLD);
        
        m_armatureGetWin[wi]->setPosition(ccp(450, 180));
        m_armatureGetWin[wi]->setVisible(true);
        m_armatureGetWin[wi]->getAnimation()->playWithIndex(0);
        
        this->schedule(schedule_selector(DoubleLayer::isCanCloseThisUI), GET_MONEY_TIME);
    }
    else if(-19 == m_starNum)
    {
        m_starNum = 0;
        
//        ((KCGameLayer*)this->getParent())->m_page = m_page;
        
        for (int i = 0; i < m_arrCardImg->count(); i++)
        {
            m_cliper->removeChild((CCNode*)m_arrCardImg->objectAtIndex(i));
            
        }
        
        m_arrCardImg->removeAllObjects();
        this->unschedule(schedule_selector(DoubleLayer::autoGetScore));
        this->unschedule(schedule_selector(DoubleLayer::ishaveChange));
        
        //背景图片
        resetBGcard();
        
        isBtnCanTouch = true;
        touchEvent(tBtnget, TOUCH_EVENT_ENDED);
        
    }
}

void DoubleLayer::runStarAction(int id)
{
    ((UIImageView*)m_arrStar->objectAtIndex(id - 2))->setVisible(true);
    ((UIImageView*)m_arrStar->objectAtIndex(id - 2))->runAction(CCScaleTo::create(0.2f, 1.0f));
    
    //粒子特效
    m_ptc = CCParticleSystemQuad::create("par.plist");
    m_ptc->setPosition(m_p8[id - 2]);
    m_ptc->setPositionType(kCCPositionTypeFree);
    m_ptc->setAutoRemoveOnFinish(true);
    patBatchNode->addChild(m_ptc);
    
}

void DoubleLayer::setWinCanSee()
{
    m_spWin->setVisible(true);
}

void DoubleLayer::setLabelWinNum()
{
    m_LabelWinNum->setScale(1.0f);
    m_LabelWinNum->setPosition(ccp(500, 320));
    m_LabelWinNum->setVisible(false);
    
    m_armatureGet->setPosition(ccp(350, 320));
    m_armatureGet->setVisible(false);
    
    GameLogicMeg2Sever tmpMeg1;
    tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQDOUBLEGETGOLD;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
    
    this->schedule(schedule_selector(DoubleLayer::ishaveChange), 5.0f);
    
    for (int i = 0; i < m_arrCardImg->count(); i++)
    {
        m_cliper->removeChild((CCNode*)m_arrCardImg->objectAtIndex(i));
        
    }
    
    m_arrCardImg->removeAllObjects();
    
    HeadView::getInstance()->setBean(HeadView::getInstance()->getBean() + m_winGold);
    
    //赢的金币大于2亿 直接返回大厅界面
    if(m_winGold >= 200000000)
    {
//        GameLogicMeg2Sever tmpMeg1;
//        tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQDOUBLEGETGOLD;
//        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
        
        for (int i = 0; i < m_arrCardImg->count(); i++)
        {
            m_cliper->removeChild((CCNode*)m_arrCardImg->objectAtIndex(i));
            
        }
        
        m_arrCardImg->removeAllObjects();
        this->unschedule(schedule_selector(DoubleLayer::autoGetScore));
        m_starNum = 0;
        
//        ((KCGameLayer*)this->getParent())->m_page = 2;//返回大厅界面
        
    }
    
    ((KCGameLayer*)this->getParent())->m_page = m_page;
}

void DoubleLayer::runMoneyFly()
{
    m_armatureChiBangLeft->setVisible(true);
    m_armatureChiBangRight->setVisible(true);
    m_armatureChiBangLeft->getAnimation()->playWithIndex(0);
    m_armatureChiBangRight->getAnimation()->playWithIndex(0);
    
    
    float posx = 440 + 25*strlen(m_atl->getStringValue());
    m_armatureChiBangRight->setPositionX(posx);
    
    float posY = m_atl->getPositionY();
    m_atl->runAction(CCMoveTo::create(0.4f, ccp(m_atl->getPositionX(), posY + 50)));
    m_atl->runAction(CCSequence::create(CCFadeOut::create(0.4f),
                                        CCCallFunc::create(this, callfunc_selector(DoubleLayer::setLabelMoneyFly)),
                                        NULL));
    
    posY = m_armatureChiBangLeft->getPositionY();
    m_armatureChiBangLeft->runAction(CCMoveTo::create(0.4f, ccp(m_armatureChiBangLeft->getPositionX(), posY + 50)));
    m_armatureChiBangLeft->runAction(CCSequence::create(CCFadeOut::create(0.4f),
                                                        CCCallFunc::create(this, callfunc_selector(DoubleLayer::setActionFly0)),
                                                        NULL));

    
    posY = m_armatureChiBangRight->getPositionY();
    m_armatureChiBangRight->runAction(CCMoveTo::create(0.4f, ccp(m_armatureChiBangRight->getPositionX(), posY + 50)));
    m_armatureChiBangRight->runAction(CCSequence::create(CCFadeOut::create(0.4f),
                                                         CCCallFunc::create(this, callfunc_selector(DoubleLayer::setActionFly1)),
                                                        NULL));
}

void DoubleLayer::setLabelMoneyFly()
{
    m_atl->stopAllActions();
    m_atl->setPosition(m_pAtl);
}

void DoubleLayer::setActionFly0()
{
    m_armatureChiBangLeft->stopAllActions();
    m_armatureChiBangLeft->setVisible(false);
    m_armatureChiBangLeft->setPosition(ccp(380, 550));
}

void DoubleLayer::setActionFly1()
{
    m_armatureChiBangRight->stopAllActions();
    m_armatureChiBangRight->setVisible(false);
    m_armatureChiBangRight->setPosition(ccp(600, 550));
    
    //金币小于底金 直接返回老虎机界面
    if(m_tmpGold < m_yzGold)
    {
        /*
        GameLogicMeg2Sever tmpMeg1;
        tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQDOUBLEGETGOLD;
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
        
        for (int i = 0; i < m_arrCardImg->count(); i++)
        {
            m_cliper->removeChild((CCNode*)m_arrCardImg->objectAtIndex(i));
            
        }
        
        m_arrCardImg->removeAllObjects();
        this->unschedule(schedule_selector(DoubleLayer::autoGetScore));
        m_starNum = 0;
        this->schedule(schedule_selector(DoubleLayer::ishaveChange), 2.0f);
//        ((KCGameLayer*)this->getParent())->m_page = 1;//返回老虎机界面
        
        resetBGcard();
         */
        
        m_secondFlag = 0;
        isBtnCanTouch = true;
        this->unschedule(schedule_selector(DoubleLayer::autoGetScore));
        touchEvent(tBtnget, TOUCH_EVENT_ENDED);
        
    }
    
}

void DoubleLayer::runAwardAction()
{
    if(m_winGold >= 1 && m_winGold < 2000)
    {
        KCGameLayer::playSound(BIG_AWARD2);
        m_armatureAward[0]->setVisible(true);
        m_armatureAward[0]->getAnimation()->playWithIndex(0);
    }
    else if(m_winGold >= 2000)
    {
        KCGameLayer::playSound(BIG_AWARD3);
        m_armatureAward[1]->setVisible(true);
        m_armatureAward[1]->getAnimation()->playWithIndex(0);
    }
}

void DoubleLayer::runUIStar()
{
    this->unschedule(schedule_selector(DoubleLayer::runUIStar));
    
    UiLogicMsg tmpMeg1;
    tmpMeg1.id = 1;//
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_UILOGIC, &tmpMeg1);
}

void DoubleLayer::autoGetScore()
{
    if(5 == m_secondFlag)
    {
        m_secondFlag = 0;
        isBtnCanTouch = true;
        this->unschedule(schedule_selector(DoubleLayer::autoGetScore));
        touchEvent(tBtnget, TOUCH_EVENT_ENDED);
    }
    
    m_secondFlag++;
}

bool DoubleLayer::isNoGold(unsigned long long wingold)
{
    if(HeadView::getInstance()->getBean() < wingold)
    {
        return true;
    }
    
    return false;
}

void DoubleLayer::ishaveChange()
{
    this->unschedule(schedule_selector(DoubleLayer::ishaveChange));
    
    if(((KCGameLayer*)this->getParent())->m_page == 3)
    {
        ((KCGameLayer*)this->getParent())->m_page = 2;
    }
}
//void DoubleLayer::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
//}
