#include "GameConfig.h"
#include "cocos-ext.h"
#include "cocos2d.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace rapidjson;

GameConfig::GameConfig()
:m_nDiamondCount(0)
, m_nMarbleCount(0)
, m_fuhuoCostCoin(10)
, m_doubleAttactCost(5)
, m_hammerCost(5)
, m_freezingCost(5)
, m_showLibaoLevel(20)
, m_showPropsLevel(3)
, m_showFireLevel(20)
, m_reorderLevel(40)
, m_splitSquareLevel(60)
, m_addFireLevel(80)
, m_gravityLevel(120)
, m_showIronLevel(160)
, m_yijian(0)
{
	/*memset(m_nDiamond, 0, sizeof(int)* 10 * 5);
	memset(m_nMarble, 0, sizeof(int)* 10 * 3);
	memset(m_probability, 0, sizeof(int)* 10 * 3);*/
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

		Value &probability1 = root["probability1"];
		if (probability1.IsArray()){
			m_probabilityCount1 = probability1.Size();
			for (int i = 0; i < m_probabilityCount1; i++){
				Value &prop = probability1[i];
				if (!prop.IsArray()){
					break;
				}
				for (int j = 0; j < 3; j++){
					m_probability1[i][j] = prop[j].GetInt();
				}
			}
		}

		Value &probability2 = root["probability2"];
		if (probability2.IsArray()){
			m_probabilityCount2 = probability2.Size();
			for (int i = 0; i < m_probabilityCount2; i++){
				Value &prop = probability2[i];
				if (!prop.IsArray()){
					break;
				}
				for (int j = 0; j < 3; j++){
					m_probability2[i][j] = prop[j].GetInt();
				}
			}
		}

		auto key = "fuhuoCostCoin";
		if (!root[key].IsNull())
		{
			m_fuhuoCostCoin = root[key].GetInt();
		}

		key = "doubleAttactCost";
		if (!root[key].IsNull())
		{
			m_doubleAttactCost = root[key].GetInt();
		}

		key = "hammerCost";
		if (!root[key].IsNull())
		{
			m_hammerCost = root[key].GetInt();
		}

		key = "freezingCost";
		if (!root[key].IsNull())
		{
			m_freezingCost = root[key].GetInt();
		}

		key = "showLibaoLevel";
		if (!root[key].IsNull())
		{
			m_showLibaoLevel = root[key].GetInt();
		}

		key = "showPropsLevel";
		if (!root[key].IsNull())
		{
			m_showPropsLevel = root[key].GetInt();
		}

		key = "showFireLevel";
		if (!root[key].IsNull())
		{
			m_showFireLevel = root[key].GetInt();
		}

		key = "reorderLevel";
		if (!root[key].IsNull())
		{
			m_reorderLevel = root[key].GetInt();
		}

		key = "splitSquareLevel";
		if (!root[key].IsNull())
		{
			m_splitSquareLevel = root[key].GetInt();
		}

		key = "addFireLevel";
		if (!root[key].IsNull())
		{
			m_addFireLevel = root[key].GetInt();
		}

		key = "gravityLevel";
		if (!root[key].IsNull())
		{
			m_gravityLevel = root[key].GetInt();
		}

		key = "showIronLevel";
		if (!root[key].IsNull())
		{
			m_showIronLevel = root[key].GetInt();
		}
		
		key = "yijian";
		if (!root[key].IsNull())
		{
			m_yijian = root[key].GetInt();
		}

	}
}