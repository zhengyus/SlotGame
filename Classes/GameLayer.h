//
//  GameLayer.h
//  Slot
//
//  Created by huxf on 14-9-1.
//
//

#ifndef __Slot__GameLayer__
#define __Slot__GameLayer__


#include "BaseLayer.h"
#include "HeadView.h"
#include "NoticeView.h"
#include "KCGameLayer.h"
#include "GameHallLayer.h"
#include "DoubleLayer.h"
#include "DataManager.h"
#include "HallDataManager.h"
#include "CreateRoleDialog.h"


class GameLayer : public BaseLayer
{
    
public:
    
    GameLayer();
    ~GameLayer();
    
public:
    
    CREATE_FUNC(GameLayer);
    static cocos2d::CCScene* scene();
    
public:
    
    bool init();
    void onEnter();
    void update(float dt);
    
public:
    
    void showLayer(CCLayer* layer);
    void exitRoom();                    //  退出房间
    
public:
    void enterRoomCallback();
    void onClickBackEvent();
    void clickedSureCallback();
    
    void receiveBaseMsg(CCObject* obj) override;
    void receiveUIMsg(CCObject* obj);
    
public:
    
    HeadView*       _headView;
    
    GameHallLayer*  _hallLayer;
    KCGameLayer*    _gameLayer;

public:
    bool                    _flag;
    bool                    _first;
    
    DataManager*            _dataManager;
    HallDataManager*        _hallData;
    bool                    _continue_receive;
    
    
};

#endif /* defined(__Slot__GameLayer__) */
