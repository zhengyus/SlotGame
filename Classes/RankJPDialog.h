//
//  StarRankDialog.h
//  Slot
//
//  Created by ZivHoo on 14/8/28.
//
//

#ifndef __Slot__StarRankDialog__
#define __Slot__StarRankDialog__

#include "ext/ui/ListDialog.h"
#include "NoticeView.h"
#include "Hdefine.h"

/**
 * 排行榜
 */
class RankJPDialog : public ListDialog<MyJPRankList>
{
    
    typedef enum
    {
        JPRankType_Star = 0,
        JPRankType_Big = 1
    } JPRankType;
    
public:
    
    RankJPDialog();
    ~RankJPDialog();
    
public:
    
    bool initWithData(vector<MyJPRankList>[2]);
    static RankJPDialog* create(vector<MyJPRankList> data[2]);
    
public:
    void initNoticeView();
    
public:
    
    void clickedStarEvent(CCObject, TouchEventType event);
    void clickedBigEvent(CCObject, TouchEventType event);
    
private:

    NoticeView* _noticeView;
    vector<MyJPRankList> _data[2];
    
};

#endif /* defined(__Slot__StarRankDialog__) */
