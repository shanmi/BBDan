#ifndef __MY_ADVERTISE_H__
#define __MY_ADVERTISE_H__

class MyAdvertise
{
public:
	static MyAdvertise *getInstance();

public:
	void showBannerAdvertise();
	void showScreenAdvertise();
	void showVideoAdvertise();
};
#endif