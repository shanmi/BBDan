#ifndef __LUCKY_UTIL_H__
#define __LUCKY_UTIL_H__

#include "CommonMacros.h"
#include "cocos2d.h"

struct ILuckyView
{
	virtual void updateView(){}
	virtual void onVideoCallback(){}
};

class LuckyUtil
{
public:
	static LuckyUtil *getInstance();

public:
	void updateView(){ NOTIFY_VIEWS(updateView); }
	void onVideoCallback(){ NOTIFY_VIEWS(onVideoCallback); }

	float getRotateDegree(int &probType);
	void addView(ILuckyView *view);
	void removeView(ILuckyView *view);

private:
	std::vector<ILuckyView*> m_views;
};

#endif