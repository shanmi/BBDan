#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "cocos2d.h"

class UiLayout;
class MainMenu : public cocos2d::CCLayer
{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();

	void onStartGameOne(CCObject* pSender);
	void onStartGameTwo(CCObject* pSender);

	CREATE_FUNC(MainMenu);

	void onEnter();
	void onExit();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void keyBackClicked();

public:
	void initLayout();

private:
	UiLayout *m_mainLayout;
};
#endif