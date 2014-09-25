//
//  Dialog.h
//  SLOT
//
//  Created by ZivHoo on 14/8/17.
//
//
#ifndef __SLOT_LOGINDIALOG_H__
#define __SLOT_LOGINDIALOG_H__

#include "ext/ui/Dialog.h"
#include "LoginSocket.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
using namespace extension;


class LoginDialog : public Dialog {
    
    
public:
    
    LoginDialog();
    ~LoginDialog();

public:
    
    CREATE_FUNC(LoginDialog);
    
public:
    bool init();
    void onEnter();
    
    virtual void onClickedLogin(CCObject *sender, TouchEventType event);
    virtual void onClickedRegister(CCObject *sender, TouchEventType event);
    
public:
    void eventLogin(CCObject* obj);
    void recEventFromMeg(CCObject* obj);
    
protected:
    CCEditBox* _account;
    CCEditBox* _password;    
};

#endif
