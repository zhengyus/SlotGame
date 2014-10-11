//
//  CreateRoleDialog.h
//  Slot
//
//  Created by huxf on 14-9-16.
//
//

#ifndef __Slot__CreateRoleDialog__
#define __Slot__CreateRoleDialog__

#include "ext/ui/Dialog.h"

class CreateRoleDialog : public Dialog
{

public:
    
    CREATE_FUNC(CreateRoleDialog);
    
public:
    
    CreateRoleDialog();
    bool init();
    
public:
    
    void onSelectedSexEvent(CCObject* pSender, TouchEventType type);
    void onCreateRoleEvent(CCObject* pSender, TouchEventType type);
    void ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent){}
    
public:
    
    void showMsg(string msg);
    
protected:
    void selectSex();
    
private:
    
    Label*  _msg;
    
    ImageView*  _imgHead[2];
    Button*     _btnHead[2];
    ImageView*  _radio[2];
    
    CCEditBox*  _roleName;  //角色名称
    int         _sex;       //角色性别
};

#endif /* defined(__Slot__CreateRoleDialog__) */
