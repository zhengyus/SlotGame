//
//  GameLayer.cpp
//  Slot
//
//  Created by huxf on 14-9-1.
//
//

#include "GameLayer.h"
#include "ext/ui/RevolveView.h"

#include "RankDialog.h"
#include "RankJPDialog.h"
#include "MsgDialog.h"
#include "PetDialog.h"
#include "ChoosePrizeDialog.h"
#include "HallDataManager.h"
#include "ext/ui/Alert.h"


GameLayer::GameLayer():BaseLayer(EVENT_REC_FROM_MSG_ALL , true , 2),
_flag(false),_gameLayer(NULL)
{
    _first = true;
    _dataManager = DataManager::sharedDataManager();
    _hallData = HallDataManager::getInstance();
}

GameLayer::~GameLayer()
{
    _hallLayer->release();
    if (NULL != _gameLayer)
    {
        _gameLayer->release();
    }
}

CCScene* GameLayer::scene()
{
    CCScene* scene = CCScene::create();
    scene->setTag(SceneTypeGame);
    auto layer = GameLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameLayer::init()
{
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    addContentWithJsonFile("UI4Game.ExportJson");
    
    _headView = HeadView::getInstance();
    addNodeTo("Panel_head",_headView);
    
    _hallLayer = GameHallLayer::create();
    _hallLayer->retain();
    addNodeTo("Panel_main", _hallLayer);
    
    this->scheduleUpdate();
    
    return true;
}

void  GameLayer::onEnter()
{
    BaseLayer::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameLayer::receiveUIMsg), EVENT_REC_FROM_MSG_UILOGIC, NULL);
}

void GameLayer::update(float dt)
{
    if (!_flag)
    {
        _flag = true;
    }
    else
    {
        LoginMeg2 logmeg2;
        logmeg2.login1.set_userid(_dataManager->userid);
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_LINKGAME2MSG, &logmeg2);
        
        this->unscheduleUpdate();
    }
}

void GameLayer::receiveUIMsg(CCObject* obj)
{
    UiLogicMsg* msg = dynamic_cast<UiLogicMsg*>(obj);
    
    switch (msg->id) {
        case 1://金币动画
        {
            _headView->playGoldAnimation();
            break;
        }
        default:
            break;
    }
}

