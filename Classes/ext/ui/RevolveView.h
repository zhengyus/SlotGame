/*
 * RevolveView.h
 *
 *  Created on: 2014年8月20日
 *      Author: Ziv
 */

#ifndef __LIB_REVOLVEVIEW_H__
#define __LIB_REVOLVEVIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;

template <typename T>
class RevolveView : public cocos2d::CCLayer
{
    typedef enum
    {
        DirectionType_LEFT,
        DirectionType_RIGHT
    }DirectionType;
   
public:
    
    RevolveView(string itemJson):_showIndex(1)
    {
        _itemJson = itemJson;
        _fSpacing = 300;
        _miniscale = .6;
        
        _onInitItemEvent = NULL;
        _onSelectedItemEvent = NULL;
    }
    
    ~RevolveView()
    {
    }
    
public:
    bool init()
    {
        if ( !CCLayer::init() )
            return false;
        
        setTouchEnabled(true);
        setTouchPriority(0);
        
        return true;
    }
    
    void onEnter()
    {
        CCLayer::onEnter();
        setContentSize(m_pParent->getContentSize());
    }
    
    void registerWithTouchDispatcher()
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    }
    
    bool ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent)
    {
        CCNode* parent = getParent();
        _spos = pTouch->getLocationInView();
        
        if (parent->boundingBox().containsPoint(CCDirector::sharedDirector()->convertToGL(_spos)))
        {
            struct timeval tv;
            gettimeofday(&tv,NULL);
            _touchBeginTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
            return true;
        }
        
        return false;
    }
    
    void ccTouchMoved (CCTouch *pTouch, CCEvent *pEvent)
    {
        CCPoint pos = pTouch->getLocationInView() - _spos;
        int max = 8;
        if (pos.x > max)
        {
            pos.x = max;
        }
        else if (pos.x < max*-1)
        {
            pos.x = max*-1;
        }
        
        _directionType = pos.x > 0 ? DirectionType_RIGHT : DirectionType_LEFT;
        
        updateItems(pos);
        
        _spos = pTouch->getLocationInView();
    }
    
    void ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent)
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        
        CCPoint pos = pTouch->getLocationInView();
        
        //选择房间
        if((tv.tv_sec * 1000 + tv.tv_usec / 1000 - _touchBeginTime < 200) && NULL != _onSelectedItemEvent)
            selectRoom(pos);
        //Item归位
        else
            resetPos();
    }
    
public:
    
    static RevolveView* createWithJsonItem(Widget* parent , string itemJson)
    {
        RevolveView* p = new RevolveView(itemJson);
        if (p && p->init())
        {
            p->autorelease();
            parent->addNode(p);
            return p;
        }
        CC_SAFE_DELETE(p);
        return NULL;
    }
    
public:
    
    void AddData(T t)
    {
        _data.push_back(t);
    }
    
    void addData(vector<T> data)
    {
        _data.clear();
        for (int i = 0; i < data.size(); ++i)
        {
            _data.push_back(data[i]);
        }
    }
    
    void display()
    {
        initData();
        initUI();
    }
    
public:
    
    void setOnInitItemEvent(void (*onInitItemEvent)(Widget*,T))
    {
        _onInitItemEvent = onInitItemEvent;
    }
    
    void setSelectedItemEvent(void(*onSelectedItemEvent)(Widget*,T))
    {
        _onSelectedItemEvent = onSelectedItemEvent;
    }
    
