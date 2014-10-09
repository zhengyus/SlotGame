//
//  OrderDialog.h
//  Slot
//
//  Created by ZivHoo on 14/8/26.
//
//

#ifndef __Slot__MsgContentDialog__
#define __Slot__MsgContentDialog__

#include "ext/ui/Dialog.h"
#include "Hdefine.h"
#include "AlertText.h"

/**
 * 排行榜
 */
class MsgContentDialog : public Dialog{
    
    typedef enum
    {
        MailType_Read = 1,
        MailType_Receive = 3
    } MailType;
    
public:
    MsgContentDialog(MyMailMeg&);
    ~ MsgContentDialog();
    
public:
    
    bool init();
    void registerWithTouchDispatcher();
    
    static MsgContentDialog* create(MyMailMeg&);
    
public:
    
    void show();
    void updateMail(MailType);
    
    void setTitle(string title);
    void setContent(string content);
    void setPrize(int num);
    void setNumber();
    
public:
    
    void clickedReceive(CCObject*,TouchEventType);
    
private:
    
    Label*      _title;     //邮件标题
    Label*      _content;   //邮件内容
    Label*      _number;    //奖品数量
    Widget*     _wprize;    //奖品面板
    ImageView*  _prize;     //奖品图片
    Button*     _button;    //确定|领取
    
    MyMailMeg*   _data;
    
};

#endif /* defined(__Slot__OrderDialog__) */
