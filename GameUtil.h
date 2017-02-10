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
	cocos2d::CCParticleExplosion *getRandomExplodeEffect();
	cocos2d::CCParticleSystemQuad *getBombEffect();
	cocos2d::CCMotionStreak *getMotionStreak();

	std::string getBlockImage(int type, int score);

	cocos2d::CCAction *getScaleAction();
	cocos2d::CCAction *getBlinkAction();
	cocos2d::CCAction *getFadeInOutAction();
	cocos2d::CCLabelAtlas *getImageNum(std::string fontPath, int num);
	cocos2d::CCLabelAtlas *getImageNum(std::string fontPath, std::string numStr);

	void putInt(int src, FILE* file);
	void getInt(int &temp, FILE* file);
	void putString(std::string str, FILE* file);
	void getString(std::string &str, FILE* file);
	int xorEncDecInt(int ch);
	int saveGameInfo();
	int loadGameInfo();
	int clearGameInfo();

	std::vector<int> buildRandomSequence(int length);
}

#endif