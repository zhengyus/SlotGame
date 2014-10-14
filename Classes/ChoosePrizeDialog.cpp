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
        selectCard(_imgs[0]);
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

        selectCard(target);
    }
}

void ChoosePrizeDialog::selectCard(ImageView* img)
{
    //取消定时器
    unschedule(schedule_selector(ChoosePrizeDialog::time));
    _imgTime->setVisible(false);
    _confirm->setVisible(true);
    
    int target = img->getTag();
    
    MyPetFAward prize;
    vector<ImageView*> imgs;
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
        
        tmpImg = _imgs[ti];
        tmpImg->setTag(i);
        tmpImg->setTouchEnabled(false);
        
        if (prize.Aok)
        {
            imgs.insert(imgs.begin() , tmpImg);
        }
        else
        {
            imgs.push_back(tmpImg);
        }
        
//        setCardAndPrize(i,card,prizeStr);
//        
//
//        tmpImg->loadTexture(card.c_str(),UI_TEX_TYPE_PLIST);
//        type = static_cast<Label*>(tmpImg->getChildByName("Label_type"));
//        num = static_cast<Label*>(tmpImg->getChildByName("Label_num"));
//        
//        type->setVisible(true);
//        num->setVisible(true);
//        
//        type->setText(prizeStr);
//        num->setText(CCString::createWithFormat("%lld",prize.Anum)->getCString());
    }
    
    playSelectAnimation(imgs);
}

void ChoosePrizeDialog::playSelectAnimation(vector<ImageView*> imgs)
{
    CCAction* action = NULL;
    ImageView* img =   NULL;
    for (int i = 0; i < 3; ++i)
    {
        img = imgs[i];
        if(0 == i)
        {
            action = CCSequence::create(
                                                    CCScaleTo::create(.4, 0,1),
                                                    CCCallFuncO::create(this, callfuncO_selector(ChoosePrizeDialog::playAnimation), img),NULL);
        }
        else
        {
            action = CCSequence::create(
                                                    CCScaleTo::create(.4, 0,1),
                                                    CCCallFuncO::create(this, callfuncO_selector(ChoosePrizeDialog::setTexture), img),
                                                    CCScaleTo::create(.2, 1,1),NULL);
            
            
        }
        img->runAction(action);
    }
}


void ChoosePrizeDialog::playAnimation(CCObject* obj)
{
    string      prizeStr;
    string      card;
    
    ImageView* img = static_cast<ImageView*>(obj);
    
    setCardAndPrize(img->getTag(),card,prizeStr);
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCString::createWithFormat("%s%s",card.c_str(),".ExportJson")->getCString());
    
    CCArmature* armature = CCArmature::create(card.c_str());
    armature->setScaleX(0);
    armature->setAnchorPoint(ccp(.5, 0));
    armature->setPositionX(armature->getContentSize().width / 2);
    static_cast<Widget*>(img->getParent())->addNode(armature);
    armature->getAnimation()->playWithIndex(0);
    
    armature->runAction(CCScaleTo::create(.4, 1,1));
    setWord(img,prizeStr);
    img->removeFromParent();
}

void ChoosePrizeDialog::setTexture(CCObject* obj)
{
    string      prizeStr;
    string      card;
    
    
    ImageView* img      = static_cast<ImageView*>(obj);
    
    
    setCardAndPrize(img->getTag(),card,prizeStr);
    
    img->loadTexture(card.c_str(),UI_TEX_TYPE_PLIST);
    setWord(img,prizeStr);
    
}

void ChoosePrizeDialog::setWord(ImageView* img,string prizeStr)
{
    MyPetFAward prize   = _data[img->getTag()];
    Widget*     parent   = static_cast<Widget*>(img->getParent());
    
    Label*      type = static_cast<Label*>(parent->getChildByName("Label_type"));
    Label*      num = static_cast<Label*>(parent->getChildByName("Label_num"));
    
    type->setVisible(true);
    num->setVisible(true);
    type->setZOrder(10);
    num->setZOrder(10);
    
    type->setScaleX(0);
    num->setScaleX(0);
    
    type->setText(prizeStr);
    num->setText(CCString::createWithFormat("%lld",prize.Anum)->getCString());
    
    type->runAction(CCScaleTo::create(.4, 1,1));
    num->runAction(CCScaleTo::create(.4, 1,1));
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
        tmp.append("_grey.png");
    
    card = tmp;
}