#include "SquareModel.h"
#include "Config.h"
#include "CommonMacros.h"
#include "GameController.h"
#include "Box2dFactory.h"
#include "CircleNode.h"
#include "GameConfig.h"
#include "cocos2d.h"
#include "GameUtil.h"
#include "MarbleModel.h"

USING_NS_CC;

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

SquareNode *SquareModel::createSquareNode(int type, int shap /* = -1 */)
{
	SquareNode *node;
	switch (type)
	{
	case kType_Square:
		node = SquareNode::create();
		break;
	case kType_Triangle:
		node = TriangleNode::create(shap);
		break;
	case kType_AddMarble:
		node = CircleAddMarbleNode::create();
		break;
	case kType_EliminateRow:
		node = CircleEliRowNode::create();
		break;
	case kType_EliminateCol:
		node = CircleEliColNode::create();
		break;
	case kType_EliminateCross:
		node = CircleEliCrossNode::create();
		break;
	case kType_Rebound:
		node = CircleReboundNode::create();
		break;
	case kType_AddCoin:
		node = CircleAddCoinNode::create();
		break;
	case kType_BossEatMarble:
		node = BossEatMarbleNode::create();
		break;
	case kType_Iron:
		node = IronNode::create();
		break;
	case kType_Protect:
		node = CircleProtectNode::create();
		break;
	case kType_Shotguns:
		node = CircleShotgunsNode::create();
		break;
	case kType_Blood:
		node = CircleBloodNode::create();
		break;
	case kType_Rocket:
		node = CircleRocketNode::create();
		break;
	case kType_Empty:
		node = nullptr;
		break;
	default:
		CCLog("type==================%d", type); node = nullptr;
		//assert(false && "not exit square type!");
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

SquareNode *SquareModel::getSquareByIndex(Index index)
{
	for (auto iter = m_squares.begin(); iter != m_squares.end(); iter++)
	{
		auto square = *iter;
		Index tempIndex = square->getIndex();
		if (tempIndex == index)
		{
			return square;
		}
	}
	return NULL;
}

void SquareModel::removeAllSquares()
{
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto &square = *iter;
		square->addScore(-square->getScore());
	}
}

std::vector<SquareNode*> SquareModel::loadSquareList()
{
	if (m_squares.size() == 0)
	{
		return createSquareList();
	}
	return m_squares;
}

std::vector<SquareNode*> SquareModel::createSquareList(bool autoAddScore /* = true */)
{
	srand(time(NULL));
	std::vector<SquareNode*> nodes;

	std::vector<int> ballListType;
	if (autoAddScore)
	{
		ballListType = getBallListType1();
	}
	else
	{
		ballListType = getBallListType2();
	}
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
			node->setIndex(i, 0);
			nodes.push_back(node);
		}
	}
	if (autoAddScore)
	{
		m_curScore++;
	}
	else
	{
		m_curScore++;
		if (m_curScore > 48)
		{
			m_curScore = 1;
		}
	}
	return nodes;
}

std::vector<int> SquareModel::getBallListType1()
{
	auto probabilitys = GameConfig::getInstance()->m_probability1;
	int count = GameConfig::getInstance()->m_probabilityCount1;
	std::vector<int> types;
	for (int i = 0; i < BALL_COL_SIZE; i++)
	{
		int type = getBallType(probabilitys, count);
		if (type == kType_AddMarble || type == kType_AddCoin || type == kType_Rebound || type == kType_EliminateRow || 
			type == kType_EliminateCol || type == kType_EliminateCross || type == kType_BossEatMarble)
		{
			auto iter = find(types.begin(), types.end(), type);
			while (iter != types.end())
			{
				type = getBallType(probabilitys, count);
				iter = find(types.begin(), types.end(), type);
			}
		}
		int curLevel = SquareModel::theModel()->getCurrentScore();
		int m_showPropsLevel = GameConfig::getInstance()->m_showPropsLevel;
		if (curLevel < m_showPropsLevel)
		{
			if (type == kType_Rebound || type == kType_EliminateRow || type == kType_EliminateCol || type == kType_EliminateCross || type == kType_AddCoin)
			{
				type = kType_Square;
			}
		}
		int showFireLevel = GameConfig::getInstance()->m_showFireLevel;
		if (curLevel < showFireLevel)
		{
			if (type == kType_BossEatMarble)
			{
				type = kType_Triangle;
			}
		}
		int showIronLevel = GameConfig::getInstance()->m_showIronLevel;
		if (curLevel < showIronLevel)
		{
			if (type == kType_Iron)
			{
				type = kType_Triangle;
			}
		}
		types.push_back(type);
	}
	return types;
}

std::vector<int> SquareModel::getBallListType2()
{
	auto probabilitys = GameConfig::getInstance()->m_probability2;
	int count = GameConfig::getInstance()->m_probabilityCount2;
	std::vector<int> types;
	for (int i = 0; i < BALL_COL_SIZE; i++)
	{
		int type = getBallType(probabilitys, count);
		if (type == kType_AddCoin || type == kType_EliminateRow || type == kType_EliminateCol || type == kType_EliminateCross || type == kType_Protect)
		{
			auto iter = find(types.begin(), types.end(), type);
			while (iter != types.end())
			{
				type = getBallType(probabilitys, count);
				iter = find(types.begin(), types.end(), type);
			}
		}
		types.push_back(type);
	}
	return types;
}

