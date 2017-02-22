#include "LuckyUtil.h"
#include "GameConfig.h"
#include "CommonMacros.h"

LuckyUtil *LuckyUtil::getInstance()
{
	static LuckyUtil *instance;
	return instance;
}

float LuckyUtil::getRotateDegree(int &probType)
{
	int probAll = 0;
	std::vector<int> probs;
	int size = GameConfig::getInstance()->m_luckyprobCount;
	auto probsArr = GameConfig::getInstance()->m_luckyprob;
	for (int i = 0; i < size; i++)
	{
		int prob = probsArr[i][1];
		probAll += prob;
		probs.push_back(prob);
	}

	int random = rand() % (probAll);
	int total = 0;
	for (auto iter = probs.begin(); iter != probs.end(); ++iter)
	{
		int prob = (*iter);
		total += prob;
		if (random < total)
		{
			int index = iter - probs.begin();
			probType = probsArr[index][0];
			break;
		}
	}
	float degree = (360.0f - 360.0f * probType / kLucky_Max) - 360.0f / (kLucky_Max * 2);
	return degree;
}

