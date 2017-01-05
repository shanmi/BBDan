#include "GameController.h"
#include "MarbleModel.h"
#include "SquareModel.h"
GameController::GameController()
{
	m_counter = -1;
	m_bIsRoundOver = true;
	m_targetPos = ccp(0, 0);
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

bool GameController::checkGameOver()
{
	auto squares = SquareModel::theModel()->getSquares();
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		auto sqaure = (*iter);
		if (sqaure->getPositionY() - sqaure->getContentSize().height < m_targetPos.y)
		{
			showGameOver();
			return true;
		}
	}
	return false;
}