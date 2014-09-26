//
//  LoginLayer.cpp
//  Slot
//
//  Created by huxf on 14-9-10.
//
//

#include "LoginLayer.h"
#include "StartLayer.h"
#include "SqliteUtil.h"
#include "UserAccountService.h"


LoginLayer::LoginLayer()
{
    _list = NULL;
    _isClickDelete = false;
}

LoginLayer::~LoginLayer()
{
    
}

bool LoginLayer::init()
{
    if ( !BaseLayer::init() )
        return false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getWinSize();
    
    addContentWithJsonFile("UI4Login.ExportJson");
    
    _msg = static_cast<Label*>(getWidgetByName("Label_msg"));
    _list = static_cast<ListView*>(getWidgetByName("ListView_list"));
    
    CCSize size = CCSize(450, 85);
    
    _account = CCEditBox::create(size,CCScale9Sprite::createWithSpriteFrameName("input_login.png"));
    _password = CCEditBox::create(size,CCScale9Sprite::createWithSpriteFrameName("input_login.png"));
    
    _account->setPosition(ccp(visibleSize.width/2,visibleSize.height/2 + 68));
    _password->setPosition(ccp(_account->getPositionX(),_account->getPositionY()-105));
    
    ccColor3B color = {147,140,115};
    _account->setFont("TrebuchetMS-Bold",30);
    _account->setFontColor(color);
    _account->setPlaceholderFontColor(color);
    _account->setPlaceHolder("联众账号");
    _account->setMaxLength(16);
    _account->setSelected(true);
    _account->setReturnType(kKeyboardReturnTypeDone);
    
    _password->setFont("TrebuchetMS-Bold",30);
    _password->setFontColor(color);
    _password->setPlaceholderFontColor(color);
    _password->setPlaceHolder("密码");
    _password->setMaxLength(16);
    _password->setInputFlag(kEditBoxInputFlagPassword);
    _password->setInputMode(kEditBoxInputModeSingleLine);
    _password->setZOrder(-1);
    
    _uilayer->addChild(_account);
    _uilayer->addChild(_password);
    
    _up = Button::create();
    _down = Button::create();
    
    _up->loadTextures("button_up.png", "", "",UI_TEX_TYPE_PLIST);
    _down->loadTextures("button_down.png", "", "",UI_TEX_TYPE_PLIST);
    
    CCPoint point = ccp(_account->getContentSize().width - 40, _account->getContentSize().height/2);
    _up->setPosition(point);
    _down->setPosition(point);
    
    UILayer* layer = UILayer::create();
    layer->addWidget(_down);
    layer->addWidget(_up);
    _account->addChild(layer);
    
    _list->setAnchorPoint(ccp(0, 1));
    _list->setItemModel(GUIReader::shareReader()->widgetFromJsonFile("UI4ItemAccount.ExportJson"));
    _list->addEventListenerListView(this, listvieweventselector(LoginLayer::onListViewEvent));
    
    bindTouchEvent("btn_login", this, toucheventselector(LoginLayer::onClickedLogin));
    bindTouchEvent("btn_register", this, toucheventselector(LoginLayer::onClickedRegister));
    
    _up->addTouchEventListener(this, toucheventselector(LoginLayer::onClickedUp));
    _down->addTouchEventListener(this, toucheventselector(LoginLayer::onClickedDown));
    
//    setAccountAndPwd();
    
    toggleAccouts(false);
    return true;
}

void LoginLayer::setAccountAndPwd()
{
    UserAccountService userAccountService;
    userAccountService.createTable();
    
    for (int i = 1; i < 13; ++i)
    {
        const char* str = CCString::createWithFormat("fishtest%03d",i)->getCString();
        userAccountService.addUserAccount(str,str);
    }
    
    string account = CCUserDefault::sharedUserDefault()->getStringForKey(SAVE_H_ACCOUNT);
    string password = CCUserDefault::sharedUserDefault()->getStringForKey(SAVE_H_PASSWORD);
    
    if (!account.empty())
    {
        _account->setText(account.c_str());
        _password->setText(password.c_str());
    }
}

void LoginLayer::showMsg(string msg)
{
    _msg->setText(msg.c_str());
}

