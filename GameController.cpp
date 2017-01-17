#include "GameController.h"
#include "MarbleModel.h"
#include "SquareModel.h"
#include "UserInfo.h"
#include "BallHintModel.h"
#include "MainMenu.h"
USING_NS_CC;

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
	m_counter = 0;// MarbleModel::theModel()->getMarbles().size();
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
		if (square->getBody() == NULL)
		{
			square->setBody();
			GameController::getInstance()->addSquareNode(square);
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
	int coinCount = UserInfo::getInstance()->getCoins();
	if (coinCount >= DOUBLE_ATTACT_COST_COIN)
	{
		return true;
	}
	return false;
}

void GameController::addSquareNode(SquareNode *node)
{
	NOTIFY_VIEWS(addSquareNode, node);
}

void GameController::createPropByMarble(MarbleNode *marble)
{
	MarbleModel::theModel()->reboundMarbles();
	auto targetPos = GameController::getInstance()->getTargetPos();
	auto node = CCSprite::create("brick.png");
	float posX = (node->getContentSize().width / 2 + SQUARE_SPACING) + 3 * (node->getContentSize().width + SQUARE_SPACING);
	float posY = targetPos.y + node->getContentSize().height / 2 - marble->getContentSize().height / 2;
	while (posY < marble->getPositionY())
	{
		posY += (node->getContentSize().height + SQUARE_SPACING);
	}
	posY = posY - node->getContentSize().height / 2 - SQUARE_SPACING;
	node->setPosition(ccp(posX, posY));

	auto squares = SquareModel::theModel()->getSquares();
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		auto square = (*iter);
		if (square->getCollisionType() != kCollision_EliminateRow && square->getCollisionType() != kCollision_EliminateCol)
		{
			auto rect = square->boundingBox();
			if (rect.containsPoint(ccp(posX, posY)))
			{
				return;
			}
		}
	}
	auto square = SquareModel::theModel()->createSquareNode(kType_Rebound);
	square->setPosition(ccp(posX, posY));
	square->addScore(-square->getScore());
}

void GameController::backToMainMenu()
{
	MarbleModel::theModel()->clearMarbles();
	BallHintModel::theModel()->clearHints();
	SquareModel::theModel()->clearSquares();
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCScene *pScene = MainMenu::scene();
	pDirector->replaceScene(pScene);
}