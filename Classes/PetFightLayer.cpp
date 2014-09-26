#include "PetFightLayer.h"

//to do fix
#include "KCGameLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace ui;

UIImageView* imageView;
UIImageView* floorView;
UIImageView* floorView2;
UILayer *BgUIlayer;

UILoadingBar* rightBloodBar;
UILoadingBar* leftBloodBar;

UIImageView *bloodBg;

int petId;

string floorName;

int whatStateScene;
int floorY = -90;

int petX = -400;
int petY = -30;

int petFightX = -200;
int petFightY = -25;
int bossX = 200;
int bossY = -15;

int currentPetX =0;

int DoorPosX = 380;
int DoorPosY = -15;

int bloodPosX = 0;
int bloodPosY = -91;


PetFightLayer::PetFightLayer()
{
    armature1 = NULL;
    armature2 = NULL;
    
    imageView = NULL;
    floorView = NULL;
    floorView2 = NULL;
    BgUIlayer = NULL;
    rightBloodBar = NULL;
    leftBloodBar = NULL;
    bloodBg = NULL;
}

PetFightLayer::~PetFightLayer()
{
    
}

bool PetFightLayer::init()
{
    if (!CCLayer::init())
        return false;
    //boss 资源
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("BossIdle0.png", "BossIdle0.plist", "BossIdle.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("BossLighted0.png", "BossLighted0.plist", "BossLighted.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("BossLost0.png", "BossLost0.plist", "BossLost.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("BossOnAttacked0.png", "BossOnAttacked0.plist", "BossOnAttacked.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("BossAttack0.png", "BossAttack0.plist", "BossAttack.ExportJson");
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Door0.png", "Door0.plist", "Door.ExportJson");
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("lionAttack0.png", "LionAttack0.plist", "LionAttack.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("LionOnAttacked0.png", "LionOnAttacked0.plist", "LionOnAttacked.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("lionIdle0.png", "LionIdle0.plist", "LionIdle.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("lionWalk0.png", "LionWalk0.plist", "LionWalk.ExportJson");
    
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("PandaAttack0.png", "PandaAttack0.plist", "PandaAttack.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("PandaOnAttacked0.png", "PandaOnAttacked0.plist", "PandaOnAttacked.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("PandaIdle0.png", "PandaIdle0.plist", "PandaIdle.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("PandaWalk0.png", "PandaWalk0.plist", "PandaWalk.ExportJson");
    
    
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("DogAttack0.png", "DogAttack0.plist", "DogAttack.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("DogOnAttacked0.png", "DogOnAttacked0.plist", "DogOnAttacked.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("DogIdle0.png", "DogIdle0.plist", "DogIdle.ExportJson");
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("DogWalk0.png", "DogWalk0.plist", "DogWalk.ExportJson");
    
    
    //ui 容器
    BgUIlayer = UILayer::create();
    this->addChild(BgUIlayer);
    return true;
}


void PetFightLayer::setPetName(int petName)
{
    petId =petName;
    
    switch (petName)
    {
        case LION:
            animName = "Lion";
            break;
        case DOG:
            animName = "Dog";
            break;
        case PANDA:
            animName = "Panda";
            break;
        case BOSS:
            animName = "Boss";
            break;
    }

}

void PetFightLayer::boosPlay(int bossState)
{
    //animName
    std::string boosStateName;
    switch (bossState)
    {
        case BOSS_IDLE:
        {
             boosStateName = "BossIdle";
            
           
        }
            break;
        case BOSS_ONATTACKED:
        {
            boosStateName = "BossOnAttacked";
            
            KCGameLayer::playSound(BOSS_ON_ATTACKED);
        }
            break;
        case BOSS_LIGHTED:
        {
            boosStateName = "BossLighted";
            KCGameLayer::playSound(LION_BIG_ATTACK);
            KCGameLayer::playSound(BOSS_ON_ATTACKED);
        }
            break;
        case BOSS_LOST:
        {
            boosStateName = "BossLost";
            
            KCGameLayer::playSound(BOSS_OVER);
        }
            break;
        case BOSS_ATTACT:
        {
            boosStateName = "BossAttack";
            KCGameLayer::playSound(BOSS_LAUGH);
            KCGameLayer::playSound(BOSS_ATTACK);
        }
            break;
        default:
            break;
    }
    
    if (armature2) {
        armature2->removeAllChildren();
        removeChild(armature2);
        armature2 = NULL;
    }
    armature2 = CCArmature::create(boosStateName.c_str());
    armature2->getAnimation()->playWithIndex(0);
    armature2->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(PetFightLayer::onBossAnimationEvent));
    armature2->setPosition(ccp(bossX, bossY));
    addChild(armature2);

}

void PetFightLayer::petPlay(int petState)
{
    switch (petState)
    {
        case IDLE:
        {
            animType = animName+"Idle";
        }
            break;
        case WALK:
        {
            animType = animName+"Walk";
        }
            break;
        case ATTACK:
        {
            animType = animName+"Attack";
            
            
            switch (petId)
            {
                case LION:
                    KCGameLayer::playSound(LION_ATTACK);
                    break;
                case DOG:
                    KCGameLayer::playSound(DOG_ATTACK);
                    break;
                case PANDA:
                    KCGameLayer::playSound(PANDA_ATTACK);
                    break;
            }
        }
            break;
        case ONATTACKED:
        {
            animType = animName+"OnAttacked";
            
            switch (petId)
            {
                case LION:
                    KCGameLayer::playSound(LION_ON_ATTACK);
                    break;
                case DOG:
                    KCGameLayer::playSound(DOG_ON_ATTACK);
                    break;
                case PANDA:
                    KCGameLayer::playSound(PANDA_ON_ATTACK);
                    break;
            }
            
           
        }
            break;
        default:
            break;
    }
    
    
    
    
    
    if (whatStateScene ==0) {
        this->schedule(schedule_selector(PetFightLayer::moveFloor));
    }
    
    if (armature1) {
        armature1->removeAllChildren();
        removeChild(armature1);
        armature1 = NULL;
    }

    armature1 = CCArmature::create(animType.c_str());
    armature1->getAnimation()->playWithIndex(0);
    armature1->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(PetFightLayer::onPetAnimationEvent));
    armature1->setPosition(ccp(currentPetX, petY));
    addChild(armature1);
}


void PetFightLayer::setPetPos(int petPosX)
{
    currentPetX =petX+petPosX;
    armature1->setPosition(ccp(currentPetX, petY));
}

void PetFightLayer::movePet(int distance)
{
    currentPetX = petX+distance;
    CCLOG("currentPetX: %d",currentPetX);
    CCMoveTo *petMoveto = CCMoveTo::create(5, ccp(currentPetX, petY));
    armature1->runAction(petMoveto);
}

void PetFightLayer::stop()
{
    this->unschedule(schedule_selector(PetFightLayer::moveFloor));
    
    if (armature1) {
        armature1->removeAllChildren();
        removeChild(armature1);
        armature1 = NULL;
    }
    
    armature1 = CCArmature::create((animName + "Idle").c_str());
    armature1->getAnimation()->playWithIndex(0);
    //armature1->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(PetFightLayer::onPetAnimationEvent));
    armature1->setPosition(ccp(currentPetX, petY));
    
    CCLOG("stop current pet x is :%d",currentPetX);
    addChild(armature1);
    
    
}


void PetFightLayer::setBackground(int bg)
{
    if (!imageView) {
        imageView = UIImageView::create();
        BgUIlayer->addWidget(imageView);
    }
    switch (bg)
    {
        case EG:
        {
            imageView->loadTexture("e.png");
            floorName = "egFloor.png";
        }
            break;
            
        case CH:
        {
            imageView->loadTexture("c.png");
            floorName = "chFloor.png";
        }
            break;
            
        case US:
        {
            imageView->loadTexture("a.png");
            floorName = "usFloor.png";
            break;
        }
        case FT:
        {
            imageView->loadTexture("f.png");
            floorName = "ftFloor.png";
            break;
        }
            
        default:
            break;
            
    }
    PetFightLayer::createFloor();
    //srfloorView->loadTexture(floorName.c_str(), UI_TEX_TYPE_LOCAL);
}

void PetFightLayer::setGameScene(int gameScene)
{
    
    
    switch (gameScene)
    {
        case NOR:
        {
            
            if (armature1) {
                armature1->removeAllChildren();
                removeChild(armature1);
                armature1 = NULL;
            }
           
            petX = -400;
            currentPetX = -400;
            armature1 = CCArmature::create((animName + "Idle").c_str());
            armature1->getAnimation()->playWithIndex(0);
            armature1->setPosition(ccp(petX, petY));
            addChild(armature1);
            
            if (armature2) {
                armature2->removeAllChildren();
                removeChild(armature2);
                armature2 = NULL;
            }
            armature2 = CCArmature::create("Door");
            armature2->getAnimation()->playWithIndex(0);
            armature2->setPosition(ccp(DoorPosX, DoorPosY));
            addChild(armature2);
            
           
            
            whatStateScene = 0;
            
            if (bloodBg !=NULL) {
                bloodBg->setVisible(false);
                rightBloodBar->setVisible(false);
                leftBloodBar->setVisible(false);
            }
            
            
        }
            break;
            case FIGHT:
        {
            

            petX = -200;
            currentPetX = -200;
            armature1->setPosition(ccp(petFightX, petFightY));
            
            if (armature2) {
                armature2->removeAllChildren();
                removeChild(armature2);
                armature2 = NULL;
            }
            armature2 = CCArmature::create("BossIdle");
            armature2->getAnimation()->playWithIndex(0);
            armature2->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(PetFightLayer::onBossAnimationEvent));
            armature2->setPosition(ccp(bossX, bossY));
            addChild(armature2);
            
             whatStateScene = 1;
            if (bloodBg !=NULL) {
                bloodBg->setVisible(true);
                rightBloodBar->setVisible(true);
                leftBloodBar->setVisible(true);
                return;
            }
            bloodBg = UIImageView::create();
            bloodBg->loadTexture("bloodBg.png");
            bloodBg->setPosition(ccp(bloodPosX, bloodPosY));
            BgUIlayer->addWidget(bloodBg);
            
            //BgUIlayer->addWidget(<#cocos2d::ui::Widget *widget#>)
            // Create the loading bar
            rightBloodBar = UILoadingBar::create();
            rightBloodBar->setName("rightBloodBar");
            rightBloodBar->loadTexture("bloodBar.png");
            rightBloodBar->setDirection(LoadingBarTypeRight);
            rightBloodBar->setPercent(100);
            rightBloodBar->setPosition(ccp(bloodPosX-158,bloodPosY-4));
            BgUIlayer->addWidget(rightBloodBar);
            
            
            leftBloodBar = UILoadingBar::create();
            leftBloodBar->setName("leftBloodBar");
            leftBloodBar->loadTexture("bloodBar.png");
            leftBloodBar->setDirection(LoadingBarTypeLeft);
            leftBloodBar->setPercent(100);
            leftBloodBar->setPosition(ccp(bloodPosX+163,bloodPosY-4));
            BgUIlayer->addWidget(leftBloodBar);
            
            
           
        }
            break;
        default:
            break;
    }
    
    
}


void PetFightLayer::setPetBloodValue(int value)
{
    rightBloodBar->setPercent(value);
}

void PetFightLayer::setBossBloodValue(int value)
{
    leftBloodBar->setPercent(value);
}



void PetFightLayer::onPetAnimationEvent(cocos2d::extension::CCArmature *pArmature, cocos2d::extension::MovementEventType eventType, const char *animationID)
{
    //CCLog("int %d",LOOP_COMPLETE);
    //CCLog("int %d",eventType);
    //CCLog("int %s",animationID);
    //CCLOG("onPetAnimationEvent");
    
    if (eventType == COMPLETE)
    {
        if (whatStateScene == 1)
        {
            if (armature1) {
                armature1->removeAllChildren();
                removeChild(armature1);
                armature1 = NULL;
            }
            armature1 = CCArmature::create((animName + "Idle").c_str());
            armature1->getAnimation()->playWithIndex(0);
            //armature1->setAnchorPoint(ccp(0, 0));
            //armature1->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(PetFightLayer::onAnimationEvent));
            armature1->setPosition(ccp(petFightX, petFightY));
            addChild(armature1);
        }
    }
}


void PetFightLayer::onBossAnimationEvent(cocos2d::extension::CCArmature *pArmature, cocos2d::extension::MovementEventType eventType, const char *animationID)
{
    if (eventType == COMPLETE)
    {
        if (whatStateScene == 1)
        {
            if (armature2)
            {
                armature2->removeAllChildren();
                removeChild(armature2);
                armature2 = NULL;
            }
            armature2 = CCArmature::create("BossIdle");
            armature2->getAnimation()->playWithIndex(0);
            armature2->setPosition(ccp(bossX, bossY));
            addChild(armature2);
        }
    }
}

void PetFightLayer::createFloor()
{
    BgUIlayer->removeWidget(floorView);
    BgUIlayer->removeWidget(floorView2);
    
    float x,y;
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    x = size.width;
    y = size.height+origin.y;
   
    floorView = UIImageView::create();
    floorView2 = UIImageView::create();
    
    
    floorView->loadTexture(floorName.c_str());
    BgUIlayer->addWidget(floorView);
    floorView->setAnchorPoint(CCPoint(0,0.5));
    floorView->setPosition(CCPoint(-size.width/2,floorY));
    
    
    floorView2->loadTexture(floorName.c_str());
    BgUIlayer->addWidget(floorView2);
    floorView2->setAnchorPoint(CCPoint(0,0.5));
    floorView2->setPosition(CCPoint(size.width/2,floorY));
}

void PetFightLayer::moveFloor(float times)
{
    float x,y;
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    x = size.width;
    y = size.height+origin.y;
    
    floorView->setPosition(CCPoint(floorView->getPositionX()-10.5,floorY));
    floorView2->setPosition(CCPoint(floorView2->getPositionX()-10.5,floorY));
    
    if(floorView->getPositionX()<=-floorView->getContentSize().width)
    {
        floorView->setPosition(CCPoint(floorView2->getPositionX() + size.width/2,floorY));
    }
    
    if(floorView2->getPositionX()<=-floorView2->getContentSize().width)
    {
        floorView2->setPosition(CCPoint(floorView->getPositionX() + size.width/2,floorY));
    }
}



