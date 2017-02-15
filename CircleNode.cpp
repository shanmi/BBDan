#include "CircleNode.h"

#include "cocos2d.h"
#include "Box2dFactory.h"
#include "MarbleModel.h"
#include "GameController.h"
#include "SquareModel.h"
#include "UserInfo.h"
#include "Config.h"
#include "ActionSequence.h"
#include "CCFunctionAction.h"
#include "GameUtil.h"
#include "SoundMgr.h"
USING_NS_CC;

CircleAddMarbleNode *CircleAddMarbleNode::create()
{
	CircleAddMarbleNode *node = new CircleAddMarbleNode();
	node->autorelease();
	node->init();
	return node;
}

bool CircleAddMarbleNode::init()
{
	m_score = 1;
	auto sizeSprite = CCSprite::create("squares/fangkuai_hong1.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("squares/fangkuai_+.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	return true;
}

void CircleAddMarbleNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), true);
}

void CircleAddMarbleNode::runRemoveAction()
{
	removeFromParent();
}

void CircleAddMarbleNode::doCollisionAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Pop1);
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);// delete

	doScaleAction();
	MarbleModel::theModel()->addMarblesCount();
	GameController::getInstance()->updateMarbles();
}

///////////////////////////////////////////////////////
CircleEliRowNode *CircleEliRowNode::create()
{
	CircleEliRowNode *node = new CircleEliRowNode();
	node->autorelease();
	node->init();
	return node;
}

bool CircleEliRowNode::init()
{
	m_score = 1;
	auto sizeSprite = CCSprite::create("squares/fangkuai_hong1.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("squares/fangkuai_hengxiao.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	return true;
}

void CircleEliRowNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), true);
}

void CircleEliRowNode::runRemoveAction()
{
	removeFromParent();
}

void CircleEliRowNode::doCollisionAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Lazer);

	int kColorHintSize = 15;
	auto parent = getParent();
	auto size = parent->getContentSize();
	auto colorRow = CCLayerColor::create(ccc4(255, 255, 0, 120));
	colorRow->setContentSize(ccp(size.width, kColorHintSize));
	parent->addChild(colorRow);
	colorRow->setPositionY(getPositionY() - kColorHintSize / 2);
	auto actions = ActionSequence::create(colorRow);
	auto marbles = MarbleModel::theModel()->getMarbles();
	auto action1 = CCBlink::create(0.1f, 1);
	auto action2 = CCFunctionAction::create([=]()
	{
		colorRow->removeFromParent();
	});
	actions->addAction(action1);
	actions->addAction(action2);
	actions->runActions();

	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);// delete

	doScaleAction();
	SquareModel::theModel()->elimateSameRowSquare(this);
}


///////////////////////////////////////////////////////
CircleEliColNode *CircleEliColNode::create()
{
	CircleEliColNode *node = new CircleEliColNode();
	node->autorelease();
	node->init();
	return node;
}

bool CircleEliColNode::init()
{
	m_score = 1;
	auto sizeSprite = CCSprite::create("squares/fangkuai_hong1.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("squares/fangkuai_suxiao.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	return true;
}

void CircleEliColNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), true);
}

void CircleEliColNode::runRemoveAction()
{
	removeFromParent();
}

void CircleEliColNode::doCollisionAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Lazer);

	int kColorHintSize = 15;
	auto parent = getParent();
	auto size = parent->getContentSize();
	auto colorCol = CCLayerColor::create(ccc4(255, 255, 0, 120));
	colorCol->ignoreAnchorPointForPosition(false);
	colorCol->setAnchorPoint(ccp(0.5f, 0.5f));
	colorCol->setContentSize(ccp(kColorHintSize, size.height*0.7f));
	parent->addChild(colorCol);
	colorCol->setPosition(ccp(getPositionX(), size.height * 0.51f));
	auto actions = ActionSequence::create(colorCol);
	auto marbles = MarbleModel::theModel()->getMarbles();
	auto action1 = CCBlink::create(0.1f, 1);
	auto action2 = CCFunctionAction::create([=]()
	{
		colorCol->removeFromParent();
	});
	actions->addAction(action1);
	actions->addAction(action2);
	actions->runActions();

	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);// delete

	doScaleAction();
	SquareModel::theModel()->elimateSameColSquare(this);
}

///////////////////////////////////////////////////////
CircleEliCrossNode *CircleEliCrossNode::create()
{
	CircleEliCrossNode *node = new CircleEliCrossNode();
	node->autorelease();
	node->init();
	return node;
}

bool CircleEliCrossNode::init()
{
	m_score = 1;
	auto sizeSprite = CCSprite::create("squares/fangkuai_hong1.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("squares/fangkuai_sizixiao.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	return true;
}

void CircleEliCrossNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), true);
}

void CircleEliCrossNode::runRemoveAction()
{
	removeFromParent();
}

