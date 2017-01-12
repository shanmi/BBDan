#ifndef _GAME_SHOOTER_MODE_H_
#define _GAME_SHOOTER_MODE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GameController.h"

class UiLayout;
class MarbleNode;
class GameShooterMode 
	: public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init();
	static cocos2d::CCScene *scene();
	static GameShooterMode *create();
	GameShooterMode();
	~GameShooterMode();

	virtual void onEnter();
	virtual void onExit();
	virtual void update(float dt);
	virtual void draw(); //for debug

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

public:
	void oneRoundEnd();
	void updateMarbles();
	void updateCoins();
	void updateMarbleCount();
	void addSquareNode(SquareNode *node);
	void showGameOver();

public:

	void onDoubleAttact(cocos2d::CCObject *pSender);
	void onClearScreen(cocos2d::CCObject *pSender);
	void onFreezing(cocos2d::CCObject *pSender);
	void initTopLayout();
	void initBottomLayout();
	void initGameLayout();
	void initPhysicBorder();
	void initMarbles();
	void initSquares();
	void addSquares();
	void updateStreak(float dt);

private:
	b2World *m_world;
	UiLayout *m_topLayout;
	UiLayout *m_bottomLayout;
	cocos2d::CCSprite *m_touchPoint;
	cocos2d::CCSprite *m_arrow;
	cocos2d::CCLabelTTF *m_marbleCount;
	float m_topLinePos;
	float m_bottomLinePos;
	float m_shootDegree;

	bool m_bIsShoot;
	int m_addMarbleCount;
};
#endif