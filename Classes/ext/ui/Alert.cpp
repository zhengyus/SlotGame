//
//  Alert.cpp
//  Slot
//
//  Created by huxf on 14-9-1.
//
//

#include "Alert.h"

using namespace std;

Alert::Alert(string content, string title,CCObject* target,SEL_TouchSureEvent selector):Dialog("dialog_simple.png")
{
    _title = title;
    _content = content;
    _sureEventListener = target;
    _sureEventSelector = selector;
}

Alert::~Alert()
{
    
}

Alert* Alert::create(string content, string title,CCObject* target,SEL_TouchSureEvent selector)
{
    Alert *pRet = new Alert(content,title,target,selector);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool Alert::init()
{
    if (! Dialog::initContentWithJsonFile("UI4DialogAlert.ExportJson"))
        return  false;
    
    _btnClose->setTouchEnabled(false);
    _btnClose->setVisible(false);
    
    _lblContent = static_cast<Label*>(getWidgetByName("Label_content"));
    _lblContent->setFontName("Heiti SC");
    
    bindTouchEvent("Button_confirm", this, toucheventselector(Alert::clickedSureEvent));
    
    return true;
}

void Alert::show()
{
//    _lblTitle->setText(_title);
    _lblContent->setText(_content);
    
    
    Dialog::show();
}

void Alert::clickedSureEvent(CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        if( NULL != _sureEventListener && NULL != _sureEventSelector)
        {
            (_sureEventListener->*_sureEventSelector)();
        }
        
        Dialog::onClickCloseEvent(pSender, type);
    }
}
