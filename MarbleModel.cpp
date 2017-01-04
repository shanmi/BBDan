#include "MarbleModel.h"
#include "MarbleNode.h"

MarbleModel::MarbleModel()
{
	m_attr.skin = 1;
	m_attr.damage = 1;
}
MarbleModel *MarbleModel::theModel()
{
	static MarbleModel model;
	return &model;
}

MarbleNode* MarbleModel::createMarble()
{
	MarbleNode *marble = MarbleNode::create(m_attr);
	m_marbles.push_back(marble);
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