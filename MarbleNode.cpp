#include "MarbleNode.h"
#include "cocos2d.h"
#include "Config.h"
#include "Box2dFactory.h"
USING_NS_CC;

MarbleNode::MarbleNode(MarbleAttr attr)
: m_attr(attr)
, m_bIsMoving(false)
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

	return true;
}

void MarbleNode::shoot(float degree)
{
	b2Vec2 v_t;
	v_t.x = BALL_MOVE_PACE * cos(CC_DEGREES_TO_RADIANS(degree));
	v_t.y = BALL_MOVE_PACE * sin(CC_DEGREES_TO_RADIANS(degree));
	m_body->SetLinearVelocity(v_t);
	m_bIsMoving = true;
}

void MarbleNode::stop()
{
	b2Vec2 v_t;
	v_t.x = 0;
	v_t.y = 0;
	m_body->SetLinearVelocity(v_t);
	m_bIsMoving = false;

}