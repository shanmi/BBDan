#include "MarbleModel.h"
#include "MarbleNode.h"
#include "Config.h"

MarbleModel::MarbleModel()
{
	m_attr.skin = 1;
	m_attr.damage = 1;

	m_marblesCount = 0;
}
MarbleModel *MarbleModel::theModel()
{
	static MarbleModel model;
	return &model;
}

MarbleNode* MarbleModel::createMarble()
{
	MarbleNode *marble = MarbleNode::create(m_attr);
	marble->setVisible(false);
	m_marbles.push_back(marble);
	m_marblesCount = m_marbles.size();
	return marble;
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