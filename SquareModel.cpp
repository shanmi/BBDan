#include "SquareModel.h"
#include "Config.h"
#include "CommonMacros.h"
#include "GameController.h"
#include "Box2dFactory.h"

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
	case kType_Square:
		node = SquareNode::create(type, m_curScore);
		break;
	case kType_Triangle:
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
	Box2dFactory::getInstance()->removeBody(node->getBody());
	node->removeFromParent();
}

std::vector<SquareNode*> SquareModel::createSquareList()
{
	srand(time(NULL));
	std::vector<SquareNode*> nodes;

	std::vector<int> ballListType = getBallListType();
	for (int i = 0; i < BALL_COL_SIZE; i++)
	{
		int type = ballListType.at(i);
		SquareNode *node;
		switch (type)
		{
		case kType_Empty:
			break;
		case kType_Square:
		case kType_Triangle:
			node = createSquareNode(type);
			node->setIndex(i);
			nodes.push_back(node);
			break;
		case kType_AddMarble:
			break;
		case kType_AddCoin:
			break;
		case kType_Rebound:
			break;
		case kType_EliminateRow:
			break;
		case kType_EliminateCol:
			break;
		default:
			break;
		}
	}
	m_curScore++;
	return nodes;
}

std::vector<int> SquareModel::getBallListType()
{
	std::vector<int> types;
	for (int i = 0; i < BALL_COL_SIZE; i++)
	{
		int type = getBallType();
		types.push_back(type);
	}
	return types;
}

void SquareModel::squareMoveDown()
{
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		(*iter)->moveDown();
	}
}

int SquareModel::getBallType()
{
	std::vector<int> types;
	types.push_back(kType_Empty);
	types.push_back(kType_Square);
	types.push_back(kType_Triangle);
	types.push_back(kType_AddMarble);
	types.push_back(kType_AddCoin);
	types.push_back(kType_Rebound);
	types.push_back(kType_EliminateRow);
	types.push_back(kType_EliminateCol);

	int random = rand() % 100;
	int total = 0;
	for (auto iter = types.begin(); iter != types.end(); ++iter)
	{
		int type = (*iter);
		total += type;
		if (random < total)
		{
			return type;
		}
	}

}

void SquareModel::removeBelowSquares()
{
	auto bottomPos = GameController::getInstance()->getTargetPos();
	auto squares = m_squares;
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		auto &sqaure = (*iter);
		if (sqaure->getPositionY() - sqaure->getContentSize().height * 3 < bottomPos.y)
		{
			removeSquareNode(*iter);
		}
	}
}