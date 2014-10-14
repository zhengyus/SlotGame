#include "TestLayer.h"
#include "RoomView.h"
#include "CreateRoleDialog.h"
#include "ChoosePrizeDialog.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

TestLayer::TestLayer()
{

}

TestLayer::~TestLayer()
{
}

CCScene* TestLayer::scene()
{
    CCScene* scene = CCScene::create();
    auto layer = TestLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

static void onSelectedRoomEvent(MyRoomList room)
{
    
}

static void initRoomItem(Widget* widget,MyRoomList room)
{
    ImageView* img = static_cast<ImageView*>(widget->getChildByName("Image_room"));
    img->loadTexture(CCString::createWithFormat("room%d.png",room.roomid)->getCString(),UI_TEX_TYPE_PLIST);
}

bool TestLayer::init()
{
    if ( !CCLayer::init() )
        return false;
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dialog.plist", "dialog.png");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("card.plist", "card.png");
    
//
//    RoomView<MyRoomList>* view = RoomView<MyRoomList>::createWithItemJson("UI4ItemRoom.ExportJson");
//    view->setInitItemCallback(initRoomItem);
//    view->setSelectedItemCallback(onSelectedRoomEvent);
//    
//    MyRoomList room;
//    for (int i = 1; i <= 3; i++)
//    {
//        room.roomid = i;
//        view->AddData(room);
//    }
//    addChild(view);
//    view->display();
    
//    CCDirector::sharedDirector()
    
    CCControlButton* btn = CCControlButton::create("test", "Arial", 30);
    btn->setPosition(1136/2, 640/2);
    btn->addTargetWithActionForControlEvents(this, cccontrol_selector(TestLayer::click), CCControlEventTouchDown);
    addChild(btn);


    
    return true;
}

void TestLayer::click(CCObject*,CCControlEvent eventType)
{
    
        ChoosePrizeDialog::createWithData(_data)->show();
}

void TestLayer::onEnter()
{
    CCLayer::onEnter();
    
    MyPetFAward d1;
    d1.Atype = 1;
    d1.Anum = 1000;
    d1.Aok = 0;
    
    MyPetFAward d2;
    d2.Atype = 3;
    d2.Anum = 2000;
    d2.Aok = 0;
    
    MyPetFAward d3;
    d3.Atype = 2;
    d3.Anum = 3000;
    d3.Aok = 1;
    
    _data.push_back(d1);
    _data.push_back(d2);
    _data.push_back(d3);
    
    ChoosePrizeDialog::createWithData(_data)->show();
}