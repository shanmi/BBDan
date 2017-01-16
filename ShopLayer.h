#ifndef __SHOP_LAYER_H__
#define __SHOP_LAYER_H__

#include "cocos2d.h"
#include "GameController.h"

class UiLayout;
class ShopLayer 
	: public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();
	CREATE_FUNC(ShopLayer);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

public:
	virtual void updateCoins();
public:
	void initLayout();
	void closePanel(cocos2d::CCObject *pSender);
	void toCoinPanel(cocos2d::CCObject *pSender);
	void toBallPanel(cocos2d::CCObject *pSender);

	void payCallback();

private:
	UiLayout *m_mainLayout;
};

#endif