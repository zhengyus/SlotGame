//
//  ChoosePrizeDialog.cpp
//  Slot
//
//  Created by huxf on 14-9-4.
//
//

#include "ChoosePrizeDialog.h"

ChoosePrizeDialog::ChoosePrizeDialog(vector<MyPetFAward> data):
Dialog("dialog_ChoosePrize.png")
{
    _times = 3;
    _data = data;
}

ChoosePrizeDialog::~ChoosePrizeDialog()
{
    
}

ChoosePrizeDialog* ChoosePrizeDialog::createWithData(vector<MyPetFAward> data)
{
    ChoosePrizeDialog *pRet = new ChoosePrizeDialog(data);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void ChoosePrizeDialog::closeCallback()
{
    Meg2UIDate sendmeg;
    sendmeg.m_id = SET_ALL_THINGS_RUN;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_K_REC_MEG_FROM_SEVER, &sendmeg);
}

bool ChoosePrizeDialog::init()
{
    if ( !Dialog::initContentWithJsonFile("UI4DialogChoosePrize.ExportJson") )
        return false;
    
    _btnClose->setTouchEnabled(false);
    _btnClose->setVisible(false);
    
    _imgTime = getWidgetByName("Image_time");
    _confirm = getWidgetByName("Button_confirm");
    _time = static_cast<Label* >(getWidgetByName("Label_time"));
    
    for (int i = 0; i < 3; ++i)
    {
        _imgs[i] = static_cast<ImageView*>(getWidgetByName(CCString::createWithFormat("Image_card%d", i)->getCString()));
        _imgs[i]->setTouchEnabled(true);
        _imgs[i]->setTag(i);
        _imgs[i]->addTouchEventListener(this, toucheventselector(ChoosePrizeDialog::onClickedCardEvent));
    }

    _confirm->addTouchEventListener(this, toucheventselector(ChoosePrizeDialog::onClickedConfirmEvent));
    setCloseListener(this, closeeventselector(ChoosePrizeDialog::closeCallback));
    schedule(schedule_selector(ChoosePrizeDialog::time), 1.0f);
    return true;
}

void ChoosePrizeDialog::time()
{
    if (0 < _times)
    {
        _time->setText(CCString::createWithFormat("抽奖倒计时 %d",--_times)->getCString());
    }
    else
    {
        //自动选择奖励
        chooseCard(_imgs[0]);
    }
}

void ChoosePrizeDialog::onClickedConfirmEvent(CCObject*,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        closeCallback();
        Dialog::close();
    }
}

void ChoosePrizeDialog::onClickedCardEvent(CCObject* pSender,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        ImageView* target = static_cast<ImageView*>(pSender);

        chooseCard(target);
    }
}

void ChoosePrizeDialog::chooseCard(ImageView* img)
{
    //取消定时器
    unschedule(schedule_selector(ChoosePrizeDialog::time));
    _imgTime->setVisible(false);
    _confirm->setVisible(true);
    
    int target = img->getTag();
    
    Label*      type = NULL;
    Label*      num = NULL;
    string      prizeStr;
    string      card;
    MyPetFAward prize;
    ImageView*  tmpImg;
    
    int ti = 0;
    for (int i = 0, j = 0; i < _data.size() ; ++i)
    {
        prize = _data[i];
        
        if (prize.Aok)//中奖
        {
            ti = target;
        }
        else if(j == target)
        {
            ti = ++j;
            ++j;
        }
        else
        {
            ti = j++;
        }
        
        CCLog("ti:%d,%d",ti,j);

        tmpImg = _imgs[ti];
        setCardAndPrize(i,card,prizeStr);
        
        tmpImg->setTouchEnabled(false);//
        tmpImg->loadTexture(card.c_str(),UI_TEX_TYPE_PLIST);
        type = static_cast<Label*>(tmpImg->getChildByName("Label_type"));
        num = static_cast<Label*>(tmpImg->getChildByName("Label_num"));
        
        type->setVisible(true);
        num->setVisible(true);
        
        type->setText(prizeStr);
        num->setText(CCString::createWithFormat("%lld",prize.Anum)->getCString());
    }
}

void ChoosePrizeDialog::setCardAndPrize(int i,string&card,string&prizeStr)
{
    string tmp;
    //类型 1金币 2角色经验 3宠物经验 4宠物 5威客
    switch (_data[i].Atype) {
        case 1:
        {
            prizeStr = "万能豆奖励";
            tmp = "card_bean";
            break;
        }
        case 2:
        {
            prizeStr = "角色经验奖励";
            tmp = "card_exp";
            break;
        }
        case 3:
        {
            prizeStr = "宠物经验奖励";
            tmp = "card_petExp";
            break;
        }
        case 4:
        {
            prizeStr = "宠物奖励";
            tmp = "card_bean";
            break;
        }
        case 5:
        {
            prizeStr = "威客奖励";
            tmp = "card_bean";
            break;
        }
        default:
        {
            prizeStr = "xxx奖励";
            tmp = "card_exp";
            break;
        }
    }
    
    if ( !_data[i].Aok )
    {
        tmp.append("_grey");
    }
    tmp.append(".png");
    card = tmp;
}