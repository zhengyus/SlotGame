//
//  StarRankDialog.cpp
//  Slot
//
//  Created by ZivHoo on 14/8/28.
//
//

#include "PetDialog.h"
#include "BuyStoneDialog.h"

PetDialog::PetDialog():
Dialog("dialog_pet.png")
{
}

PetDialog::~ PetDialog()
{
}

bool PetDialog::init()
{
    if (! Dialog::init())
    {
        return false;
    }
    
    initContentWithJsonFile("UI4DialogPet.ExportJson");
    
    _name = static_cast<Label*>(getWidgetByName("Label_name"));
    _character = static_cast<Label*>(getWidgetByName("Label_character"));
    _level = static_cast<Label*>(getWidgetByName("Label_level"));
    _pet = static_cast<ImageView*>(getWidgetByName("Image_pet"));
    _expbar = static_cast<LoadingBar*>(getWidgetByName("ProgressBar_exp"));
    
    _strength = static_cast<Label*>(getWidgetByName("Label_strength"));
    _attack = static_cast<Label*>(getWidgetByName("Label_attack"));
    _prize = static_cast<Label*>(getWidgetByName("Label_prize"));
    
    _strengthBase = static_cast<Label*>(getWidgetByName("Label_strength_base"));
    _attackBase = static_cast<Label*>(getWidgetByName("Label_attack_base"));
    _prizeBase = static_cast<Label*>(getWidgetByName("Label_prize_base"));
    
    _strengthGrow = static_cast<Label*>(getWidgetByName("Label_strength_grow"));
    _attackGrow = static_cast<Label*>(getWidgetByName("Label_attack_grow"));
    _prizeGrow = static_cast<Label*>(getWidgetByName("Label_prize_grow"));
    
    _stone = static_cast<Button*>(getWidgetByName("Button_stone"));
    _reset = static_cast<Button*>(getWidgetByName("Button_reset"));
    _help = static_cast<Button*>(getWidgetByName("Button_help"));
    
    bindTouchEvent("Button_stone", this, toucheventselector(PetDialog::clickedBuyStoneEvent));
    bindTouchEvent("Button_reset", this, toucheventselector(PetDialog::clickedResetEvent));
    bindTouchEvent("Button_help", this, toucheventselector(PetDialog::clickedHelpEvent));
    
    _stone->setVisible(false);
    _stone->setTouchEnabled(false);
    _reset->setVisible(false);
    _reset->setTouchEnabled(false);
    _help->setVisible(false);
    _help->setTouchEnabled(false);
    
    _pets = HallDataManager::getInstance()->_pets;
    
    initUI();
    
    return true;
}

void PetDialog::initUI()
{
    if (_pets.size() <= 0)
    {
        return;
    }
    
    for (int i = 0; i < 3 && i < _pets.size(); ++i)//初始化宠物列表
    {
        MyPetList pet = _pets[i];
        Button* button = static_cast<Button*>(getWidgetByName(CCString::createWithFormat("Button_head%d",i)->getCString()));
        button->setTag(i);
        
        button->loadTextureNormal(CCString::createWithFormat("pethead%d.png",pet.csvid-1)->getCString(), UI_TEX_TYPE_PLIST);
        button->addTouchEventListener(this, toucheventselector(PetDialog::clickedPetEvent));
    }
    
    setPetProperty(0);//设置宠物属性
}

void PetDialog::setPetProperty(int i)
{
    MyPetList pet = _pets[i];
    
    string quality;
    if (1 == pet.petpinzhi)
        quality = "精英";
    else if( 2 == pet.petpinzhi)
        quality = "稀有";
    else
        quality = "传说";
    
    
    _name->setText(pet.petName.c_str());
    _character->setText(CCString::createWithFormat("%s",quality.c_str())->getCString());
    _level->setText(CCString::createWithFormat("Lv%d",pet.petlevel)->getCString());
    _pet->loadTexture(CCString::createWithFormat("pet%d.png",pet.csvid - 1)->getCString(),UI_TEX_TYPE_PLIST);
    _expbar->setPercent(pet.petexp * 1.0 / pet.petreqexp * 100);
    
    _strengthGrow->setText(CCString::createWithFormat("%d",pet.petbloodPlus)->getCString());
    _attackGrow->setText(CCString::createWithFormat("%d",pet.petattackPlus)->getCString());
    _prizeGrow->setText(CCString::createWithFormat("%.2f",pet.petawardPlus*0.01)->getCString());
    
    _strength->setText(CCString::createWithFormat("%d",pet.petblood + pet.petbloodPlus)->getCString());
    _attack->setText(CCString::createWithFormat("%d",pet.petattack + pet.petattackPlus)->getCString());
    _prize->setText(CCString::createWithFormat("%.2f%%",pet.petaward * 0.01 + pet.petawardPlus * 0.01)->getCString());
    
    _strengthBase->setText(CCString::createWithFormat("%d",pet.petblood)->getCString());
    _attackBase->setText(CCString::createWithFormat("%d",pet.petattack)->getCString());
    _prizeBase->setText(CCString::createWithFormat("%.2f",pet.petaward * 0.01)->getCString());
    
    setColor(_strengthBase, pet.petblood, pet.petbloodMax);
    setColor(_attackBase, pet.petattack, pet.petattackMax);
    setColor(_prizeBase, pet.petaward, pet.petawardMax);
}


void PetDialog::setColor(Label* lbl,float value,float max)
{
    lbl->setColor(value >= max*0.94?ccc3(255, 0, 0):ccc3(79, 173, 44));
}

void PetDialog::clickedPetEvent(CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        Button* button = static_cast<Button*>(pSender);
        setPetProperty(button->getTag());
    }
}

void PetDialog::clickedBuyStoneEvent(CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        BuyStoneDialog::create()->show();
    }
}

void PetDialog::clickedResetEvent(CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        
    }
}

void PetDialog::clickedHelpEvent(CCObject *pSender, TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        
    }
}

