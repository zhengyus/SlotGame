#include "GameHallLayer.H"
#include "define.h"

#include "RankDialog.h"
#include "RankJPDialog.h"
#include "MsgDialog.h"
#include "PetDialog.h"
#include "HallDataManager.h"
#include "DataManager.h"
#include "RoomView.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;


typedef void (CCObject::*SEL_InitItemEvent)(Widget*, MyRoomList);
#define inititemventselector(_SELECTOR) (SEL_InitItemEvent)(&_SELECTOR)

GameHallLayer::GameHallLayer()
{
    _hallData = HallDataManager::getInstance();
}

GameHallLayer::~GameHallLayer()
{
}

bool GameHallLayer::init()
{
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    addContentWithJsonFile("UI4GameHall.ExportJson");
    
    _wrooms = _uilayer->getWidgetByName("panel_rooms");
    
    Widget* widget = _uilayer->getWidgetByName("Panel_notice");
    _noticeView = NoticeView::create();
    widget->addNode(_noticeView);
    
    bindTouchEvent("btn_rank", this, toucheventselector(GameHallLayer::clickedRankEvent));
    bindTouchEvent("btn_star_rank", this, toucheventselector(GameHallLayer::clickedJPRankEvent));
    bindTouchEvent("btn_petground", this, toucheventselector(GameHallLayer::clickedPetEvent));
    bindTouchEvent("btn_message", this, toucheventselector(GameHallLayer::clickedMessageEvent));
    
    return true;
}

void GameHallLayer::onEnter()
{
    BaseLayer::onEnter();
    
    schedule(schedule_selector(GameHallLayer::updateNoticeView), 5.0f);
}

void GameHallLayer::onExit()
{
    unschedule(schedule_selector(GameHallLayer::updateNoticeView));
    BaseLayer::onExit();
}

void GameHallLayer::initRoomItemCallback(Widget* widget, MyRoomList room)
{
    ImageView* img = static_cast<ImageView*>(UIHelper::seekWidgetByName(widget, "Image_back"));
    Button* enter = static_cast<Button*>(img->getChildByName("Button_enter"));
    Label*  lblWord = static_cast<Label*>(enter->getChildByName("Label_word"));
    Widget* panelFree = UIHelper::seekWidgetByName(widget, "Image_free_back");
    
    
    string room_back;
    string word;
    int level = DataManager::sharedDataManager()->currLevel;
    unsigned long long gold = DataManager::sharedDataManager()->currGold;
    
    if ( 1 == room.roomid || (2 == room.roomid && level >= 5) || (3 == room.roomid && gold >=1000000 ))
    {
        word = "进入";
        room_back = CCString::createWithFormat("room%d.png",room.roomid)->getCString();
        
        //显示免费转动次数
        if (DataManager::sharedDataManager()->currFreeNum > 0)
        {
            ImageView* freeIcon = static_cast<ImageView*>(UIHelper::seekWidgetByName(panelFree, "Image_free"));
            Label* freeNum = static_cast<Label*>(UIHelper::seekWidgetByName(panelFree, "Label_freeNum"));
            
            panelFree->setVisible(true);
            freeIcon->loadTexture(CCString::createWithFormat("free%d.png",room.roomid)->getCString(),UI_TEX_TYPE_PLIST);
            freeNum->setText(CCString::createWithFormat("x%d",DataManager::sharedDataManager()->currFreeNum)->getCString());
        }
    }
    else
    {
        panelFree->setVisible(false);
        if (2 == room.roomid)
        {
            word = "5级";
        }else{
            word = "100万";
        }
        room_back = CCString::createWithFormat("room%d_grey.png",room.roomid)->getCString();
    }
    

    string room_btn = CCString::createWithFormat("button_room%d.png",room.roomid)->getCString();
    
  
    enter->loadTextureNormal(room_btn.c_str(),UI_TEX_TYPE_PLIST);
    img->loadTexture(room_back.c_str(),UI_TEX_TYPE_PLIST);
    lblWord->setText(word);
}

void onSelectedRoomEvent( MyRoomList room)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
    GameLogicMeg2Sever tmpMeg1;
    tmpMeg1.m_id = OGID_TEXAS_SLOTS_JOINROOM;
    tmpMeg1.roomid = room.roomid;
    
    CCLog("----------selected room %d",room.roomid);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
}

void GameHallLayer::showRooms(vector<MyRoomList> rooms)
{
    RoomView<MyRoomList>* view = RoomView<MyRoomList>::createWithItemJson("UI4ItemRoom.ExportJson");
    view->setInitItemCallback(this,inititemventselector(GameHallLayer::initRoomItemCallback));
    view->setSelectedItemCallback(onSelectedRoomEvent);
    _wrooms->addNode(view);
    view->addData(rooms);
    view->display();
}

void GameHallLayer::updateNoticeView(float)
{
    static int i = 0;
    
    if ( _hallData->_notices.empty() )
    {
        return;
    }
    
    if ( i >= _hallData->_notices.size() )
    {
        i = 0;
    }
    
    Notice notice = _hallData->_notices[i];
    
    const char* rank = CCString::createWithFormat("奖励 %lld", notice.gold)->getCString();
    _noticeView->setNoticeWithAnimation(notice.title, notice.name, rank);
    
    ++i;
}

static void initRankItem(int i,Widget* item,MyRankList rank)
{
    char tmp[20];
    CCLOG("MyRankList---------%s",rank.rankName.c_str());
    Widget* wgt1 = UIHelper::seekWidgetByName(item,"Panel_num");
    Label* lbl2 = static_cast<Label*>(UIHelper::seekWidgetByName(item,"Label_nickname"));
    Label* lbl3 =  static_cast<Label*>(UIHelper::seekWidgetByName(item,"Label_scroe"));
    
    Widget* w = NULL;
    
    if (i<3)
    {
        char tmp[20];
        sprintf(tmp, "rank%d.png",i);
        ImageView* img = ImageView::create();
        img->loadTexture(tmp);
        w = img;
    }
    else
    {
        Label* lbl = Label::create();
        sprintf(tmp, "%d",i + 1);
        lbl->setText(string(tmp));
        w = lbl;
    }
    
    if (NULL!=w)
    {
        wgt1->addChild(w);
    }
    
    lbl2->setText(rank.rankName);
    
    sprintf(tmp, "%lld",rank.rankGold);
    lbl3->setText(string(tmp));
    
    if (i<3)
    {
        ccColor3B color = ccc3(255,197,5);
        lbl2->setColor(color);
        lbl3->setColor(color);
    }
    
}

void GameHallLayer::clickedRankEvent(CCObject*,TouchEventType event)
{
    if (event==TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        Dialog* dialog = RankDialog::create(_hallData->_ranks);
        dialog->show();
    }
}

void GameHallLayer::clickedJPRankEvent(CCObject*,TouchEventType event)
{
    if (event==TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        RankJPDialog::create(_hallData->_JPRanks)->show();
    }
}

void GameHallLayer::clickedPetEvent(CCObject*,TouchEventType event)
{
    if (event==TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        PetDialog::create()->show();
    }
}

void GameHallLayer::clickedMessageEvent(CCObject*,TouchEventType event)
{
    if (event==TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        Dialog* dialog = MsgDialog::create(_hallData->_mails);
        dialog->show();
    }
}

void GameHallLayer::clickedHelpEvent(CCObject*,TouchEventType event)
{
    if (event==TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        CCLOG("onHelp");
    }
}