#include "MarbleNode.h"
#include "cocos2d.h"
#include "Config.h"
#include "Box2dFactory.h"
#include "CCFunctionAction.h"
#include "ActionSequence.h"
#include "GameController.h"
#include "GameUtil.h"
#include "MarbleModel.h"
USING_NS_CC;

MarbleNode::MarbleNode(MarbleAttr attr)
: m_attr(attr)
, m_streak(NULL)
, m_body(NULL)
, m_bIsMoving(false)
, m_bTrueStop(false)
, m_reboundTimes(0)
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
	sprintf(temp, "marbles/ball_%d.png", m_attr.skin);
	m_playerBall = CCSprite::create(temp);
	m_playerBall->setPosition(ccp(m_playerBall->getContentSize().width * 0.5f, m_playerBall->getContentSize().height * 0.5f));
	m_playerBall->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(m_playerBall);
	auto size = m_playerBall->getContentSize();

	setTag(kTag_Marble);
	setContentSize(size);
	setAnchorPoint(ccp(0.5f, 0.5f));

	schedule(schedule_selector(MarbleNode::updateStreak));

	return true;
}

void MarbleNode::initWithFile(const char* imagePath)
{
	m_playerBall->initWithFile(imagePath);
}

void MarbleNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createMarble(this);
}

void MarbleNode::removeBody()
{
	if (m_body)
	{
		Box2dFactory::getInstance()->removeBody(m_body);
	}
}

void MarbleNode::setPosition(const CCPoint &position)
{
	CCNode::setPosition(position);
	if (!isMoving() && m_body)
	{
		b2Vec2 post = b2Vec2((float)(position.x / PTM_RATIO), (float)(position.y / PTM_RATIO));
		float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
		m_body->SetTransform(post, angle);
	}
}

void MarbleNode::shoot(float degree)
{
	if (m_attr.skin == kMarble_Dispersed)
	{
		auto marbles = MarbleModel::theModel()->getMarbles();
		auto iter = find(marbles.begin(), marbles.end(), this);
		if (iter != marbles.begin())
		{
			degree = rand() % 40 - 20 + degree;
			if (degree < 6)
			{
				degree = 6;
			}
			else if (degree > 174)
			{
				degree = 174;
			}
		}
	}
	m_bTrueStop = false;
	b2Vec2 v_t;
	v_t.x = BALL_MOVE_PACE * cos(CC_DEGREES_TO_RADIANS(degree)) * m_attr.speed;
	v_t.y = BALL_MOVE_PACE * sin(CC_DEGREES_TO_RADIANS(degree)) * m_attr.speed;
	m_body->SetLinearVelocity(v_t);
}

void MarbleNode::shooterShoot(float degree)
{
	if (m_attr.skin == kMarble_Dispersed)
	{
		
	}
	m_bTrueStop = false;
	b2Vec2 v_t;
	v_t.x = BALL_MOVE_PACE * cos(CC_DEGREES_TO_RADIANS(degree)) * m_attr.speed;
	v_t.y = BALL_MOVE_PACE * sin(CC_DEGREES_TO_RADIANS(degree)) * m_attr.speed;
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
	setPosition(pos);
	//auto actions = ActionSequence::create(this);
	//auto move = CCMoveTo::create(0.5f, pos);
	int count = GameController::getInstance()->getCounter();
	bool isCounterFull = GameController::getInstance()->isCounterFull();
	if (isCounterFull)
	{
		GameController::getInstance()->oneRoundEnd();
		GameController::getInstance()->resetCounter();
	}
	auto callback = CCFunctionAction::create([=]()
	{
		
	});
	
	/*actions->addAction(move);
	actions->addAction(callback);
	actions->runActions();*/
}

void MarbleNode::addMotionStreak()
{
	m_streak = CCMotionStreak::create(0.1f, 1, 22, ccGREEN, "particle/streak.png");
	m_streak->setAnchorPoint(ccp(0.5f, 0.8f));
	CCActionInterval *colorAction;
	switch (m_attr.skin)
	{
	case kMarble_Normal:
		colorAction = CCRepeatForever::create(CCSequence::create(
			CCTintTo::create(0.2f, 255, 255, 255),
			NULL));
		break;
	case kMarble_Dispersed:
		colorAction = CCRepeatForever::create(CCSequence::create(
			CCTintTo::create(0.2f, 255, 0, 0),
			CCTintTo::create(0.2f, 0, 255, 0),
			CCTintTo::create(0.2f, 0, 0, 255),
			CCTintTo::create(0.2f, 0, 255, 255),
			CCTintTo::create(0.2f, 255, 255, 0),
			CCTintTo::create(0.2f, 255, 0, 255),
			CCTintTo::create(0.2f, 255, 255, 255),
			NULL));
		break;
	case kMarble_Bigger:
		colorAction = CCRepeatForever::create(CCSequence::create(
			CCTintTo::create(0.2f, 255, 201, 155),
			CCTintTo::create(0.2f, 219, 188, 138),
			NULL));
		break;
	case kMarble_Faster:
		colorAction = CCRepeatForever::create(CCSequence::create(
			CCTintTo::create(0.2f, 68, 233, 236),
			NULL));
		break;
	case kMarble_Bomb:
		colorAction = CCRepeatForever::create(CCSequence::create(
			CCTintTo::create(0.2f, 147, 81, 75),
			CCTintTo::create(0.2f, 220, 120, 21),
			NULL));
		break;
	}
	m_streak->runAction(colorAction);
}

void MarbleNode::updateStreak(float delta)
{
	if (m_streak == NULL && getParent() != NULL)
	{
		addMotionStreak();
		getParent()->addChild(m_streak);
	}
	if (m_streak)
	{
		m_streak->setPosition(getPosition());
		if (m_bIsMoving)
		{
			m_streak->setVisible(true);
		}
		else
		{
			m_streak->setVisible(false);
		}
	}
}

void MarbleNode::runRemoveAction()
{
	unschedule(schedule_selector(MarbleNode::updateStreak));
	if (m_streak)
	{
		m_streak->removeFromParent();
	}
	removeBody();
	removeFromParent();
}