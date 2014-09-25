#ifndef __SLOT_STARTLAYER_H__
#define __SLOT_STARTLAYER_H__

#include "BaseLayer.h"
#include "LoginLayer.h"
#include "RegisterLayer.h"
#include "CreateRoleDialog.h"

class StartLayer : public BaseLayer
{
    
public:
    
    StartLayer(int tarIndex);
    ~StartLayer();
    
public:
    
    static StartLayer* create(int tarIndex)
    {
        StartLayer *pRet = new StartLayer(tarIndex);
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    
    static cocos2d::CCScene* scene(int = 0);
    
public:
    
    bool init();
    void onEnter();
    void loadResources();
    
    void toggleLayer(int = 0,bool = true);
    void showCallback();
    
    void back();
    void login(string account,string pwd);
    void enabledStart();
    
    void loadCallback(CCObject*);
    void registerCallback(CCObject*);
    void createRoleCallback(CCObject*);
    void receiveBaseMsg(CCObject* obj)override;
    void clickedRegCallback();
public:
    
    void clickedStartEvent(CCObject *sender, TouchEventType event);
    
private:

    ImageView*      _imageStart;
    ImageView*      _imageProgress;
    LoadingBar*     _progressLoad;
    Button*         _btnStart;
    
    int             _showIndex;                 //默认显示 开始游戏
    CC_SYNTHESIZE(int, _tarIndex, TarIndex);    //要显示的
    
    CCNode*         _nodes[3];  //0 开始游戏，1 登陆， 3注册
    
    CreateRoleDialog*   _roleDialog;
    
    //切换layer需要
    CCNode* _showLayer;
    CCNode* _hideLayer;
    
    int             _loadSetup;
    CCArray*        _textures;
    CCUserDefault*  _userDefault;
};

#endif