void GameLayer::receiveBaseMsg(CCObject* obj)
{
    Meg2UIDate* data = (Meg2UIDate*)obj;
    
    switch (data->m_id)
    {
        case ERR_RET:
        {
            CCLOG("ERR_RET %d",ERR_RET);
            Alert::create(data->errmeg,"提示",this,touchesureventselector(GameLayer::clickedSureCallback))->show();
            break;
        }
        case OGID_TEXAS_SLOTS_PETLIST://宠物信息
        {
            _headView->setName(data->roleName);
            _headView->setLevel(data->roleLv);
            _headView->setExp(data->currExp, data->needExp);
            _headView->setBean(data->gold);
            
            //Socket 链接
            int sex = DataManager::sharedDataManager()->sex;
            if (0 != sex && 1 != sex)
            {
                sex = 0;
            }
            _headView->setHead(CCString::createWithFormat("head_small%d.png",sex)->getCString());
            
            _hallData->_pets = data->mPetList;
            loadStepup();   
            break;
        }
        case OGID_TEXAS_SLOTS_PLAYERINFO:
        {
            _headView->setLevel(_dataManager->currLevel);
            _headView->setExp(_dataManager->currExp, _dataManager->needExp);
            _headView->setBean(_dataManager->currGold);
            break;
        }
        case OGID_TEXAS_SLOTS_JACKPOT://JP巨奖
        {
            _headView->setRank(data->jp);
            break;
        }
        case OGID_TEXAS_SLOTS_ROOMLIST://房间列表
        {
            if (_first)
            {
                _first = false;
                _hallLayer->showRooms(data->mroomList);
                loadStepup();
            }
            break;
        }
        case OGID_TEXAS_SLOTS_ACKJACKPOTLIST://中奖记录
        {
            for (int i = 0; i < 2; ++i)
            {
                _hallData->_JPRanks[i] = data->mJPRankList[i];
            }
            
            if (!_hallData->_JPRanks[0].empty())
            {
                MyJPRankList JPrank = _hallData->_JPRanks[0][0];
                string title = "JACKPOT明星";
                Notice notice = {title,JPrank.JPrankName,JPrank.JPrankwinGold};
                _hallData->_notices.push_back(notice);
            }
            break;
        }
        case OGID_TEXAS_SLOTS_RANKINGDATA://积分排行
        {
            /**
             * 保存周一、周四排行榜及其历史排行榜到全局
             * 保存周一、周四历史排行榜第一名到公告列表中
             */
            for (int i = 0; i < 4; ++i)
            {
                _hallData->_ranks[i] = data->mrankListArr[i];
                if (i > 1 && !_hallData->_ranks[i].empty())
                {
                    MyRankList rank = _hallData->_ranks[i][0];
                    string title = i==2 ? "周一榜冠军" : "周四榜冠军";
                    Notice notice = {title,rank.rankName,rank.rankGold};
                    _hallData->_notices.push_back(notice);
                }
            }
            break;
        }
        case OGID_TEXAS_SLOTS_BORADCAST://广播消息（公告）
        {
            break;
        }
        
        case OGID_TEXAS_SLOTS_ACKMAILS ://邮件，有新邮件显示new图片
        {
            _hallData->_mails = data->mMailvector;
            break;
        }
        case OGID_TEXAS_SLOTS_ROOMSITE://选择房间服务器返回信息
        {
            enterRoomCallback();
            break;
        }
        case OGID_TEXAS_SLOTS_FIGHTREWARD://战斗胜利抽奖
        {
            CCLog("%d",data->mPetAWDList[0].Aok);
            ChoosePrizeDialog::createWithData(data->mPetAWDList)->show();
            break;
        }
        case GAME_LAYER_BACK://游戏界面后退到大厅
        {
            onClickBackEvent();
            break;
        }
        default:
            break;
    }
}

void GameLayer::enterRoomCallback()
{
    CCLog("select room----------------- %d",_dataManager->ret);
    
    switch (_dataManager->ret) {
        case 0:
        {
            if (NULL == _gameLayer)
            {
                _gameLayer = KCGameLayer::create();
                _gameLayer->retain();
            }
            else
            {
                _gameLayer->initUI();
            }
            showLayer(_gameLayer);
            break;
        }
        default:
        {
            switch (_dataManager->err) {
                case 0:
                {
                    Alert::create(ALERTTEXT_UNKNOWN)->show();
                    break;
                }
                case 1:
                {
                    Alert::create(ALERTTEXT_LEVEL)->show();
                    exitRoom();
                    break;
                }
                case 2:
                {   Alert::create(ALERTTEXT_GOLD)->show();
                    exitRoom();
                    break;
                }
                case 3:
                {
                    Alert::create(ALERTTEXT_ROOM)->show();
                    break;
                }
                case 4:
                {
                    Alert::create(ALERTTEXT_NO_ROOM)->show();
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
    }
}

void GameLayer::clickedSureCallback()
{
    SceneManager::getInstance()->replaceScene(SceneTypeStart);
}

void GameLayer::onClickBackEvent()
{
    showLayer(_hallLayer);
}

void GameLayer::showLayer(CCLayer* layer)
{
    _headView->setMenuVisible(layer == _hallLayer);
    
    Widget* widget = _uilayer->getWidgetByName("Panel_main");
    widget->removeAllNodes();
    widget->addNode(layer);
}

void GameLayer::exitRoom()
{
    GameLogicMeg2Sever tmpMeg1;
    tmpMeg1.m_id = OGID_TEXAS_SLOTS_EXITGAME;
    tmpMeg1.msgtyoe = 1;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SEND_MEG2SEVER, &tmpMeg1);
}