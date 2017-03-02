#ifndef __MY_ADVERTISE_H__
#define __MY_ADVERTISE_H__

#include "cocos2d.h"
class MyAdvertise
{
public:
	static MyAdvertise *getInstance();

public:
	void callStringMethod(std::string method);
	void showBannerAdvertise();
	void closeBannerAdvertise();

	void showScreenAdvertise();
	void closeScreenAdvertise();

	void showVideoAdvertise();
	void closeVideoAdvertise();

};
#endif