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
	auto sizeSprite = CCSprite::create("brick.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("item_0.png");
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
	auto sizeSprite = CCSprite::create("brick.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("item_1.png");
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
	int kColorHintSize = 15;
	auto parent = getParent();
	auto size = parent->getContentSize();
	auto colorCol = CCLayerColor::create(ccc4(255, 255, 0, 255));
	colorCol->setContentSize(ccp(size.width, kColorHintSize));
	parent->addChild(colorCol);
	colorCol->setPositionY(getPositionY() - kColorHintSize / 2);
	colorCol->stopAllActions();
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
	auto sizeSprite = CCSprite::create("brick.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("item_2.png");
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
	int kColorHintSize = 15;
	auto parent = getParent();
	auto size = parent->getContentSize();
	auto colorCol = CCLayerColor::create(ccc4(255, 255, 0, 255));
	colorCol->setContentSize(ccp(kColorHintSize, size.height));
	parent->addChild(colorCol);
	colorCol->setPositionX(getPositionX() - kColorHintSize / 2);
	colorCol->stopAllActions();
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
	auto sizeSprite = CCSprite::create("brick.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("item_3.png");
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
	auto sizeSprite = CCSprite::create("brick.png");
	auto size = sizeSprite->getContentSize();
	setContentSize(size);

	m_image = CCSprite::create("coin.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	auto *flip3d = CCOrbitCamera::create(2, -45, 0, 90, 180, 0, 0);
	auto repeat = CCRepeatForever::create(flip3d);
	m_image->runAction(repeat);

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
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate); //delete

	UserInfo::getInstance()->addCoins(ADD_COINS);
	GameController::getInstance()->updateCoins();
}