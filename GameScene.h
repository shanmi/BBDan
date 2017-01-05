#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GameController.h"

class UiLayout;
class MarbleNode;
class GameScene 
	: public cocos2d::CCLayer
	, public INotifyView
{
public:
	virtual bool init();
	static cocos2d::CCScene *scene();
	static GameScene *create();
	GameScene();
	~GameScene();

	virtual void onEnter();
	virtual void onExit();
	virtual void update(float dt);
	virtual void draw(); //for debug

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

public:
	void updateSquares();
	void showGameOver();

public:
	void initTopLayout();
	void initBottomLayout();
	void initGameLayout();
	void initPhysicBorder();
	void initMarbles();
	void initSquares();
	void addSquares();

private:
	b2World *m_world;
	UiLayout *m_topLayout;
	UiLayout *m_bottomLayout;
	cocos2d::CCSprite *m_touchPoint;
	cocos2d::CCSprite *m_arrow;
	float m_topLinePos;
	float m_bottomLinePos;
	float m_shootDegree;

	bool m_bIsShoot;
};
#endif