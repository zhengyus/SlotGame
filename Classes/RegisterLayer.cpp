//
//  RegisterLayer.cpp
//  Slot
//
//  Created by huxf on 14-9-10.
//
//

#include "RegisterLayer.h"
#include "StartLayer.h"

RegisterLayer::RegisterLayer()
{
    
}

RegisterLayer::~RegisterLayer()
{
    
}

bool RegisterLayer::init()
{
    if ( !BaseLayer::init() )
        return false;
    
    addContentWithJsonFile("UI4Register.ExportJson");
    
    _msg = static_cast<Label*>(getWidgetByName("Label_msg"));
    _btnReg = getWidgetByName("Button_register");
    
    
    CCSize vsize = CCDirector::sharedDirector()->getVisibleSize();
    
    
    _btnBack = getWidgetByName("Button_back");
    
    //设置返回按钮的位置，以防分辨率不同导致返回按钮不完全显示
    LayoutParameter* param = RelativeLayoutParameter::create();
    param->setMargin(Margin(_btnBack->getPositionX() * 1136 / vsize.width, 40, 0, 0));
    _btnBack->setLayoutParameter(param);
    
    CCSize wsize = CCDirector::sharedDirector()->getWinSize();
    
    CCSize size = CCSize(450, 85);
    _account = CCEditBox::create(size,CCScale9Sprite::createWithSpriteFrameName("input_login.png"));
    _password = CCEditBox::create(size,CCScale9Sprite::createWithSpriteFrameName("input_login.png"));
    
    _account->setPosition(ccp(wsize.width/2,wsize.height/2 + 68));
    _password->setPosition(ccp(_account->getPositionX(),_account->getPositionY()-105));
    
    ccColor3B color = {147,140,115};
    _account->setFont("TrebuchetMS-Bold",20);
    _account->setFontColor(color);
    _account->setPlaceholderFontColor(color);
    _account->setPlaceHolder("5-19位的英文字母、数字或下划进行组合");
    _account->setMaxLength(19);
    _account->setSelected(true);
    _account->setReturnType(kKeyboardReturnTypeDone);
    
    _password->setFont("TrebuchetMS-Bold",20);
    _password->setFontColor(color);
    _password->setPlaceholderFontColor(color);
    _password->setPlaceHolder("8-15位的由英文字母、数字加其他字符组合");
    _password->setMaxLength(15);
    _password->setInputFlag(kEditBoxInputFlagPassword);
    _password->setInputMode(kEditBoxInputModeSingleLine);
    _password->setZOrder(-1);
    
    _uilayer->addChild(_account);
    _uilayer->addChild(_password);
    
    bindTouchEvent("Button_back", this, toucheventselector(RegisterLayer::onClickedBack));
    _btnReg->addTouchEventListener(this, toucheventselector(RegisterLayer::onClickedRegister));
    
    return true;
}

void RegisterLayer::showMsg(string str)
{
    _msg->setText(str.c_str());
}

void RegisterLayer::setEnabledRegister(bool enable)
{
    _btnReg->setTouchEnabled(enable);
}

void RegisterLayer::setVisibleBack(bool visible)
{
    _btnBack->setTouchEnabled(visible);
    _btnBack->setVisible(visible);
}

void RegisterLayer::onClickedBack(CCObject *sender, TouchEventType event)
{
    if(event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        StartLayer* start = static_cast<StartLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildren()->lastObject());
        start->back();
    }
}

void RegisterLayer::onClickedRegister(CCObject *sender, TouchEventType event)
{
    if(event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        
        _msg->setText("");
        LoginMeg msg;
        msg.name = _account->getText();
        msg.pwd = _password->getText();
        
        if (msg.name.empty())
        {
            _msg->setText("请输入新的联众账户！");
            return;
        }
        
        if (msg.pwd.empty())
        {
            _msg->setText("请输入账户密码！");
            return;
        }
        
        setEnabledRegister(false);
        CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
        userDefault->setStringForKey(SAVE_H_REGACCOUNT,msg.name);
        userDefault->setStringForKey(SAVE_H_REGPASSWORD,msg.pwd);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REG2MEG, &msg);
    }
}