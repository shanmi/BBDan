#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__
#include "MarbleNode.h"
#include "cocos2d.h"
#include "GameController.h"
class UiLayout;
class MainMenu
	: public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();

	CREATE_FUNC(MainMenu);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void keyBackClicked();

	virtual void updateMarbles();

public:
	void initLayout();
	void toStartGameOne(CCObject* pSender);
	void toStartGameTwo(CCObject* pSender);
	void toShopLayer(cocos2d::CCObject *pSender);
	void toSettingLayer(cocos2d::CCObject *pSender);
	void toSkinLayer(cocos2d::CCObject *pSender);
	void toMoreGame(cocos2d::CCObject *pSender);
	void toLibao1(cocos2d::CCObject *pSender);
	void toLibao2(cocos2d::CCObject *pSender);
	void toSoundSwitch(cocos2d::CCObject *pSender);
	cocos2d::CCAction *getAnimation(float duration, float rotation);

private:
	UiLayout *m_mainLayout;
	MarbleNode *m_marble;
};
#endif