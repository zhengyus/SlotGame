//
//  OrderDialog.cpp
//  Slot
//
//  Created by ZivHoo on 14/8/26.
//
//

#include "RankDialog.h"

RankDialog::RankDialog():
ListDialog("dialog_rank.png")
{
    
}

RankDialog::~ RankDialog()
{
    
}


void initRankItem(int i, Widget* item, MyRankList dataItem)
{
    Widget* node = NULL;
    if (i < 3)
    {
        ImageView* img = ImageView::create();
        img->loadTexture(CCString::createWithFormat("no%d.png",i)->getCString(),UI_TEX_TYPE_PLIST);
        node = img;
    }
    else
    {
        Label* lbl = Label::create();
        lbl->setFontSize(20);
        lbl->setText(CCString::createWithFormat("%d",i + 1)->getCString());
        node = lbl;
    }
    
    if (NULL != node)
    {
        RelativeLayoutParameter* param = RelativeLayoutParameter::create();
        param->setAlign(RELATIVE_CENTER_IN_PARENT);
        node->setLayoutParameter(param);
        
        Widget* panel = UIHelper::seekWidgetByName(item,"Panel_num");
        
        panel->addChild(node);
    }
    
    Label* lbl2 = static_cast<Label*>(UIHelper::seekWidgetByName(item,"Label_nickname"));
    Label* lbl3 =  static_cast<Label*>(UIHelper::seekWidgetByName(item,"Label_scroe"));
    
    lbl2->setText(dataItem.rankName);
    lbl3->setText(CCString::createWithFormat("%lld",dataItem.rankGold)->getCString());
    
    if (i < 3)
    {
        ccColor3B color = ccc3(255,197,5);
        lbl2->setColor(color);
        lbl3->setColor(color);
    }
}

bool RankDialog::initWithData(vector<MyRankList> data[4])
{
    if (! ListDialog::initWithJson("UI4DialogRank.ExportJson", "UI4ItemRank.ExportJson", data[0]) )
    {
        return false;
    }
    
    for (int i = 0; i < 4; ++i)
    {
        _data[i] = data[i];
    }
    
    setInitListItem(initRankItem);
    
    Widget* w = getWidgetByName("btn_myrank");
    w->setVisible(false);
    w = getWidgetByName("btn_rankrule");
    w->setVisible(false);
    
    bindTouchEvent("btn_monday", this, toucheventselector(RankDialog::clickedMondayEvent));
    bindTouchEvent("btn_thursday", this, toucheventselector(RankDialog::clickedThursdayEvent));
    bindTouchEvent("btn_mondayh", this, toucheventselector(RankDialog::clickedMondayHistoryEvent));
    bindTouchEvent("btn_thursdayh", this, toucheventselector(RankDialog::clickedThursdayHistoryEvent));
    
    return true;
}

RankDialog* RankDialog::create(vector<MyRankList> data[4])
{
    RankDialog *pRet = new RankDialog();
    if (pRet && pRet->initWithData(data))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    
}

void RankDialog::clickedMondayEvent(CCObject, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        refresh(_data[RankType_Monday]);
    }
}

void RankDialog::clickedThursdayEvent(CCObject, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        refresh(_data[RankType_Thursday]);
    }
}

void RankDialog::clickedMondayHistoryEvent(CCObject, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        refresh(_data[RankType_MondayHistory]);
    }
}

void RankDialog::clickedThursdayHistoryEvent(CCObject, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        refresh(_data[RankType_ThursdayHistory]);
    }
}