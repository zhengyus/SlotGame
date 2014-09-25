//
//  SceneManager.cpp
//  LineHero
//
//  Created by ZivHoo on 14/8/19.
//
//

#include "SceneManager.h"
#include "StartLayer.h"
#include "GameLayer.h"

static SceneManager* s_sceneManager = NULL;

SceneManager::SceneManager()
{
    
}

SceneManager::~SceneManager()
{
    
}

SceneManager* SceneManager::shareSceneManager()
{
    return getInstance();
}

SceneManager* SceneManager::getInstance()
{
    if (NULL == s_sceneManager) {
        s_sceneManager = new SceneManager();
    }
    return s_sceneManager;
}

CCScene* SceneManager::createScene(SceneType type)
{
    CCScene* scene = NULL;
    switch (type)
    {
        case SceneTypeStart:
            scene = StartLayer::scene();
            break;
            
        case SceneTypeGame:
            scene = GameLayer::scene();
            break;
            
        default:
            break;
    }
    return scene;
}

void SceneManager::startWithScene(SceneType type)
{
    _currentScene = type;
    CCDirector::sharedDirector()->runWithScene(createScene(type));
}

void SceneManager::replaceScene(SceneType type)
{
    _currentScene = type;
    CCDirector::sharedDirector()->replaceScene(createScene(type));
}