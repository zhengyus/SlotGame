#include "BaseLayer.h"


BaseLayer::BaseLayer(string notificationName ,bool loadingEnable, int loadTotal)
{
    _loading = NULL;
    _loadStep = 0;
    _loadTotal = loadTotal;
    _loadingEnable = loadingEnable;
    _notificationName = notificationName;
}

BaseLayer::~BaseLayer()
{
    
}

bool BaseLayer::init()
{
    if (!CCLayer::init())
        return false;
 
    _uilayer = UILayer::create();
    addChild(_uilayer);
    
    return true;
}

Widget* BaseLayer::addContentWithJsonFile(string fileName,bool visible,int zorder)
{
    Widget* widget = GUIReader::shareReader()->widgetFromJsonFile(fileName.c_str());
    widget->setZOrder(zorder);
    widget->setVisible(visible);
//    _widget->setAnchorPoint(ccp(0.5, 0.5));
    _uilayer->addWidget(widget);
    return widget;
}

void BaseLayer::onEnter()
{
    CCLayer::onEnter();
    
    //注册通知处理函数
    if (!_notificationName.empty())
    {
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaseLayer::receiveBaseMsg), _notificationName.c_str(), NULL);
    }
    
    showLoading();
}

void BaseLayer::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCLayer::onExit();
}

void BaseLayer::showLoading()
{
    //显示加载对话框
    if (_loadingEnable)
    {
        if (NULL == _loading)
        {
            _loading = LoadingDialog::create();
        }
        _loading->show();
    }
}

void BaseLayer::loadStepup()
{
    if (!_loadingEnable)
    {
        return;
    }
    
    ++_loadStep;
    if (_loadStep >= _loadTotal)
    {
        _loadingEnable = false;
        _loading->close();
    }
}

void BaseLayer::receiveBaseMsg(CCObject* obj)
{
    CCLog("base receive");
}

void BaseLayer::bindTouchEvent(std::string name,CCObject *target, cocos2d::ui::SEL_TouchEvent selector)
{
//    auto widget = UIHelper::seekWidgetByName(_widget, name.c_str());
    auto widget = _uilayer->getWidgetByName(name.c_str());
    widget->addTouchEventListener(target, selector);
}

Widget* BaseLayer::getWidgetByName(string name)
{
    return _uilayer->getWidgetByName(name.c_str());
}

void BaseLayer::addNodeTo(std::string name,CCNode* node)
{
    Widget* widget = _uilayer->getWidgetByName(name.c_str());
    widget->addNode(node);
}

void BaseLayer::addChildTo(std::string name,CCNode* node)
{
    Widget* widget = _uilayer->getWidgetByName(name.c_str());
    widget->addChild(node);
}