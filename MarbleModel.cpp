#include "MarbleModel.h"
#include "MarbleNode.h"
#include "Config.h"
#include "CommonMacros.h"
#include "Box2dFactory.h"
#include "UserInfo.h"

MarbleModel::MarbleModel()
{
	int type = UserInfo::getInstance()->getCurMarbleType();
	switch (type)
	{
	case kMarble_Normal:
		m_attr = NormalMarle();
		break;
	case kMarble_Faster:
		m_attr = FasterMarle();
		break;
	case kMarble_Biger:
		m_attr = BiggerMarle();
		break;
	case kMarble_Dispersed:
		m_attr = DispersedMarle();
		break;
	case kMarble_Bomb:
		m_attr = BombMarle();
		break;
	case kMarble_Across:
		m_attr = AcrossdMarle();
		break;
	default:
		m_attr = NormalMarle();
		break;
	}

	m_marblesCount = 0;
	m_attactRate = 1;
	m_tempAddCount = 0;
}
MarbleModel *MarbleModel::theModel()
{
	static MarbleModel model;
	return &model;
}

MarbleNode* MarbleModel::createMarble()
{
	//m_attr = FasterMarle();
	MarbleNode *marble = MarbleNode::create(m_attr);
	marble->setVisible(false);
	m_marbles.push_back(marble);
	m_marblesCount = m_marbles.size();
	return marble;
}

void MarbleModel::removeMarble(MarbleNode *node)
{
	auto iter = find(m_marbles.begin(), m_marbles.end(), node);
	if (iter != m_marbles.end())
	{
		m_marbles.erase(iter);
	}
	Box2dFactory::getInstance()->removeBody(node->getBody());
	node->removeFromParent();
}

void MarbleModel::setMarbleAttr(MarbleAttr attr)
{
	m_attr = attr;
	UserInfo::getInstance()->setCurMarbleType(m_attr.skin);
}

bool MarbleModel::haveMarbleMoving()
{
	for (auto iter = m_marbles.begin(); iter != m_marbles.end(); ++iter)
	{
		if (!(*iter)->isTrueStop())
		{
			return true;
		}
	}
	return false;
}

void MarbleModel::addMarblesCount()
{
	m_tempAddCount++;
}

int MarbleModel::checkMarblesCount()
{
	if (m_tempAddCount >= m_attactRate)
	{
		m_marblesCount += m_tempAddCount / m_attactRate;
		m_tempAddCount = m_tempAddCount % m_attactRate;
	}
	if (m_marblesCount >= 100)
	{
		m_marblesCount = m_marblesCount / 2;
		m_attactRate++;
	}

	if (m_marblesCount == (m_marbles.size() + 1) / 2)
	{
		for (int i = 0; i<m_marblesCount; i++)
		{
			removeMarble(m_marbles[i]);
		}
	}
	int curCount = m_marbles.size();
	if (m_marblesCount > curCount)
	{
		return m_marblesCount - curCount;
	}
	return 0;
}

bool MarbleModel::isMarblesNerverStop()
{
	for (auto iter = m_marbles.begin(); iter != m_marbles.end(); ++iter)
	{
		auto marble = *iter;
		auto body = marble->getBody();
		auto speed = body->GetLinearVelocity();
		if ((*iter)->getReboundTimes() >= REBOUND_TIMES)
		{
			CCLOG("fabs(speed.x) === %f,  fabs(speed.y) === %f", fabs(speed.x), fabs(speed.y));
			if (fabs(speed.x) > 1 && fabs(speed.y) < 1)
			{
				return true;
			}
		}
	}
	return false;
}

void MarbleModel::reboundMarbles()
{
	for (auto iter = m_marbles.begin(); iter != m_marbles.end(); ++iter)
	{
		auto &marble = *iter;
		marble->addReboundTimes(-marble->getReboundTimes());
	}
}

void MarbleModel::clearMarbles()
{
	m_marbles.clear();
}