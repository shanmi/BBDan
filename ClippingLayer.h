#ifndef __CLIPPING_LAYER_H__
#define __CLIPPING_LAYER_H__
#include "cocos2d.h"

class ClippingLayer
	:public cocos2d::CCLayer
{
public:
	CREATE_FUNC(ClippingLayer);
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
};

#endif