#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__
#include "cocos2d.h"

#define GAME_CONFIG "data/data_config.json"
#define GAME_DATA "game.dat"

#define GAME_DATA_VERSION 1

#define OK 1
#define ERROR 0
#define OVER_FLOW -2

class GameConfig
{
public:
	static GameConfig *getInstance();
	~GameConfig();

public:
	void preloadData();
	static std::string getDataFileName(const char* fileName);

public:
	int m_nDiamondCount;
	int m_nDiamond[10][5];

	int m_nMarbleCount;
	int m_nMarble[10][3];

	int m_probabilityCount1;
	int m_probability1[20][3];

	int m_probabilityCount2;
	int m_probability2[20][3];

	int m_showLibaoLevel;
	int m_showPropsLevel;
	int m_showFireLevel;
	int m_reorderLevel;
	int m_splitSquareLevel;
	int m_addFireLevel;
	int m_gravityLevel;
	int m_showIronLevel;

	int m_yijian;
private:
	GameConfig();

};

#endif