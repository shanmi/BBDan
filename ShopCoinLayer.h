#ifndef __SHOP_COIN_LAYER_H__
#define __SHOP_COIN_LAYER_H__

#include "cocos2d.h"
#include "GameController.h"

class UiLayout;
class ShopCoinLayer
	: public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();
	CREATE_FUNC(ShopCoinLayer);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

public:
	virtual void notifyViews();
public:
	void initLayout();
	void initCoinsLayout();
	void updateCoins();
	void closePanel(cocos2d::CCObject *pSender);
	void buyCoinItem(cocos2d::CCObject *pSender);

	void payCallback();

private:
	UiLayout *m_mainLayout;
};

#endif