#ifndef __LIBAO_DIALOG_H__
#define __LIBAO_DIALOG_H__

#include "cocos2d.h"
#include "GameController.h"
class UiLayout;
class LibaoDialog
	:public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init();
	static LibaoDialog *create(int type);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void updateCoins();

public:
	void initLayout();
	void closePanel(cocos2d::CCObject *pSender);
	void buyLibao(cocos2d::CCObject *pSender);

private:
	LibaoDialog(int type);

private:
	UiLayout *m_mainLayout;
	int m_type;
};

#endif