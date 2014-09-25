//
//  OrderDialog.h
//  Slot
//
//  Created by ZivHoo on 14/8/26.
//
//

#ifndef __Slot__RankDialog__
#define __Slot__RankDialog__

#include "ext/ui/ListDialog.h"
#include "Hdefine.h"

/**
 * 排行榜
 */
class RankDialog : public ListDialog<MyRankList>
{
    
    typedef enum {
        RankType_Monday = 0,
        RankType_Thursday = 1,
        RankType_MondayHistory = 2,
        RankType_ThursdayHistory = 3
    } RankType;
    
public:
    RankDialog();
    ~ RankDialog();
    
public:
    
    bool initWithData(vector<MyRankList> data[4]);
    static RankDialog* create(vector<MyRankList> data[4]);
    
public:
    
    void clickedMondayEvent(CCObject, TouchEventType event);
    void clickedThursdayEvent(CCObject, TouchEventType event);
    void clickedMondayHistoryEvent(CCObject, TouchEventType event);
    void clickedThursdayHistoryEvent(CCObject, TouchEventType event);
    
    
private:
    vector<MyRankList> _data[4];
};

#endif /* defined(__Slot__OrderDialog__) */
