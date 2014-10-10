//
//  SettingDialog.cpp
//  Slot
//
//  Created by huxf on 14-10-8.
//
//

#include "SettingDialog.h"
#include "StartLayer.h"

bool SettingDialog::init()
{
    if ( !Dialog::initContentWithJsonFile("UI4DialogSet.ExportJson") )
        return false;
    
    button_music = dynamic_cast<Button*>(getWidgetByName("Button_music"));
    button_effect = dynamic_cast<Button*>(getWidgetByName("Button_effect"));
    
    progress_music =  dynamic_cast<LoadingBar*>(getWidgetByName("ProgressBar_music"));
    progress_effect = dynamic_cast<LoadingBar*>(getWidgetByName("ProgressBar_effect"));
    
    bindTouchEvent("Button_switchAccount", this, toucheventselector(SettingDialog::clickedSwitchAccount));
    
    button_music->addTouchEventListener(this, toucheventselector(SettingDialog::regulateSound));
    return true;
}

void SettingDialog::regulateSound(CCObject* obj,TouchEventType type)
{
    static bool clicked = false;
    
    switch (type)
    {
        case TOUCH_EVENT_BEGAN:
            clicked = true;
            break;
        case TOUCH_EVENT_MOVED:
        {
            int i = progress_music->getPercent();
//            progress_music->setPercent(i+1);
        }
            break;
        default:
            clicked = false;
            break;
    }
}

void SettingDialog::clickedSwitchAccount(CCObject* obj,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        CCScene* scene = StartLayer::scene(1);
        CCDirector::sharedDirector()->replaceScene(scene);
    }
}

