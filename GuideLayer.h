#ifndef __GUIDE_LAYER_H__
#define __GUIDE_LAYER_H__

#include "cocos2d.h"

class GuideLayer
	:public cocos2d::CCLayer
{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();
	CREATE_FUNC(GuideLayer);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

};
#endif