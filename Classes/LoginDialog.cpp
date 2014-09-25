//
//  Dialog.cpp
//  LineHero
//
//  Created by ZivHoo on 14/8/17.
//
//

#include "LoginDialog.h"
#include "RegDialog.h"
#include "GameHallLayer.h"

#define ACCOUND "accound"
#define PASSWORD "password"

LoginDialog::LoginDialog():
Dialog("dialog_default")
{
    
}

LoginDialog::~LoginDialog()
{
    
}

bool LoginDialog::init()
{
    if (!Dialog::init())
        return false;
    
    initContentWithJsonFile("UI4DialogLogin.ExportJson");
    
    Widget * tmp = _uilayer->getWidgetByName("panel_account");
    _account = CCEditBox::create(CCSize(300, 35), CCScale9Sprite::create("green_edit.png"));
    addChild(_account);
    
    _account->setMaxLength(16);
    _account->setTouchPriority(0);
    _account->setFontColor(ccWHITE);
    _account->setPlaceHolder("联众账号");
    _account->setPlaceholderFontColor(ccWHITE);
    
    _account->setInputMode(kEditBoxInputModeSingleLine);
    _account->setReturnType(kKeyboardReturnTypeDone);
    _account->setInputFlag(kEditBoxInputFlagSensitive);
    
    CCPoint tmpPoint = tmp->convertToWorldSpace(ccp(tmp->boundingBox().size.width/2+47,0));
    _account->setPosition(tmpPoint);
    
    
    tmp = _uilayer->getWidgetByName("panel_password");
    _password = CCEditBox::create(CCSize(300, 35), CCScale9Sprite::create("green_edit.png"));
    
    _password->setMaxLength(16);
    _password->setTouchPriority(0);
    _password->setFontColor(ccWHITE);
    _password->setPlaceHolder("密码");
    _password->setPlaceholderFontColor(ccWHITE);
    
    _password->setReturnType(kKeyboardReturnTypeDone);
    _password->setInputFlag(kEditBoxInputFlagPassword);
    
    tmpPoint = tmp->convertToWorldSpace(ccp(tmp->boundingBox().size.width/2+47, 0));
    _password->setPosition(tmpPoint);
    addChild(_password);
    
    string text = CCUserDefault::sharedUserDefault()->getStringForKey(SAVE_H_ACCOUNT,"fishtest007");
    _account->setText(text.c_str());
    text = CCUserDefault::sharedUserDefault()->getStringForKey(SAVE_H_PASSWORD,"fishtest007");
    _password->setText(text.c_str());
    
    bindTouchEvent("btn_login", this, toucheventselector(LoginDialog::onClickedLogin));
    bindTouchEvent("btn_register", this, toucheventselector(LoginDialog::onClickedRegister));
    
    return true;
}

void LoginDialog::onEnter()
{
    Dialog::onEnter();
    
}

void LoginDialog::onClickedLogin(CCObject *sender, cocos2d::ui::TouchEventType event)
{
    if (event==TOUCH_EVENT_ENDED)
    {
        LoginMeg tmpMeg;
        tmpMeg.name = _account->getText();
        tmpMeg.pwd = _password->getText();
        
        CCUserDefault::sharedUserDefault()->setStringForKey(SAVE_H_ACCOUNT, tmpMeg.name);
        CCUserDefault::sharedUserDefault()->setStringForKey(SAVE_H_PASSWORD, tmpMeg.pwd);
        CCUserDefault::sharedUserDefault()->flush();
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_LOGIN2MEG, &tmpMeg);
    }
}

void LoginDialog::onClickedRegister(CCObject *sender, cocos2d::ui::TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        Dialog* dialog = RegDialog::create();
        dialog->show();
        this->removeFromParent();
    }
}


void LoginDialog::eventLogin(CCObject* obj)
{
    
    LoginMeg* msg = static_cast<LoginMeg*>(obj);
    
    CCLog("LoginDialog:eventLogin name: %s",msg->name.c_str());
    CCLog("LoginDialog:eventLogin pwd: %s",msg->pwd.c_str());
}