void LoginLayer::onClickedLogin(CCObject *sender, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        
        _msg->setText("");
        LoginMeg tmpMeg;
        tmpMeg.name = _account->getText();
        tmpMeg.pwd = _password->getText();
        
        if (tmpMeg.name.empty())
        {
            _msg->setText("请输入联众账号！");
            return;
        }
        
        if (tmpMeg.pwd.empty())
        {
            _msg->setText("请输入账号密码！");
            return;
        }
        
        CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
        userDefault->setStringForKey(SAVE_H_ACCOUNT, tmpMeg.name);
        userDefault->setStringForKey(SAVE_H_PASSWORD, tmpMeg.pwd);
        userDefault->flush();
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_LOGIN2MEG, &tmpMeg);
    }
}

void LoginLayer::onClickedRegister(CCObject *sender, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REC_FROM_MSG_UILOGIC);
    }
}

void LoginLayer::onClickedUp(CCObject *sender, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        toggleAccouts(false);
    }
}

void LoginLayer::onClickedDown(CCObject *sender, TouchEventType event)
{
    if (event == TOUCH_EVENT_ENDED)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
        refreshUserAccounts();
        showUserAccounts();
        toggleAccouts(true);
    }
}

void LoginLayer::onClickedDelete(CCObject *sender, TouchEventType event)
{
    switch (event) {
        case TOUCH_EVENT_BEGAN:
        {
            _isClickDelete = true;
            break;
        }
        case TOUCH_EVENT_ENDED:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLICK);
            Widget* widget = dynamic_cast<Widget*>(sender);
            UserAccountService userAccountService;
            userAccountService.deleteUserAccount(widget->getTag());
            _isClickDelete = false;
            
            refreshUserAccounts();
            showUserAccounts();
            
            if (_userAccounts.size() <= 0)
            {
                toggleAccouts(false);
            }
            
            break;
        }
        default:
            break;
    }
}

void LoginLayer::toggleAccouts(bool visible)
{
    _up->setVisible(visible);
    _up->setTouchEnabled(visible);
    
    _list->setVisible(visible);
    _list->setEnabled(visible);
    
    _down->setVisible(!visible);
    _down->setTouchEnabled(!visible);
}

void LoginLayer::refreshUserAccounts()
{
    UserAccountService userAccountService;
    _userAccounts = userAccountService.queryAllUserAccount();
}

void LoginLayer::showUserAccounts()
{
    _list->removeAllItems();
    int length = _userAccounts.size();
    for (int i = 0; i < length; ++i)
    {
        _list->pushBackDefaultItem();
        initAccountItem(_list->getItem(i),_userAccounts[i]);
    }
    
    CCSize size = CCSizeMake(_list->getSize().width,170);
    if (0 == length)
    {
        size.height = 85;
    }
    else if (length < 3)
    {
        size.height = 85 * length;
    }
    _list->setSize(size);
}

void LoginLayer::initAccountItem(Widget* widget,UserAccount user)
{
    Label* lblAccount = static_cast<Label*>(UIHelper::seekWidgetByName(widget, "Label_account"));
    Button* btnDelete = static_cast<Button*>(UIHelper::seekWidgetByName(widget, "Button_delete"));
    
    lblAccount->setText(user.account);

    btnDelete->setTag(atoi(user.id.c_str()));
    btnDelete->addTouchEventListener(this, toucheventselector(LoginLayer::onClickedDelete));
}

void LoginLayer::onListViewEvent(CCObject* sender,ListViewEventType event)
{
    static long start;
    struct timeval tv;
    
    if (_list->getItems()->count()<=0)
    {
        return;
    }
    
    switch (event) {
        case LISTVIEW_ONSELECTEDITEM_START:
        {
            gettimeofday(&tv,NULL);
            start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
            break;
        }
        case LISTVIEW_ONSELECTEDITEM_END:
        {
            gettimeofday(&tv , NULL);
            
            if (tv.tv_sec * 1000 + tv.tv_usec / 1000 - start < 200 && !_isClickDelete)
            {
                int i = _list->getCurSelectedIndex();
                
                _account->setText(_userAccounts[i].account.c_str());
                _password->setText(_userAccounts[i].password.c_str());
                
                toggleAccouts(false);
            }
            break;
        }
        default:
            break;
    }
}