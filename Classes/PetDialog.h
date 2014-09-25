//
//  StarRankDialog.h
//  Slot
//
//  Created by ZivHoo on 14/8/28.
//
//

#ifndef __Slot__PetDialog__
#define __Slot__PetDialog__

#include "ext/ui/Dialog.h"
#include "HallDataManager.h"

/**
 * 排行榜
 */
class PetDialog :public Dialog{
    
public:
    PetDialog();
    ~ PetDialog();
    
public:
    
    bool init();
    
    void initUI();
    
    CREATE_FUNC(PetDialog);
    
public:
    
    void setPetProperty(int i);
    
    void setColor(Label*,float,float);
    
public:
    void clickedPetEvent(CCObject *pSender, TouchEventType type);
    void clickedBuyStoneEvent(CCObject *pSender, TouchEventType type);
    void clickedResetEvent(CCObject *pSender, TouchEventType type);
    void clickedHelpEvent(CCObject *pSender, TouchEventType type);
    
private:
    
    Label* _name;
    Label* _character;
    Label* _level;
    ImageView* _pet;
    LoadingBar* _expbar;
    
    Label* _strength;
    Label* _attack;
    Label* _prize;
    
    Label* _strengthBase;
    Label* _attackBase;
    Label* _prizeBase;
    
    Label* _strengthGrow;
    Label* _attackGrow;
    Label* _prizeGrow;
    
    Button* _stone;
    Button* _reset;
    Button* _help;
    
private:
    vector<MyPetList> _pets;
    
};

#endif /* defined(__Slot__StarRankDialog__) */
