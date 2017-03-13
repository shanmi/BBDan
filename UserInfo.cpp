#include "UserInfo.h"
#include "CCUserDefaultEx.h"
#include "CommonMacros.h"
#include "SquareModel.h"
#include "GameConfig.h"

UserInfo *UserInfo::getInstance()
{
	static UserInfo instance;
	return &instance;
}

void UserInfo::setMute(bool isMute)
{
	CCUserDefaultEx::sharedUserDefault()->setBoolForKey(GAME_MUTE, isMute);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

bool UserInfo::isMute()
{
	return CCUserDefaultEx::sharedUserDefault()->getBoolForKey(GAME_MUTE, false);
}

void UserInfo::addCoins(int count)
{
	int curCount = getCoins();
	CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(GAME_COINS_COUNT, curCount + count);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

int UserInfo::getCoins()
{
	return CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(GAME_COINS_COUNT, 0);
}

void UserInfo::setBestScore(int score)
{
	int bestScore = getBestScore();
	if (score > bestScore)
	{
		CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(GAME_BEST_SCORE, score);
		CCUserDefaultEx::sharedUserDefault()->flush();
	}
}

int UserInfo::getBestScore()
{
	return CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(GAME_BEST_SCORE, 0);
}

void UserInfo::setCurMarbleType(int type)
{
	CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(GAME_MARBLE_TYPE, type);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

int UserInfo::getCurMarbleType()
{
	return CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(GAME_MARBLE_TYPE, kMarble_Normal);
}

void UserInfo::unlockMarble(int type)
{
	char temp[50] = { 0 };
	sprintf(temp, "%s-%d", GAME_UNLOCK_MARBLE, type);
	CCUserDefaultEx::sharedUserDefault()->setBoolForKey(temp, true);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

void UserInfo::unlockAllMarble()
{
	unlockMarble(kMarble_Faster);
	unlockMarble(kMarble_Bigger);
	unlockMarble(kMarble_Dispersed);
	unlockMarble(kMarble_Across);
	unlockMarble(kMarble_Bomb);
}

bool UserInfo::isUnlock(int type)
{
	if (type == kMarble_Normal)
	{
		return true;
	}
	char temp[50] = { 0 };
	sprintf(temp, "%s-%d", GAME_UNLOCK_MARBLE, type);
	return CCUserDefaultEx::sharedUserDefault()->getBoolForKey(temp, false);
}

void UserInfo::setFishGuide(bool finish)
{
	CCUserDefaultEx::sharedUserDefault()->setBoolForKey(GAME_FISH_GUIDE, finish);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

bool UserInfo::isFishGuideFinish()
{
	return CCUserDefaultEx::sharedUserDefault()->getBoolForKey(GAME_FISH_GUIDE, false);
}

void UserInfo::addPropsCount(int type, int count)
{
	int curCount = getPropsCount(type);
	char temp[50] = { 0 };
	sprintf(temp, "%s-%d", GAME_PROP_COUNT, type);
	CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(temp, curCount + count);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

int UserInfo::getPropsCount(int type)
{
	char temp[50] = { 0 };
	sprintf(temp, "%s-%d", GAME_PROP_COUNT, type);
	return CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(temp, 0);
}

bool UserInfo::hasGetLuckyLevel(int level)
{
	char temp[50] = { 0 };
	sprintf(temp, "%s-%d", GAME_LUCKY_LEVEL, level);
	return CCUserDefaultEx::sharedUserDefault()->getBoolForKey(temp, false);
}

void UserInfo::setLuckyLevel(int level)
{
	char temp[50] = { 0 };
	sprintf(temp, "%s-%d", GAME_LUCKY_LEVEL, level);
	CCUserDefaultEx::sharedUserDefault()->setBoolForKey(temp, true);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

void UserInfo::resetLuckyLevel()
{
	char temp[50] = { 0 };
	int curLevel = SquareModel::theModel()->getCurrentScore();
	auto luckyLevel = GameConfig::getInstance()->m_luckyLevel;
	for (int i = 1; i <= curLevel; i++)
	{
		if (luckyLevel[0] == i)
		{
			continue;
		}
		sprintf(temp, "%s-%d", GAME_LUCKY_LEVEL, i);
		CCUserDefaultEx::sharedUserDefault()->setBoolForKey(temp, false);
		CCUserDefaultEx::sharedUserDefault()->flush();
	}
}

void UserInfo::setTargetLevel()
{
	int curTarget = getTargetLevel();
	CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(GAME_TARGET_LEVEL, curTarget * 2);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

int UserInfo::getTargetLevel()
{
	return CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(GAME_TARGET_LEVEL, 10);
}

void UserInfo::resetTargetLevel()
{
	CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(GAME_TARGET_LEVEL, 10);
	CCUserDefaultEx::sharedUserDefault()->flush();
}