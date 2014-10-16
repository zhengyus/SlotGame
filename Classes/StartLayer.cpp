#include "StartLayer.h"
#include "LoginLayer.h"
#include "Alert.h"
#include "define.h"
#include "GameMsgDefine.h"
#include "UserAccountService.h"
#include "DataManager.h"
#include "ChoosePrizeDialog.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

StartLayer::StartLayer(int tarIndex):BaseLayer(EVENT_LOGINMES_FROM_MSG)
{
    _loadSetup = 0;
    _showIndex = 0;
    _tarIndex = tarIndex;
    memset(_nodes, 0, sizeof(CCNode*)*3);
}

StartLayer::~StartLayer()
{
    for (int i = 0; i < 3; ++i) {
        if (_nodes[i]!=NULL) {
            _nodes[i]->release();
        }
    }
}

CCScene* StartLayer::scene(int tarIndex)
{
    CCScene* scene = CCScene::create();
    scene->setTag(SceneTypeStart);
    auto layer = StartLayer::create(tarIndex);
    
    scene->addChild(layer);
    
    return scene;
}

bool StartLayer::init()
{
    if ( !BaseLayer::init() )
        return false;
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    _userDefault = CCUserDefault::sharedUserDefault();
    
    CCSprite* back = CCSprite::create("back_start.png");
    back->setZOrder(-1);
    back->setPosition(ccp(s.width/2, s.height/2));
    addChild(back);
    
    _nodes[0] = addContentWithJsonFile("UI4Start.ExportJson");
    _nodes[0]->retain();
    
    _imageStart = static_cast<ImageView*>(getWidgetByName("Image_start"));
    _imageProgress = static_cast<ImageView*>(getWidgetByName("Image_progress"));
    _progressLoad = static_cast<LoadingBar*>(getWidgetByName("ProgressBar_load"));
    _btnStart = static_cast<Button*>(getWidgetByName("Button_start"));
    
    _btnStart->addTouchEventListener(this, toucheventselector(StartLayer::clickedStartEvent));
    _progressLoad->setPercent(0);
    
    loadResources();
    
    return true;
}

void StartLayer::onEnter()
{
    BaseLayer::onEnter();
    //界面回调
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StartLayer::clickedRegCallback), EVENT_REC_FROM_MSG_UILOGIC, NULL);
    //用户注册
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StartLayer::registerCallback), EVENT_REGMES_FROM_MSG, NULL);
    //创建角色
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StartLayer::createRoleCallback), CREATE_ROLE_EVENT_RET, NULL);
}

void StartLayer::loadResources()
{
    CCTextureCache * textureCache = CCTextureCache::sharedTextureCache();
    
    CCDictionary* dir = CCDictionary::createWithContentsOfFile("load.plist");
    _textures = (CCArray*)dir->objectForKey("texture");
    _textures->retain();
    for (int i = 0; i < _textures->count(); ++i)
    {
        string str = ((CCString*)_textures->objectAtIndex(i))->getCString();
        
        textureCache->addImageAsync(str.c_str(), this, callfuncO_selector(StartLayer::loadCallback));
    }
}

void StartLayer::clickedStartEvent(CCObject *sender, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        _btnStart->setTouchEnabled(false);
        
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        
        string account = _userDefault->getStringForKey(SAVE_H_ACCOUNT,"");
        string pwd = _userDefault->getStringForKey(SAVE_H_PASSWORD,"");
        if ( !account.empty() && !pwd.empty() )
        {
            login(account, pwd);
        }
        else
        {
            toggleLayer(1,true);
        }
    }
}

void StartLayer::login(string account,string pwd)
{
    LoginMeg tmpMeg;

    tmpMeg.name = account;
    tmpMeg.pwd = pwd;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_LOGIN2MEG, &tmpMeg);
}

void StartLayer::enabledStart()
{
    _btnStart->setTouchEnabled(true);
}

void StartLayer::receiveBaseMsg(CCObject* obj)
{
    Ret2UILoginMeg* data = (Ret2UILoginMeg*)obj;
    
    if (-1 == data->ret)
    {
        Alert::create(ALERTTEXT_CONNECTERROR,"提示",this,touchesureventselector(StartLayer::enabledStart))->show();
    }
    else if (0 == data->ret)
    {
        _btnStart->setTouchEnabled(true);
        ((LoginLayer*)_nodes[1])->showMsg("很抱歉，您输入的密码错误！");
    }
    else if( 1 == data->ret)//登陆成功
    {
        if (DataManager::sharedDataManager()->userid.empty())
        {   //未创建角色，显示创建角色框
            _roleDialog = CreateRoleDialog::create();
            _roleDialog->show();
        }
        else
        {   //登陆，跳转到游戏界面
            string account = _userDefault->getStringForKey(SAVE_H_ACCOUNT);
            string password = _userDefault->getStringForKey(SAVE_H_PASSWORD);
            
            UserAccountService userAccountService;
            userAccountService.addUserAccount(account.c_str(),password.c_str());
            
            SceneManager::shareSceneManager()->replaceScene(SceneTypeGame);
        }
    }
    else
    {
        _userDefault->setStringForKey(SAVE_H_ACCOUNT,"");
        _userDefault->setStringForKey(SAVE_H_PASSWORD,"");
    }
    _userDefault->flush();
}

