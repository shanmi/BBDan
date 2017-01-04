#include "GameController.h"

GameController::GameController()
{
	m_bIsRoundOver = true;
}

GameController *GameController::getInstance()
{
	static GameController instance;
	return &instance;
}

void GameController::startOneRound()
{
	m_bIsRoundOver = false;
}

void GameController::endOneRound()
{
	m_bIsRoundOver = true;
}

