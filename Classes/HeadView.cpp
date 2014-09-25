//
//  HeadView.cpp
//  Slot
//
//  Created by ZivHoo on 14/8/28.
//
//

#include "HeadView.h"
#include "StartLayer.h"

static HeadView* instance = NULL;

HeadView::HeadView()
{
    
}

bool HeadView::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    UILayer* uilayer = UILayer::create();
    Widget* widget = GUIReader::shareReader()->widgetFromJsonFile("UI4GameHead.ExportJson");
    uilayer->addWidget(widget);
    addChild(uilayer);
    
    _head = static_cast<ImageView*>(uilayer->getWidgetByName("img_head"));
    _name = static_cast<Label*>(uilayer->getWidgetByName("Label_name"));
    _level = static_cast<Label*>(uilayer->getWidgetByName("Label_level"));
    _lexp = static_cast<Label*>(uilayer->getWidgetByName("Label_exp"));
    _pexp = static_cast<LoadingBar*>(uilayer->getWidgetByName("ProgressBar_exp"));
    _bean = static_cast<LabelAtlas*>(uilayer->getWidgetByName("AtlasLabel_bean"));
    _rank = static_cast<LabelAtlas*>(uilayer->getWidgetByName("AtlasLabel_rank"));
    _back = static_cast<Button*>(uilayer->getWidgetByName("Button_back"));
    _menu = uilayer->getWidgetByName("Button_menu");
    Widget* btnAdd = uilayer->getWidgetByName("Button_add");
    
    widget = uilayer->getWidgetByName("Panel_gold");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("glitter.ExportJson");
    _goldAnimation = CCArmature::create("glitter");
    _goldAnimation->setVisible(false);
    _goldAnimation->setPosition(ccp(widget->getSize().width/2, widget->getSize().height/2));
    widget->addNode(_goldAnimation);
    

    btnAdd->addTouchEventListener(this, toucheventselector(HeadView::clickedAddEvent));
    _menu->addTouchEventListener(this, toucheventselector(HeadView::clickedMenuEvent));
    
    setName("");
    setLevel(0);
    setExp(0,0);
    setBean(0);
    setRank(0);
    
    return true;
}

void HeadView::setHead(string head)
{
    _head->loadTexture(head.c_str(),UI_TEX_TYPE_PLIST);
}

void HeadView::setName(string name)
{
    _name->setText(name.c_str());
}

void HeadView::setLevel(int level)
{
    _level->setText(CCString::createWithFormat("Lv%d",level)->getCString());
}

void HeadView::setExp(int exp,int exptotal)
{
    _lexp->setText(CCString::createWithFormat("%d / %d",exp,exptotal)->getCString());
    _pexp->setPercent(exp/(exptotal * 1.0) * 100);
}

void HeadView::setBean(unsigned long long bean)
{
    _bean->setStringValue(CCString::createWithFormat("%lld",bean)->getCString());
}

void HeadView::setRank(unsigned long long rank)
{
    _rank->setStringValue(CCString::createWithFormat("%lld",rank)->getCString());
}

void HeadView::setMenuVisible(bool visible)
{
    _menu->setTouchEnabled(visible);
    _menu->setVisible(visible);
    setBackVisible(!visible);
}

void HeadView::setBackVisible(bool visible)
{
    _back->setVisible(visible);
}

void HeadView::playGoldAnimation()
{
    _goldAnimation->setVisible(true);
    _goldAnimation->getAnimation()->playWithIndex(0);
}

void HeadView::clickedAddEvent(CCObject*,TouchEventType event)
{
    if (TOUCH_EVENT_ENDED == event)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        CCLOG("addBean");
    }
}

void HeadView::clickedMenuEvent(CCObject*,TouchEventType event)
{
    if (TOUCH_EVENT_ENDED == event)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        
        CCScene* scene = StartLayer::scene(1);
        CCDirector::sharedDirector()->replaceScene(scene);
    }
}

HeadView* HeadView::getInstance()
{
    if (NULL == instance)
    {
        HeadView *tmp = new HeadView();
        if (tmp && tmp->init())
        {
            tmp->autorelease();
            instance = tmp;
            return instance;
        }
        CC_SAFE_DELETE(tmp);
    }
    return instance;
}

