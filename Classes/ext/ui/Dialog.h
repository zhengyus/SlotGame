//
//  Dialog.h
//  LineHero
//
//  Created by ZivHoo on 14/8/17.
//
//

#ifndef __LINEHERO_DIALOG_H__
#define __LINEHERO_DIALOG_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SoundNameDefine.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
using namespace CocosDenshion;

class Dialog:public CCLayer{

public:

    Dialog(string = "dialog_default.png",bool modal = false);
    ~Dialog();

public:

    bool init();
    bool initContentWithJsonFile(const char*);
    bool initContentWithWidget(Widget*);
    
public:
    
    virtual void show();
    virtual void close();

    Widget* getWidgetByName(string name);
    void bindTouchEvent(std::string name,CCObject *target, cocos2d::ui::SEL_TouchEvent selector);
    
public:
    
    void setOnCloseCallback(void (*onCloseCallback)());
    virtual void onClickCloseEvent(CCObject *pSender, TouchEventType type);

public:

    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved (CCTouch *pTouch, CCEvent *pEvent){};
    virtual void ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent){};

protected:
    
    string      _bgPath;

    UILayer*    _uilayer;
    
    Layout*     _colorPanel;
    ImageView*  _background;
    Layout*     _content;
    Button*     _btnClose;
    
private:
    
    void (*_onCloseCallback)();
    
};
#endif
