#ifndef __PAUSE_LAYER_H__
#define __PAUSE_LAYER_H__

#include "cocos2d.h"
#include "GameController.h"
class UiLayout;
class PauseLayer
	:public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init();
	CREATE_FUNC(PauseLayer);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void notifyViews();

public:
	void initLayout();
	void closePanel(cocos2d::CCObject *pSender);
	void backToHome(cocos2d::CCObject *pSender);

private:
	PauseLayer(){}

private:
	UiLayout *m_mainLayout;
	int m_type;
};

#endif