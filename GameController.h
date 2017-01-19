#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "CommonMacros.h"
#include "Config.h"

class SquareNode;
class MarbleNode;
struct INotifyView
{
	virtual void oneRoundEnd(){}
	virtual void updateMarbles(){}
	virtual void updateCoins(){}
	virtual void showGameOver(){}
	virtual void addSquareNode(SquareNode *node){}
};

class GameController
{
public:
	static GameController *getInstance();
	~GameController(){}

public:
	void addView(INotifyView *view);
	void removeView(INotifyView *view);
	void oneRoundEnd(){ NOTIFY_VIEWS(oneRoundEnd); }
	void showGameOver() { NOTIFY_VIEWS(showGameOver); }
	void updateMarbles() { NOTIFY_VIEWS(updateMarbles); }
	void updateCoins() { NOTIFY_VIEWS(updateCoins); }
	void addSquareNode(SquareNode *node);
public:
	void startOneRound();

	void addCounter(){ ++m_counter; }
	void resetCounter(){ m_counter = 0; }
	int getCounter(){ return m_counter; }
	void setTargetPos(cocos2d::CCPoint pos){ m_targetPos = pos; }
	cocos2d::CCPoint getTargetPos(){ return m_targetPos; }

	bool isCounterFull();
	bool isRoundOver(){ return m_bIsRoundOver; }
	void setRoundState(bool isOver) { m_bIsRoundOver = isOver; }
	void checkSquares(bool isCheckTool = false);
	void updateMarblePos();
	bool checkGameOver();

	void setDoubleAttact(){ m_attactRate = ATTACT_RATE * 2; }
	void resetAttactRate(){ m_attactRate = ATTACT_RATE; }
	int getAttactRate(){ return m_attactRate; }

	bool checkCoinsEnought();
	void createPropByMarble(MarbleNode *marble);

	void backToMainMenu();

	bool isFirshInGame(){ return m_bISFirstIn; }
	void setFirstInGame(bool firtIn){ m_bISFirstIn = firtIn; }
private:
	GameController();

private:
	cocos2d::CCPoint m_targetPos;
	std::vector<INotifyView*> m_views;
	int m_counter;
	int m_attactRate;
	bool m_bIsRoundOver;

	bool m_bISFirstIn;
};

#endif