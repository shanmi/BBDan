#include "SquareNode.h"
#include "GameUtil.h"
#include "Box2dFactory.h"
#include "GameController.h"
#include "ActionSequence.h"
#include "CCFunctionAction.h"
#include "Config.h"
#include "MarbleModel.h"
#include "SquareModel.h"

USING_NS_CC;

SquareNode::SquareNode()
:m_scoreLabel(NULL)
, m_collisionType(kCollision_Square)
{
	
}

SquareNode *SquareNode::create()
{
	SquareNode *node = new SquareNode();
	node->autorelease();
	node->init();
	return node;
}

bool SquareNode::init()
{
	m_score = SquareModel::theModel()->getCurrentScore();

	auto node = CCSprite::create("brick.png");
	auto color = ccc3(255, 255 - (m_score * 13) % 256, (m_score * 7) % 256);
	node->setColor(color);
	addChild(node);
	auto fanIn = CCFadeIn::create(0.6f);
	node->runAction(fanIn);

	auto size = node->getContentSize();
	setContentSize(size);

	std::string scoreStr = GameUtil::intToString(m_score);
	m_scoreLabel = CCLabelTTF::create(scoreStr.c_str(), "fonts/SF Square Root.ttf", 34);
	m_scoreLabel->setColor(color);
	addChild(m_scoreLabel);

	m_body = Box2dFactory::getInstance()->createSquare(this);

	return true;
}

void SquareNode::setPosition(const cocos2d::CCPoint &position)
{
	CCNode::setPosition(position);
	b2Vec2 post;
	post = b2Vec2((float)(getPosition().x / PTM_RATIO), (float)((getPosition().y) / PTM_RATIO));
	float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
	m_body->SetTransform(post, angle);
}

void SquareNode::addScore(int score)
{
	m_score += score;
	std::string scoreStr = GameUtil::intToString(m_score);
	if (m_scoreLabel)
	{
		m_scoreLabel->setString(scoreStr.c_str());
	}
}

void SquareNode::moveDown()
{
	auto actions = ActionSequence::create(this);
	auto delay = CCFadeIn::create(0.6f);
	auto move = CCMoveBy::create(1.0f, ccp(0, -getContentSize().height-SQUARE_SPACING));
	auto callback = CCFunctionAction::create([=]()
	{
		GameController::getInstance()->setRoundState(true);
		GameController::getInstance()->checkGameOver();
	});
	actions->addAction(delay);
	actions->addAction(move);
	actions->addAction(callback);
	actions->runActions();
}

bool SquareNode::shouldRemoveDirectly()
{
	switch (m_collisionType)
	{
	case kCollision_Square:
	case kCollision_Triangle:
	case kCollision_AddMarble:
	case kCollision_AddCoin:
		return true;
		break;
	case kCollision_Rebound:
	case kCollision_EliminateRow:
	case kCollision_EliminateCol:
		return false;
		break;
	default:
		break;
	}
	return false;
}

void SquareNode::doScaleAction()
{
	auto scaleTo = CCScaleBy::create(0.1f, 1.2f);
	auto scaleBack = scaleTo->reverse();
	auto actions = CCSequence::create(scaleTo, scaleBack, NULL);
	runAction(actions);
}

void SquareNode::doCollisionAction()
{
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);
}

/////////////////////////////////////////////////////////////////////////////////////
TriangleNode *TriangleNode::create()
{
	TriangleNode *node = new TriangleNode();
	node->autorelease();
	node->init();
	return node;
}

bool TriangleNode::init()
{
	m_score = SquareModel::theModel()->getCurrentScore();

	int type = rand() % 4;
	char temp[50] = { 0 };
	sprintf(temp, "half_%d.png", type);
	auto node = CCSprite::create(temp);
	auto color = ccc3(255, 255 - (m_score * 13) % 256, (m_score * 7) % 256);
	node->setColor(color);
	addChild(node);
	auto fanIn = CCFadeIn::create(0.6f);
	node->runAction(fanIn);

	auto size = node->getContentSize();
	setContentSize(size);

	std::string scoreStr = GameUtil::intToString(m_score);
	m_scoreLabel = CCLabelTTF::create(scoreStr.c_str(), "fonts/SF Square Root.ttf", 34);
	m_scoreLabel->setColor(color);
	switch (type)
	{
	case 0:
		m_scoreLabel->setPosition(ccp(-size.width*0.2f, -size.height*0.2f));
		break;
	case 1:
		m_scoreLabel->setPosition(ccp(-size.width*0.2f, size.height*0.3f));
		break;
	case 2:
		m_scoreLabel->setPosition(ccp(size.width*0.3f, size.height*0.3f));
		break;
	case 3:
		m_scoreLabel->setPosition(ccp(size.width*0.3f, -size.height*0.2f));
		break;
	}
	addChild(m_scoreLabel);
	this->setTag(type);

	m_body = Box2dFactory::getInstance()->createTriangle(this);

	return true;
}

void TriangleNode::setPosition(const cocos2d::CCPoint &position)
{
	CCNode::setPosition(position);
	b2Vec2 post;
	post = b2Vec2((float)((getPosition().x - getContentSize().width / 2) / PTM_RATIO), (float)(((getPosition().y - getContentSize().height / 2)) / PTM_RATIO));
	float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
	m_body->SetTransform(post, angle);
}

void TriangleNode::doCollisionAction()
{
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);
}
