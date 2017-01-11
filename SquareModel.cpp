#include "SquareModel.h"
#include "Config.h"
#include "CommonMacros.h"
#include "GameController.h"
#include "Box2dFactory.h"
#include "CircleNode.h"

SquareModel::SquareModel()
:m_curScore(1)
, m_bIsFreezing(false)
{

}

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
		node = SquareNode::create();
		break;
	case kType_Triangle:
		node = TriangleNode::create();
		break;
	case kType_AddMarble:
		node = CircleAddMarbleNode::create();
		break;
	case kType_EliminateRow:
		//node = CircleEliRowNode::create();
		node = BossEatMarbleNode::create();
		break;
	case kType_EliminateCol:
		//node = CircleEliRowNode::create();
		node = BossEatMarbleNode::create();
		break;
	case kType_Rebound:
		node = CircleReboundNode::create();
		break;
	case kType_AddCoin:
		node = CircleAddCoinNode::create();
		break;
	case kType_Empty:
		node = nullptr;
		break;
	default:
		assert(false && "not exit square type!");
		break;
	}
	if (node != nullptr)
	{
		m_squares.push_back(node);
	}
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
	node->runRemoveAction();
}

void SquareModel::clearSquares()
{
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto &square = *iter;
		//Box2dFactory::getInstance()->removeBody(square->getBody());
		square->addScore(-square->getScore());
	}
	//m_squares.clear();
}

std::vector<SquareNode*> SquareModel::createSquareList()
{
	srand(time(NULL));
	std::vector<SquareNode*> nodes;

	std::vector<int> ballListType = getBallListType();
	for (int i = 0; i < BALL_COL_SIZE; i++)
	{
		int type = kType_AddCoin;// ballListType.at(i);
		int random = rand() % 2;
		if (random == 1)
		{
			type = kType_Square;
		}

		type = ballListType.at(i);
		SquareNode *node = createSquareNode(type);
		if (node != nullptr)
		{
			node->setBody();
			node->setIndex(i);
			nodes.push_back(node);
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

void SquareModel::squareMoveDown()
{
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		bool isLastOne = false;
		if (iter == m_squares.end() - 1)
		{
			isLastOne = true;
		}
		(*iter)->moveDown(isLastOne);
	}
}

void SquareModel::removeBelowSquares()
{
	auto bottomPos = GameController::getInstance()->getTargetPos();
	auto squares = m_squares;
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		auto &square = (*iter);
		if (square->getPositionY() - square->getContentSize().height * 3 < bottomPos.y)
		{
			removeSquareNode(square);
		}
	}
}

void SquareModel::elimateSameRowSquare(SquareNode *node)
{
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto &square = (*iter);
		if ((square->getCollisionType() == kCollision_Square || square->getCollisionType() == kCollision_Triangle) && square->getPositionY() == node->getPositionY())
		{
			square->addScore(-1);
		}
	}
}

void SquareModel::elimateSameColSquare(SquareNode *node)
{
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto &square = (*iter);
		if ((square->getCollisionType() == kCollision_Square || square->getCollisionType() == kCollision_Triangle) && square->getPositionX() == node->getPositionX())
		{
			square->addScore(-1);
		}
	}
}

void SquareModel::setSquareFreezing(bool isFreezing)
{
	m_bIsFreezing = isFreezing;
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto &square = (*iter);
		square->setFreezing(isFreezing);
	}
}