protected:
    
    void selectRoom(CCPoint& pos)
    {
        for (int i = 0; i < _items.size(); ++i)
        {
            Widget* widget = _items[i];
            if (widget->getTag())
                continue;
            
            if (widget->boundingBox().containsPoint(CCDirector::sharedDirector()->convertToGL(pos)))
            {
                _onSelectedItemEvent(_items[i],_data[i]);
                break;
            }
        }
    }
    
    void resetPos()
    {
        
    }
    
    void updateItems(CCPoint& offsetPos)
    {
        vector<Widget*>::iterator iter;
        for (iter = _items.begin(); iter != _items.end();++iter)
        {
            Widget* item = *iter;
            
            if (item->getTag() == 0)//前面显示item
            {
                setFrontPos(item,offsetPos.x);
                setScale(item);
                
                float x = item->getPosition().x;
                
                if (((x < _sX) && (DirectionType_LEFT == _directionType )) || ((x > _eX) && (DirectionType_RIGHT == _directionType)))
                {
                    item->setZOrder(0);
                    item->setTag(1);
                }
            }
            else
            {
                setBackPos(item,offsetPos.x);
                setAngle(item);
                
                float x = item->getPosition().x;
                if (((x < _sX) && (DirectionType_RIGHT == _directionType)) || ((x > _eX) && (DirectionType_LEFT == _directionType)))
                {
                    item->runAction(CCOrbitCamera::create(0, 1, 0, 0, 0, 0, 0));
                
                    setScale(item);
                    item->setZOrder(1);
                    item->setTag(0);
                }
            }
        }
    }
    
    void setFrontPos(Widget* item,float offsetX)
    {
        float tmp = item->getPosition().x + offsetX;
        
        if (tmp > _eX || tmp < _sX)
        {
            item->setScale(.5);
            item->setTag(1);
            
            if(DirectionType_LEFT == _directionType)
            {
                tmp -= _sX;
                item->setPositionX(_sX);
                CCLog("Front < _sX %f",tmp);
            }
            else
            {
                tmp -= _eX;
                item->setPositionX(_eX);
                CCLog("Front > _eX %f",tmp);
            }
            
            item->setZOrder(0);
            setBackPos(item,tmp);
        }
        else
        {
            item->setPositionX(tmp);
        }
    }
    
    void setBackPos(Widget* item,float offsetX)
    {
        float tmp = item->getPosition().x + offsetX * _spac * -1;
        
        if (tmp > _eX || tmp < _sX)
        {
            item->setTag(0);
            item->runAction(CCOrbitCamera::create(0, 1, 0, 90, 0, 0, 0));
            
            if(DirectionType_LEFT == _directionType)
            {
                tmp -= _eX;
                item->setPositionX(_eX);
                CCLog("Back > _eX %f",tmp);
            }
            else
            {
                tmp -= _sX;
                item->setPositionX(_sX);
                CCLog("Back > _sX %f",tmp);
            }
            
            item->setZOrder(1);
            setFrontPos(item,tmp*-1);
        }
        else
        {
            item->setPositionX(tmp);
        }
    }
    
    void setScale(Widget* widget)
    {
        float offsetx = _fSpacing - fabsf(widget->getPositionX() - m_obContentSize.width / 2);
        
        float scale = _miniscale + offsetx * _scale;
        widget->setScale(scale);
    }
    
    void setAngle(Widget* widget)
    {
        widget->runAction(CCOrbitCamera::create(0, 1, 0, _angle * (widget->getPositionX() - _sX), 0, 0, 0));
    }
    
    virtual void initData()
    {
        CCSize size = boundingBox().size;
        
        int mx = size.width * 1.0 / 2;
        _sX = mx - _fSpacing;
        _eX = mx + _fSpacing;
        _y = size.height / 2;
        
        _bSpacing = (_eX - _sX) / (_data.size() - 2);
        _angle = 180 * 1.0 / (_eX - _sX);
        _scale = (1 - _miniscale) / _fSpacing;
        
        _spac = _bSpacing * 1.0 / _fSpacing;
    }
    
    void initUI()
    {
        removeAllChildren();
        
        Widget* widget = NULL;
        
        int size = _data.size();
        
        for (int i = 0; i < size ; ++i)
        {
            widget = GUIReader::shareReader()->widgetFromJsonFile(_itemJson.c_str());
            widget->setAnchorPoint(ccp(.5, .5));
            widget->setTag(i);
            
            if(NULL != _onInitItemEvent)
            {
                _onInitItemEvent(widget,_data[i]);
            }
            
            if (i<3)
            {
                widget->setPosition(ccp(_sX + _fSpacing * i, _y));
                if (i!=1)
                {
                    widget->setZOrder(1);
                    widget->setScale(_miniscale);
                    
                }else{
                    widget->setZOrder(2);
                }
                
                //                widget->setVisible(false);
                widget->setTag(0);
            }
            else
            {
                //                widget->setVisible(false);
                widget->setZOrder(0);
                widget->setScale(.5);
                
                widget->setPosition(ccp(_sX + (i - 2)*_bSpacing, _y));
                setAngle(widget);
                
                widget->setTag(1);
            }
            _items.push_back(widget);
            
            addChild(widget);
        }
    }
    
private:
    
    vector<Widget*> _items;     //所有Item。Tag 0：前面Item，1：后面Item
    
    vector<T>       _data;
    string          _itemJson;
    int             _showIndex;
    
    int _y;
    float _sX;          //Item开始x位置
    float _eX;          //Item结束x位置
    
    int _fSpacing;      //前面Item间距
    int _bSpacing;      //后面Item间距
    float _spac;        //前后Item间距比例
    
    float _miniscale;   //最小缩放
    float _scale;       //前缩放比例
    float _angle;       //角度比例
    
    CCPoint _spos;                  //touch开始坐标
    DirectionType _directionType;   //touch方向
    
    long _touchBeginTime;    //开始时间，用于判断touch和selected
    
    
    void (*_onInitItemEvent)(Widget*,T);
    void (*_onSelectedItemEvent)(Widget*,T);
    
};

#endif
