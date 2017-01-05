#include "SquareModel.h"
#include "Config.h"

SquareModel *SquareModel::theModel()
{
	static SquareModel model;
	return &model;
}

SquareNode *SquareModel::createSquareNode(int type)
{
	SquareNode *node;
	switch (type)
	{
	case TYPE_SQUARE:
		node = SquareNode::create(type, m_curScore);
		break;
	case TYPE_TRIANGLE:
		node = TriangleNode::create(type, m_curScore);
		break;
	}
	m_squares.push_back(node);
	return node;
}

void SquareModel::removeSquareNode(SquareNode *node)
{
	auto iter = find(m_squares.begin(), m_squares.end(), node);
	if (iter != m_squares.end())
	{
		m_squares.erase(iter);
	}
	node->removeFromParent();
}

std::vector<SquareNode*> SquareModel::createSquareList()
{
	std::vector<SquareNode*> nodes;

	std::vector<int> ballListIndex = getBallListIndex();
	std::vector<int> ballListType = getBallListType();
	int index = 0;
	for (int i = 0; i < BALL_COL_SIZE; i++)
	{
		if (find(ballListIndex.begin(), ballListIndex.end(), i) != ballListIndex.end())
		{
			auto node = createSquareNode(ballListType.at(index++));
			node->setIndex(i);
			nodes.push_back(node);
		}
	}
	m_curScore++;
	return nodes;
}

std::vector<int> SquareModel::getBallListIndex()
{
	return{ 0, 2, 4, 6 };
}

std::vector<int> SquareModel::getBallListType()
{
	return{ 0, 1, 0, 1 };
}

void SquareModel::squareMoveDown()
{
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		(*iter)->moveDown();
	}
}