void CircleEliCrossNode::doCollisionAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Lazer);

	int kColorHintSize = 15;
	auto parent = getParent();
	auto size = parent->getContentSize();
	auto colorCol = CCLayerColor::create(ccc4(255, 255, 0, 120));
	colorCol->ignoreAnchorPointForPosition(false);
	colorCol->setAnchorPoint(ccp(0.5f, 0.5f));
	colorCol->setContentSize(ccp(kColorHintSize, size.height*0.7f));
	parent->addChild(colorCol);
	colorCol->setPosition(ccp(getPositionX(), size.height * 0.51f));
	auto actions = ActionSequence::create(colorCol);
	auto marbles = MarbleModel::theModel()->getMarbles();
	auto action1 = CCBlink::create(0.1f, 1);
	auto action2 = CCFunctionAction::create([=]()
	{
		colorCol->removeFromParent();
	});
	actions->addAction(action1);
	actions->addAction(action2);
	actions->runActions();

	auto colorRow = CCLayerColor::create(ccc4(255, 255, 0, 120));
	colorRow->setContentSize(ccp(size.width, kColorHintSize));
	parent->addChild(colorRow);
	colorRow->setPositionY(getPositionY() - kColorHintSize / 2);
	actions = ActionSequence::create(colorRow);
	auto action3 = CCBlink::create(0.1f, 1);
	auto action4 = CCFunctionAction::create([=]()
	{
		colorRow->removeFromParent();
	});
	actions->addAction(action3);
	actions->addAction(action4);
	actions->runActions();

	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);// delete

	doScaleAction();
	SquareModel::theModel()->elimateSameColSquare(this);
	SquareModel::theModel()->elimateSameRowSquare(this);
}

///////////////////////////////////////////////////////
CircleReboundNode *CircleReboundNode::create()
{
	CircleReboundNode *node = new CircleReboundNode();
	node->autorelease();
	node->init();
	return node;
}

bool CircleReboundNode::init()
{
	m_score = 1;
	auto sizeSprite = CCSprite::create("squares/fangkuai_hong1.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("squares/fangkuai_suiji.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	return true;
}

void CircleReboundNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), true);
}

void CircleReboundNode::runRemoveAction()
{
	removeFromParent();
}

void CircleReboundNode::doCollisionAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Pop2);
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);// delete

	doScaleAction();
}


///////////////////////////////////////////////////////
CircleAddCoinNode *CircleAddCoinNode::create()
{
	CircleAddCoinNode *node = new CircleAddCoinNode();
	node->autorelease();
	node->init();
	return node;
}

bool CircleAddCoinNode::init()
{
	m_score = 1;
	auto sizeSprite = CCSprite::create("squares/fangkuai_hong1.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("squares/fangkuai_zhuanshi.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	auto *flip3d = CCOrbitCamera::create(2, -45, 0, 90, 180, 0, 0);
	auto repeat = CCRepeatForever::create(flip3d);
	//m_image->runAction(repeat);

	return true;
}

void CircleAddCoinNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), true);
}

void CircleAddCoinNode::runRemoveAction()
{
	removeFromParent();
}

void CircleAddCoinNode::doCollisionAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Coin);
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate); //delete

	UserInfo::getInstance()->addCoins(ADD_COINS);
	GameController::getInstance()->updateCoins();
}

///////////////////////////////////////////////////////
CircleProtectNode *CircleProtectNode::create()
{
	CircleProtectNode *node = new CircleProtectNode();
	node->autorelease();
	node->init();
	return node;
}

bool CircleProtectNode::init()
{
	m_score = 1;
	auto sizeSprite = CCSprite::create("squares/fangkuai_hong1.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("squares/fangkuai_baohuzhao.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	return true;
}

void CircleProtectNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), true);
}

void CircleProtectNode::runRemoveAction()
{
	removeFromParent();
}

void CircleProtectNode::doCollisionAction()
{
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate); //delete

	GameController::getInstance()->useProtectEffect();
}

///////////////////////////////////////////////////////
CircleShotgunsNode *CircleShotgunsNode::create()
{
	CircleShotgunsNode *node = new CircleShotgunsNode();
	node->autorelease();
	node->init();
	return node;
}

bool CircleShotgunsNode::init()
{
	m_score = 1;
	auto sizeSprite = CCSprite::create("squares/fangkuai_hong1.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("squares/fangkuai_sandanqiang.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	return true;
}

void CircleShotgunsNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), true);
}

void CircleShotgunsNode::runRemoveAction()
{
	removeFromParent();
}

void CircleShotgunsNode::doCollisionAction()
{
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate); //delete

	GameController::getInstance()->useShotGunsEffect();
}

///////////////////////////////////////////////////////
CircleBloodNode *CircleBloodNode::create()
{
	CircleBloodNode *node = new CircleBloodNode();
	node->autorelease();
	node->init();
	return node;
}

bool CircleBloodNode::init()
{
	m_score = 1;
	auto sizeSprite = CCSprite::create("squares/fangkuai_hong1.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("squares/fangkuai_xueliangbao.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	return true;
}

void CircleBloodNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), true);
}

void CircleBloodNode::runRemoveAction()
{
	removeFromParent();
}

void CircleBloodNode::doCollisionAction()
{
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate); //delete

	GameController::getInstance()->getBloodEffect();
}

///////////////////////////////////////////////////////
CircleRocketNode *CircleRocketNode::create()
{
	CircleRocketNode *node = new CircleRocketNode();
	node->autorelease();
	node->init();
	return node;
}

bool CircleRocketNode::init()
{
	m_score = 1;
	auto sizeSprite = CCSprite::create("squares/fangkuai_hong1.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("squares/fangkuai_huojianpao.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	return true;
}

void CircleRocketNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), true);
}

void CircleRocketNode::runRemoveAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Pop3);

	auto emitter = GameUtil::getBombEffect();
	emitter->setPosition(getPosition());
	getParent()->addChild(emitter, kZOrder_Square + 1);

	/*auto explore = GameUtil::getRandomExplodeEffect();
	explore->setPosition(getPosition());
	getParent()->addChild(explore);*/
	removeFromParent();
}

void CircleRocketNode::doCollisionAction()
{
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate); //delete

	SquareModel::theModel()->removeAroundSquare(this);
}