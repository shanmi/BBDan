#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__
#include "cocos2d.h"

#define DATA_FILE "data/data_config.json"

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
	int m_nDiamond[][5];
private:
	GameConfig();

};

#endif