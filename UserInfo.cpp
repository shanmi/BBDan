#include "UserInfo.h"
#include "CCUserDefaultEx.h"

UserInfo *UserInfo::getInstance()
{
	static UserInfo instance;
	return &instance;
}

void UserInfo::addCoins(int count)
{
	int curCount = getCoins();
	CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(GAME_COINS_COUNT, curCount + count);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

int UserInfo::getCoins()
{
	return CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(GAME_COINS_COUNT);
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