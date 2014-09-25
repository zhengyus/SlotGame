//
//  DialogBuyStone.cpp
//  Slot
//
//  Created by huxf on 14-9-18.
//
//

#include "BuyStoneDialog.h"

BuyStoneDialog::BuyStoneDialog():Dialog("dialog_simple.png")
{
    _num = 1;
}

bool BuyStoneDialog::init()
{
    if (!Dialog::initContentWithJsonFile("UI4DialogPetStone.ExportJson"))
        return false;
    
    _btnClose->setTouchEnabled(false);
    _btnClose->setVisible(false);
    
    _txtNum = dynamic_cast<TextField*>(getWidgetByName("TextField_num"));
    _lblBean = dynamic_cast<Label*>(getWidgetByName("Label_bean"));
    
    _txtNum->addEventListenerTextField(this, textfieldeventselector(BuyStoneDialog::textFieldListener));
    bindTouchEvent("Button_reduce", this, toucheventselector(BuyStoneDialog::clickedReduceEvent));
    bindTouchEvent("Button_add", this, toucheventselector(BuyStoneDialog::clickedAddEvent));
    bindTouchEvent("Button_confirm", this, toucheventselector(BuyStoneDialog::clickedConfirmEvent));
    
    return true;
}

void BuyStoneDialog::textFieldListener(CCObject*, TextFiledEventType type)
{
    if (type == TEXTFIELD_EVENT_INSERT_TEXT || type == TEXTFIELD_EVENT_DELETE_BACKWARD)
    {
//        int num = atoi(_txtNum->getStringValue());
//        _txtNum->setText(CCString::createWithFormat("%d",num));
    }
}

void BuyStoneDialog::clickedReduceEvent(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        number(-1);
    }
}

void BuyStoneDialog::clickedAddEvent(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        number(1);
    }
}

void BuyStoneDialog::clickedConfirmEvent(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {

    }
}

void BuyStoneDialog::number(int i)
{
    if (-1 == i && _num > 1)
    {
        --_num;
    }
    else if( i == 1)
    {
        ++_num;
    }
    else
    {
        return;
    }
    
    _txtNum->setText(CCString::createWithFormat("%d",_num)->getCString());
    _lblBean->setText(CCString::createWithFormat("%d",_num*1000)->getCString());
}