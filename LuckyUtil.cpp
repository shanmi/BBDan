#include "LuckyUtil.h"
#include "GameConfig.h"
#include "CommonMacros.h"
#include <algorithm>

LuckyUtil *LuckyUtil::getInstance()
{
	static LuckyUtil instance;
	return &instance;
}

void LuckyUtil::addView(ILuckyView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter == m_views.end())
	{
		m_views.push_back(view);
	}
}

void LuckyUtil::removeView(ILuckyView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter != m_views.end())
	{
		m_views.erase(iter);
	}
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
	float degree = (360.0f - 360.0f * probType / kLucky_MaxCount) + 360.0f / (kLucky_MaxCount);
	return degree;
}

