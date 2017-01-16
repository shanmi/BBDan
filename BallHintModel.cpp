#include "BallHintModel.h"
#include "Config.h"
#include "GameUtil.h"

USING_NS_CC;

BallHintModel *BallHintModel::theModel()
{
	static BallHintModel model;
	return &model;
}

std::vector<cocos2d::CCSprite*> BallHintModel::createBallHints()
{
	for (int i = 0; i < HINT_BALL_SIZE; i++)
	{
		auto hint = CCSprite::create("hint.png");
		hint->setScale(0.2f);
		hint->setContentSize(hint->getContentSize()*0.6f);
		hint->setVisible(false);
		m_ballHints.push_back(hint);
	}
	return m_ballHints;
}

void BallHintModel::setHintVisible(bool visible)
{
	for (auto iter = m_ballHints.begin(); iter != m_ballHints.end(); ++iter)
	{
		(*iter)->setVisible(visible);
	}
}

void BallHintModel::updatePosition(CCPoint pos1, CCPoint pos2, cocos2d::CCPoint startPos, float arrowWidth)
{
	float degree = GameUtil::getDegreeTwoPoints(pos1, pos2);
	float distance = ccpDistance(pos1, pos2);
	//CCLog("distance=-============%f", distance);

	float radian = GameUtil::getRadian(degree);
	float x = cos(radian);
	float y = sin(radian);

	float scale = distance / 120.0f + 1;
	if (scale > 2)
	{
		scale = 2;
	}
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	for (int i = 0; i < m_ballHints.size(); i++)
	{
		auto hint = m_ballHints[i];
		float newX = startPos.x + (hint->getContentSize().width * i * scale + arrowWidth) * x;
		float newY = startPos.y + (hint->getContentSize().height * i * scale + arrowWidth) * y + 20;
		/*if (newX < 0)
		{
		newX = -newX;
		}
		else if (newX > winSize.width)
		{
		radian = GameUtil::getRadian(degree + 10);
		newX = winSize.width - (newX - winSize.width);
		}*/
		hint->setPosition(ccp(newX, newY));
	}
}

void BallHintModel::clearHints()
{
	m_ballHints.clear();
}