#ifndef __LUCKY_UTIL_H__
#define __LUCKY_UTIL_H__

class LuckyUtil
{
public:
	static LuckyUtil *getInstance();

	float getRotateDegree(int &probType);
};

#endif