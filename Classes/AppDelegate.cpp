#include "AppDelegate.h"
#include "StartLayer.h"
#include "TestLayer.h"
#include "AlertText.h"
#include "ext/ui/Alert.h"


USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
   
}

bool AppDelegate::applicationDidFinishLaunching()
{
    
    
    m_conriller = new ControllerMeg();
    
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
    
    
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1136, 640, kResolutionNoBorder);
    
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    //
//    CCScene *pScene = TestLayer::scene();
    CCScene *pScene = StartLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
//    Alert::create(ALERTTEXT_UNCONNECT,"",this,touchesureventselector(AppDelegate::clickedSureCallback))->show();
    
    CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
    if (SceneTypeStart != scene->getTag())
    {
        SceneManager::getInstance()->replaceScene(SceneTypeStart);
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
