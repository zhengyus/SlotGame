#ifndef __SLOT_GAMEHALLLAYER_H__
#define __SLOT_GAMEHALLLAYER_H__

#include "BaseLayer.h"
#include "HeadView.h"
#include "NoticeView.h"
#include "HallDataManager.h"
#include "RoomView.h"

USING_NS_CC;

class GameHallLayer : public BaseLayer
{
    
public:
    
    GameHallLayer();
    ~GameHallLayer();
    
public:
    
    CREATE_FUNC(GameHallLayer);
    
public:
    
    bool init();
    void onEnter();    
    void onExit();
public:
    
    void showRooms(vector<MyRoomList>);   //初始化房间
    void updateNoticeView(float);
    
    void updateMsgStatus();
    
    void initRoomItemCallback(Widget* widget, MyRoomList room);
    void setEnableRoom(bool);
    
public:
    
    void clickedRankEvent(CCObject*,TouchEventType);
    void clickedJPRankEvent(CCObject*,TouchEventType);
    void clickedMessageEvent(CCObject*,TouchEventType);
    void clickedPetEvent(CCObject*,TouchEventType);
    void clickedHelpEvent(CCObject*,TouchEventType);
    
    void selectedRoomEvent(CCObject*,TouchEventType);
    
public:
    
    Widget*     _wrooms;
    Widget*     _newMsg;
    NoticeView* _noticeView;
    RoomView<MyRoomList>* _roomView;
public:
    HallDataManager* _hallData;
};

#endif
