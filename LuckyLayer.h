#ifndef __LUCKY_LAYER_H__
#define __LUCKY_LAYER_H__

#include "cocos2d.h"
#include "LuckyUtil.h"

enum LuckyType
{
	kLucky_Free,
	kLucky_Video,
	kLucky_Close,
};

class UiLayout;
class LuckyLayer
	:public cocos2d::CCLayer
	, public ILuckyView
{
public:
	virtual bool init();
	static LuckyLayer *create(int type = kLucky_Free);

	virtual void onEnter();
	virtual void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void updateView();
	virtual void onVideoCallback();

public:
	void initLayout(bool refresh = false);
	void initItemLayout();
	void closePanel(cocos2d::CCObject *pSender);
	void startDraw(cocos2d::CCObject *pSender);
	void showVideoDialog(cocos2d::CCObject *pSender);
	void getLuckyItem(int type);

private:
	LuckyLayer(int type);

private:
	UiLayout *m_mainLayout;
	UiLayout *m_itemLayout;
	int m_type;
};

#endif