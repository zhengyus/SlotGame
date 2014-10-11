//
//  CreateRoleDialog.cpp
//  Slot
//
//  Created by huxf on 14-9-16.
//
//

#include "CreateRoleDialog.h"
#include "define.h"


CreateRoleDialog::CreateRoleDialog():Dialog("dialog_role.png")
{
    _sex = 1;//默认性别为男
}

bool CreateRoleDialog::init()
{
    if (!Dialog::initContentWithJsonFile("UI4DialogRole.ExportJson"))
    {
        return false;
    }
    
    _btnClose->setTouchEnabled(false);
    _btnClose->setVisible(false);
    
    _msg = static_cast<Label*>(getWidgetByName("Label_msg"));
    
    for (int i = 0; i < 2; ++i)
    {
        Widget* widget = getWidgetByName(CCString::createWithFormat("Panel_sex%d",i)->getCString());
        
        _imgHead[i] = static_cast<ImageView*>(widget->getChildByName("Image_head"));
        _btnHead[i] = static_cast<Button*>(widget->getChildByName("Button_head"));
        _radio[i] = static_cast<ImageView*>(widget->getChildByName("Image_radio"));
        
        _btnHead[i]->addTouchEventListener(this, toucheventselector(CreateRoleDialog::onSelectedSexEvent));
        _radio[i]->addTouchEventListener(this, toucheventselector(CreateRoleDialog::onSelectedSexEvent));
    }
    
    Button* confirm = static_cast<Button*>(getWidgetByName("Button_confirm"));
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLog("%f",size.width);
    
    _roleName = CCEditBox::create(CCSize(337, 64),CCScale9Sprite::createWithSpriteFrameName("input_role.png"));
    _roleName->setMaxLength(8);
    _roleName->setFontColor(ccWHITE);
    _roleName->setTouchPriority(-128);
    _roleName->setPlaceHolder("角色名称");
    _roleName->setFont("TrebuchetMS-Bold", 25);
    _roleName->setPlaceholderFontColor(ccWHITE);
    _roleName->setInputMode(kEditBoxInputModeSingleLine);
    _roleName->setReturnType(kKeyboardReturnTypeDone);
    _roleName->setInputFlag(kEditBoxInputFlagSensitive);
    _roleName->setPosition(ccp(size.width/2, confirm->getPositionY() + 170));
    addChild(_roleName);
    
    confirm->addTouchEventListener(this, toucheventselector(CreateRoleDialog::onCreateRoleEvent));
    return true;
}

void CreateRoleDialog::onSelectedSexEvent(CCObject* pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (pSender == _btnHead[i] || pSender == _radio[i])
            {
                _sex = i == 0 ? 1 : 0;  //设置性别
                _imgHead[i]->setVisible(true);
                _radio[i]->loadTexture("state_selected.png",UI_TEX_TYPE_PLIST);
            }else{
                _imgHead[i]->setVisible(false);
                _radio[i]->loadTexture("state_none.png",UI_TEX_TYPE_PLIST);
            }
        }
    }
}

void CreateRoleDialog::onCreateRoleEvent(CCObject* pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        string name =_roleName->getText();
        if (name.empty())
        {
            _msg->setText("请输入角色名称！");
            return;
        }
        
        string account = CCUserDefault::sharedUserDefault()->getStringForKey(SAVE_H_ACCOUNT);
        
        GameLogicMeg2Sever tmpMeg1;
        tmpMeg1.m_id = CREATE_ROLE_EVENT;
        tmpMeg1.Cmaleid = _sex;
        tmpMeg1.CroleName = name;
        tmpMeg1.CuserName = account;
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
    }
}

void CreateRoleDialog::showMsg(string msg)
{
    _msg->setText(msg.c_str());
}