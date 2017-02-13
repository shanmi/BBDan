#ifndef __DARKNESS_LAYER_H__
#define __DARKNESS_LAYER_H__

#include "cocos2d.h"
class DarknessLayer
	:public cocos2d::CCLayer
{
public:
	virtual bool init();
	CREATE_FUNC(DarknessLayer);

	virtual void onEnter();
	virtual void onExit();

private:
	cocos2d::CCLayerGradient *mGradientLayer;
};
#endif