//
//  NoticeView.h
//  Slot
//
//  Created by ZivHoo on 14/8/26.
//
//

#ifndef __Slot__NoticeView__
#define __Slot__NoticeView__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;


class NoticeView :public CCLayer {
    
    
public:
    NoticeView();
    ~NoticeView();
    
    
public:
    
    CREATE_FUNC(NoticeView);
    
    bool init();
    
public:
    
    void setIcon(string icon);
    void setTitle(string title);
    void setName(string name);
    void setRank(string rank);
    
    void setNotice(string title,string name,string rank);
    void setNoticeWithAnimation(string title,string name,string rank);
    
    void resetNotice();
    
private:
    
    Widget*     _contents[2];
    int         _showIndex;
    
};

#endif /* defined(__Slot__NoticeView__) */
