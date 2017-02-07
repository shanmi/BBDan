#ifndef __LOGIN_LAYER_H__ 
#define __LOGIN_LAYER_H__ 
#include "cocos2d.h"

class UiLayout;
class LoginLayer :
	public cocos2d::CCLayer
{
public:
	~LoginLayer(){}
	CREATE_FUNC(LoginLayer);

	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();

	void getLoginReward();
	void closePannel(cocos2d::CCObject *pSender);

private:
	LoginLayer(){}
private:
	UiLayout *m_mainLayout;
};

#endif