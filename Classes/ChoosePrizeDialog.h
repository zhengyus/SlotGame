//
//  ChoosePrizeDialog.h
//  Slot
//
//  Created by huxf on 14-9-4.
//
//

#ifndef __Slot__ChoosePrizeDialog__
#define __Slot__ChoosePrizeDialog__

#include "ext/ui/Dialog.h"
#include "Hdefine.h"
#include "define.h"

class ChoosePrizeDialog : public Dialog {
    
    
public:
    
    ChoosePrizeDialog(vector<MyPetFAward>);
    ~ChoosePrizeDialog();
    
public:
    
    static ChoosePrizeDialog* createWithData(vector<MyPetFAward> data);
public:
    
    bool init();
    
public:
    void time();
    void chooseCard(ImageView*);//选择卡牌
    void setCardAndPrize(int,string&,string&);
    
    void onClickedConfirmEvent(CCObject*,TouchEventType);
    void onClickedCardEvent(CCObject*,TouchEventType);
    
private:
    vector<MyPetFAward>     _data;
    ImageView*              _imgs[3];
    
    Label*                  _time;
    int                     _times;
    
    Widget*              _imgTime;
    Widget*              _confirm;
};

#endif /* defined(__Slot__ChoosePrizeDialog__) */
