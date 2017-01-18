#include "MarbleModel.h"
#include "MarbleNode.h"
#include "Config.h"
#include "CommonMacros.h"
#include "Box2dFactory.h"

MarbleModel::MarbleModel()
{
	m_attr = NormalMarle();

	m_marblesCount = 0;
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

int MarbleModel::checkMarblesCount()
{
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
		if ((*iter)->getReboundTimes() >= REBOUND_TIMES)
		{
			return true;
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