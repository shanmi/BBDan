#include "SquareNode.h"
#include "GameUtil.h"
#include "Box2dFactory.h"

USING_NS_CC;

SquareNode::SquareNode(int score)
:m_score(score)
{

}

SquareNode *SquareNode::create(int score)
{
	SquareNode *node = new SquareNode(score);
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

	auto size = node->getContentSize();
	setContentSize(size);

	std::string scoreStr = GameUtil::intToString(m_score);
	m_scoreLabel = CCLabelTTF::create(scoreStr.c_str(), "fonts/SF Square Root.ttf", 34);
	m_scoreLabel->setColor(color);
	addChild(m_scoreLabel);

	return true;
}

void SquareNode::addScore(int score)
{
	m_score += score;
	std::string scoreStr = GameUtil::intToString(m_score);
	m_scoreLabel->setString(scoreStr.c_str());
}

void SquareNode::moveDown()
{
	auto move = CCMoveBy::create(1.0f, ccp(0, -this->getContentSize().height));
	this->runAction(move);
	b2Vec2 post = b2Vec2((float)(this->getPosition().x / PTM_RATIO), (float)((this->getPosition().y - this->getContentSize().height) / PTM_RATIO));
	float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
	m_body->SetTransform(post, angle);
}

/////////////////////////////////////////////////////////////////////////////////////
TriangleNode *TriangleNode::create(int score)
{
	TriangleNode *node = new TriangleNode(score);
	node->autorelease();
	node->init();
	return node;
}

bool TriangleNode::init()
{
	srand(time(NULL));
	int type = rand() % 4;
	char temp[50] = { 0 };
	sprintf(temp, "half_%d.png", type);
	auto node = CCSprite::create(temp);
	auto color = ccc3((m_score * 3) % 256, (m_score * 10) % 256, (m_score * 17) % 256);
	node->setColor(color);
	addChild(node);

	auto size = node->getContentSize();
	setContentSize(size);

	std::string scoreStr = GameUtil::intToString(m_score);
	auto scoreLabel = CCLabelTTF::create(scoreStr.c_str(), "fonts/SF Square Root.ttf", 34);
	scoreLabel->setColor(color);
	scoreLabel->setPosition(ccpMult(size, -0.2f));
	addChild(scoreLabel);

	return true;
}