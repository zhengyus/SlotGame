//
//  OrderDialog.cpp
//  Slot
//
//  Created by ZivHoo on 14/8/26.
//
//

#include "MsgContentDialog.h"
#include "define.h"
#include "ext/ui/Alert.h"

MsgContentDialog::MsgContentDialog(MyMailMeg& data)
{
    _data = &data;
}

MsgContentDialog::~ MsgContentDialog()
{
    
}

MsgContentDialog* MsgContentDialog::create(MyMailMeg& data)
{
    MsgContentDialog *pRet = new MsgContentDialog(data);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    
    return NULL;
}

bool MsgContentDialog::init()
{
    if (! Dialog::initContentWithJsonFile("UI4DialogMsgContent.ExportJson"))
        return false;
    
    _wprize = _uilayer->getWidgetByName("Panel_prize");
    _title = static_cast<Label*>(_uilayer->getWidgetByName("Label_title"));
    _content = static_cast<Label*>(_uilayer->getWidgetByName("Label_content"));
    _number = static_cast<Label*>(_uilayer->getWidgetByName("Label_num"));
    _prize = static_cast<ImageView*>(_uilayer->getWidgetByName("Image_prize"));
    _button = static_cast<Button*>(_uilayer->getWidgetByName("Button_button"));
    
    return true;
}

void MsgContentDialog::clickedReceive(CCObject*,TouchEventType type)
{
    if (TOUCH_EVENT_ENDED == type)
    {
        if (_data->isOK)//确定执行关闭
        {
            close();
        }
        else//执行领取操作
        {
            if (_data->isOK)//显示已经领取提示
            {
                Alert::create(ALERTTEXT_ATTACHMENT)->show();
            }
            else//发送领取消息,提示领取成功
            {
                _data->isOK = 1;
                _button->setTouchEnabled(false);
                updateMail(MailType_Receive);
                Alert::create(ALERTTEXT_ATTACHMENT_SUCCESS)->show();
            }
        }
    }
}

void MsgContentDialog::updateMail(MailType mailType)
{
    if (mailType == MailType_Read && _data->isRead)
        return;
    
    GameLogicMeg2Sever tmpMeg0;
    tmpMeg0.m_id = OGID_TEXAS_SLOTS_REQMAILSTATE;
    tmpMeg0.mailtype = mailType;//1-读取  3-收取附件
    tmpMeg0.mailid = _data->mailid;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg0);
    
    GameLogicMeg2Sever tmpMeg1;
    tmpMeg1.m_id = OGID_TEXAS_SLOTS_REQMAILS;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
}

void MsgContentDialog::show()
{
    Dialog::show();
    
    setTitle(_data->mailTitle);
    setContent(_data->mailContent);
    setPrize(_data->mailNum);
    
    updateMail(MailType_Read);
}



void MsgContentDialog::setTitle(string title)
{
    _title->setText(title.c_str());
}

void MsgContentDialog::setContent(string content)
{
    _content->setText(content.c_str());
}

void MsgContentDialog::setPrize(int num)
{
    
    if (num > 0)
    {
        _prize->loadTexture(CCString::createWithFormat("prize%d.png",_data->mailType)->getCString(),UI_TEX_TYPE_PLIST);
    }
    else
    {
        _wprize->setVisible(false);
    }
    
    if ( _data->isOK )
    {
        _button->loadTextures("button_confirm.png", NULL, NULL , UI_TEX_TYPE_PLIST);//确定
        _button->addTouchEventListener(this, toucheventselector(Dialog::close));
    }
    else
    {
        _number->setText(CCString::createWithFormat("%D",num)->getCString());
        _button->loadTextures("button_receive.png", NULL, NULL,UI_TEX_TYPE_PLIST);//领取
        _button->addTouchEventListener(this, toucheventselector(MsgContentDialog::clickedReceive));
    }
}