#include "GameConfig.h"
#include "cocos-ext.h"
USING_NS_CC;
using namespace rapidjson;

GameConfig::GameConfig()
{

}

GameConfig::~GameConfig()
{

}

GameConfig *GameConfig::getInstance()
{
	static GameConfig instance;
	return &instance;
}

void GameConfig::preloadData()
{
	unsigned long bufSize = 0;
	unsigned char* buf = CCFileUtils::sharedFileUtils()->getFileData(DATA_FILE, "r", &bufSize);
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
	}
}