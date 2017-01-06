#include "CircleNode.h"

#include "cocos2d.h"
#include "Box2dFactory.h"
#include "MarbleModel.h"
#include "GameController.h"
#include "SquareModel.h"
#include "GameData.h"
#include "Config.h"
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

	auto node = CCSprite::create("item_0.png");
	addChild(node);
	auto fanIn = CCFadeIn::create(0.6f);
	node->runAction(fanIn);

	m_body = Box2dFactory::getInstance()->createCircle(this, node->getContentSize(), true);

	return true;
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

	auto node = CCSprite::create("item_1.png");
	addChild(node);
	auto fanIn = CCFadeIn::create(0.6f);
	node->runAction(fanIn);

	m_body = Box2dFactory::getInstance()->createCircle(this, node->getContentSize(), true);

	return true;
}

void CircleEliRowNode::doCollisionAction()
{
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

	auto node = CCSprite::create("item_2.png");
	addChild(node);
	auto fanIn = CCFadeIn::create(0.6f);
	node->runAction(fanIn);

	m_body = Box2dFactory::getInstance()->createCircle(this, node->getContentSize(), true);

	return true;
}

void CircleEliColNode::doCollisionAction()
{
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

	auto node = CCSprite::create("item_3.png");
	addChild(node);
	auto fanIn = CCFadeIn::create(0.6f);
	node->runAction(fanIn);

	m_body = Box2dFactory::getInstance()->createCircle(this, node->getContentSize(), false);

	return true;
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

	auto node = CCSprite::create("coin.png");
	addChild(node);
	auto fanIn = CCFadeIn::create(0.6f);
	node->runAction(fanIn);

	m_body = Box2dFactory::getInstance()->createCircle(this, node->getContentSize(), true);

	return true;
}

void CircleAddCoinNode::doCollisionAction()
{
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate); //delete

	GameData::getInstance()->addCoins(ADD_COINS);
	GameController::getInstance()->updateCoins();
}