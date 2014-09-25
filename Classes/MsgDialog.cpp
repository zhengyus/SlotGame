//
//  OrderDialog.cpp
//  Slot
//
//  Created by ZivHoo on 14/8/26.
//
//

#include "MsgDialog.h"
#include "MsgContentDialog.h"

MsgDialog::MsgDialog():
ListDialog("dialog_msg.png")
{
}

MsgDialog::~ MsgDialog()
{
    
}

void initMsgItem(int i,Widget* item,MyMailMeg meg)
{
    ImageView* state = static_cast<ImageView*>(UIHelper::seekWidgetByName(item,"Image_state"));
    Label* lbl1 = static_cast<Label*>(UIHelper::seekWidgetByName(item,"Label_msg"));
    Label* lbl2 = static_cast<Label*>(UIHelper::seekWidgetByName(item,"Label_date"));
    
    const char* str = CCString::createWithFormat("state_read_%d.png",meg.isRead?1:0)->getCString();
    state->loadTexture(str,UI_TEX_TYPE_PLIST);
    lbl1->setText(meg.mailTitle);
    string date = meg.mailTime;
    str = CCString::createWithFormat("%c%c%c%c.%c%c.%c%c",date[0],date[1],date[2],date[3],date[5],date[6],date[8],date[9])->getCString();
    lbl2->setText(str);
}

bool MsgDialog::initWithData(vector<MyMailMeg> &data)
{
    if (! ListDialog::initWithJson("UI4DialogMsg.ExportJson", "UI4ItemMsg.ExportJson", data) )
    {
        return false;
    }
    
    setInitListItem(initMsgItem);
    
    _listView->addEventListenerListView(this, listvieweventselector(MsgDialog::selectedItemEvent));
    
    return true;
}

MsgDialog* MsgDialog::create(vector<MyMailMeg>& data)
{
    MsgDialog *pRet = new MsgDialog();
    if (pRet && pRet->initWithData(data))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    
    return NULL;
}

void MsgDialog::selectedItemEvent(CCObject*,ListViewEventType eventType)
{
    static long start;
    struct timeval tv;
    
    switch (eventType) {
        case LISTVIEW_ONSELECTEDITEM_START:
        {
            gettimeofday(&tv,NULL);
            start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
            break;
        }
        case LISTVIEW_ONSELECTEDITEM_END:
        {
            gettimeofday(&tv,NULL);
            if (tv.tv_sec * 1000 + tv.tv_usec / 1000 - start < 250)
            {
                int i = _listView->getCurSelectedIndex();
               
                MsgContentDialog::create((*_data)[i])->show();
                
                if (!(*_data)[i].isRead)//修改邮件图标为已读状态
                {
                    Widget* item = _listView->getItem(i);
                    (*_data)[i].isRead = true;
                    initMsgItem(i,item,(*_data)[i]);
                }
            }
            break;
        }
        default:
            break;
    }
}