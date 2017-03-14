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
	cocos2d::CCParticleExplosion *getExplodeEffect(std::string image);
	cocos2d::CCParticleExplosion *getRandomExplodeEffect();
	cocos2d::CCParticleSystemQuad *getBombEffect();
	cocos2d::CCParticleSystemQuad *getBombEffect2();
	cocos2d::CCMotionStreak *getMotionStreak();

	std::string getBlockImage(int type, int score);

	cocos2d::CCAction *getRepeatScaleAction();
	cocos2d::CCAction *getOnceScaleAction();
	cocos2d::CCAction *getBlinkAction();
	cocos2d::CCAction *getFadeInOutAction();
	cocos2d::CCSprite *getAchievementEffect(int type);

	cocos2d::CCLabelAtlas *getImageNum(std::string fontPath, int num);
	cocos2d::CCLabelAtlas *getImageNum(std::string fontPath, std::string numStr);

	std::vector<int> buildRandomSequence(int length);

	int getLuckyLevel();
	int getLastLuckyLevel();

	cocos2d::CCParticleMeteor *getBossSkillEffect(cocos2d::CCPoint pos1, cocos2d::CCPoint pos2);
}

#endif