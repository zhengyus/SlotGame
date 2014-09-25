#ifndef __SLOT_TestLayer_H__
#define __SLOT_TestLayer_H__

#include "BaseLayer.h"

class TestLayer : public CCLayer
{
    
public:
    
    TestLayer();
    ~TestLayer();
    
public:
    
    CREATE_FUNC(TestLayer);
    
    static cocos2d::CCScene* scene();
    
public:
    
    bool init();
    
    void onEnter();
};

#endif
