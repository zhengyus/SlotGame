//
//  LoginLayer.h
//  Slot
//
//  Created by huxf on 14-9-10.
//
//

#ifndef __Slot__LoginLayer__
#define __Slot__LoginLayer__

#include "BaseLayer.h"

class LoginLayer : public BaseLayer {
    
public:
    
    LoginLayer();
    ~LoginLayer();
    
public:
    
    bool init();
    
    CREATE_FUNC(LoginLayer);
    
public:
    
    void showMsg(string);
    
public:
    
    void onClickedLogin(CCObject *sender, TouchEventType event);
    void onClickedRegister(CCObject *sender, TouchEventType event);
    void onClickedUp(CCObject *sender, TouchEventType event);
    void onClickedDown(CCObject *sender, TouchEventType event);
    
    void onClickedDelete(CCObject *sender, TouchEventType event);
    void onListViewEvent(CCObject*,ListViewEventType);
    //
    
protected:
    void setAccountAndPwd();
    void toggleAccouts(bool);                   //显示隐藏历史账户
    void refreshUserAccounts();                 //刷新化历史账户
    void showUserAccounts();
    void initAccountItem(Widget*,UserAccount);  //初始化历史账户项

private:
    
    vector<UserAccount> _userAccounts;
    bool _isClickDelete;
    
private:
    
    Label*      _msg;
    CCEditBox*  _account;
    CCEditBox*  _password;
    Button*     _up;
    Button*     _down;
    ListView*   _list;
};

#endif /* defined(__Slot__LoginLayer__) */
