//
//  SettingDialog.h
//  Slot
//
//  Created by huxf on 14-10-8.
//
//

#ifndef __Slot__SettingDialog__
#define __Slot__SettingDialog__

#include "ext/ui/Dialog.h"

class SettingDialog : public Dialog {
    
    
public:
    
    CREATE_FUNC(SettingDialog);
    
    bool init()override;

    virtual bool ccTouchBegan (CCTouch *pTouch, CCEvent *pEvent)override;
    virtual void ccTouchMoved (CCTouch *pTouch, CCEvent *pEvent)override;
    virtual void ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent)override;
    
    void regulateSound(CCObject* obj,TouchEventType type);
    void clickedSwitchAccount(CCObject* obj,TouchEventType type);
private:
    Button* button_music;
    Button* button_effect;
    
    LoadingBar* progress_music;
    LoadingBar* progress_effect;
    
    Button* button_select;
    
};

#endif /* defined(__Slot__SettingDialog__) */
