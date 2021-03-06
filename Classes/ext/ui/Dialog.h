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

typedef void (CCObject::*SEL_closeEvent)();
#define closeeventselector(_SELECTOR) (SEL_closeEvent)(&_SELECTOR)

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
    
    void setCloseListener(CCObject* , SEL_closeEvent);
    virtual void onClickCloseEvent(CCObject *pSender, TouchEventType type);

public:

    void registerWithTouchDispatcher();
    bool ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent) override;

protected:
    string      _bgPath;

    UILayer*    _uilayer;
    
    Layout*     _colorPanel;
    ImageView*  _background;
    Layout*     _content;
    Button*     _btnClose;
    
private:
    
    SEL_closeEvent  _closeEventSelector;
    CCObject*       _closeEventListener;
};

#endif
