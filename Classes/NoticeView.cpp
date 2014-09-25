//
//  NoticeView.cpp
//  Slot
//
//  Created by ZivHoo on 14/8/26.
//
//

#include "NoticeView.h"

NoticeView::NoticeView()
{
    _showIndex = 0;
}

NoticeView::~NoticeView()
{
    
}

bool NoticeView::init()
{
    
    if (!CCLayer::init()) {
        return  false;
    }
    
    Widget* widget = GUIReader::shareReader()->widgetFromJsonFile("UI4Notice.ExportJson");
    addChild(widget);
    
    _contents[0] = UIHelper::seekWidgetByName(widget, "Panel_content0");
    _contents[1] = UIHelper::seekWidgetByName(widget, "Panel_content1");
    
    return true;
}

void NoticeView::setNotice(string title,string name,string rank)
{
    setTitle(title);
    setName(name);
    setRank(rank);
}

void NoticeView::setNoticeWithAnimation(string title,string name,string rank)
{
    Widget* cur = _contents[_showIndex++];
    _showIndex = _showIndex > 1 ? 0 : _showIndex;
    Widget* next = _contents[_showIndex];
    
    setNotice(title,name,rank);
    
    CCMoveTo* mto1 = CCMoveTo::create(1, ccp(cur->getPositionX(), cur->boundingBox().size.height));
    CCSequence* seq = CCSequence::create(mto1,CCCallFunc::create(this, callfunc_selector(NoticeView::resetNotice)),NULL);
    CCMoveTo* mto2 = CCMoveTo::create(1, ccp(cur->getPositionX(), 0));

    cur->runAction(seq);
    next->runAction(mto2);
}

void NoticeView::resetNotice()
{
    int i = _showIndex+1;
    i = i > 1 ? 0 : i;
    
    Widget* w = _contents[i];
    w->setPosition(ccp(w->getPositionX(), w->getSize().height * -1));
}

/*
 
 
 Label_title
 Label_name
 Label_rank
 
 _title = static_cast<Label*>(UIHelper::seekWidgetByName(widget      , "Label_title"));
 _name = static_cast<Label*>(UIHelper::seekWidgetByName(widget       , "Label_name"));
 _rank = static_cast<Label*>(UIHelper::seekWidgetByName(widget       , "Label_rank"));
 */
void NoticeView::setIcon(string icon)
{
    ImageView* img = static_cast<ImageView*>(_contents[_showIndex]->getChildByName("Image_icon"));
    img->loadTexture(icon.c_str());
}

void NoticeView::setTitle(string title)
{
    Label* label = static_cast<Label*>(_contents[_showIndex]->getChildByName("Label_title"));
    label->setText(title);
}

void NoticeView:: setName(string name)
{
    Label* label = static_cast<Label*>(_contents[_showIndex]->getChildByName("Label_name"));
    label->setText(name.c_str());
}

void NoticeView::setRank(string rank)
{
    Label* label = static_cast<Label*>(_contents[_showIndex]->getChildByName("Label_rank"));
    label->setText(rank.c_str());
}
