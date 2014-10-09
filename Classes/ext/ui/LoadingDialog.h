//
//  LoadingDialog.h
//  Slot
//
//  Created by huxf on 14-9-5.
//
//

#ifndef __Slot__LoadingDialog__
#define __Slot__LoadingDialog__

#include "ext/ui/Dialog.h"

class LoadingDialog :public Dialog{
    
    
public:
    
    LoadingDialog();
    ~LoadingDialog();
    
public:
    
    bool init();
    void show();
    void ccTouchEnded (CCTouch *pTouch, CCEvent *pEvent){}
    
    CREATE_FUNC(LoadingDialog);
    
    
};

#endif /* defined(__Slot__LoadingDialog__) */
