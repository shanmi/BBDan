#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GameController.h"

class UiLayout;
class MarbleNode;
class CharacterView;
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
	virtual void keyBackClicked();

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);

public:
	void oneRoundEnd();
	void updateMarbles();
	void updateCoins();
	void updateScore();
	void updatePropsCount();
	void updateMarbleCount();
	void addSquareNode(SquareNode *node);
	void showGameOver();
	void showAddCoinEffect(SquareNode *node);
	void showAddMarbleEffect(SquareNode *node);
	void updateMarbleType(int type);
public:

	void onDoubleAttact(cocos2d::CCObject *pSender);
	void onClearScreen(cocos2d::CCObject *pSender);
	void onFreezing(cocos2d::CCObject *pSender);
	void onPauseGame(cocos2d::CCObject *pSender);
	void onHelpPanel(cocos2d::CCObject *pSender);
	void onMarbleChange(cocos2d::CCObject *pSender);
	void initMainLayout();
	void initTopLayout();
	void initBottomLayout();
	void initGameLayout();
	void initPhysicBorder();
	void initMarbles();
	void initSquares();
	void addSquares();

	void checkFishGuide();
	void showLibaoDiaolg();
	void checkLibaoShow();

	void characterMove(float offsetX);
	void onTouchCallback();

	void updateProgress();

	void showBannerView(float dt);

private:
	b2World *m_world;
	UiLayout *m_mainLayout;
	UiLayout *m_topLayout;
	UiLayout *m_bottomLayout;
	CharacterView *m_characterView;
	cocos2d::CCSprite *m_touchPoint;
	cocos2d::CCSprite *m_touchCircle;
	cocos2d::CCSprite *m_arrow;
	cocos2d::CCLabelAtlas *m_marbleCount;
	cocos2d::CCPoint m_touchPos;
	cocos2d::CCProgressTimer *m_progressTimer;
	float m_topLinePos;
	float m_bottomLinePos;
	float m_shootDegree;

	bool m_bIsDoubleAttact;
	int m_addMarbleCount;
	bool m_bShowingBanner;
};
#endif