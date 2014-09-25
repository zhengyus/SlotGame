//
//  SceneManager.h
//  SLOT
//
//  Created by ZivHoo on 14/8/19.
//
//

#ifndef __SLOT_SCENEMANAGER_H__
#define __SLOT_SCENEMANAGER_H__

#include "cocos2d.h"

USING_NS_CC;

typedef  enum{
    SceneTypeStart,     //游戏开始
    SceneTypeGame,      //游戏大厅
} SceneType;

class SceneManager {
    
public:
    ~SceneManager();
    
public:
    static SceneManager* shareSceneManager();
    static SceneManager* getInstance();
    
public:
    CCScene* createScene(SceneType type);
    
    void startWithScene(SceneType type);
    void replaceScene(SceneType type);
    
private:
    SceneManager();
    
protected:
    CC_SYNTHESIZE_PASS_BY_REF(SceneType, _currentScene, CurrentScene);
    
private:
};

#endif
