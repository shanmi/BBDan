#ifndef _GAME_UTIL_H_
#define _GAME_UTIL_H_

#include "cocos2d.h"

enum BlockType
{
	kBlock_Square = 1,
	kBlock_Triangle
};

namespace GameUtil
{
	float getDegreeTwoPoints(cocos2d::CCPoint p1, cocos2d::CCPoint p2);
	float getRadian(float fDegree);
	float getDegree(float fRadian);
	std::string intToString(int value);

	cocos2d::CCParticleExplosion *getExplodeEffect();
	cocos2d::CCMotionStreak *getMotionStreak();

	std::string getBlockImage(int type, int score);

	cocos2d::CCAction *getScaleAction();
	cocos2d::CCAction *getBlinkAction();
	cocos2d::CCAction *getFadeInOutAction();
	cocos2d::CCLabelAtlas *getImageNum(std::string fontPath, int num);
	cocos2d::CCLabelAtlas *getImageNum(std::string fontPath, std::string numStr);
}

#endif