#include "GameUtil.h"

USING_NS_CC;

float GameUtil::getDegreeTwoPoints(CCPoint p1, CCPoint p2)
{
	float delta_x = (p2.x - p1.x);
	float delta_y = (p2.y - p1.y);
	int degree = 0;
	float fRadian = 0.f;
	float const_pi = 180.0f / (float)M_PI;
	if (delta_y >= 0)
	{
		if (delta_x > 0)
		{
			fRadian = atan((float)(delta_y / delta_x));
			degree = const_pi * fRadian;
		}
		else if (delta_x < 0)
		{
			fRadian = atan((float)(delta_y / delta_x)) * (-1);
			degree = const_pi * (M_PI - fRadian);
		}
		else if (delta_x == 0)
		{
			degree = 90;
		}
	}
	else if (delta_y < 0)
	{
		if (delta_x > 0)
		{
			fRadian = atan((float)(delta_y / delta_x)) * (-1);
			degree = const_pi * (2 * M_PI - fRadian);
		}
		else if (delta_x < 0)
		{
			fRadian = atan((float)(delta_y / delta_x));
			degree = const_pi * (M_PI + fRadian);
		}
		else if (delta_x == 0)
		{
			degree = 270;
		}
	}
	return degree;
}

float GameUtil::getRadian(float fDegree)
{
	return M_PI * fDegree / 180.0f;
}


float GameUtil::getDegree(float fRadian)
{
	return 180.0f * fRadian / M_PI;
}

std::string GameUtil::intToString(int value)
{
	char temp[50] = { 0 };
	sprintf(temp, "%d", value);
	return temp;
}

CCParticleExplosion *GameUtil::getExplodeEffect(){
	CCParticleExplosion *pEmitter = CCParticleExplosion::create();
	pEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle.png"));
	pEmitter->setAutoRemoveOnFinish(true);

	ccColor4F c4Start = { 0.3, 0.4, 0.5, 0.6 };
	ccColor4F c4End = { 1, 1, 1, 1 };
	pEmitter->setStartColor(c4Start);
	pEmitter->setEndColor(c4End);
	pEmitter->setStartColorVar(c4Start);
	pEmitter->setEndColorVar(c4End);

	pEmitter->setTotalParticles(50);

	pEmitter->setSpeed(80);
	pEmitter->setSpeedVar(20);

	pEmitter->setLife(2);
	pEmitter->setLifeVar(0.3f);
	pEmitter->setScale(0.5f);

	return pEmitter;
}

CCMotionStreak *GameUtil::getMotionStreak()
{
	auto streak = CCMotionStreak::create(0.1f, 3, 32, ccGREEN, "streak.png");

	CCActionInterval *colorAction = CCRepeatForever::create(CCSequence::create(
		CCTintTo::create(0.2f, 255, 0, 0),
		CCTintTo::create(0.2f, 0, 255, 0),
		CCTintTo::create(0.2f, 0, 0, 255),
		CCTintTo::create(0.2f, 0, 255, 255),
		CCTintTo::create(0.2f, 255, 255, 0),
		CCTintTo::create(0.2f, 255, 0, 255),
		CCTintTo::create(0.2f, 255, 255, 255),
		NULL));

	streak->runAction(colorAction);
	return streak;
}