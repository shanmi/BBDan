#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__
#include "cocos2d.h"

#define GAME_CONFIG "data/data_config.json"
#define GAME_DATA "game.dat"

#define GAME_DATA_VERSION 1

#define OK 1
#define ERROR 0
#define OVER_FLOW -2

#define SQUARE_SIZE 10

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

	int m_probability[SQUARE_SIZE][3];

	int m_showLibaoLevel;

	int m_yijian;
private:
	GameConfig();

};

#endif