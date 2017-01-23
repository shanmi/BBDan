#include "GameConfig.h"
#include "cocos-ext.h"
#include "cocos2d.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace rapidjson;

GameConfig::GameConfig()
:m_nDiamondCount(0)
, m_nMarbleCount(0)
, m_showLibaoLevel(20)
, m_yijian(0)
{
	memset(m_nDiamond, 0, sizeof(int)* 10 * 5);
	memset(m_nMarble, 0, sizeof(int)* 10 * 3);
	memset(m_probability, 0, sizeof(int)* SQUARE_SIZE * 3);
}

GameConfig::~GameConfig()
{
	//CC_SAFE_FREE(m_probability);
}

GameConfig *GameConfig::getInstance()
{
	static GameConfig instance;
	return &instance;
}

void GameConfig::preloadData()
{
	unsigned long bufSize = 0;
	unsigned char* buf = CCFileUtils::sharedFileUtils()->getFileData(GAME_CONFIG, "r", &bufSize);
	Document root;
	buf[bufSize] = '\0';
	root.Parse<0>((const char*)buf);
	if (!root.HasParseError())
	{
		Value &diamonds = root["diamonds"];
		if (diamonds.IsArray()){
			m_nDiamondCount = diamonds.Size();
			for (int i = 0; i < m_nDiamondCount; i++){
				Value &diamond = diamonds[i];
				if (!diamond.IsArray()){
					break;
				}
				for (int j = 0; j < 5; j++){
					m_nDiamond[i][j] = diamond[j].GetInt();
				}
			}
		}

		Value &marbles = root["marbles"];
		if (marbles.IsArray()){
			m_nMarbleCount = marbles.Size();
			for (int i = 0; i < m_nMarbleCount; i++){
				Value &marble = marbles[i];
				if (!marble.IsArray()){
					break;
				}
				for (int j = 0; j < 3; j++){
					m_nMarble[i][j] = marble[j].GetInt();
				}
			}
		}

		Value &probability = root["probability"];
		if (probability.IsArray()){
			for (int i = 0; i < 9; i++){
				Value &prop = probability[i];
				if (!prop.IsArray()){
					break;
				}
				for (int j = 0; j < 3; j++){
					m_probability[i][j] = prop[j].GetInt();
				}
			}
		}

		auto key = "showLibaoLevel";
		if (!root[key].IsNull())
		{
			m_showLibaoLevel = root[key].GetInt();
		}

		key = "yijian";
		if (!root[key].IsNull())
		{
			m_yijian = root[key].GetInt();
		}

	}
}