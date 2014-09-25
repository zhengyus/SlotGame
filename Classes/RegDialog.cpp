//
//  Dialog.cpp
//  LineHero
//
//  Created by ZivHoo on 14/8/17.
//
//

#include "RegDialog.h"
#include "LoginDialog.h"
#include "ext/ui/Alert.h"

RegDialog::RegDialog():
Dialog("dialog_default.png")
{
    
}

RegDialog::~ RegDialog()
{
    
}

bool RegDialog::init()
{
    if (! Dialog::init())
    {
        return false;
    }
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RegDialog::onReceiveMsg), EVENT_REGMES_FROM_MSG, NULL);
    
    initContentWithJsonFile("UI4Register.ExportJson");
    
    bindTouchEvent("Button_reg", this, toucheventselector(RegDialog::onRegister));
    return  true;
}

void RegDialog::onEnter()
{
    Dialog::onEnter();
    
    Widget * widget = getWidgetByName("Panel_account");
    _account = CCEditBox::create(CCSize(300, 35), CCScale9Sprite::create("green_edit.png"));
    _account->setFontColor(ccWHITE);
    _account->setPlaceHolder("账号");
    _account->setPlaceholderFontColor(ccWHITE);
    _account->setMaxLength(16);
    _account->setReturnType(kKeyboardReturnTypeDone);
    _account->setTouchPriority(0);
    
    CCPoint tmpPoint = widget->convertToWorldSpace(ccp(widget->boundingBox().size.width/2+47,0));
    _account->setPosition(tmpPoint);
    addChild(_account);
    
    widget = getWidgetByName("Panel_password");
    _password = CCEditBox::create(CCSize(300, 35), CCScale9Sprite::create("green_edit.png"));
    _password->setInputFlag(kEditBoxInputFlagPassword);
    _password->setFontColor(ccWHITE);
    _password->setPlaceHolder("密码");
    _password->setPlaceholderFontColor(ccWHITE);
    _password->setMaxLength(16);
    _password->setReturnType(kKeyboardReturnTypeDone);
    _password->setTouchPriority(0);
    
    tmpPoint = widget->convertToWorldSpace(ccp(widget->boundingBox().size.width/2+47,0));
    _password->setPosition(tmpPoint);
    addChild(_password);
}

void RegDialog::onRegister(CCObject *sender, TouchEventType event)
{
    if(event == TOUCH_EVENT_ENDED)
    {
        LoginMeg msg;
        msg.name = _account->getText();
        msg.pwd = _password->getText();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REG2MEG, &msg);
    }
}

void RegDialog::onReceiveMsg(CCObject* obj)
{
    Ret2UIRegMeg* meg = static_cast<Ret2UIRegMeg*>(obj);
    //0-失败 1-成功 2-用户名重复 3-用户格式非法
    switch (meg->ret) {
        case 0:
            Alert::create("注册失败！")->show();
            break;
        case 1:
            
            Alert::create("注册成功")->show();
            CCUserDefault::sharedUserDefault()->setStringForKey(SAVE_H_ACCOUNT,_account->getText());
            CCUserDefault::sharedUserDefault()->setStringForKey(SAVE_H_PASSWORD,_password->getText());
            this->close();
            break;
        case 2:
            
            Alert::create("用户名重复")->show();
            break;
        case 3:
            
            Alert::create("用户格式非法")->show();
            break;
        default:
            break;
    }
}