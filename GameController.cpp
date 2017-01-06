#include "GameController.h"
#include "MarbleModel.h"
#include "SquareModel.h"
#include "GameData.h"
GameController::GameController()
:m_counter(-1)
, m_bIsRoundOver(true)
, m_targetPos(ccp(0, 0))
, m_attactRate(ATTACT_RATE)
{
}

void GameController::addView(INotifyView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter == m_views.end())
	{
		m_views.push_back(view);
	}
}

void GameController::removeView(INotifyView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter != m_views.end())
	{
		m_views.erase(iter);
	}
}
GameController *GameController::getInstance()
{
	static GameController instance;
	return &instance;
}

void GameController::startOneRound()
{
	m_counter = 0;
}

void GameController::addCounter()
{
	m_counter++;
}

bool GameController::isCounterFull()
{
	if (m_counter >= MarbleModel::theModel()->getMarbles().size())
	{
		return true;
	}
	return false;
}

void GameController::checkSquares(bool isCheckTool /* = false */)
{
	auto squares = SquareModel::theModel()->getSquares();
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		auto square = *iter;
		if (square->getScore() <= 0)
		{
			if (square->shouldRemoveDirectly() || isCheckTool)
			{
				SquareModel::theModel()->removeSquareNode(square);
			}
		}
	}
}
bool GameController::checkGameOver()
{
	auto squares = SquareModel::theModel()->getSquares();
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		auto &square = (*iter);
		if (square->getPositionY() - square->getContentSize().height < m_targetPos.y)
		{
			if (square->getCollisionType() == kCollision_Square || square->getCollisionType() == kCollision_Triangle)
			{
				showGameOver();
				return true;
			}
			SquareModel::theModel()->removeSquareNode(square);
		}
	}
	return false;
}

bool GameController::checkCoinsEnought()
{
	int coinCount = GameData::getInstance()->getCoins();
	if (coinCount >= DOUBLE_ATTACT_COST_COIN)
	{
		return true;
	}
	return false;
}