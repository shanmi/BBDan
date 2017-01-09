#ifndef _GAME_UTIL_H_
#define _GAME_UTIL_H_

#include "cocos2d.h"
namespace GameUtil
{
	float getDegreeTwoPoints(cocos2d::CCPoint p1, cocos2d::CCPoint p2);
	float getRadian(float fDegree);
	float getDegree(float fRadian);
	std::string intToString(int value);

	cocos2d::CCParticleExplosion *getExplodeEffect();
}

#endif