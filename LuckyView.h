#ifndef __LUCKY_VIEW_H__
#define __LUCKY_VIEW_H__

#include "cocos2d.h"
#include "GameController.h"

class UiLayout;
class LuckyView
	:public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init();
	static LuckyView *create(int type);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void setOkCallback(std::function<void()> callback){ m_callback = callback; }
	virtual void updateCoins();

public:
	void initLayout();
	void closePanel(cocos2d::CCObject *pSender);
	void show(cocos2d::CCObject *pSender);
	void initRewardItem();

private:
	LuckyView(int type);

private:
	UiLayout *m_mainLayout;
	int m_type;
	std::function<void()> m_callback;
};

#endif