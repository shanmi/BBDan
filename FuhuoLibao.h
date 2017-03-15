#ifndef __FUHUO_LIBAO_H__
#define __FUHUO_LIBAO_H__

#include "cocos2d.h"
#include "GameController.h"

class UiLayout;
class FuhuoLibao
	:public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();
	CREATE_FUNC(FuhuoLibao);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

public:
	virtual void notifyViews();
public:
	void initLayout();
	void closePanel(cocos2d::CCObject *pSender);
	void buyLibao(cocos2d::CCObject *pSender);
	void pauseGame(float dt);

private:
	UiLayout *m_mainLayout;
};

#endif