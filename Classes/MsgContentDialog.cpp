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
        _data->isOK = 1;
        _button->setTouchEnabled(false);
        updateMail(MailType_Receive);
        Alert::create(ALERTTEXT_ATTACHMENT_SUCCESS,"",this,touchesureventselector(Dialog::close))->show();
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
    string texture = "button_confirm.png";
    SEL_TouchEvent func = toucheventselector(MsgContentDialog::close);
    
    //奖励物品数量大于0 并且 奖励物品的范围，以防奖励不存在的物品
    if (num > 0 && _data->mailType > 0 && _data->mailType < 5)
    {
        int    d;
        string prizeStr;
        switch (_data->mailType) {
            case 1://宠物奖励
            {
                d = _data->itemId - 1;//宠物图片从0开始计数，服务端数据从1开始计数
                prizeStr = "pethead%d.png";
                break;
            }
            case 2://道具奖励
            {
                d = _data->itemId;
                prizeStr = "prop%d.png";
                break;
            }
            default:
            {
                d = _data->mailType;
                prizeStr = "prize%d.png";
                break;
            }
        }
        
        prizeStr = CCString::createWithFormat(prizeStr.c_str(),d)->getCString();
        
        _number->setText(CCString::createWithFormat("%d",num)->getCString());
        _prize->loadTexture(prizeStr.c_str(),UI_TEX_TYPE_PLIST);
        
        if ( !_data->isOK )
        {
            texture = "button_receive.png";
            func = toucheventselector(MsgContentDialog::clickedReceive);
        }
    }
    else
    {
        _wprize->setVisible(false);
    }
    
    _button->loadTextures(texture.c_str(), NULL, NULL , UI_TEX_TYPE_PLIST);//确定
    _button->addTouchEventListener(this, func);

}