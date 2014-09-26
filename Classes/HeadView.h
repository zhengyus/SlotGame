//
//  HeadView.h
//  Slot
//
//  Created by ZivHoo on 14/8/28.
//
//

#ifndef __Slot__HeadView__
#define __Slot__HeadView__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SoundNameDefine.h"
#include "SimpleAudioEngine.h"
#include "SceneManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
using namespace CocosDenshion;

/**
 * 游戏头部
 */
class HeadView : public CCLayer
{
    
public:
   
    ~HeadView();
    static HeadView* getInstance();
    
    bool init();
    
public:
    
    void setHead(string head);
    void setName(string name);
    void setLevel(int level);
    void setExp(int exp,int exptotal);
    void setBean(unsigned long long bean);
    unsigned long long getBean();
    void setRank(unsigned long long rank);
    void playGoldAnimation();
    
    void setMenuVisible(bool);
    void setBackVisible(bool);
    
public:
    void clickedAddEvent(CCObject*,TouchEventType event);
    void clickedMenuEvent(CCObject*,TouchEventType event);
    
private:
    HeadView();
    
private:
    ImageView*  _head;
    Label*      _name;
    Label*      _level;
    Label*      _lexp;
    LoadingBar* _pexp;
    LabelAtlas* _bean;
    LabelAtlas* _rank;
    Widget*     _menu;
    CCArmature* _goldAnimation;//金币动画
    Button*     _back;
};

#endif /* defined(__Slot__HeadView__) */
