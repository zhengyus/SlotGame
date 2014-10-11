//
//  Alert.h
//  Slot
//
//  Created by huxf on 14-9-1.
//
//

#ifndef __Slot__Alert__
#define __Slot__Alert__

#include "ext/ui/Dialog.h"

typedef void (CCObject::*SEL_TouchSureEvent)();
#define touchesureventselector(_SELECTOR) (SEL_TouchSureEvent)(&_SELECTOR)

class Alert : public Dialog {
    
    
public:
    
    Alert(string content, string title,CCObject* target = NULL,SEL_TouchSureEvent = NULL);
    ~Alert();
    
public:
    
    bool init();
    void ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent){};
    
    static Alert* create(string content ,string title = "提示",CCObject* = NULL,SEL_TouchSureEvent = NULL);
    
public:
    
    void show();
    
    void clickedSureEvent(CCObject *pSender, TouchEventType type);
    
private:
    string _title;
    string _content;
    
private:
//    Label* _lblTitle;
    Label* _lblContent;
    
    SEL_TouchSureEvent  _sureEventSelector;
    CCObject*           _sureEventListener;
};

#endif /* defined(__Slot__Alert__) */
