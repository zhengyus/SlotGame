#ifndef __LINEHERO_PetFightLayer_H__
#define __LINEHERO_PetFightLayer_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                          使用例子
////
////                                  pv = new PetFightLayer();
////                                  pv->init();
////                                  pv->setPetName(0);
////                                  pv->setBackground(0);
////                                  pv->setGameScene(0);
////                                  pv->setPosition(ccp(480,320));
////                                  addChild(pv);
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum PetState
{
    IDLE = 0,      //默认
    WALK,          //行走
    ATTACK,         //射击
    ONATTACKED        //被攻击
};

enum BossState
{
    BOSS_IDLE = 0,      //默认
    BOSS_LIGHTED,          //被闪电
    BOSS_ONATTACKED,         //被攻击
    BOSS_ATTACT,        //攻击
    BOSS_LOST            //失败
};

enum PetType
{
    LION = 0,      // 狮子
    DOG,          //神狗
    PANDA,         //熊猫
    BOSS        //boss
};

enum BgType
{
    EG = 0,      // 埃及
    CH,          //中国
    US,          //美国
    FT//战斗
};

enum FightSceneType
{
    NOR = 0,      // 正常
    FIGHT          // 对战
};

class PetFightLayer : public cocos2d::CCLayer
{
    
public:
    
    PetFightLayer();
    virtual ~PetFightLayer();
    
public:
    
    virtual bool init();
    
    void setPetName(int petName); //设置宠物名称
    void setGameScene(int gameScenetype); //设置游戏场景 0 普通 1 战斗
    void setBackground(int bg); // 设置背景
    void petPlay(int petState);
    void boosPlay(int bossState);
    void stop();  //宠物停止跑动 地面停止运动
    void setPetBloodValue(int value); //设置宠物血量 100%
    void setBossBloodValue(int value);//设置Boss血量 100%
    void movePet(int distance);//移动宠物
    
protected:
    std::string animType;
    std::string animName;
    CCArmature *armature1;
    CCArmature *armature2;
    void onPetAnimationEvent(CCArmature *pArmature, MovementEventType eventType, const char *animationID);
    void onBossAnimationEvent(CCArmature *pArmature, MovementEventType eventType, const char *animationID);
    void moveFloor(float times);
    void createFloor();
    
};

#endif
