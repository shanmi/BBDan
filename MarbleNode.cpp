#include "MarbleNode.h"
#include "cocos2d.h"
#include "Config.h"
#include "Box2dFactory.h"
#include "CCFunctionAction.h"
#include "ActionSequence.h"
#include "GameController.h"
USING_NS_CC;

MarbleNode::MarbleNode(MarbleAttr attr)
: m_attr(attr)
, m_bIsMoving(false)
, m_bTrueStop(false)
{

}

MarbleNode::~MarbleNode()
{

}

MarbleNode *MarbleNode::create(MarbleAttr attr)
{
	MarbleNode *node = new MarbleNode(attr);
	node->autorelease();
	node->init();
	return node;
}

bool MarbleNode::init()
{
	char temp[30] = { 0 };
	sprintf(temp, "play_ball_%d.png", m_attr.skin);
	CCSprite *playerBall = CCSprite::create(temp); 
	addChild(playerBall);
	auto size = playerBall->getContentSize();

	setContentSize(size);
	m_body = Box2dFactory::getInstance()->createCircle(this);

	return true;
}

void MarbleNode::setPosition(const CCPoint &position)
{
	CCNode::setPosition(position);
	if (!isMoving())
	{
		b2Vec2 post = b2Vec2((float)(position.x / PTM_RATIO), (float)(position.y / PTM_RATIO));
		float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
		m_body->SetTransform(post, angle);
	}
}

void MarbleNode::shoot(float degree)
{
	m_bTrueStop = false;
	b2Vec2 v_t;
	v_t.x = BALL_MOVE_PACE * cos(CC_DEGREES_TO_RADIANS(degree));
	v_t.y = BALL_MOVE_PACE * sin(CC_DEGREES_TO_RADIANS(degree));
	m_body->SetLinearVelocity(v_t);
}

void MarbleNode::stop()
{
	b2Vec2 v_t;
	v_t.x = 0;
	v_t.y = 0;
	m_body->SetLinearVelocity(v_t);
	m_bIsMoving = false;

}

void MarbleNode::moveToTargetPos()
{
	if (m_bTrueStop)
	{
		return;
	}
	m_bTrueStop = true;

	auto pos = GameController::getInstance()->getTargetPos();
	auto actions = ActionSequence::create(this);
	auto move = CCMoveTo::create(0.5f, pos);
	bool isFirst = GameController::getInstance()->getCounter() == 1;
	bool isCounterFull = GameController::getInstance()->isCounterFull();
	auto callback = CCFunctionAction::create([=]()
	{
		if (!isFirst)
		{
			this->setVisible(false);
		}
		if (isCounterFull)
		{
			GameController::getInstance()->updateSquares();
		}
	});
	actions->addAction(move);
	actions->addAction(callback);
	actions->runActions();
}