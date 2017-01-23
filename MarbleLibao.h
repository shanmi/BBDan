#ifndef __MARBLE_LIBAO_H__
#define __MARBLE_LIBAO_H__

#include "cocos2d.h"

class UiLayout;
class MarbleLibao
	:public cocos2d::CCLayer
{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();
	CREATE_FUNC(MarbleLibao);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

public:
	void initLayout();
	void closePanel(cocos2d::CCObject *pSender);
	void buyLibao(cocos2d::CCObject *pSender);

private:
	UiLayout *m_mainLayout;
};

#endif