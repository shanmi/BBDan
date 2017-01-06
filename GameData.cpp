#include "GameData.h"
#include "CCUserDefaultEx.h"

GameData *GameData::getInstance()
{
	static GameData instance;
	return &instance;
}

void GameData::addCoins(int count)
{
	int curCount = getCoins();
	CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(GAME_COINS_COUNT, curCount + count);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

int GameData::getCoins()
{
	return CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(GAME_COINS_COUNT);
}