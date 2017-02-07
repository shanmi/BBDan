#include "LoginUtils.h"
#include "CCUserDefaultEx.h"
#include "cocos2d.h"

LoginUtils *LoginUtils::getInstance()
{
	static LoginUtils *instance;
	return instance;
}

int LoginUtils::getAfterLoginDays()
{
	int currentDay = (long)(time(NULL) / DAY_SECOND);
	int lastDay = CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(LOGIN_DAYS, currentDay);
	return (currentDay - lastDay) % 7 + 1;
}

void LoginUtils::setLoginTime()
{
	bool loginFirst = CCUserDefaultEx::sharedUserDefault()->getBoolForKey(LOGIN_FIRST, true);
	if (!loginFirst)
	{
		return;
	}
	CCUserDefaultEx::sharedUserDefault()->setBoolForKey(LOGIN_FIRST, false);
	int currentDay = (long)(time(NULL) / DAY_SECOND);
	CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(LOGIN_DAYS, currentDay);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

int LoginUtils::getTargetDays()
{
	return CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(TARGET_DAYS, 1);
}

void LoginUtils::setTargetDays()
{
	int lastTarget = getTargetDays();
	if (lastTarget >= 7)
	{
		lastTarget = 1;
	}
	else
	{
		lastTarget++;
	}
	int currentDay = (long)(time(NULL) / DAY_SECOND);
	CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(LOGIN_DAYS, currentDay);
	CCUserDefaultEx::sharedUserDefault()->setIntegerForKey(TARGET_DAYS, lastTarget);
	CCUserDefaultEx::sharedUserDefault()->flush();
}

bool LoginUtils::checkLoginReward()
{
	int currentDay = (long)(time(NULL) / DAY_SECOND);
	int lastDay = CCUserDefaultEx::sharedUserDefault()->getIntegerForKey(LOGIN_DAYS, 0);
	if (currentDay > lastDay)
	{
		return true;
	}
	return false;
}

