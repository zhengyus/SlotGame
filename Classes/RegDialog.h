//
//  Dialog.h
//  SLOT
//
//  Created by ZivHoo on 14/8/17.
//
//
#ifndef __SLOT_REGDIALOG_H__
#define __SLOT_REGDIALOG_H__

#include "ext/ui/Dialog.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
using namespace extension;


class RegDialog : public Dialog {
    
    
public:
    
    RegDialog();
    ~RegDialog();
    
public:
    CREATE_FUNC(RegDialog);
    
public:
    bool init();
    
    virtual void onEnter();
    
    void onRegister(CCObject *sender, TouchEventType event);
    
    void onReceiveMsg(CCObject* obj);
    
private:
    Widget* _widget;
    CCEditBox* _account;
    CCEditBox* _password;
    CCEditBox* _repassword;
};

#endif
