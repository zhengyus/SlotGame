//
//  Dialog.cpp
//  LineHero
//
//  Created by ZivHoo on 14/8/17.
//
//

#include "Dialog.h"
#include "../../Hdefine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;

Dialog::Dialog(string bgPath,bool modal):_onCloseCallback(NULL)
{                                       
    _bgPath = bgPath;
}

Dialog::~Dialog()
{
}

bool Dialog::init()
{
    if(!CCLayer::init())
        return false;
    
    setTouchEnabled(true);
    
    Widget* widget = GUIReader::shareReader()->widgetFromJsonFile("UI4Dialog.ExportJson");
    widget->setAnchorPoint(ccp(0.5, 0.5));
    widget->setPosition(ccp(m_obContentSize.width / 2, m_obContentSize.height / 2));
    
    _uilayer = UILayer::create();
    _uilayer->setTouchPriority(-101);
    _uilayer->addWidget(widget);
    
    _colorPanel = static_cast<UIPanel*>(getWidgetByName("Panel_color"));
    _background = static_cast<UIImageView*>(getWidgetByName("Image_background"));
    _content = static_cast<UIPanel*>(getWidgetByName("Panel_content"));
    _btnClose = static_cast<Button*>(getWidgetByName("Button_close"));
    
    _btnClose->addTouchEventListener(this,toucheventselector(Dialog::onClickCloseEvent));
    
    if ( !_bgPath.empty() )
    {
        _background->loadTexture( _bgPath.c_str(), UI_TEX_TYPE_PLIST);
    }
    
    addChild(_uilayer);    
    return true;
}


bool Dialog::initContentWithJsonFile(const char* fileName)
{
    if( !Dialog::init() )
        return false;
    
    _content->removeAllChildren();

    Widget* widget = GUIReader::shareReader()->widgetFromJsonFile(fileName);
    widget->setAnchorPoint(ccp(0.5, 0.5));
    CCSize size = _content->boundingBox().size;
    widget->setPosition(ccp(size.width / 2, size.height / 2));
    _content->addChild(widget);
    
    return  true;
}

void Dialog::bindTouchEvent(std::string name,CCObject *target, cocos2d::ui::SEL_TouchEvent selector)
{
    auto widget = _uilayer->getWidgetByName(name.c_str());
    widget->addTouchEventListener(target, selector);
}

void Dialog::show()
{
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

void Dialog::close()
{
    if (NULL != _onCloseCallback)
    {
        _onCloseCallback();
    }
    removeFromParent();
}

Widget* Dialog::getWidgetByName(string name)
{
    return _uilayer->getWidgetByName(name.c_str());
}

void Dialog::onClickCloseEvent(CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        this->close();
    }
}

void Dialog::setOnCloseCallback(void (*onCloseCallback)())
{
    _onCloseCallback = onCloseCallback;
}

void Dialog::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -100, true);
}

bool Dialog::ccTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void Dialog::ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent)
{
    close();
}