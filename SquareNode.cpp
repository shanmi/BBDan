#include "SquareNode.h"
#include "GameUtil.h"
#include "Box2dFactory.h"
#include "GameController.h"
#include "ActionSequence.h"
#include "CCFunctionAction.h"
#include "Config.h"

USING_NS_CC;

SquareNode::SquareNode(int type, int score)
:m_type(type)
,m_score(score)
{

}

SquareNode *SquareNode::create(int type, int score)
{
	SquareNode *node = new SquareNode(type, score);
	node->autorelease();
	node->init();
	return node;
}

bool SquareNode::init()
{
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
	switch (m_type)
	{
	case kType_Square:
		post = b2Vec2((float)(getPosition().x / PTM_RATIO), (float)((getPosition().y) / PTM_RATIO));
		break;
	case kType_Triangle:
		post = b2Vec2((float)((getPosition().x - getContentSize().width / 2) / PTM_RATIO), (float)(((getPosition().y - getContentSize().height / 2)) / PTM_RATIO));
		break;
	}
	float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
	m_body->SetTransform(post, angle);
}

void SquareNode::addScore(int score)
{
	m_score += score;
	std::string scoreStr = GameUtil::intToString(m_score);
	m_scoreLabel->setString(scoreStr.c_str());
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

/////////////////////////////////////////////////////////////////////////////////////
TriangleNode *TriangleNode::create(int type, int score)
{
	TriangleNode *node = new TriangleNode(type, score);
	node->autorelease();
	node->init();
	return node;
}

bool TriangleNode::init()
{
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