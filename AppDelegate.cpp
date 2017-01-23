#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
#include "GameShooterMode.h"
#include "MainMenu.h"
#include "SoundMgr.h"
#include "GameConfig.h"
#include "MyPurchase.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

	pDirector->setOpenGLView(pEGLView);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	pEGLView->setFrameSize(480, 864);
#endif
	CCSize designSize = CCSizeMake(480, 864);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	pEGLView->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedWidth);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
	pEGLView->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedWidth);
#else
	pEGLView->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedHeight);
#endif

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	GameConfig::getInstance()->preloadData();
	SoundMgr::theMgr()->init();
	MyPurchase::sharedPurchase()->loadIAPPlugin();

    // create a scene. it's an autorelease object
	CCScene *pScene = MainMenu::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
