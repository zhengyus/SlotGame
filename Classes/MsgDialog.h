//
//  OrderDialog.h
//  Slot
//
//  Created by ZivHoo on 14/8/26.
//
//

#ifndef __Slot__MsgDialog__
#define __Slot__MsgDialog__

#include "ext/ui/ListDialog.h"
#include "Hdefine.h"

/**
 *
 */
class MsgDialog : public ListDialog<MyMailMeg>
{
    
public:
    
    MsgDialog();
    ~ MsgDialog();
    
public:
    
    bool initWithData(vector<MyMailMeg> &data);
    static MsgDialog* create(vector<MyMailMeg> &data);
    
public:
    
    void selectedItemEvent(CCObject*,ListViewEventType);
    
};

#endif /* defined(__Slot__OrderDialog__) */