/**
 * 
 * i:   -1上一个 0 当前 1下一个
 * ani: 启用动画
 *
 */
void StartLayer::toggleLayer(int i,bool ani)
{
  
    CCAssert(i >= -1 && i <= 1, "The showIndex must -1 <= i <= 1");
    
    int hideIndex = _showIndex;
    if (0 == i)
    {
        _showIndex = _tarIndex;
    }
    else
    {
        _showIndex += i;
    }
    
    _showLayer = _nodes[_showIndex];
    _hideLayer = _nodes[hideIndex];
    
    if (_showIndex == hideIndex)
    {
        return;
    }
    
    if (ani)
    {
        CCSize s = CCDirector::sharedDirector()->getWinSize();
        
        float tmpW = -1 == i ? s.width*-1 : s.width;
        CCMoveTo* move = CCMoveTo::create(.5, ccp(tmpW, 0));
        CCSequence* seq = CCSequence::create(move,CCCallFunc::create(this, callfunc_selector(StartLayer::showCallback)),NULL);
        
        _hideLayer->runAction(seq);
    }
    else
    {
        _hideLayer->removeFromParent();
        addChild(_showLayer);
    }
    
}

void StartLayer::showCallback()
{
    _hideLayer->removeFromParent();
    _showLayer->setPosition(ccp(0, 0));
    _showLayer->runAction(CCFadeIn::create(.5));
    addChild(_showLayer);
}

void StartLayer::back()
{
    toggleLayer(-1);
}

void StartLayer::loadCallback(CCObject* pSender)
{
    ++_loadSetup;
    
    int count = _textures->count();
    _progressLoad->setPercent(_loadSetup * 1.0 / count * 100);
    
    if (_loadSetup >= count)
    {
        for (int i = 0; i < count; ++i)
        {
            string str = ((CCString*)_textures->objectAtIndex(i))->getCString();
            str = str.replace(str.end() - 3, str.end(), "plist");
            
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(str.c_str());
        }
        _textures->release();
        
        _nodes[1] = LoginLayer::create();
        _nodes[1]->retain();
        
        _nodes[2] = RegisterLayer::create();
        _nodes[2]->retain();
        
        _imageProgress->setVisible(false);
        _imageStart->setVisible(true);
        
        toggleLayer(0,false);
    }
}

void StartLayer::registerCallback(CCObject* obj)
{
    Ret2UIRegMeg* meg = (Ret2UIRegMeg*)obj;
    
    if (1 == meg->ret)//注册成功，登陆
    {
        LoginMeg tmpMeg;
        tmpMeg.name = _userDefault->getStringForKey(SAVE_H_REGACCOUNT);
        tmpMeg.pwd = _userDefault->getStringForKey(SAVE_H_REGPASSWORD);
        _userDefault->setStringForKey(SAVE_H_REGACCOUNT,"");
        _userDefault->setStringForKey(SAVE_H_REGPASSWORD, "");
        
        _userDefault->setStringForKey(SAVE_H_ACCOUNT,tmpMeg.name);
        _userDefault->setStringForKey(SAVE_H_PASSWORD,tmpMeg.pwd);
        _userDefault->flush();
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_LOGIN2MEG, &tmpMeg);
    }
    else//注册失败，显示错误信息
    {
        string msg = meg->errmessage;
        if (msg.empty())
        {
            msg = "服务器正在维护！";
        }
        ((RegisterLayer*)_nodes[2])->setEnabledRegister(true);
        ((RegisterLayer*)_nodes[2])->showMsg(msg);
    }
}

void StartLayer::createRoleCallback(CCObject* obj)
{
    Ret2UIRegMeg* meg = (Ret2UIRegMeg*)obj;
    
    if (1 == meg->ret)//角色创建成功，再登陆一次
    {
        string account = _userDefault->getStringForKey(SAVE_H_ACCOUNT);
        string pwd = _userDefault->getStringForKey(SAVE_H_PASSWORD);
        
        login(account,pwd);
    }
    else//角色创建失败，显示错误信息
    {
        _roleDialog->showMsg(meg->errmessage);
    }
}

void StartLayer::clickedRegCallback()
{
    toggleLayer(1);
}