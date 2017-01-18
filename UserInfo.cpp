#include "UserInfo.h"
#include "CCUserDefaultEx.h"
#include "CommonMacros.h"

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
		CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(GAME_BEST_SCORE, bestScore);
		CCUserDefaultEx::sharedUserDefault()->flush();
	}
}

int UserInfo::getBestScore()
{
	return CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(GAME_BEST_SCORE, 0);
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
	unlockMarble(kMarble_Biger);
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