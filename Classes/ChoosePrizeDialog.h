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
    
    void selectCard(ImageView*);//选择卡牌
    void closeCallback();
    
    void onClickedConfirmEvent(CCObject*,TouchEventType);
    void onClickedCardEvent(CCObject*,TouchEventType);
    
protected:
    
    void setCardAndPrize(int,string&,string&);
    void setTexture(CCObject*);
    void setWord(ImageView* img,string prizeStr);
    
    void playSelectAnimation(vector<ImageView*> imgs);
    void playAnimation(CCObject*);
    void playOtherAnimation(vector<ImageView*> imgs);
    
private:
    vector<MyPetFAward>     _data;
    ImageView*              _imgs[3];
    
    Label*                  _time;
    int                     _times;
    
    Widget*              _imgTime;
    Widget*              _confirm;
};

#endif /* defined(__Slot__ChoosePrizeDialog__) */
