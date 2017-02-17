#include "MarbleModel.h"
#include "MarbleNode.h"
#include "Config.h"
#include "CommonMacros.h"
#include "Box2dFactory.h"
#include "UserInfo.h"

MarbleModel::MarbleModel()
{
	int type = UserInfo::getInstance()->getCurMarbleType();
	m_attr = getMarbleAttrByType(type);

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

MarbleAttr MarbleModel::getMarbleAttrByType(int type)
{
	MarbleAttr attr;
	switch (type)
	{
	case kMarble_Normal:
		attr = NormalMarle();
		break;
	case kMarble_Faster:
		attr = FasterMarle();
		break;
	case kMarble_Biger:
		attr = BiggerMarle();
		break;
	case kMarble_Dispersed:
		attr = DispersedMarle();
		break;
	case kMarble_Bomb:
		attr = BombMarle();
		break;
	case kMarble_Across:
		attr = AcrossdMarle();
		break;
	default:
		attr = NormalMarle();
		break;
	}
	return attr;
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
		auto velocity = body->GetLinearVelocity();
		if ((*iter)->getReboundTimes() >= REBOUND_TIMES)
		{
			CCLOG("fabs(velocity.x) === %f,  fabs(velocity.y) === %f", fabs(velocity.x), fabs(velocity.y));
			if (fabs(velocity.x) > 1 && fabs(velocity.y) < 1)
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
	for (auto iter = m_marbles.begin(); iter != m_marbles.end(); ++iter)
	{
		auto &marble = *iter;
		marble->removeFromParent();
	}
	m_marbles.clear();
}