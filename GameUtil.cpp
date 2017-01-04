#include "GameUtil.h"

USING_NS_CC;

float GameUtil::getDegreeTwoPoints(CCPoint p1, CCPoint p2)
{
	float delta_x = (p2.x - p1.x);
	float delta_y = (p2.y - p1.y);
	int degree = 0;
	float fRadian = 0.f;
	float const_pi = 180.0f / (float)M_PI;
	if (delta_y >= 0)
	{
		if (delta_x > 0)
		{
			fRadian = atan((float)(delta_y / delta_x));
			degree = const_pi * fRadian;
		}
		else if (delta_x < 0)
		{
			fRadian = atan((float)(delta_y / delta_x)) * (-1);
			degree = const_pi * (M_PI - fRadian);
		}
		else if (delta_x == 0)
		{
			degree = 90;
		}
	}
	else if (delta_y < 0)
	{
		if (delta_x > 0)
		{
			fRadian = atan((float)(delta_y / delta_x)) * (-1);
			degree = const_pi * (2 * M_PI - fRadian);
		}
		else if (delta_x < 0)
		{
			fRadian = atan((float)(delta_y / delta_x));
			degree = const_pi * (M_PI + fRadian);
		}
		else if (delta_x == 0)
		{
			degree = 270;
		}
	}
	return degree;
}

float GameUtil::getRadian(float fDegree)
{
	return M_PI * fDegree / 180.0f;
}


float GameUtil::getDegree(float fRadian)
{
	return 180.0f * fRadian / M_PI;
}

std::string GameUtil::intToString(int value)
{
	char temp[50] = { 0 };
	sprintf(temp, "%d", value);
	return temp;
}

