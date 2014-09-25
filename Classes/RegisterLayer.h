//
//  RegisterLayer.h
//  Slot
//
//  Created by huxf on 14-9-10.
//
//

#ifndef __Slot__RegisterLayer__
#define __Slot__RegisterLayer__

#include "BaseLayer.h"

class RegisterLayer : public BaseLayer {
    
public:
    
    RegisterLayer();
    ~RegisterLayer();
    
public:
    
    bool init();
    
    CREATE_FUNC(RegisterLayer);
    
public:
    void showMsg(string);
    
    void onClickedBack(CCObject *sender, TouchEventType event);
    void onClickedRegister(CCObject *sender, TouchEventType event);
    
private:
    
    Label*      _msg;
    CCEditBox*  _account;
    CCEditBox*  _password;
    
};

#endif /* defined(__Slot__RegisterLayer__) */
