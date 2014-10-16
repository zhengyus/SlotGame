//
//  RoomView.h
//  Slot
//
//  Created by ZivHoo on 14/9/15.
//
//

#ifndef __Slot__RoomView__
#define __Slot__RoomView__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;

template <typename T>
class RoomView :public CCLayer{
    
public:
    
    RoomView(string itemJson)
    {
        _itemJson = itemJson;
        _spacing = 300;
        _miniscale = .6;
        
        _initItemCallback = NULL;
        _selectedItemCallback = NULL;
    }
    
    virtual ~RoomView()
    {
        
    }
    
    static RoomView<T>* createWithItemJson(string itemJson)
    {
        RoomView<T>* p = new RoomView<T>(itemJson);
        if (p && p->init())
        {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return NULL;
    }
    
    void onEnter()
    {
        CCLayer::onEnter();
        _enabledRoom = true;
        setContentSize(m_pParent->getContentSize());
    }
    
    void AddData(MyRoomList t)
    {
        _data.push_back(t);
    }
    
    void addData(vector<T> data)
    {
        _data.clear();
        
        _data.push_back(data[1]);
        _data.push_back(data[0]);
        _data.push_back(data[2]);
    }
    
    void display()
    {
        initData();
        initUI();
    }
    
    void setInitItemCallback(CCObject* ccObj,void (CCObject::*initItemCallback)(Widget*,T))
    {
        _ccObj = ccObj;
        _initItemCallback = initItemCallback;
    }
    
    void setSelectedItemCallback(void(*selectedItemCallback)(T))
    {
        _selectedItemCallback = selectedItemCallback;
    }
    
    void onSelectedRoomEvent(CCObject* pSender, TouchEventType type)
    {
        if ( !_enabledRoom )
        {
            return;
        }
        
        static bool scale = false;
        Widget* widget = dynamic_cast<Widget*>(pSender);
        
        switch (type) {
            case TOUCH_EVENT_BEGAN:
            {
                if (!scale)
                {
                    scale = true;
                    widget->setScale(.95);
                }
            }
                break;
            case TOUCH_EVENT_MOVED:
                break;
            case TOUCH_EVENT_ENDED:
            {
                scale = false;
                _enabledRoom = false;
                
                widget->setScale(1);
                if (NULL != _selectedItemCallback)
                    _selectedItemCallback(_data[widget->getTag()]);
            }
                break;
            default:
            {
                widget->setScale(1);
                scale = false;
            }
                break;
        }
    }
    
protected:
    
    virtual void initData()
    {
        CCSize size = boundingBox().size;
        
        _mX = size.width / 2.0;
        _sX = _mX - _spacing;
        _eX = _mX + _spacing;
        _y = size.height / 2.0;
    }
    
    void initUI()
    {
        removeAllChildren();
        UILayer* root = UILayer::create();
        Widget* widget = NULL;
        Widget* room = NULL;
        Widget* room_btn = NULL;
        for (int i = 0; i < _data.size(); ++i)
        {
            widget = GUIReader::shareReader()->widgetFromJsonFile(_itemJson.c_str());
            widget->setAnchorPoint(ccp(.5, .5));
            widget->setScale(.9);
            widget->setPosition(ccp(_sX + _spacing * i, _y));
            
            room = widget->getChildByName("Image_back");
            room_btn = room->getChildByName("Image_enter");
            room->setTouchEnabled(true);
            room->setTag(i);
            room_btn->setTag(i);
            room->addTouchEventListener(this, toucheventselector(RoomView::onSelectedRoomEvent));
            room_btn->addTouchEventListener(this, toucheventselector(RoomView::onSelectedRoomEvent));
            if(NULL != _initItemCallback)
            {
                (_ccObj->*_initItemCallback)(widget,_data[i]);
            }
            
            root->addWidget(widget);
            _rooms.push_back(room);
        }
        addChild(root);
    }
    
private:
    
    vector<Widget*> _rooms;
    vector<T>       _data;
    string          _itemJson;
    bool            _enabledRoom;
    
    int _y;             //所有Item y位置
    float _sX;          //Item开始x位置
    float _mX;          //Item中间x位置
    float _eX;          //Item结束x位置
    
    int _spacing;       //前面Item间距
    float _miniscale;   //最小缩放
    
    void (CCObject::*_initItemCallback)(Widget*,T);
    void (*_selectedItemCallback)(T);
    
    CCObject* _ccObj;
};

#endif /* defined(__Slot__RoomView__) */
