//
//  DialogBuyStone.h
//  Slot
//
//  Created by huxf on 14-9-18.
//
//

#ifndef __Slot__DialogBuyStone__
#define __Slot__DialogBuyStone__

#include "ext/ui/Dialog.h"

class BuyStoneDialog :public Dialog{
   
public:
    
    BuyStoneDialog();
    CREATE_FUNC(BuyStoneDialog);
    
public:
    
    bool init();
    
    void number(int);
    
public:
    
    void textFieldListener(CCObject*, TextFiledEventType type);
    void clickedReduceEvent(CCObject* pSender,TouchEventType type);
    void clickedAddEvent(CCObject* pSender,TouchEventType type);
    void clickedConfirmEvent(CCObject* pSender,TouchEventType type);
    
private:
    
    TextField* _txtNum;
    Label* _lblBean;
    
    int _num;
    
};

#endif /* defined(__Slot__DialogBuyStone__) */
