#ifndef __HELP_LAYER_H__
#define __HELP_LAYER_H__

#include "cocos2d.h"
#include "GameController.h"
class UiLayout;
class HelpLayer
	:public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init();
	CREATE_FUNC(HelpLayer);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

public:
	void initLayout();
	void closePanel(cocos2d::CCObject *pSender);

private:
	HelpLayer(){}

private:
	UiLayout *m_mainLayout;
	int m_type;
};

#endif