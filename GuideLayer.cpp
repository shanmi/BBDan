#include "GuideLayer.h"
#include "CommonMacros.h"

USING_NS_CC;

void GuideLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Guide, false);
}

void GuideLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool GuideLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

CCScene* GuideLayer::scene()
{
	CCScene *scene = CCScene::create();
	GuideLayer *layer = GuideLayer::create();
	scene->addChild(layer);
	return scene;
}

bool GuideLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto finger = CCSprite::create("guide/finger.png");
	finger->setPosition(ccpMult(winSize, 0.6f));
	addChild(finger);

	auto fadeIn = CCFadeIn::create(0.5f);
	auto moveBy = CCMoveBy::create(1.2f, ccp(100, -200));
	auto fadeOut = CCFadeOut::create(0.1f);
	auto moveBack = CCMoveBy::create(0.1f, ccp(-100, 200));
	auto sequence = CCSequence::create(fadeIn, moveBy, fadeOut, moveBack, NULL);
	auto actions = CCRepeatForever::create(sequence);
	finger->runAction(actions);

	return true;
}
