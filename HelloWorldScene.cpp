#include "HelloWorldScene.h"
#include "SquareNode.h"
#include "SquareModel.h"
#include "Config.h"
#include "GameUtil.h"
#include "Box2dFactory.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	Box2dFactory::getInstance()->initPhysics(false);

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
	for (int n = 0; n < 1; n++)
	{
		auto squares = SquareModel::theModel()->createSquareList();
		for (int i = 0; i < squares.size(); i++)
		{
			auto node = squares[i];
			node->setPosition(ccp(node->getContentSize().width / 2 + 4 + node->getIndex().x * (node->getContentSize().width + 4), node->getContentSize().height / 2 + n*(node->getContentSize().height+4)));
			//addChild(node);
		}
	}

	/*auto m_emitter = CCParticleGalaxy::create();
	addChild(m_emitter, 10);
	m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/explode.png"));
	m_emitter->setScale(0.3f);
	auto moveTo = CCMoveBy::create(2.0f, ccp(0,500));
	auto moveBack = moveTo->reverse();
	auto sequence = CCSequence::create(moveTo, moveBack, NULL);
	auto actions = CCRepeatForever::create(sequence);
	runAction(actions);*/

	CCParticleSun* par = CCParticleSun::create();
	par->setStartSize(60);
	par->setEmissionRate(100);
	par->setAnchorPoint(ccp(0.5f, 0.5f));
	par->setPosition(0, 0);
	par->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/fire.png"));
	addChild(par, 10);

	//auto fire = CCParticleMeteor::create();
	//par->setStartSize(60);
	//fire->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	////fire->setRotation(30);
	//fire->setPosition(230, 600);
	//addChild(fire);

	auto delay = CCDelayTime::create(1.0f);
	auto moveTo = CCMoveBy::create(1.0f, ccp(0, -500));
	auto moveBack = moveTo->reverse();
	auto sequence = CCSequence::create(delay, moveTo, moveBack, NULL);
	auto actions = CCRepeatForever::create(sequence);
	//par->runAction(actions);

    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	/*CCPoint position = pTouch->getLocation();
	auto pEmitter = GameUtil::getBombEffect2();
	pEmitter->setPosition(position);
	addChild(pEmitter);*/

	/*CCPoint position = pTouch->getLocation();
	auto m_emitter = new CCParticleSystemQuad();
	std::string filename = "particle/explore.plist";
	m_emitter->initWithFile(filename.c_str());
	m_emitter->setPosition(position);
	addChild(m_emitter, 10);*/

	CCPoint position = pTouch->getLocation();
	auto effect = GameUtil::getAchievementEffect(1);
	effect->setPositionY(position.y);
	addChild(effect);

	return true;
}

void HelloWorld::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}

void HelloWorld::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}