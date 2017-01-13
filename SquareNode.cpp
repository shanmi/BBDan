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
, m_body(NULL)
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
	if (m_score % 10 == 0)
	{
		int random = rand() % 100;
		if (random < 50)
		{
			m_score = m_score * 2;
		}
	}

	m_image = CCSprite::create("brick.png");
	auto color = ccc3(255, 255 - (m_score * 13) % 256, (m_score * 7) % 256);
	m_image->setColor(color);
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	auto size = m_image->getContentSize();
	setContentSize(size);

	std::string scoreStr = GameUtil::intToString(m_score);
	m_scoreLabel = CCLabelTTF::create(scoreStr.c_str(), "fonts/SF Square Root.ttf", 34);
	m_scoreLabel->setColor(color);
	addChild(m_scoreLabel);

	return true;
}

void SquareNode::setPosition(const cocos2d::CCPoint &position)
{
	CCNode::setPosition(position);
	if (m_body)
	{
		b2Vec2 post;
		post = b2Vec2((float)(getPosition().x / PTM_RATIO), (float)((getPosition().y) / PTM_RATIO));
		float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
		m_body->SetTransform(post, angle);
	}
}

void SquareNode::addScore(int score)
{
	m_score += score;
	std::string scoreStr = GameUtil::intToString(m_score);
	if (m_scoreLabel)
	{
		auto color = ccc3(255, 255 - (m_score * 13) % 256, (m_score * 7) % 256);
		m_scoreLabel->setString(scoreStr.c_str());
		m_image->setColor(color);
		m_scoreLabel->setColor(color);
	}
}

void SquareNode::moveDown(bool isLastOne /* = false */)
{
	auto actions = ActionSequence::create(this);
	auto delay = CCFadeIn::create(0.6f);
	auto move = CCMoveBy::create(1.0f, ccp(0, -getContentSize().height - SQUARE_SPACING));
	auto callback = CCFunctionAction::create([=]()
	{
		GameController::getInstance()->setRoundState(true);
		GameController::getInstance()->checkGameOver();
	});
	actions->addAction(delay);
	actions->addAction(move);
	if (isLastOne)
	{
		actions->addAction(callback);
	}
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
	default:
		break;
	}
	return false;
}

bool SquareNode::canRemoveByProps()
{
	switch (m_collisionType)
	{
	case kCollision_Square:
	case kCollision_Triangle:
	case kCollision_BossEatMarble:
		return true;
		break;
	default:
		break;
	}
	return false;
}
void SquareNode::runRemoveAction()
{
	auto explore = GameUtil::getExplodeEffect();
	explore->setPosition(getPosition());
	getParent()->addChild(explore);
	removeFromParent();
}

void SquareNode::doScaleAction()
{
	setScale(1.0f);
	auto scaleTo = CCScaleTo::create(0.1f, 1.2f);
	auto scaleBack = CCScaleTo::create(0.1f, 1.0f);
	auto actions = CCSequence::create(scaleTo, scaleBack, NULL);
	runAction(actions);
}

void SquareNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createSquare(this);
}

void SquareNode::doCollisionAction()
{
	showBombAction();

	int damage = MarbleModel::theModel()->getMarbleAttr().damage;
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate*damage);
}

void SquareNode::showBombAction()
{
	int skin = MarbleModel::theModel()->getMarbleAttr().skin;
	if (skin == kMarble_Bomb)
	{
		auto explore = GameUtil::getExplodeEffect();
		explore->setPosition(getPosition());
		getParent()->addChild(explore);
	}
}

void SquareNode::setFreezing(bool isFreezing)
{
	if (isFreezing)
	{
		auto freezing = CCSprite::create("freezing.png");
		addChild(freezing, 100, 100);
	}
	else
	{
		removeChildByTag(100);
	}
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
	if (m_score % 10 == 0)
	{
		int random = rand() % 100;
		if (random < 50)
		{
			m_score = m_score * 2;
		}
	}

	int type = rand() % 4;
	char temp[50] = { 0 };
	sprintf(temp, "half_%d.png", type);
	m_image = CCSprite::create(temp);
	auto color = ccc3(255, 255 - (m_score * 13) % 256, (m_score * 7) % 256);
	m_image->setColor(color);
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);

	auto size = m_image->getContentSize();
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

	return true;
}

void TriangleNode::setPosition(const cocos2d::CCPoint &position)
{
	CCNode::setPosition(position);
	if (m_body)
	{
		b2Vec2 post;
		post = b2Vec2((float)((getPosition().x - getContentSize().width / 2) / PTM_RATIO), (float)(((getPosition().y - getContentSize().height / 2)) / PTM_RATIO));
		float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
		m_body->SetTransform(post, angle);
	}
}

void TriangleNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createTriangle(this);
}

void TriangleNode::doCollisionAction()
{
	showBombAction();

	int damage = MarbleModel::theModel()->getMarbleAttr().damage;
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate*damage);
}


/////////////////////////////////////////////////////////////////////////////////////
BossEatMarbleNode *BossEatMarbleNode::create()
{
	BossEatMarbleNode *node = new BossEatMarbleNode();
	node->autorelease();
	node->init();
	return node;
}

bool BossEatMarbleNode::init()
{
	m_score = 1;// SquareModel::theModel()->getCurrentScore();

	/*m_image = CCSprite::create("boss_0.png");
	addChild(m_image);
	auto fanIn = CCFadeIn::create(0.6f);
	m_image->runAction(fanIn);*/

	CCParticleSun* sun = CCParticleSun::create();
	sun->setStartSize(60);
	sun->setEmissionRate(100);
	sun->setAnchorPoint(ccp(0.5f, 0.5f));
	sun->setPosition(0, 0);
	sun->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	addChild(sun);

	m_image = CCSprite::create("boss_0.png");
	auto size = m_image->getContentSize();
	setContentSize(size);

	/*auto color = ccc3(255, 255 - (m_score * 13) % 256, (m_score * 7) % 256);
	std::string scoreStr = GameUtil::intToString(m_score);
	m_scoreLabel = CCLabelTTF::create(scoreStr.c_str(), "fonts/SF Square Root.ttf", 34);
	m_scoreLabel->setColor(color);
	addChild(m_scoreLabel);*/

	return true;
}

void BossEatMarbleNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_image->getContentSize(), false);
}

void BossEatMarbleNode::doCollisionAction()
{
	showBombAction();

	int damage = MarbleModel::theModel()->getMarbleAttr().damage;
	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate*damage);
}
