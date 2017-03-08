#ifndef __SHOP_SKIN_LAYER_H__
#define __SHOP_SKIN_LAYER_H__

#include "cocos2d.h"
#include "GameController.h"

class UiLayout;
class PageView;
class ShopSkinLayer 
	: public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init(int marbleType);
	static cocos2d::CCScene* scene();
	static ShopSkinLayer *create(int marbleType = kMarble_Normal);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

public:
	virtual void notifyViews();
public:
	void initLayout();
	void initSkinLayout();
	void updateCoins();
	void closePanel(cocos2d::CCObject *pSender);
	void toLeftPanel(cocos2d::CCObject *pSender);
	void toRightPanel(cocos2d::CCObject *pSender);
	void updatePage(int pageIndex);
	void moveToPageIndex(int pageIndex);

	void payMarbleItem(cocos2d::CCObject *pSender);
	void useMarbleItem(cocos2d::CCObject *pSender);

private:
	ShopSkinLayer(){}

private:
	UiLayout *m_mainLayout;
	PageView *m_pageView;
	int m_pageIndex;
};

#endif