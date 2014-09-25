//
//  LoadingDialog.cpp
//  Slot
//
//  Created by huxf on 14-9-5.
//
//

#include "LoadingDialog.h"

LoadingDialog::LoadingDialog():Dialog("loading.png")
{
    
}

LoadingDialog::~LoadingDialog()
{
    
}

bool LoadingDialog::init()
{
    if (!Dialog::init()) {
        return  false;
    }
    
    _btnClose->setTouchEnabled(false);  //禁用关闭按钮
    _btnClose->setVisible(false);       //隐藏关闭按钮
    
    return true;
}

void LoadingDialog::show()
{
    Dialog::show();
    
    _background->setAnchorPoint(ccp(0.5, 0.5));
    CCRotateTo* rotate1 = CCRotateTo::create(.5, 180);
    CCRotateTo* rotate2 = CCRotateTo::create(.5, 360);
    
    CCRepeatForever* forever = CCRepeatForever::create(CCSequence::create(rotate1 , rotate2/*, rotate3 */, NULL));
    _background->runAction(forever);
}