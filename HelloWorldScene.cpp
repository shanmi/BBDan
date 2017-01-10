#include "HelloWorldScene.h"
#include "SquareNode.h"
#include "SquareModel.h"
#include "Config.h"
#include "GameUtil.h"

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
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("12345", "fonts/SF Square Root.ttf", 34);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

	for (int n = 0; n < 1; n++)
	{
		auto squares = SquareModel::theModel()->createSquareList();
		for (int i = 0; i < squares.size(); i++)
		{
			auto node = squares[i];
			node->setPosition(ccp(node->getContentSize().width / 2 + 4 + node->getIndex() * (node->getContentSize().width + 4), node->getContentSize().height / 2 + n*(node->getContentSize().height+4)));
			addChild(node);
		}
	}
	/*auto m_emitter = new CCParticleSystemQuad();
	std::string filename = "SpookyPeas.plist";
	m_emitter->initWithFile(filename.c_str());
	addChild(m_emitter, 10);*/


	/*auto m_emitter = CCParticleGalaxy::create();
	addChild(m_emitter, 10);
	m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle.png"));
	m_emitter->setScale(0.3f);
	auto moveTo = CCMoveBy::create(2.0f, ccp(0,500));
	auto moveBack = moveTo->reverse();
	auto sequence = CCSequence::create(moveTo, moveBack, NULL);
	auto actions = CCRepeatForever::create(sequence);
	runAction(actions);*/

	CCSize s = CCDirector::sharedDirector()->getWinSize();

	// the root object just rotates around
	m_root = CCSprite::create("item_4.png");
	addChild(m_root, 1);
	m_root->setPosition(ccp(s.width / 2, s.height / 2));
	auto moveTo = CCMoveBy::create(2.0f, ccp(200, 500));
	auto moveBack = moveTo->reverse();
	auto sequence = CCSequence::create(moveTo, moveBack, NULL);
	auto actions = CCRepeatForever::create(sequence);
	m_root->runAction(actions);

	// create the streak object and add it to the scene
	m_streak = CCMotionStreak::create(2, 3, 32, ccGREEN, "streak.png");
	addChild(m_streak);
	// schedule an update on each frame so we can syncronize the streak with the target
	schedule(schedule_selector(HelloWorld::onUpdate));

	CCActionInterval *colorAction = CCRepeatForever::create(CCSequence::create(
		CCTintTo::create(0.2f, 255, 0, 0),
		CCTintTo::create(0.2f, 0, 255, 0),
		CCTintTo::create(0.2f, 0, 0, 255),
		CCTintTo::create(0.2f, 0, 255, 255),
		CCTintTo::create(0.2f, 255, 255, 0),
		CCTintTo::create(0.2f, 255, 0, 255),
		CCTintTo::create(0.2f, 255, 255, 255),
		NULL));

	m_streak->runAction(colorAction);

    return true;
}

void HelloWorld::onUpdate(float delta)
{
	m_streak->setPosition(m_root->convertToWorldSpace(CCPointZero));
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
	CCPoint position = pTouch->getLocation();
	auto pEmitter = GameUtil::getExplodeEffect();
	pEmitter->setPosition(position);
	addChild(pEmitter);

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