//
//  StarRankDialog.cpp
//  Slot
//
//  Created by ZivHoo on 14/8/28.
//
//

#include "RankJPDialog.h"

RankJPDialog::RankJPDialog():
ListDialog<MyJPRankList>("dialog_jprank.png")
{
    
}

RankJPDialog::~RankJPDialog()
{
    
}

static void initRankItem(int i,Widget* item,MyJPRankList dataItem)
{
    Label* lbl1 = static_cast<Label*>(UIHelper::seekWidgetByName(item,"Label_nickname"));
    Label* lbl2 = static_cast<Label*>(UIHelper::seekWidgetByName(item,"Label_bean"));
    Label* lbl3 =  static_cast<Label*>(UIHelper::seekWidgetByName(item,"Label_rankpool"));
    Label* lbl4 =  static_cast<Label*>(UIHelper::seekWidgetByName(item,"Label_date"));
    
    lbl1->setText(dataItem.RoleName.c_str());
    
    char tmp[20];
    sprintf(tmp, "%lld",dataItem.JPrankwinGold);
    lbl2->setText(string(tmp));
    
    sprintf(tmp, "%lld",dataItem.JPrankcurrGold);
    lbl3->setText(string(tmp));
    
    string date = dataItem.JPrankTime;
    sprintf(tmp, "%c%c-%c%c %c%c:%c%c",date[5],date[6],date[8],date[9],date[11],date[12],date[14],date[15]);
    lbl4->setText(tmp);
    
    if (i<3)
    {
        ccColor3B color = ccc3(255,197,5);
        lbl1->setColor(color);
        lbl2->setColor(color);
        lbl3->setColor(color);
        lbl4->setColor(color);
    }
}

bool RankJPDialog::initWithData(vector<MyJPRankList> data[2])
{
    
    if ( !ListDialog::initWithJson("UI4DialogRankJP.ExportJson", "UI4ItemRankJP.ExportJson", data[JPRankType_Star]) )
    {
        return false;
    }
    
    for (int i = 0; i < 2; ++i)
    {
        _data[i] = data[i];
    }
    
    setInitListItem(initRankItem);
    
    _noticeView = NoticeView::create();
    Widget* widget = _uilayer->getWidgetByName("Panel_notice");
    widget->addNode(_noticeView);
    initNoticeView();
    
    bindTouchEvent("Button_starRank", this, toucheventselector(RankJPDialog::clickedStarEvent));
    bindTouchEvent("Button_bigRank", this, toucheventselector(RankJPDialog::clickedBigEvent));
    
    return true;
}

RankJPDialog* RankJPDialog::create(vector<MyJPRankList> data[2])
{
    RankJPDialog *pRet = new RankJPDialog();
    if (pRet && pRet->initWithData(data))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void RankJPDialog::initNoticeView()
{
    if (_data[0].empty())
    {
        return;
    }
    
    const char* rank = CCString::createWithFormat("奖励 %lld", _data[0][0].JPrankwinGold)->getCString();
    _noticeView->setNotice("JACKPOT明星", _data[0][0].JPrankName, rank);
}

void RankJPDialog::clickedStarEvent(CCObject, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        refresh(_data[JPRankType_Star]);
    }
}

void RankJPDialog::clickedBigEvent(CCObject, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        refresh(_data[JPRankType_Big]);
    }
}