int SquareModel::getBallType(int probsArr[10][3], int size)
{
	std::vector<int> probs;
	int probAll = 0;
	for (int i = 0; i < size; i++)
	{
		int prob = probsArr[i][1];
		int curScore = SquareModel::theModel()->getCurrentScore();
		int m_showLibaoLevel = GameConfig::getInstance()->m_showLibaoLevel;
		if (curScore % m_showLibaoLevel == 0)
		{
			prob += probsArr[i][2] * (curScore / m_showLibaoLevel);
		}
		probAll += prob;
		probs.push_back(prob);
	}

	int random = rand() % (probAll);
	int total = 0;
	for (auto iter = probs.begin(); iter != probs.end(); ++iter)
	{
		int prob = (*iter);
		total += prob;
		if (random < total)
		{
			int index = iter - probs.begin();
			return probsArr[index][0];
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
		auto &square = *iter;
		square->moveDown(isLastOne);
		Index index = square->getIndex();
		square->setIndex(index.x, index.y + 1);
	}
}

void SquareModel::removeBelowSquares()
{
	auto bottomPos = GameController::getInstance()->getTargetPos();
	auto squares = m_squares;
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		auto &square = (*iter);
		if (square->getPositionY() - square->getContentSize().height * 5 < bottomPos.y)
		{
			removeSquareNode(square);
		}
	}
}

void SquareModel::elimateSameRowSquare(SquareNode *node)
{
	Index index = node->getIndex();
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto &square = (*iter);
		Index squareIndex = square->getIndex();
		if (square->canRemoveByProps() && index.y == squareIndex.y)
		{
			square->addScore(-1);
		}
	}
}

void SquareModel::elimateSameColSquare(SquareNode *node)
{
	Index index = node->getIndex();
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto &square = (*iter);
		Index squareIndex = square->getIndex();
		if (square->canRemoveByProps() && index.x == squareIndex.x)
		{
			square->addScore(-1);
		}
	}
}

void SquareModel::elimateAroundSquare(SquareNode *node)
{
	Index index = node->getIndex();
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto &square = (*iter);
		Index squareIndex = square->getIndex();
		if (square != node && square->canRemoveByProps() && abs(index.x - squareIndex.x) <= 1 && abs(index.y - squareIndex.y) <= 1)
		{
			square->stopAllActions();
			square->setRotation(0);
			auto rotate = CCRotateBy::create(0.1f, 6);
			auto rotateBack = rotate->reverse();
			auto sequence = CCSequence::create(rotate, rotateBack, NULL);
			square->runAction(sequence);
			int attactRate = GameController::getInstance()->getAttactRate();
			square->addScore(-attactRate);
		}
	}
}

void SquareModel::removeAroundSquare(SquareNode *node)
{
	Index index = node->getIndex();
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto &square = (*iter);
		Index squareIndex = square->getIndex();
		if (square->canRemoveByProps() && abs(index.x - squareIndex.x) <= 1 && abs(index.y - squareIndex.y) <= 1)
		{
			square->addScore(-square->getScore());
		}
	}
}

void SquareModel::removeSameRowSquare(SquareNode *node)
{
	Index index = node->getIndex();
	auto squares = getSquares();
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		auto square = (*iter);
		Index squareIndex = square->getIndex();
		if (square->canRemoveByProps() && index.y == squareIndex.y)
		{
			removeSquareNode(square);
		}
	}
}

void SquareModel::exchangeSquarePosition()
{
	int curLevel = SquareModel::theModel()->getCurrentScore();
	if (curLevel < GameConfig::getInstance()->m_reorderLevel)
	{
		return;
	}
	std::vector<SquareNode*> squares;
	std::vector<Index> indexs;
	CCPointArray *points = CCPointArray::create(m_squares.size());
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto square = *iter;
		if (square->canRemoveByProps() && square->getScore() > 0)
		{
			squares.push_back(square); 
			indexs.push_back(square->getIndex());
			points->addControlPoint(square->getPosition());
		}
	}
	auto seq = GameUtil::buildRandomSequence(squares.size());
	for (int i = 0; i < squares.size(); i++)
	{
		auto square = squares[i];
		auto index = indexs.at(seq[i]);
		square->setIndex(index);
		auto pos = points->getControlPointAtIndex(seq[i]);
		CCMoveTo *moveTo = CCMoveTo::create(0.4f, pos);
		square->runAction(CCEaseBackInOut::create(moveTo));
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
void SquareModel::clearSquares()
{
	m_squares.clear();
	m_curScore = 1;
}

int SquareModel::getRemainSqaure()
{
	int count = 0;
	for (auto iter = m_squares.begin(); iter != m_squares.end(); ++iter)
	{
		auto square = *iter;
		if (square->getSquareType() == kType_Square || square->getSquareType() == kType_Triangle)
		{
			count++;
		}
	}
	return count;
}