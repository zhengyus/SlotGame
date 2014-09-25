#ifndef __SLOT_BASELAYER_H__
#define __SLOT_BASELAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SceneManager.h"
#include "SoundNameDefine.h"
#include "SimpleAudioEngine.h"
#include "ext/ui/LoadingDialog.h"
#include "define.h"
#include "AlertText.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
using namespace CocosDenshion;

class BaseLayer : public cocos2d::CCLayer
{
    
public:
    
    BaseLayer(string notificationName = "",bool loadingEnable = false, int loadTotal = 0);
    virtual ~BaseLayer();
    
public:
    
    bool init();
    void onEnter();
    void onExit();
    
public:
    
    virtual void receiveBaseMsg(CCObject* obj);
    
protected:
    
    void showLoading();
    void loadStepup();
    void addNodeTo(std::string,CCNode*);
    void addChildTo(std::string,CCNode*);
    Widget* addContentWithJsonFile(std::string jsonFile,bool visible = true,int zorder = 0);
    void bindTouchEvent(std::string name,CCObject *target, cocos2d::ui::SEL_TouchEvent selector);
    
    Widget* getWidgetByName(string name);
protected:
    
    cocos2d::ui::UILayer*   _uilayer;
//    cocos2d::ui::Widget*    _widget;
    LoadingDialog*          _loading;
    
private:
    int         _loadStep;          //加载步骤
    int         _loadTotal;         //加载总数
    bool        _loadingEnable;     //是否启用loading
    
    std::string _notificationName;  //注册通知名称
};

#endif
